module Main where

import System.Environment(getArgs)
import Data.Char(isSpace)
import Data.List(isPrefixOf)
import Numeric(showIntAtBase)
import Decode

--translate A instruction
instruction ('@':xs) = '0' : bin15 (read xs::Int)
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

despace ls = map (trimLine) ls where
	trimLine l = (reverse.(dropWhile isSpace).reverse.(dropWhile isSpace)) l

decomment ls = filter (not.isPrefixOf "//") ls

decode [] = []
decode (l:ls) = case filter (not.isSpace) l of
	"" -> decode ls
	xs -> instruction xs:decode ls

main = do
	fileName <- fmap head getArgs
	contents <- readFile fileName
	putStrLn $ unlines (assembly (lines contents)) where
		assembly :: [String] -> [String]
		assembly = decode.decomment.despace
