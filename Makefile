STEM = clang++ -pedantic -Wall -Werror -g

all: main.out outline.pdf

main.out:	oopnn.cpp resources.hpp
	$(STEM) -o main.out oopnn.cpp

outline.pdf: outline.tex
	pdflatex outline.tex

clean:
	rm -rf *.out *.o *.pdf