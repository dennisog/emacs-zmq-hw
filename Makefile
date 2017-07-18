EMACS_SRC_DIR=../../emacs-build/src/emacs-git
CC=c++
FLAGS=-Wall -g -fPIC -I$(EMACS_SRC_DIR)/src -std=gnu++14
LIBS=-lzmq -lboost_system
MODNAME=emacs-zmq-hw
SOURCES=$(MODNAME).cc connection.cc

all: server module

server:
	$(CC) hwserver.cc $(FLAGS) -o hwserver $(LIBS)

module:
	$(CC) $(SOURCES) -shared $(FLAGS) -o $(MODNAME).so $(LIBS)

clean:
	rm -rf *.o hwserver $(MODNAME).so
