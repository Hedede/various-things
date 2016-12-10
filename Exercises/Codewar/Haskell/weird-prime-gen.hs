module Codewars.G964.Weirdgenprime where
import Data.List

as = 7 : zipWith term [2..] as
     where
      term x y = (gcd x y) + y

gs = 1 : zipWith (-) (tail as) as

primes = nub $ filter (/= 1) gs

countOnes :: Int -> Int
countOnes n = sum $ filter (== 1) (take n gs)

maxPn :: Int -> Integer
maxPn n = toInteger $ maximum $ (take n primes)

anOverAverage :: Int -> Int
anOverAverage n = 3 -- see weird-prime-gen.txt


-- anOverAverage --
asgs = zip as gs

replaceNotOnes :: [(Int,Int)] -> [(Int,Int)]
replaceNotOnes l = zipWith replace l [1..]
     where
           replace (x, 1) n = (x, 0)
           replace (x, y) n = (x, n)

unzipWith :: (a -> b -> c) -> [(a,b)] -> [c]
unzipWith f = map (\x -> f (fst x) (snd x))

anOver = unzipWith div $ filter (\x -> (snd x) /= 0) $ replaceNotOnes $ asgs

anOverAverageSlow :: Int -> Int
anOverAverageSlow n = (sum (take n anOver)) `div` n
