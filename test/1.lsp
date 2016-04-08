"a"

(set `defmacro (macro `(name args body) `(set name (macro args body))))
(set `defun (macro `(name args body) `(set name (lambda args body))))

defmacro

(+ 2 3)

(defun a (x y) (+ x y))
(defmacro b (x y) (+ x y))


(a (+ 2 3) 3)
(b (+ 2 3) 3)

(defun f (x) (lambda `(y) (`+ x `y)))



(set f1 (f `(+ 2.0 3.0)))
(set f2 (f 2) )

(f1 2)
(f1 3)
(f1 4)
(f1 5)
(f2 5)
(f2 6)