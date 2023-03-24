# NOTICE: This library depends on jgraph and SDL2
# This library is protected by a GPLv3 license

#####################################################

STEM = clang++ -pedantic -Wall -Werror -g
SUFFIX = `jorbnet-flags`

#####################################################

all: main.out npool.out image.out imagepool.out

#####################################################

main.out:	network.cpp /usr/include/jorbnet
	$(STEM) -o main.out network.cpp `jorbnet-flags`

npool.out:	poolmain.cpp /usr/include/jorbnet
	$(STEM) -o npool.out poolmain.cpp `jorbnet-flags`

image.out:	image.cpp /usr/include/jorbnet
	$(STEM) -o image.out image.cpp `jorbnet-flags`

imagepool.out:	imagepool.cpp /usr/include/jorbnet
	$(STEM) -o imagepool.out imagepool.cpp `jorbnet-flags`

#####################################################

clean:
	rm -rf *.out *.o

#####################################################
