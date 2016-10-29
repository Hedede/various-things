charmap = zip [0..]['\0'..]
itoch i = head [ snd x | x <- charmap, (fst x) == i ]
chtoi c = head [ fst x | x <- charmap, (snd x) == c ]

n'a = chtoi 'a'
n'A = chtoi 'A'

toupper' :: Char -> Char
toupper' c = if (c `elem` ['a'..'z'])
             then itoch ((chtoi c) - n'a + n'A)
             else c

toupper :: [Char] -> [Char]
toupper str = [ toupper' c | c <- str ]


tolower' :: Char -> Char
tolower' c
         | c `elem` range = itoch ((chtoi c) - n'A + n'a)
         | otherwise = c
         where range = ['A'..'Z']

tolower :: String -> String
tolower ""      = ""
tolower (c:str) = (tolower' c):(tolower str)
