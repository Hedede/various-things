module SquareDigit where

digits :: (Integral i) => i -> [i]
digits 0 = []
digits x = reverse $ x `mod` 10 : (reverse $ digits (x `div` 10))

squareDigit :: (Show i, Read i, Integral i) => i -> i
squareDigit x = concat $ map (^2) (digits x)
        where
        concat = read . concatMap show
        --concat = read . foldr ((++) . show) []
