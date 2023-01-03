
NAME = getchan
OUTPUT = getchan
SOURCES = getchan.c
COMPILER = cc
INSTALL_DIR = /usr/bin/

all: getchan


getchan: $(SOURCES)
	$(COMPILER) $(SOURCES) -lcurl -o $(OUTPUT)

install:
	cp ./$(OUTPUT) $(INSTALL_DIR)

clean: 
	rm -f ./$(OUTPUT)


