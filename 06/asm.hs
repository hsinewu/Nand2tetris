import System.Environment(getArgs)

translate :: [String] -> [String]
translate [] = []
translate (l:lines) = case l of
	('/':'/':xs) -> translate lines
	otherwise -> l:translate lines

main = do
	fileName <- fmap head getArgs
	contents <- readFile fileName
	putStrLn $ unlines (translate (lines contents))
