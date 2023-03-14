# NOTICE: This library depends on jgraph and SDL2
# This library is protected by a GPLv3 license

#####################################################

STEM = clang++ -pedantic -Wall -Werror -g
SUFFIX = `jorbnet-flags`

#####################################################

all: main.out npool.out

#####################################################

main.out:	oopnn.cpp
	$(STEM) -o main.out oopnn.cpp `jorbnet-flags`

npool.out:	poolmain.cpp
	$(STEM) -o npool.out poolmain.cpp `jorbnet-flags`

#####################################################

clean:
	rm -rf *.out *.o

#####################################################
