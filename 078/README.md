VM translator
===
### About
This is an implementation of vm translator that will translate vm code to hack assembly, described in the nand2tetris course.

### How to use it
#### Compile
Compile with `-std=c++11` flag, or just
`$ make`

#### Run
`./translator [option] [input]`

option is `--noinit` which forbid auto generated init code, specified in the course.

input is either a vm file or a directory contains vm files.

example:
`./translator myfile.vm` will output myfile.asm that contains hack assembly.
`./translator mydir` TODO

#### Test
1. Download course material from nand2tetris.org (which includes test scripts and simulator)
2. Set `COURSE` variable in Makefile to that folder
3. Run `make test -s`

### Notes
There are some label names `VM_XX` being used for implementation purpose.