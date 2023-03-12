STEM = clang++ -pedantic -Wall -Werror -g

all: main.out outline.pdf

main.out:	oopnn.cpp resources.o
	$(STEM) resources.o -o main.out oopnn.cpp `jgraph-flags`

resources.o:	resources.hpp resources.cpp
	$(STEM) -c -o resources.o resources.cpp

outline.pdf: outline.tex
	pdflatex outline.tex

clean:
	rm -rf *.out *.o *.pdf