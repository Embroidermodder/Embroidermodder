;;; Common utilities for tinyscheme
;;;
;;; 2012/04 OL


;;; standard reduce
(define (reduce fn base-value lis)
  (if (null? lis)
      base-value
      (fn (car lis)
	  (reduce fn base-value (cdr lis)))))


;;; partial application of arguments to function
(define (partial fun . args)
  (lambda x
    (apply fun (append args x))))


;;; creates a new hash-map
;;; (hash-map & keyvals)
;;; keyval => key val
(define (hash-map . keyvals)
  (letrec
      ((rec (lambda (hash keyvals)
	      (if (pair? keyvals)
		  (rec (cons (cons (car keyvals) (cadr keyvals)) hash) (cddr keyvals))
		  hash))))
    (rec '() keyvals)))


;;; retrieves values for key from hash
(define (hash-get hash key)
  (if (eqv? (caar hash) key)
      (cdar hash)
      (hash-get (cdr hash) key)))


;;; utility function missing in tiny-scheme
(define (with-input-from-string s p)
  (let ((inport (open-input-string s)))
    (if (eq? inport #f)
	#f
	(let ((prev-inport (current-input-port)))
	  (set-input-port inport)
	  (let ((res (p)))
	    (close-input-port inport)
	    (set-input-port prev-inport)
	    res)))))


;;; print arguments without newline to standard output
(define (print . args)
  (write args))


;;; print arguments followed by newline to standard output
(define (println . args)
  (apply print args)
  (newline))
