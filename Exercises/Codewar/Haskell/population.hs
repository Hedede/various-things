module Codewars.G964.Arge where

nbYear :: Int -> Double -> Int -> Int -> Int
nbYear p0 percent aug p
 | pop >= p    = 1
 | otherwise   = 1 + nbYear pop percent aug p
 where pop = p0 + floor ((fromIntegral p0) * percent / 100.0) + aug
