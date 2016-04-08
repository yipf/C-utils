
(eval `(sin 2.0))

; (gc)


(car `(1 2 3))
car


; (set `a (lambda (x) (sin x)))

; a

; (a (sin 1.0))

; (set `defun (macro (id args body) (set `id (lambda args body))))
; (set `defmacro (macro (id args body) (set `id (macro args body))))

; (defmacro a (x) (sin x))

; (defun c (x y) (cons x y))

; (a  (sin 1.0))

; (c `1.0 `(2.0 3.0))

(car `(a b c))


; (list a 1 `(1 2))

; (set `defun  (macro (id args body) (set `id (lambda args body))))

; defun

; (defun a (x) (sin x) )

; ((macro (id args body) (set `id (lambda args body))) a (x) (sin x))

; a

; (a 1.0)

; (set `b (lambda `(x) `(sin (sin x))))
; (set `a (macro (x) (sin (sin x))))
"eval"
; (eval `(sin 2.0))
"lambda"
; ((lambda (x) (sin x) ) (sin 2.0))
"macro"
; ((macro (x) (sin x) ) (sin 2.0))

; (set `x 3.0)
"a"
; (a 1.0)
; (sin (sin 2.0))
; (a 2.0)

; x

"map"
; (map sin `(0.5 1 2))
; `a
; a


; (label b a)
True

False
; `a b "a"

; (a (2 3 4))
; (b (2 3 4))

