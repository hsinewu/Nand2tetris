module Main where

import System.Environment(getArgs)
import Data.Char(isSpace)
import Numeric(showIntAtBase)
import Sub

--translate A instruction
instruction ('@':xs) = '0':bin15 (read xs::Int)
--translate C instruction
instruction xs = xs


translate :: [String] -> [String]
translate [] = []
translate (l:ls) = case filter (not.isSpace) l of
	('/':'/':xs) -> translate ls
	xs -> instruction xs:translate ls

main = do
	fileName <- fmap head getArgs
	contents <- readFile fileName
	putStrLn $ unlines (translate (lines contents))
