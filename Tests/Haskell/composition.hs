oddSquareSum :: Integer -> Integer
oddSquareSum n =
    let oddSquares = filter odd . map (^2)
        belowLimit = takeWhile (<n)
    in  sum . belowLimit . oddSquares $ [1..]
