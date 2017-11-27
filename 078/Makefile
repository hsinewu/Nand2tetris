CC=g++
TR=translator
COURSE=~/pj/nand2tetris_course
EMU=${COURSE}/tools/CPUEmulator.sh

D8=~/pj/nand2tetris_course/projects/08
DIR=${D8}/FunctionCalls/FibonacciElement

VMS7=${COURSE}/projects/07/*/*/*.vm
TSTS7=`grep -L "VME" ${COURSE}/projects/07/*/*/*.tst`
VMS8=${COURSE}/projects/08/ProgramFlow/*/*.vm
TSTS8=`grep -L "VME" ${COURSE}/projects/08/ProgramFlow/*/*.tst`

build:
	${CC} -std=c++11 main.cpp -o ${TR}

test: rm-asm test7 test8a test8b

rm-asm:
	rm -f ${COURSE}/projects/07/*/*/*.asm
	rm -f ${COURSE}/projects/08/*/*/*.asm

test7: build
	for vm in ${VMS7}; do ./${TR} --noinit $$vm ;done
	for tst in ${TSTS7}; do ${EMU} $$tst ;done

test8a: build
	for vm in ${VMS8}; do ./${TR} --noinit $$vm ;done
	for tst in ${TSTS8}; do ${EMU} $$tst ;done

test8b: testf testn tests

fib: build
	./${TR} ${DIR}/Main.vm
	./${TR} ${DIR}/Sys.vm
	cd ${DIR} && cat Main.asm Sys.asm > FibonacciElement.asm && rm Main.asm Sys.asm

testf: fib
	${EMU} ${DIR}/FibonacciElement.tst

testn: build
	./${TR} ${D8}/FunctionCalls/NestedCall/NestedCall.vm
	${EMU} ${D8}/FunctionCalls/NestedCall/NestedCall.tst

static: build
	./${TR} ${D8}/FunctionCalls/StaticsTest/Class1.vm
	./${TR} ${D8}/FunctionCalls/StaticsTest/Class2.vm
	./${TR} ${D8}/FunctionCalls/StaticsTest/Sys.vm
	cd ${D8}/FunctionCalls/StaticsTest && cat Class1.asm Class2.asm Sys.asm > StaticsTest.asm && rm Class1.asm Class2.asm Sys.asm

tests: static
	${EMU} ${D8}/FunctionCalls/StaticsTest/StaticsTest.tst
