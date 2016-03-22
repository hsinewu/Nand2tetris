import System.Environment(getArgs)
import Data.Char(isSpace,isAlpha)
import Data.List(isPrefixOf)
import qualified Data.Map as M
import Numeric(showIntAtBase)
import Decode(decode)
import SymTab(buildSymTab)

despaces = filter (not.(=="")) . map trimLine where
    trimLine = reverse.(dropWhile isSpace).reverse.(dropWhile isSpace)    -- trim both ends

decomments ls = filter (not.isPrefixOf "//") ls

decodes ls = map decode ls

desymbols ls = map desym (delabels ls) where
    symtab = buildSymTab ls
    delabels = filter ((/='(').head)
    desym l@('@':sym)
        | isAlpha (head sym) = case M.lookup sym symtab of
            Just value -> "@" ++ show value
            Nothing -> error "Symbol not found"
        | otherwise = l -- A instrunction in literal
    desym l = l -- not A instrunction, leave it as it is

main = do
    fileName <- fmap head getArgs
    fmap assembly (readFile fileName) >>= putStrLn where
        assembly = unlines.decodes.desymbols.decomments.despaces.lines
