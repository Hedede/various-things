#lang racket

(define (quickpower a n) 
  (if (integer? n)
    (if (= a 0)
      0
      (if (or (= n 0) (= a 1))
        1
        (if (< n 0)
          (quickpower (/ 1 a) (- n))
          (if (even? n)
            (quickpower (* a a) (/ n 2))
            (* a (quickpower (* a a) (/ (- n 1) 2)))
          )
	)
      )
    )
    "Power must be an integer"
  )
)

(quickpower 3 4)
(quickpower 5 -5)
