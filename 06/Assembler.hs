module Main where

import System.Environment(getArgs)
import Data.Char(isSpace)
import Data.List(isPrefixOf)
import Numeric(showIntAtBase)
import Decode

despaces = filter (not.(=="")) . map trimLine where
	trimLine = (reverse.(dropWhile isSpace).reverse.(dropWhile isSpace))

decomments ls = filter (not.isPrefixOf "//") ls

decodes ls = map decode ls

main = do
	fileName <- fmap head getArgs
	fmap (unlines.assembly.lines) (readFile fileName) >>= putStrLn where
		assembly :: [String] -> [String]
		assembly = decodes.decomments.despaces
