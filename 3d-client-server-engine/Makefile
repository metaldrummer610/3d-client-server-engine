CLIENT= client.o model.o
SERVER= server.o model.o

client: $(CLIENT)
	g++ $(CLIENT) -o bin/client.out -lenet

server: $(SERVER)
	g++ $(SERVER) -o bin/server.out

client.o: src/client/*.cpp

server.o: src/server/*.cpp

model.o: src/model/*.cpp
