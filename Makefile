STEM = clang++ -pedantic -Wall -Werror

main.out:	oopnn.cpp
	$(STEM) -o main.out oopnn.cpp

outline.pdf: outline.tex
	pdflatex outline.tex