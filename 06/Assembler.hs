module Main where

import System.Environment(getArgs)
import Data.Char(isSpace)
import Numeric(showIntAtBase)
import Decode

--translate A instruction
instruction ('@':xs) = '0':bin15 (read xs::Int)
--translate C instruction
instruction xs = "111" ++ comp ++ dest ++ jump where
	hasDest = elem '=' xs
	hasJump = elem ';' xs
	comp = decodeComp compCode where
		compCode = takeWhile (/= ';') skipDest
		skipDest = if hasDest then tail (dropWhile (/= '=') xs) else xs
	dest = decodeDest destCode where
		destCode = if hasDest then takeWhile (/= '=') xs else ""
	jump = decodeJump jumpCode where
		jumpCode = if hasJump then tail (dropWhile (/= ';') xs) else ""

translate :: [String] -> [String]
translate [] = []
translate (l:ls) = case filter (not.isSpace) l of
	"" -> translate ls
	('/':'/':xs) -> translate ls
	xs -> instruction xs:translate ls

main = do
	fileName <- fmap head getArgs
	contents <- readFile fileName
	putStrLn $ unlines (translate (lines contents))
