module Codewars.Kata.AllNoneAny where

import Prelude hiding (all, any)

all, none, any :: (a -> Bool) -> [a] -> Bool
all _ []     = True
all f (x:xs)
 | f x       = all f xs
 | otherwise = False

any _ []     = False
any f (x:xs) = (f x) || (any f xs)

none f xs = not (any f xs)
