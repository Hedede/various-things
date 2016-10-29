split :: String -> Char -> [String]
split "" _ = []
split str c = let (first,rest) = span(/=c) str
              in tolist first ++ split (drop 1 rest) c
              where tolist xs = case xs of [] -> []
                                           xs -> [xs]
