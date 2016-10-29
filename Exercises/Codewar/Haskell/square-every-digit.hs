module SquareDigit where

digits :: Int -> [Int]
digits 0 = []
digits x = digits (x `div` 10) ++ [x `mod` 10]

squareDigit :: Int -> Int
squareDigit x
  | x <  0 = negate (squareDigit(negate x))
  | x == 0 = x
  | x >  0 = let
               square = map (^2) . digits
               concat = read . concatMap show
             in
               (concat . square) x
