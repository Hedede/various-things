module Triangular where

triangular :: Integer -> Integer
triangular n
 | n > 0 = n + triangular (n - 1)
 | otherwise = 0
