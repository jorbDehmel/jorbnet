STEM = clang++ -pedantic -Wall -Werror -g

all: main.out npool.out outline.pdf

main.out:	oopnn.cpp resources.o networkSave.o
	$(STEM) resources.o networkSave.o -o main.out oopnn.cpp `jgraph-flags`

npool.out:	poolmain.cpp npool.o resources.o networkSave.o
	$(STEM) -o npool.out poolmain.cpp npool.o networkSave.o resources.o `jgraph-flags`

npool.o:	npool.hpp npool.cpp
	$(STEM) -c -o npool.o npool.cpp -pthread

resources.o:	resources.hpp resources.cpp
	$(STEM) -c -o resources.o resources.cpp

networkSave.o:	networkSave.hpp networkSave.cpp
	$(STEM) -c -o networkSave.o networkSave.cpp

outline.pdf: outline.tex
	pdflatex outline.tex

clean:
	rm -rf *.out *.o *.pdf