;;; json parsing
;;; taken from http://blog.planetxml.de/archives/31-JSON-Parser-in-Scheme.html
;;; adapted to tinyscheme by OL
;;;
;;; requires: tinymodules.scm, utils.scm
;;;
;;;
;;; Usage
;;;
;;; (define parameter-obj (json/parse-file "default_params.json"))
;;; (define json-string (json/gen-string parameter-obj))
;;; (define parameter-obj2 (json/parse-string json-string))
;;;
;;; parameter-obj and parameter-obj2 should now match
;;;

(module json
	(json/parse json/parse-string json/parse-file json/gen-string)


  ;;; test func for env
	(define (check) 42)

	(define (tokenize)
	  (let loop ((res '())
		     (token (next-token)))
	    (if (eof-object? token)
		(reverse res)
		(loop (cons token res) (next-token)))))

  ;;; helper for error reporting
	(define (->string x)
	  (cond
	   ((char? x) (string x))
	   ((symbol? x) (symbol->string x))
	   ((eof-object? x) "<EOF>")
	   (else "")))

  ;;; testing for control characters, handles only ascii and iso-8859-1 characters
	(define (char-control? ch)
	  (let ((i (char->integer ch)))
	    (or (< i 32)
		(< 127 i 160))))

	(define (lexer-error ch)
	  (error (string-append "unexpected character " (->string ch))))

	(define (parse-error token)
	  (error (string-append "unexpected token " (->string token))))

  ;;; reads a character and signals an error if it does not match the expected character
	(define (consume-char expect)
	  (let ((ch (read-char)))
	    (if (eqv? ch expect)
		ch
		(lexer-error ch))))

	(define (next-token)
	  (let ((ch (read-char)))
	    (case ch
	      ((#\space #\newline #\tab #\return) (next-token))
	      ((#\" #\') (parse-string ch))
	      ((#\- #\0 #\1 #\2 #\3 #\4 #\5 #\6 #\7 #\8 #\9) (parse-number ch))
	      ((#\[) 'open-brace)
	      ((#\]) 'close-brace)
	      ((#\{) 'open-curly)
	      ((#\}) 'close-curly)
	      ((#\:) 'colon)
	      ((#\,) 'comma)
	      ((#\t) (parse-true))
	      ((#\f) (parse-false))
	      ((#\n) (parse-null))
	      (else (if (eof-object? ch)
			ch
			(lexer-error ch))))))

  ;;; XXX parsing of numbers is not really correct
	(define (parse-number ch)
	  (let loop ((res (string ch)))
	    (let ((ch (peek-char)))
	      (case ch
		((#\- #\+ #\0 #\1 #\2 #\3 #\4 #\5 #\6 #\7 #\8 #\9 #\. #\e #\E)
		 (begin (read-char)
			(loop (string-append res (string ch)))))
		(else (string->number res))))))

  ;;; reads the symbol 'true, the first character is already read by next-token
	(define (parse-true)
	  (begin
	    (consume-char #\r)
	    (consume-char #\u)
	    (consume-char #\e)
	    #t))

  ;;; reads the symbol 'false the first character is already read by next-token
	(define (parse-false)
	  (begin
	    (consume-char #\a)
	    (consume-char #\l)
	    (consume-char #\s)
	    (consume-char #\e)
	    #f))

  ;;; reads the symbol 'null the first character is already read by next-token
	(define (parse-null)
	  (begin
	    (consume-char #\u)
	    (consume-char #\l)
	    (consume-char #\l)
	    '()))

	(define (parse-string q)
	  (let loop ((res ""))
	    (let ((ch (read-char)))
	      (cond
	       ((eqv? ch #\\) (loop (string-append res (string (parse-escape q)))))
	       ((eqv? ch q) res)
	       ((eqv? ch q) res)
	       ((not (char-control? ch)) (loop (string-append res (string ch))))
	       (else (lexer-error ch))))))

	(define (parse-escape q)
	  (let ((ch (read-char)))
	    (case ch
	      ((#\b) #\x08)
	      ((#\f) #\x0C)
	      ((#\n) #\newline)
	      ((#\r) #\return)
	      ((#\t) #\tab)
	      ((#\\) #\\)
	      ((#\u) (parse-unicode))
	      (else (if (eqv? ch q)
			q
			(lexer-error ch))))))

	(define (numeric-char-value ch)
	  (- (char->integer ch) (char->integer #\0)))

	(define (hex-char-value ch)
	  (let ((i (char->integer ch)))
	    (cond ((<= 97 i 102) (- i 87)) ; a-f
		  ((<= 65 i 70)  (- i 55)) ; A-F
		  ((<= 48 i 57)  (- i 48)) ; 0-9
		  (else (lexer-error ch)))))

  ;;; parse an unicode escape consisting of four hexadecimal characters
	(define (parse-unicode)
	  (let* ((a (hex-char-value (read-char)))
		 (b (hex-char-value (read-char)))
		 (c (hex-char-value (read-char)))
		 (d (hex-char-value (read-char))))
	    (integer->char (+ (* 4096 a)
			      (*  256 b)
			      (*   16 c)
			      d))))

	(define (parse-object)
	  (parse-object-helper (next-token)))

	(define (parse-object-helper token)
	  (cond
	   ((eqv? token 'open-curly) (parse-map))
	   ((eqv? token 'open-brace) (parse-list))
	   ((null? token) token)
	   ((string? token) token)
	   ((number? token) token)
	   ((boolean? token) token)
	   (else (parse-error token))))

	(define (parse-map)
	  (let loop ((res '(map)))
	    (let ((token (next-token)))
	      (cond
	       ((eqv? token 'close-curly) (reverse res))
	       ((string? token) (let* ((res (cons (cons (string->symbol token) (parse-map-value))
						  res))
				       (next (next-token)))
				  (cond
				   ((eqv? next 'close-curly) (reverse res))
				   ((eqv? next 'comma) (loop res))
				   (else (parse-error next)))))
	       (else (parse-error token))))))

	(define (parse-map-value)
	  (let ((token (next-token)))
	    (if (eqv? token 'colon)
		(parse-object)
		(parse-error token))))

	(define (parse-list)
	  (let loop ((res '(list)))
	    (let ((token (next-token)))
	      (cond
	       ((eqv? token 'close-brace) (reverse res))
	       (else (let* ((res (cons (parse-object-helper token) res))
			    (next (next-token)))
		       (cond
			((eqv? next 'close-brace) (reverse res))
			((eqv? next 'comma) (loop res))
			(else (parse-error next)))))))))


	(define (double-quote s)
	  (string-append "\"" s "\""))

	(define (gen-string obj)
	  (if (pair? obj)
	      (let ((tok (car obj)))
		(case tok
		  ((map) (string-append
			  "{ "
			  (reduce
			   (lambda (o s)
			     (string-append
			      (double-quote (symbol->string (car o))) ":"
			      (json/gen-string (cdr o))
			      (if ( > (string-length s) 0) ", " "")
			      s)) ""
			      (cdr obj))
			  " }"))
		  ((list) (string-append
			   "[ "
			   (reduce
			    (lambda (o s)
			      (string-append
			       (json/gen-string o)
			       (if ( > (string-length s) 0) ", " "")
			       s)) ""
			       (cdr obj))
			   " ]"))))
	      (cond
	       ((boolean? obj) (if obj "true" "false"))
	       ((number? obj) (number->string obj))
	       ((string? obj) (double-quote obj))
	       ((list? obj) "null"))))


	;; JSON-API

	(define (json/parse p)
	  (with-input-from-port p parse-object))

	(define (json/parse-string str)
	  (with-input-from-string str parse-object))

	(define (json/parse-file file)
	  (with-input-from-file file parse-object))

	(define json/gen-string gen-string)

	) ; json module
