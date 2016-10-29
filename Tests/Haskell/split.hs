tolist :: [a] -> [[a]]
tolist [] = []
tolist xs = [xs]

split :: String -> Char -> [String]
split "" _ = []
split str c = let (first,rest) = span(/=c) str
              in tolist first ++ split (drop 1 rest) c
