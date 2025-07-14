;;; params.scm
;;;
;;; parameter parser for json objects
;;; requires: json.scm
;;;
;;; 2012/04 OL
;;;
;;;
;;; requires: json.scm


(module params (read-from-json write-to-json)

;;; ---- setters/getters later implemented with C-bindings ---

;;; sample parameter setter for schema_version
;;;
;;; sample invocation:
;;; (set-schema_version "target" '(map (major 1) (minor 0)))
	(define (set-schema_version target args)
	  (let* ((args (cdr args))
		 (major (hash-get args 'major))
		 (minor (hash-get args 'minor)))
	    (write (list "->" target 'version  major minor))
	    (newline)
	    (cons 'map args)))

;;; sample parameter getter for schema_version
;;;
;;; sample invocation:
;;; (get-schema_version "target" '(map (major 1) (minor 0)))
	(define (get-schema_version target args)
	  (let* ((major 91)
		 (minor 92))
	    (write (list "<-" target 'version  major minor))
	    (newline)
	    (cons 'map (hash-map 'major major 'minor minor))))


;;; set the sampling rate
	(define (set-i32sampling target rate)
	  (write (list "->" target 'i32sampling rate))
	  (newline)
	  rate)


;;; set the sampling rate
	(define (get-i32sampling target rate)
	  (let ((new-rate 17200))
	    (write (list "<-" target 'i32sampling new-rate))
	    (newline)
	    new-rate))




;;; ---- reader/writer for marshalling/unmarshalling ---

;;; accessor for writing to or reading from target
	(define (par-accessor-pred acc-fun-prefix s target args)
	  (let ((accessor (string->symbol
			   (string-append acc-fun-prefix (symbol->string s)))))
	    (if (defined? accessor)
		(cons s (eval (list accessor target (quote args))))
		(cons s args) ;;; when not applied return as pair
		)))


;;; setter appends prefix "set-" for setter function
;;; and execute it when defined
	(define par-setter-pred (partial par-accessor-pred "set-"))


;;; setter appends prefix "set-" for setter function
;;; and execute it when defined
	(define par-getter-pred (partial par-accessor-pred "get-"))


;;; parse json object, call setter function for each json value
	(define (pparse pred json-data target-des)
	  (case (car json-data)
	    ((map)
	     (cons 'map (map (lambda (e) (pparse pred e target-des))
			     (cdr json-data))))
	    ((list)
	     (cons 'list (reduce (lambda (e l) (cons (pparse pred e target-des) l))
				 '() (cdr json-data))))
	    (else (pred (car json-data) target-des (cdr json-data)))))


;;; transfer json data to C-target with given descriptor
	(define (read-from-json obj target-des)
	  (pparse par-setter-pred obj target-des))


;;; transfer C-target with given descriptor to json data object
	(define (write-to-json obj target-des)
	  (pparse par-getter-pred obj target-des))
	)

