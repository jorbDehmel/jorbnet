# NOTICE: This library depends on jgraph and SDL2
# This library is protected by a GPLv3 license

#####################################################

STEM = clang++ -pedantic -Wall -Werror -g
OBJECTS = build/npool.o build/networkSave.o build/image.o build/network.o


#####################################################

install: /usr/include/jorbnet

/usr/include/jorbnet:	libjorbnet.a /usr/bin/jorbnet-flags docs
	sudo rm -rf /usr/include/jorbnet
	sudo mkdir -p /usr/include/jorbnet
	sudo cp -rf src/*.hpp /usr/include/jorbnet
	sudo cp -f libjorbnet.a /usr/include/jorbnet
	
#####################################################

libjorbnet.a:	$(OBJECTS)
	ar -rsv libjorbnet.a $(OBJECTS)

/usr/bin/jorbnet-flags:	jorbnet-flags.sh
	chmod +x jorbnet-flags.sh
	sudo cp jorbnet-flags.sh /usr/bin/jorbnet-flags

#####################################################

build/npool.o:	src/npool.hpp src/npool.cpp
	mkdir -p build
	$(STEM) -c -o build/npool.o src/npool.cpp -pthread

build/networkSave.o:	src/networkSave.hpp src/networkSave.cpp
	$(STEM) -c -o build/networkSave.o src/networkSave.cpp

build/image.o:	src/image.hpp src/image.cpp
	$(STEM) -c -o build/image.o src/image.cpp

build/network.o:	src/network.hpp src/network.cpp
	$(STEM) -c -o build/network.o src/network.cpp

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

reinstall:
	$(MAKE) uninstall
	$(MAKE) clean
	$(MAKE) install

#####################################################
