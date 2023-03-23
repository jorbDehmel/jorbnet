# NOTICE: This library depends on jgraph and SDL2
# This library is protected by a GPLv3 license

#####################################################

STEM = clang++ -pedantic -Wall -Werror -g
OBJECTS = build/npool.o build/resources.o build/networkSave.o build/image.o build/network.o build/sockstream.o

#####################################################

install: /usr/include/jorbnet

/usr/include/jorbnet:	/usr/include/jgraph libjorbnet.a /usr/bin/jorbnet-flags docs
	sudo rm -rf /usr/include/jorbnet
	sudo mkdir -p /usr/include/jorbnet
	sudo cp -rf src/*.hpp /usr/include/jorbnet
	sudo cp -f libjorbnet.a /usr/include/jorbnet
	
#####################################################

libjorbnet.a:	$(OBJECTS)
	ar -rsv libjorbnet.a $(OBJECTS)

/usr/include/jgraph:
	git clone https://github.com/jorbDehmel/jgraph
	$(MAKE) -C jgraph/jgraph
	make install
	cd ../..
	rm -rf jgraph

/usr/bin/jorbnet-flags:	jorbnet-flags.sh
	chmod +x jorbnet-flags.sh
	sudo cp jorbnet-flags.sh /usr/bin/jorbnet-flags

#####################################################

build/npool.o:	src/npool.hpp src/npool.cpp
	mkdir -p build
	$(STEM) -c -o build/npool.o src/npool.cpp -pthread

build/resources.o:	src/resources.hpp src/resources.cpp
	$(STEM) -c -o build/resources.o src/resources.cpp

build/networkSave.o:	src/networkSave.hpp src/networkSave.cpp
	$(STEM) -c -o build/networkSave.o src/networkSave.cpp

build/image.o:	src/image.hpp src/image.cpp
	$(STEM) -c -o build/image.o src/image.cpp

build/network.o:	src/network.hpp src/network.cpp
	$(STEM) -c -o build/network.o src/network.cpp

build/sockstream.o: src/misc/sockstream.hpp src/misc/sockstream.hpp
	$(STEM) -c -o build/sockstream.o src/misc/sockstream.cpp -pthread

#####################################################

clean:
	rm -rf build/*.o *.a

#####################################################

docs: docs/outline.pdf

docs/outline.pdf: docs/outline.tex
	pdflatex docs/outline.tex
	mv *.pdf docs
	rm -rf *.aux *.log

uninstall:
	sudo rm -f /usr/bin/jorbnet-flags
	sudo rm -rf /usr/include/jorbnet

#####################################################
