CLIENT_OBJS = objs/client.o
SERVER_OBJS = objs/server.o

CLIENT_OUT = -o bin/client
SERVER_OUT = -o bin/server

ARGS = -lenet -lGL -lGLU `sdl-config --cflags --libs`

CLIENT_FILES = src/client/client.cpp
SERVER_FILES = src/server/server.cpp
MODEL_FILES = src/model/modelFactory.cpp src/model/pyramid.cpp src/model/cube.cpp

all: client server

client: 
	g++ $(CLIENT_FILES) $(MODEL_FILES) $(CLIENT_OUT) $(ARGS)

server: 
	g++ $(SERVER_FILES) $(MODEL_FILES) $(SERVER_OUT) $(ARGS)

clean:
	@rm bin/client
	@rm bin/server
	@echo removed binaries and object files

objs/client.o: $(CLIENT_FILES)
	g++ -o objs/client.o $(CLIENT_FILES) $(MODEL_FILES) $(ARGS)
