;    Initialization file for TinySCHEME 1.41

; Per R5RS, up to four deep compositions should be defined
(define (caar x) (car (car x)))
(define (cadr x) (car (cdr x)))
(define (cdar x) (cdr (car x)))
(define (cddr x) (cdr (cdr x)))
(define (caaar x) (car (car (car x))))
(define (caadr x) (car (car (cdr x))))
(define (cadar x) (car (cdr (car x))))
(define (caddr x) (car (cdr (cdr x))))
(define (cdaar x) (cdr (car (car x))))
(define (cdadr x) (cdr (car (cdr x))))
(define (cddar x) (cdr (cdr (car x))))
(define (cdddr x) (cdr (cdr (cdr x))))
(define (caaaar x) (car (car (car (car x)))))
(define (caaadr x) (car (car (car (cdr x)))))
(define (caadar x) (car (car (cdr (car x)))))
(define (caaddr x) (car (car (cdr (cdr x)))))
(define (cadaar x) (car (cdr (car (car x)))))
(define (cadadr x) (car (cdr (car (cdr x)))))
(define (caddar x) (car (cdr (cdr (car x)))))
(define (cadddr x) (car (cdr (cdr (cdr x)))))
(define (cdaaar x) (cdr (car (car (car x)))))
(define (cdaadr x) (cdr (car (car (cdr x)))))
(define (cdadar x) (cdr (car (cdr (car x)))))
(define (cdaddr x) (cdr (car (cdr (cdr x)))))
(define (cddaar x) (cdr (cdr (car (car x)))))
(define (cddadr x) (cdr (cdr (car (cdr x)))))
(define (cdddar x) (cdr (cdr (cdr (car x)))))
(define (cddddr x) (cdr (cdr (cdr (cdr x)))))

;;;; Utility to ease macro creation
(define (macro-expand form)
  ((eval (get-closure-code (eval (car form)))) form))

(define (macro-expand-all form)
  (if (macro? form)
      (macro-expand-all (macro-expand form))
      form))

(define *compile-hook* macro-expand-all)

;;;; 
;;;; START Features ported from DZMLang by D. Madarasz.
;;;;

(define nil '())

(define (length@ items)
  (if (proper-list? items)
      (begin
        (define (iter a count)
          (if (null? a)
              count
              (iter (cdr a) (+ 1 count))))
        (iter items 0))
  (error "length: Not a proper list!")))
  

(define (length+ items)
  (if (proper-list? items)
      (begin
        (define (iter a count)
          (if (null? a)
              count
              (iter (cdr a) (+ 1 count))))
        (iter items 0))
  #f))
  
(define (attach-tag type-tag contents)
  (cons type-tag contents))

(define (type-tag x)
  (if (pair? x)
      (car x)
      (error "Bad tagged entity: TYPE-TAG" x)))

(define (contents x)
  (if (pair? x)
      (cdr x)
      (error "Bad tagged entity: CONTENT" x)))

(define (count-leaves x)
  (cond ((null? x) 0)
        ((not (pair? x)) 1)
        (else (+ (count-leaves (car x))
                 (count-leaves (cdr x))))))

(define (append list1 list2)
  (if (null? list1)
      list2
      (cons (car list1) (append (cdr list1) list2))))

(define (reverse l)
  (define (iter in out)
    (if (pair? in)
        (iter (cdr in) (cons (car in) out))
        out))
  (iter l '()))

(define (deep-reverse ls)
  (define (iter ls acc)
    (if (null? ls)
        acc
        (if (list? (car ls))
            (iter (cdr ls) (cons (deep-reverse (car ls)) acc))
            (iter (cdr ls) (cons (car ls) acc)))))
  (iter ls '()))

(define (flatten x)
  (cond ((null? x) '())
        ((pair? x) (append (flatten (car x)) (flatten (cdr x))))
        (else (list x))))

(define (reverse-string l)
  (list->string (reverse (string->list l))))

(define (tree-map proc tree)
  (map (lambda (sub-tree)
         (if (list? sub-tree)
             (tree-map proc sub-tree)
             (proc sub-tree)))
       tree))

(define (subsets ls)
  (if (null? ls)
      (list nil)
      (let ((rest (subsets (cdr ls))))
        (append rest (map (lambda (x)
                            (cons (car ls) x))
                          rest)))))

(define (repeat f n)
  (if (null? n)
      #t
      (begin
        (f)
        (repeat f (cdr n)))))

(define (contains x l)
  (cond ((nil? l) #f)
        ((eq? x (car l)) #t)
        (else (contains x (cdr l)))))

(define (filter predicate sequence)
  (cond ((null? sequence) nil)
        ((predicate (car sequence))
         (cons (car sequence)
               (filter predicate
                       (cdr sequence))))
        (else (filter predicate
                      (cdr sequence)))))

(define (accumulate op initial sequence)
  (if (null? sequence)
      initial
      (op (car sequence)
          (accumulate op
                      initial
                      (cdr sequence)))))
(define acc accumulate)

(define (enumerate-interval low high)
  (if (> low high)
      nil
      (cons low
            (enumerate-interval
              (+ low 1)
              high))))
(define range enumerate-interval)

(define (enumerate-tree tree)
  (cond ((null? tree) nil)
        ((not (list? tree)) (list tree))
        (else (append
                (enumerate-tree (car tree))
                (enumerate-tree (cdr tree))))))
(define for enumerate-tree)

(define (id o) o)
(define (flip f) (lambda (a b) (f b a)))

(define (curry f a) (lambda (b) (apply f (cons a (list b)))))
(define (compose f g) (lambda (a) (f (apply g a))))
(define nil? null?)
(define (new class params)
  (cond
    ((null? params) (class))
    (else (apply class params))))

(define (not x)
  (if x #f #t))

(define (send message obj params)
  (cond
    ((or (null? obj) (null? message)) '())
    ((null? params) (apply (obj message)))
    (else (apply (obj message) params))))

(define (last l)
  (if (nil? (cdr l))
      (car l)
      (last (cdr l))))

(define (last-pair l)
  (if (nil? (cdr l))
      l
      (last-pair (cdr l))))

(define (no-more? l)
  (if (null? (car (cdr l)))
      #t
      #f))

(define (remove n l)
  (filter (lambda (x) (not (eqv? x n)))
          l))

(define (memq n x)
  (cond ((null? x) #f)
        ((eq? n (car x)) x)
        (else (memq n (cdr x)))))

(define (apply-generic op args)
  (let ((type-tags (map type-tag args)))
    (let ((proc (get op type-tags)))
      (if proc
          (apply proc (map contents args))
          (error
            "No method for these types:
            APPLY-GENERIC"
            (list op type-tags))))))


;;; QUEUE
(define (make-queue)
  (let ((front-ptr '())
        (rear-ptr '()))
    (define (empty-queue?) (null? front-ptr))
    (define (set-front-ptr! item) (set! front-ptr item))
    (define (set-rear-ptr! item) (set! rear-ptr item))
    (define (front-queue)
      (if (empty-queue?)
          (error "FRONT called with an empty queue")
          (car front-ptr)))
    (define (insert-queue! item)
      (let ((new-pair (cons item '())))
        (cond ((empty-queue?)
               (set-front-ptr! new-pair)
               (set-rear-ptr! new-pair))
              (else
                (set-cdr! rear-ptr new-pair)
                (set-rear-ptr! new-pair)))))
    (define (delete-queue!)
      (cond ((empty-queue?)
             (error "DELETE called with an empty queue"))
            (else (set-front-ptr! (cdr front-ptr)))))
    (define (print-queue) front-ptr)
    (define (dispatch m)
      (cond ((eq? m 'empty) empty-queue?)
            ((eq? m 'front) front-queue)
            ((eq? m 'insert!) insert-queue!)
            ((eq? m 'delete!) delete-queue!)
            ((eq? m 'print) print-queue)
            ((eq? m 'type-of) (lambda() 'queue))
            (else (error "undefined operation -- QUEUE" m))))
    dispatch))

;;; SET

(define (make-set)
  (let ((set '()))
    (define (insert! x)
      (if (null? set) (set! set (list x))
          (insert-iter x set)))
    (define (insert-iter x l)
      (cond ((null? l) (list x))
            ((= x (car l)) #t)
            ((< x (car l)) (set! set (cons x l)))
            (else (set! set (cons (car l) (insert-iter x (cdr l)))))))
    (define (get) set)
    (define (self m)
      (cond ((eq? m 'insert!) insert!)
            ((eq? m 'get) get)
            ((eq? m 'type-of) (lambda() 'set))
            (else (no-msg))))
    self))

;;; TABLE

(define (fold-left op init seq)
  (define (iter ans rest)
    (if (null? rest)
        ans
        (iter (op ans (car rest))
              (cdr rest))))
  (iter init seq))

(define (make-table same-key?)
  (define (associate key records)
    (cond ((null? records) '())
          ((same-key? key (caar records)) (car records))
          (else (associate key (cdr records)))))
  
  (let ((the-table (list '*table*)))
    (define (lookup keys)
      (define (lookup-record record-list key)
        (if (not (null? record-list))
            (let ((record (associate key record-list)))
              (if (null? record)
                  '()
                  (cdr record)))
            '()))
      (fold-left lookup-record (cdr the-table) keys))
    
    (define (insert! keys value)
      (define (descend table key)
        (let ((record (associate key (cdr table))))
          (if (not (null? record))
              record
              (let ((new (cons (list key)
                               (cdr table))))
                (set-cdr! table new)
                (car new)))))
      (set-cdr! (fold-left descend the-table keys)
                value))
    
    (define (print)
      (define (indent tabs)
        (for-each (lambda (x) (display "-")) (range 0 tabs)))
      
      (define (print-record rec level)
        (indent level)
        (display (car rec))
        (display ": ")
        (if (list? (cdr rec))
            (begin (newline)
              (print-table rec (+ level 1)))
            (begin (display (cdr rec))
              (newline))))
      (define (print-table table level)
        (if (null? (cdr table))
            (begin (display "-no entries-")
              (newline))
            (for-each (lambda (record)
                        (print-record record level))
                      (cdr table))))
      (print-table the-table 0))
    
    (define (dispatch m)
      (cond ((eq? m 'lookup) lookup)
            ((eq? m 'update!) insert!)
            ((eq? m 'insert!) insert!)
            ((eq? m 'print) print)
            ((eq? m 'the-table) the-table)
            ((eq? m 'type-of) (lambda() 'table))
            (else (error "Undefined method" m))))
    dispatch))

(macro (package form)
       `(apply (lambda ()
                 ,@(cdr form)
                 (current-environment))))

;;; GENERIC

(define (char-lowercase x)
  (if (and (>= (char->integer x) 65) (<= (char->integer x) 90))
      (integer->char (+ (char->integer x) 32))
      x))

(define (char-uppercase x)
  (if (and (>= (char->integer x) 97) (<= (char->integer x) 122))
      (integer->char (- (char->integer x) 32))
      x))

(define (char-flipcase x)
  (if (and (>= (char->integer x) 65) (<= (char->integer x) 90))
      (char-lowercase x)
      (char-uppercase x)))

(define (no-msg) (error "Message not found!"))

;;;;
;;;; END
;;;;


(macro (unless form)
       `(if (not ,(cadr form)) (begin ,@(cddr form))))

(macro (when form)
       `(if ,(cadr form) (begin ,@(cddr form))))

; DEFINE-MACRO Contributed by Andy Gaynor
(macro (define-macro dform)
       (if (symbol? (cadr dform))
           `(macro ,@(cdr dform))
           (let ((form (gensym)))
             `(macro (,(caadr dform) ,form)
                     (apply (lambda ,(cdadr dform) ,@(cddr dform)) (cdr ,form))))))

; Utilities for math. Notice that inexact->exact is primitive,
; but exact->inexact is not.
(define exact? integer?)
(define (inexact? x) (and (real? x) (not (integer? x))))
(define (even? n) (= (remainder n 2) 0))
(define (odd? n) (not (= (remainder n 2) 0)))
(define (zero? n) (= n 0))
(define (positive? n) (> n 0))
(define (negative? n) (< n 0))
(define complex? number?)
(define rational? real?)
(define (abs n) (if (>= n 0) n (- n)))
(define (exact->inexact n) (* n 1.0))
(define (<> n1 n2) (not (= n1 n2)))
(define (nth l n) (if (= n 0) (car l) (nth (cdr l) (1- n))))

; min and max must return inexact if any arg is inexact; use (+ n 0.0)
(define (max . lst)
  (foldr (lambda (a b)
           (if (> a b)
               (if (exact? b) a (+ a 0.0))
               (if (exact? a) b (+ b 0.0))))
         (car lst) (cdr lst)))
(define (min . lst)
  (foldr (lambda (a b)
           (if (< a b)
               (if (exact? b) a (+ a 0.0))
               (if (exact? a) b (+ b 0.0))))
         (car lst) (cdr lst)))

(define (succ x) (+ x 1))
(define (pred x) (- x 1))
(define 1+ succ)
(define 1- pred)
(define gcd
  (lambda a
    (if (null? a)
        0
        (let ((aa (abs (car a)))
              (bb (abs (cadr a))))
          (if (= bb 0)
              aa
              (gcd bb (remainder aa bb)))))))
(define lcm
  (lambda a
    (if (null? a)
        1
        (let ((aa (abs (car a)))
              (bb (abs (cadr a))))
          (if (or (= aa 0) (= bb 0))
              0
              (abs (* (quotient aa (gcd aa bb)) bb)))))))


(define (string . charlist)
  (list->string charlist))

(define (list->string charlist)
  (let* ((len (length charlist))
         (newstr (make-string len))
         (fill-string!
           (lambda (str i len charlist)
             (if (= i len)
                 str
                 (begin (string-set! str i (car charlist))
                   (fill-string! str (+ i 1) len (cdr charlist)))))))
    (fill-string! newstr 0 len charlist)))

(define (string-fill! s e)
  (let ((n (string-length s)))
    (let loop ((i 0))
      (if (= i n)
          s
          (begin (string-set! s i e) (loop (succ i)))))))

(define (string->list s)
  (let loop ((n (pred (string-length s))) (l '()))
    (if (= n -1)
        l
        (loop (pred n) (cons (string-ref s n) l)))))

(define (string-copy str)
  (string-append str))

(define (string->anyatom str pred)
  (let* ((a (string->atom str)))
    (if (pred a) a
        (error "string->xxx: not a xxx" a))))

(define (string->number str . base)
  (let ((n (string->atom str (if (null? base) 10 (car base)))))
    (if (number? n) n #f)))

(define (anyatom->string n pred)
  (if (pred n)
      (atom->string n)
      (error "xxx->string: not a xxx" n)))

(define (number->string n . base)
  (atom->string n (if (null? base) 10 (car base))))


(define (char-cmp? cmp a b)
  (cmp (char->integer a) (char->integer b)))
(define (char-ci-cmp? cmp a b)
  (cmp (char->integer (char-downcase a)) (char->integer (char-downcase b))))

(define (char=? a b) (char-cmp? = a b))
(define (char<? a b) (char-cmp? < a b))
(define (char>? a b) (char-cmp? > a b))
(define (char<=? a b) (char-cmp? <= a b))
(define (char>=? a b) (char-cmp? >= a b))

(define (char-ci=? a b) (char-ci-cmp? = a b))
(define (char-ci<? a b) (char-ci-cmp? < a b))
(define (char-ci>? a b) (char-ci-cmp? > a b))
(define (char-ci<=? a b) (char-ci-cmp? <= a b))
(define (char-ci>=? a b) (char-ci-cmp? >= a b))

; Note the trick of returning (cmp x y)
(define (string-cmp? chcmp cmp a b)
  (let ((na (string-length a)) (nb (string-length b)))
    (let loop ((i 0))
      (cond
        ((= i na)
         (if (= i nb) (cmp 0 0) (cmp 0 1)))
        ((= i nb)
         (cmp 1 0))
        ((chcmp = (string-ref a i) (string-ref b i))
         (loop (succ i)))
        (else
          (chcmp cmp (string-ref a i) (string-ref b i)))))))


(define (string=? a b) (string-cmp? char-cmp? = a b))
(define (string<? a b) (string-cmp? char-cmp? < a b))
(define (string>? a b) (string-cmp? char-cmp? > a b))
(define (string<=? a b) (string-cmp? char-cmp? <= a b))
(define (string>=? a b) (string-cmp? char-cmp? >= a b))

(define (string-ci=? a b) (string-cmp? char-ci-cmp? = a b))
(define (string-ci<? a b) (string-cmp? char-ci-cmp? < a b))
(define (string-ci>? a b) (string-cmp? char-ci-cmp? > a b))
(define (string-ci<=? a b) (string-cmp? char-ci-cmp? <= a b))
(define (string-ci>=? a b) (string-cmp? char-ci-cmp? >= a b))

(define (list . x) x)

(define (foldr f x lst)
  (if (null? lst)
      x
      (foldr f (f x (car lst)) (cdr lst))))

(define (unzip1-with-cdr . lists)
  (unzip1-with-cdr-iterative lists '() '()))

(define (unzip1-with-cdr-iterative lists cars cdrs)
  (if (null? lists)
      (cons cars cdrs)
      (let ((car1 (caar lists))
            (cdr1 (cdar lists)))
        (unzip1-with-cdr-iterative
          (cdr lists)
          (append cars (list car1))
          (append cdrs (list cdr1))))))

(define (map proc . lists)
  (if (null? lists)
      (apply proc)
      (if (null? (car lists))
          '()
          (let* ((unz (apply unzip1-with-cdr lists))
                 (cars (car unz))
                 (cdrs (cdr unz)))
            (cons (apply proc cars) (apply map (cons proc cdrs)))))))

(define (for-each proc . lists)
  (if (null? lists)
      (apply proc)
      (if (null? (car lists))
          #t
          (let* ((unz (apply unzip1-with-cdr lists))
                 (cars (car unz))
                 (cdrs (cdr unz)))
            (apply proc cars) (apply map (cons proc cdrs))))))

(define (list-tail x k)
  (if (zero? k)
      x
      (list-tail (cdr x) (- k 1))))

(define (list-ref x k)
  (car (list-tail x k)))

(define (last-pair x)
  (if (pair? (cdr x))
      (last-pair (cdr x))
      x))

(define (head stream) (car stream))

(define (tail stream) (force (cdr stream)))

(define (vector-equal? x y)
  (and (vector? x) (vector? y) (= (vector-length x) (vector-length y))
       (let ((n (vector-length x)))
         (let loop ((i 0))
           (if (= i n)
               #t
               (and (equal? (vector-ref x i) (vector-ref y i))
                    (loop (succ i))))))))

(define (list->vector x)
  (apply vector x))

(define (vector-fill! v e)
  (let ((n (vector-length v)))
    (let loop ((i 0))
      (if (= i n)
          v
          (begin (vector-set! v i e) (loop (succ i)))))))

(define (vector->list v)
  (let loop ((n (pred (vector-length v))) (l '()))
    (if (= n -1)
        l
        (loop (pred n) (cons (vector-ref v n) l)))))

;; The following quasiquote macro is due to Eric S. Tiedemann.
;;   Copyright 1988 by Eric S. Tiedemann; all rights reserved.
;;
;; Subsequently modified to handle vectors: D. Souflis

(macro
  quasiquote
  (lambda (l)
    (define (mcons f l r)
      (if (and (pair? r)
               (eq? (car r) 'quote)
               (eq? (car (cdr r)) (cdr f))
               (pair? l)
               (eq? (car l) 'quote)
               (eq? (car (cdr l)) (car f)))
          (if (or (procedure? f) (number? f) (string? f))
              f
              (list 'quote f))
          (if (eqv? l vector)
              (apply l (eval r))
              (list 'cons l r)
              )))
    (define (mappend f l r)
      (if (or (null? (cdr f))
              (and (pair? r)
                   (eq? (car r) 'quote)
                   (eq? (car (cdr r)) '())))
          l
          (list 'append l r)))
    (define (foo level form)
      (cond ((not (pair? form))
             (if (or (procedure? form) (number? form) (string? form))
                 form
                 (list 'quote form))
             )
            ((eq? 'quasiquote (car form))
             (mcons form ''quasiquote (foo (+ level 1) (cdr form))))
            (#t (if (zero? level)
                    (cond ((eq? (car form) 'unquote) (car (cdr form)))
                          ((eq? (car form) 'unquote-splicing)
                           (error "Unquote-splicing wasn't in a list:"
                                  form))
                          ((and (pair? (car form))
                                (eq? (car (car form)) 'unquote-splicing))
                           (mappend form (car (cdr (car form)))
                                    (foo level (cdr form))))
                          (#t (mcons form (foo level (car form))
                                     (foo level (cdr form)))))
                    (cond ((eq? (car form) 'unquote)
                           (mcons form ''unquote (foo (- level 1)
                                                      (cdr form))))
                          ((eq? (car form) 'unquote-splicing)
                           (mcons form ''unquote-splicing
                                  (foo (- level 1) (cdr form))))
                          (#t (mcons form (foo level (car form))
                                     (foo level (cdr form)))))))))
    (foo 0 (car (cdr l)))))

;;;;;Helper for the dynamic-wind definition.  By Tom Breton (Tehom)
(define (shared-tail x y)
  (let ((len-x (length x))
        (len-y (length y)))
    (define (shared-tail-helper x y)
      (if
        (eq? x y)
        x
        (shared-tail-helper (cdr x) (cdr y))))
    
    (cond
      ((> len-x len-y)
       (shared-tail-helper
         (list-tail x (- len-x len-y))
         y))
      ((< len-x len-y)
       (shared-tail-helper
         x
         (list-tail y (- len-y len-x))))
      (#t (shared-tail-helper x y)))))

;;;;;Dynamic-wind by Tom Breton (Tehom)

;;Guarded because we must only eval this once, because doing so
;;redefines call/cc in terms of old call/cc
(unless (defined? 'dynamic-wind)
        (let
          ;;These functions are defined in the context of a private list of
          ;;pairs of before/after procs.
          (  (*active-windings* '())
           ;;We'll define some functions into the larger environment, so
           ;;we need to know it.
           (outer-env (current-environment)))
          
          ;;Poor-man's structure operations
          (define before-func car)
          (define after-func  cdr)
          (define make-winding cons)
          
          ;;Manage active windings
          (define (activate-winding! new)
            ((before-func new))
            (set! *active-windings* (cons new *active-windings*)))
          (define (deactivate-top-winding!)
            (let ((old-top (car *active-windings*)))
              ;;Remove it from the list first so it's not active during its
              ;;own exit.
              (set! *active-windings* (cdr *active-windings*))
              ((after-func old-top))))
          
          (define (set-active-windings! new-ws)
            (unless (eq? new-ws *active-windings*)
                    (let ((shared (shared-tail new-ws *active-windings*)))
                      
                      ;;Define the looping functions.
                      ;;Exit the old list.  Do deeper ones last.  Don't do
                      ;;any shared ones.
                      (define (pop-many)
                        (unless (eq? *active-windings* shared)
                                (deactivate-top-winding!)
                                (pop-many)))
                      ;;Enter the new list.  Do deeper ones first so that the
                      ;;deeper windings will already be active.  Don't do any
                      ;;shared ones.
                      (define (push-many new-ws)
                        (unless (eq? new-ws shared)
                                (push-many (cdr new-ws))
                                (activate-winding! (car new-ws))))
                      
                      ;;Do it.
                      (pop-many)
                      (push-many new-ws))))
          
          ;;The definitions themselves.
          (eval
            `(define call-with-current-continuation
               ;;It internally uses the built-in call/cc, so capture it.
               ,(let ((old-c/cc call-with-current-continuation))
                  (lambda (func)
                    ;;Use old call/cc to get the continuation.
                    (old-c/cc
                      (lambda (continuation)
                        ;;Call func with not the continuation itself
                        ;;but a procedure that adjusts the active
                        ;;windings to what they were when we made
                        ;;this, and only then calls the
                        ;;continuation.
                        (func
                          (let ((current-ws *active-windings*))
                            (lambda (x)
                              (set-active-windings! current-ws)
                              (continuation x)))))))))
            outer-env)
          ;;We can't just say "define (dynamic-wind before thunk after)"
          ;;because the lambda it's defined to lives in this environment,
          ;;not in the global environment.
          (eval
            `(define dynamic-wind
               ,(lambda (before thunk after)
                  ;;Make a new winding
                  (activate-winding! (make-winding before after))
                  (let ((result (thunk)))
                    ;;Get rid of the new winding.
                    (deactivate-top-winding!)
                    ;;The return value is that of thunk.
                    result)))
            outer-env)))

(define call/cc call-with-current-continuation)


;;;;; atom? and equal? written by a.k

;;;; atom?
(define (atom? x)
  (not (pair? x)))

;;;;    equal?
(define (equal? x y)
  (cond
    ((pair? x)
     (and (pair? y)
          (equal? (car x) (car y))
          (equal? (cdr x) (cdr y))))
    ((vector? x)
     (and (vector? y) (vector-equal? x y)))
    ((string? x)
     (and (string? y) (string=? x y)))
    (else (eqv? x y))))

;;;; (do ((var init inc) ...) (endtest result ...) body ...)
;;
(macro do
       (lambda (do-macro)
         (apply (lambda (do vars endtest . body)
                  (let ((do-loop (gensym)))
                    `(letrec ((,do-loop
                                (lambda ,(map (lambda (x)
                                                (if (pair? x) (car x) x))
                                              `,vars)
                                  (if ,(car endtest)
                                      (begin ,@(cdr endtest))
                                      (begin
                                        ,@body
                                        (,do-loop
                                          ,@(map (lambda (x)
                                                   (cond
                                                     ((not (pair? x)) x)
                                                     ((< (length x) 3) (car x))
                                                     (else (car (cdr (cdr x))))))
                                                 `,vars)))))))
                       (,do-loop
                         ,@(map (lambda (x)
                                  (if (and (pair? x) (cdr x))
                                      (car (cdr x))
                                      '()))
                                `,vars)))))
                do-macro)))

;;;; generic-member
(define (generic-member cmp obj lst)
  (cond
    ((null? lst) #f)
    ((cmp obj (car lst)) lst)
    (else (generic-member cmp obj (cdr lst)))))

(define (memq obj lst)
  (generic-member eq? obj lst))
(define (memv obj lst)
  (generic-member eqv? obj lst))
(define (member obj lst)
  (generic-member equal? obj lst))

;;;; generic-assoc
(define (generic-assoc cmp obj alst)
  (cond
    ((null? alst) #f)
    ((cmp obj (caar alst)) (car alst))
    (else (generic-assoc cmp obj (cdr alst)))))

(define (assq obj alst)
  (generic-assoc eq? obj alst))
(define (assv obj alst)
  (generic-assoc eqv? obj alst))
(define (assoc obj alst)
  (generic-assoc equal? obj alst))

(define (acons x y z) (cons (cons x y) z))

;;;; Handy for imperative programs
;;;; Used as: (define-with-return (foo x y) .... (return z) ...)
(macro (define-with-return form)
       `(define ,(cadr form)
          (call/cc (lambda (return) ,@(cddr form)))))

;;;; Simple exception handling
;
;    Exceptions are caught as follows:
;
;         (catch (do-something to-recover and-return meaningful-value)
;              (if-something goes-wrong)
;              (with-these calls))
;
;    "Catch" establishes a scope spanning multiple call-frames
;    until another "catch" is encountered.
;
;    Exceptions are thrown with:
;
;         (throw "message")
;
;    If used outside a (catch ...), reverts to (error "message)

(define *handlers* (list))

(define (push-handler proc)
  (set! *handlers* (cons proc *handlers*)))

(define (pop-handler)
  (let ((h (car *handlers*)))
    (set! *handlers* (cdr *handlers*))
    h))

(define (more-handlers?)
  (pair? *handlers*))

(define (throw . x)
  (if (more-handlers?)
      (apply (pop-handler))
      (apply error x)))

(macro (catch form)
       (let ((label (gensym)))
         `(call/cc (lambda (exit)
                     (push-handler (lambda () (exit ,(cadr form))))
                     (let ((,label (begin ,@(cddr form))))
                       (pop-handler)
                       ,label)))))

(define *error-hook* throw)

;;;;; Definition of MAKE-ENVIRONMENT, to be used with two-argument EVAL

(macro (make-environment form)
       `(apply (lambda ()
                 ,@(cdr form)
                 (current-environment))))

(define-macro (eval-polymorphic x . envl)
              (display envl)
              (let* ((env (if (null? envl) (current-environment) (eval (car envl))))
                     (xval (eval x env)))
                (if (closure? xval)
                    (make-closure (get-closure-code xval) env)
                    xval)))

; Redefine this if you install another package infrastructure
; Also redefine 'package'
(define *colon-hook* eval)

;;;;; I/O

(define (input-output-port? p)
  (and (input-port? p) (output-port? p)))

(define (close-port p)
  (cond
    ((input-output-port? p) (close-input-port (close-output-port p)))
    ((input-port? p) (close-input-port p))
    ((output-port? p) (close-output-port p))
    (else (throw "Not a port" p))))

(define (call-with-input-file s p)
  (let ((inport (open-input-file s)))
    (if (eq? inport #f)
        #f
        (let ((res (p inport)))
          (close-input-port inport)
          res))))

(define (call-with-output-file s p)
  (let ((outport (open-output-file s)))
    (if (eq? outport #f)
        #f
        (let ((res (p outport)))
          (close-output-port outport)
          res))))

(define (with-input-from-file s p)
  (let ((inport (open-input-file s)))
    (if (eq? inport #f)
        #f
        (let ((prev-inport (current-input-port)))
          (set-input-port inport)
          (let ((res (p)))
            (close-input-port inport)
            (set-input-port prev-inport)
            res)))))

(define (with-output-to-file s p)
  (let ((outport (open-output-file s)))
    (if (eq? outport #f)
        #f
        (let ((prev-outport (current-output-port)))
          (set-output-port outport)
          (let ((res (p)))
            (close-output-port outport)
            (set-output-port prev-outport)
            res)))))

(define (with-input-output-from-to-files si so p)
  (let ((inport (open-input-file si))
        (outport (open-input-file so)))
    (if (not (and inport outport))
        (begin
          (close-input-port inport)
          (close-output-port outport)
          #f)
        (let ((prev-inport (current-input-port))
              (prev-outport (current-output-port)))
          (set-input-port inport)
          (set-output-port outport)
          (let ((res (p)))
            (close-input-port inport)
            (close-output-port outport)
            (set-input-port prev-inport)
            (set-output-port prev-outport)
            res)))))

; Random number generator (maximum cycle)
(define *seed* 1)
(define (random-next)
  (let* ((a 16807) (m 2147483647) (q (quotient m a)) (r (modulo m a)))
    (set! *seed*
          (-   (* a (- *seed*
                       (* (quotient *seed* q) q)))
             (* (quotient *seed* q) r)))
    (if (< *seed* 0) (set! *seed* (+ *seed* m)))
    *seed*))

;; SRFI-0
;; COND-EXPAND
;; Implemented as a macro
(define *features* '(srfi-0))

(define-macro (cond-expand . cond-action-list)
              (cond-expand-runtime cond-action-list))

(define (cond-expand-runtime cond-action-list)
  (if (null? cond-action-list)
      #t
      (if (cond-eval (caar cond-action-list))
          `(begin ,@(cdar cond-action-list))
          (cond-expand-runtime (cdr cond-action-list)))))

(define (cond-eval-and cond-list)
  (foldr (lambda (x y) (and (cond-eval x) (cond-eval y))) #t cond-list))

(define (cond-eval-or cond-list)
  (foldr (lambda (x y) (or (cond-eval x) (cond-eval y))) #f cond-list))

(define (cond-eval condition)
  (cond
    ((symbol? condition)
     (if (member condition *features*) #t #f))
    ((eq? condition #t) #t)
    ((eq? condition #f) #f)
    (else (case (car condition)
            ((and) (cond-eval-and (cdr condition)))
            ((or) (cond-eval-or (cdr condition)))
            ((not) (if (not (null? (cddr condition)))
                       (error "cond-expand : 'not' takes 1 argument")
                       (not (cond-eval (cadr condition)))))
            (else (error "cond-expand : unknown operator" (car condition)))))))

(gc-verbose #f)

(define (add-feature x)
  (set! *features* (append *features* (list x))))


;; 
;; SRFI-1
;;
(add-feature 'srfi-1)

(define (xcons x y)
  (cons y x))

(define (cons* . args)
  (if (null? (cddr args))
      (cons (car args) (cadr args))
      (cons (car args) (apply cons* (cdr args)))))

(define (make-list n fill)
  (if (<= n 1)
      (cons fill NIL)
      (cons fill (make-list (1- n) fill))))

(define (list-tabulate n init-proc)
  (define (iter i init-proc)
    (if (<= (1+ i) n)
        (cons (init-proc i) NIL)
        (cons (init-proc i) (iter (1+ i) init-proc))))
  (iter 0 init-proc))

(define build-list list-tabulate)

(define (list-copy list)
  (if (null? list)
      '()
      (if (list? list)
          (cons (list-copy (car list)) (list-copy (cdr list)))
          list)))

(define (circular-list val . vals)
  (let ((circ (cons val vals)))
    (set-cdr! (last-pair circ) circ)
    circ))

(define (iota count start step)
  (let loop ((n 0) (r '()))
    (if (= n count)
        (reverse r)
        (loop (+ 1 n)
              (cons (+ start (* n step)) r)))))

(define (proper-list? x)
  (if (null? x)
      #t
      (if (pair? x)
          (if (pair? (cdr x))
              (if (null? (cddr x))
                  #t
                  (proper-list? (cdr x)))
              #f)
          #f)))

(define (dotted-list? x)
  (if (pair? x)
      (if (pair? (cdr x))
          (dotted-list? (cdr x))
          (if (not (null? (cdr x)))
              #t
              #f)
      #f)))

(define (circular-list? x)
  (define (iter l)
    (if (pair? l)
        (if (not (null? (cdr l)))
            (if (eqv? x (cdr l))
                #t
                (iter (cdr l)))
            #f)
        #f))
  (iter x))

(define null-list? null?)

(define (not-pair? x) (not (pair? x)))

(define list= equal?)

(define (first   x) (nth x 1))
(define (second  x) (nth x 2))
(define (third   x) (nth x 3))
(define (fourth  x) (nth x 4))
(define (fifth   x) (nth x 5))
(define (sixth   x) (nth x 6))
(define (seventh x) (nth x 7))
(define (eighth  x) (nth x 8))
(define (ninth   x) (nth x 9))
(define (tenth   x) (nth x 10))

(define (take x i)
  (if (or (null? x) (= i 0))
      NIL
      (cons (car x) (take (cdr x) (1- i)))))

(define (drop x i)
  (if (or (null? x) (= i 0))
      x
      (drop (cdr x) (1- i))))

(define (take-right x i)
  (drop x (- (length x) i)))

(define (drop-right x i)
  (take x (- (length x) i)))

(define (concatenate x)
  (apply append x))

(define (append-reverse head tail)
  (append (reverse head) tail))

(define (zip . lsts)
  (apply map list lsts))

(define (unzip lsts)
  (apply map list lsts))

(define (alist-cons key datum alist)
  (cons (cons key datum) alist))

(define (alist-copy alist)
  (map (lambda (el) (cons (car el) (cdr el))) alist))

(define (alist-delete key alist)
  (filter (lambda (x) (not (eqv? key (car x)))) alist))

;; 
;; SRFI-111
;;
(add-feature 'srfi-111)

(define %true-pair? pair?)
(define %box-flag 'box-flag)
(define (box? x) (and (%true-pair? x) (eq? (cdr x) %box-flag)))
(define (box  x) (cons x %box-flag))

(define (unbox x)
  (if (box? x)
      (car x)
      (error "Attempt to unbox non-box")))

(define (set-box! x v)
  (if (box? x)
      (set-car! x v)
      (error "Attempt to mutate non-box")))

(define pair?
  (lambda (x)
    (and (%true-pair? x) (not (box? x)))))

;;
;; SRFI-155
;; IMPLEMENTED AS LANGUAGE FEATURE
;;
(add-feature 'srfi-155)

(define (delay-force x) (delay (force x)))

(define (make-promise x)
  (if (promise? x)
      x
      (delay x)))