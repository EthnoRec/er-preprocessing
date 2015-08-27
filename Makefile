CC=g++
CFLAGS=-std=c++11 -I /usr/include/jsoncpp -g
LDFLAGS=-ljsoncpp -lmicrohttpd -ljsonrpccpp-common -ljsonrpccpp-server `pkg-config --libs opencv`

all:
	$(CC) $(CFLAGS) preprocessing.cpp RPCServer.cpp Image.cpp $(LDFLAGS) -o preprocessing
