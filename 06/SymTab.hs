module SymTab (
    buildSymTab
)where

import qualified Data.Map as M
import Data.Char(isAlpha)
import Data.Maybe(isNothing)

hackBuiltInSymbol = M.fromList
    [ ("SP",0), ("LCL",1), ("ARG",2), ("THIS",3), ("THAT",4)
    , ("R0",0), ("R1",1), ("R2",2), ("R3",3), ("R4",4), ("R5",5)
    , ("R6",6), ("R7",7), ("R8",8), ("R9",9), ("R10",10), ("R11",11)
    , ("R12",12), ("R13",13), ("R14",14), ("R15",15)
    , ("SCREEN",16384), ("KBD",24576) ]

--on seeing variable
--    lookup table
--        exist -> skip
--        nope -> assume it's new variable
--on seeing label
--    overwrite to table

-- varibles defined to be start from 16
buildSymTab ls = _buildSymTab ls 0 16 hackBuiltInSymbol where
    _buildSymTab [] _ _ symtab = symtab
    _buildSymTab (l:ll) lineCount varCount symtab = case l of
        --it's a new label
        '(':xs ->  _buildSymTab ll lineCount varCount insertedTab where
            label = init xs -- take out ')'
            insertedTab = M.insert label lineCount symtab
        --might be a new variable
        '@':xs -> if isVariable && notInTable then insertedTab else moreSymTab where
            isVariable = isAlpha (head xs)
            notInTable = isNothing (M.lookup xs symtab)
            insertedTab = _buildSymTab ll (lineCount+1) (varCount+1) (M.insert xs varCount symtab)
            moreSymTab = _buildSymTab ll (lineCount+1) (varCount) symtab
        otherwise -> _buildSymTab ll (lineCount+1) varCount symtab

main = putStrLn "sym"