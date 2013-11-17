(define number? integer?)

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

(define (length items)
  (define (iter a count)
    (if (null? a)
        count
        (iter (cdr a) (+ 1 count))))
  (iter items 0))

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

(define (map proc items)
    (if (null? items)
        '()
        (cons (proc (car items))
              (map proc (cdr items)))))

(define (for-each f l)
  (if (null? l)
      #t
      (begin
        (f (car l))
        (for-each f (cdr l)))))

(define (not x)
  (if x #f #t))

(define (last l)
	(if (null? (cdr l))
		(car l)
		(last (cdr l))))

(define (head l)
	(if (pair? l)
		(car l)
		'()))

(define (tail l)
	(if (pair? l)
		(cdr l)
		'()))

(define (range start end)
  (if (< start end)
    (cons start (range (+ start 1) end))
    '()))

(define (zip f list1 list2)
  (if (null? list2)
    '()
    (cons 
      (f (car list1) (car list2)) 
      (zip f (cdr list1) (cdr list2)))))

(define cowsay "cowsay")
(define fortune "fortune")
(define (prg name)
  (if (symbol? name)
    (symbol->string (quote name))
    ""))

(define (pipe str1 str2)
  (add-str str1 " | " str2))

'stdlib-loaded