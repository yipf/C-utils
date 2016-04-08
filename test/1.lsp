"a"

(set `defmacro (macro `(name args body) `(set name (macro args body))))
(set `defun (macro `(name args body) `(set name (lambda args body))))

defmacro

(+ 2 3)

(defun a (x y) (+ x y))

a

(a 2 3)