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

buildSymTab ls = _buildSymTab ls 0 [] hackBuiltInSymbol where
    _buildSymTab [] _ varList symtab = combinedTab where
        varTab = M.fromList $ zip (reverse varList) [16..]
        combinedTab = M.union symtab varTab
    _buildSymTab (l:ll) lineCount varList symtab = case l of
        --it's a new label
        '(':xs ->  _buildSymTab ll lineCount newVarList newTab where
            label = init xs -- take out ')'
            newTab = M.insert label lineCount symtab
            newVarList = filter (/=label) varList
        --might be a new variable
        '@':var -> if isVariable && isNew then insertBuild else skipBuild where
            isVariable = isAlpha (head var)
            isNew = notElem var varList && isNothing (M.lookup var symtab)
            insertBuild = _buildSymTab ll (lineCount+1) (var:varList) symtab
            skipBuild = _buildSymTab ll (lineCount+1) varList symtab
        otherwise -> _buildSymTab ll (lineCount+1) varList symtab
