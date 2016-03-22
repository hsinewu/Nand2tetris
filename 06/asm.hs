import System.Environment(getArgs)
import Data.Char(isSpace)

translate :: [String] -> [String]
translate [] = []
translate (l:ls) = case filter (not.isSpace) l of
	('/':'/':xs) -> translate ls
	xs -> xs:translate ls

main = do
	fileName <- fmap head getArgs
	contents <- readFile fileName
	putStrLn $ unlines (translate (lines contents))
