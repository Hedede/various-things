#lang racket

(define  (GimmePi!  k  epsilon)
  (+
    (/
      (+
        (/  4  (+  (*  8  k)  1))
        (-  (/  2  (+  (*  8  k)  4)))
        (-  (/  1  (+  (*  8  k)  5)))
        (-  (/  1  (+  (*  8  k)  6))))
      (expt  16  k))  
    (if
      (>
        (/
          (+
            (/  4  (+  (*  8  k)  1))
            (-  (/  2  (+  (*  8  k)  4)))
            (-  (/  1  (+  (*  8  k)  5)))
            (-  (/  1  (+  (*  8  k)  6))))
          (expt  16  k))
        epsilon)
      (GimmePi!  (+  k  1)  epsilon)
      0
    )
  )
)

(define  (GimmePi!_kickstart  n)
  (GimmePi!  0  (*  9  (expt  0.1  n)))
)

(exact->inexact  (GimmePi!_kickstart  16))

3.1415926535897932384626433832795028841971693993751058
