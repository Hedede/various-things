fact 0 = 1
fact n = n * fact (n-1)


len1 :: (Num b) => [a] -> b
len1 xs = sum [ 1 | _ <- xs ]

len2 :: (Num b) => [a] -> b
len2 []     = 0
len2 (_:xs) = 1 + len2 xs

sum' xs = case xs of []     -> 0
                     (y:ys) -> y + sum ys

remove :: Eq a => a -> [a] -> [a]
remove y xs = filter (/=y) xs

unique [] = []
unique (x:xs) = x:remove x (unique xs)
