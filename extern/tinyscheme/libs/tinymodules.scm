;;; Emulation of Chicken-Scheme module system
;;; for TinyScheme
;;;
;;; 2012-04, OL


;;; module - defines forms within environment name
;;;
;;; The symbols in the exports-list are bind to
;;; the current name space
;;;
;;; example:
;;;
;;; (module test (a c)
;;; 	(define a 42)
;;; 	(define b 43)
;;; 	(define c 44))
;;;
;;; defines the module test with the internal definition
;;; b and the exported definitions a and c.

(define-macro (module name exports . forms)
  `(begin
    (define ,name
       (apply (lambda () ,@forms (current-environment))))
    ,@(map (lambda (v) `(define ,v (eval (quote ,v) ,name))) exports))
  )

;; for testing purposes
;;(macro-expand
;; '(module test (a c) (define a 42) (define b 43) (define c 44)))
;;
;;(module test (a c) (define a 42) (define b 43) (define c 44))

;; OVERWRITE if path is different
(define module-default-path "libs/")

;;; emulation of Chicken's import procedures
;;;
;;; usage: (import module-name)
;;;        which maps to (load "module-name.scm")
(define-macro (import module-name)
  `(load (string-append module-default-path (symbol->string (quote ,module-name)) ".scm")))
