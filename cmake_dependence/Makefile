C_COMPLER ?= clang
CXX_COMPLER ?= clang++
OUTPUT_EXE ?= dependency_analyse.exe
CODE_NAME ?= dependency_analyse

CXX_STANDARD = c++20
FLAG = -O2 -Wall
PWD = $(shell pwd)

all: ${OUTPUT_EXE}

${OUTPUT_EXE}: ${CODE_NAME}.o
	${CXX_COMPLER} ${CODE_NAME}.o -o ${OUTPUT_EXE} -std=${CXX_STANDARD} ${FLAG}

${CODE_NAME}.o: ${CODE_NAME}.cpp
	${CXX_COMPLER} ${CODE_NAME}.cpp -c -o ${CODE_NAME}.o -std=${CXX_STANDARD} ${FLAG}

clean:
	rm -rf ${CODE_NAME}.o ${OUTPUT_EXE}