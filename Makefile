# NOTICE: This library depends on jgraph and SDL2
# This library is protected by a GPLv3 license

#####################################################

STEM = clang++ -pedantic -Wall -Werror -g

#####################################################

install: /usr/include/jgraph libjorbnet.a /usr/bin/jorbnet-flags docs
	sudo mkdir -p /usr/include/jorbnet/src
	sudo cp -rf src/*.hpp /usr/include/jorbnet/src
	sudo cp -f libjorbnet.a /usr/include/jorbnet
	
#####################################################

libjorbnet.a: build/npool.o build/resources.o build/networkSave.o
	ar -rsv libjorbnet.a build/npool.o build/resources.o build/networkSave.o	

/usr/include/jgraph:
	git clone https://github.com/jorbDehmel/jgraph
	cd jgraph/jgraph
	make install
	cd ../..
	rm -rf jgraph

/usr/bin/jorbnet-flags:	jorbnet-flags.sh
	chmod +x jorbnet-flags.sh
	sudo cp jorbnet-flags.sh /usr/bin/jorbnet-flags

#####################################################

build/:
	mkdir -p build

#####################################################

build/npool.o:	build/ src/npool.hpp src/npool.cpp
	$(STEM) -c -o build/npool.o src/npool.cpp -pthread

build/resources.o:	build/ src/resources.hpp src/resources.cpp
	$(STEM) -c -o build/resources.o src/resources.cpp

build/networkSave.o:	build/ src/networkSave.hpp src/networkSave.cpp
	$(STEM) -c -o build/networkSave.o src/networkSave.cpp

#####################################################

clean:
	rm -rf build/*.o *.a

#####################################################

docs:
	pdflatex docs/outline.tex

uninstall:
	sudo rm -f /usr/bin/jorbnet-flags
	sudo rm -rf /usr/include/jorbnet

#####################################################
