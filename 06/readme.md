# Description
An implementation of hack computer assembler, which is defined by the course 'nand2tetris'.
# Usage
`$runghc Assembler.hs source.asm > output.hack`

# Notes 
## Source indentation format
it's always fine to have spaces from both ends of a line, but don't expect it would work otherwise.

## Variable implementaion
Actually, could just assume everything is a variable and let label table overwrite it on combining, though it would result in unexpectable value of vaiables. This should be fine because programmers are not supposde to know what the underlying value of the variables are, but in the previous test it turns out that it won't work on given testing script(and I have no idea why), so in this version it guarantees that variables are defined in an increasing order based on their first occurence
