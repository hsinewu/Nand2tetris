import System.Environment(getArgs)
import Data.Char(isSpace,isAlpha)
import Data.List(isPrefixOf)
import qualified Data.Map as M
import Numeric(showIntAtBase)
import Decode
import SymTab

despaces = filter (not.(=="")) . map trimLine where
    trimLine = (reverse.(dropWhile isSpace).reverse.(dropWhile isSpace))

decomments ls = filter (not.isPrefixOf "//") ls

decodes ls = map decode ls

desymbols ls = map desym ls where
    symtab = buildSymTab ls
    desym l@('@':sym)
        | isAlpha (head sym) = case M.lookup sym symtab of
            Just value -> "@" ++ show value
            Nothing -> error "Symbol not found"
        | otherwise = l
    desym l = l

main = do
    fileName <- fmap head getArgs
    fmap assembly (readFile fileName) >>= putStrLn where
        assembly = unlines.decodes.desymbols.decomments.despaces.lines
