STEM = clang++ -pedantic -Wall -Werror -g

all: main.out outline.pdf

main.out:	oopnn.cpp resources.o networkSave.o
	$(STEM) resources.o networkSave.o -o main.out oopnn.cpp `jgraph-flags`

resources.o:	resources.hpp resources.cpp
	$(STEM) -c -o resources.o resources.cpp

networkSave.o:	networkSave.hpp networkSave.cpp
	$(STEM) -c -o networkSave.o networkSave.cpp

outline.pdf: outline.tex
	pdflatex outline.tex

clean:
	rm -rf *.out *.o *.pdf