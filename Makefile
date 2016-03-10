
CXX=g++
# Hey!, I am comment number 2. I want to say that CFLAGS will be the
# options I'll pass to the compiler.
CFLAGS= -c
CXXFLAGS= -Wall -W
OUTPUT=TracerouteSieci
all: TracerouteSieci

TracerouteSieci:  helpers.o socket.o main.o packet.o
	$(CXX) $(CXXFLAGS) main.o  socket.o helpers.o packet.o -o $(OUTPUT)

main.o: main.cpp
	$(CXX) $(CFLAGS) main.cpp
helpers.o: helpers.cpp
	$(CXX) $(CFLAGS) helpers.cpp helpers.h
socket.o: socket.cpp
	$(CXX) $(CFLAGS) socket.cpp socket.h
packet.o: packet.cpp
	$(CXX) $(CFLAGS) packet.cpp packet.h

clean:
	rm -rf *.o
distclean:
	rm -rf *.o $(OUTPUT)
