#makefile for FundamentalsPokerGame

# exectuable names
CLIENT = poker_client
SERVER = poker_server

CHAT_CLIENT = chat_client
CHAT_SERVER = chat_server


# define sources and their object files
# NOTE: these source files should NOT have a main or make will fail on compile
SOURCES = src/card.cpp src/game.cpp src/player.cpp src/client_communicator.cpp
OBJECTS = $(SOURCES:.cpp=.o) 
HEADERS = -I./include/ -I./asio-1.12.2/include

# define main files and their object files
CLIENT_MAIN = src/poker_client.cpp
SERVER_MAIN = src/dealer.cpp
CLIENT_OBJECT = $(CLIENT_MAIN:.cpp=.o)
SERVER_OBJECT = $(SERVER_MAIN:.cpp=.o)

CHAT_CLIENT_MAIN = src/chat_client.cpp
CHAT_SERVER_MAIN = src/chat_server.cpp
CHAT_CLIENT_OBJECT = $(CHAT_CLIENT_MAIN:.cpp=.o)
CHAT_SERVER_OBJECT = $(CHAT_SERVER_MAIN:.cpp=.o)

# define flags
CC = g++
CPPFLAGS += -c -g -std=c++11 -Wall
CXXFLAGS += -Wall -O0 -g -std=c++11
GTKFLAGS = `/usr/bin/pkg-config gtkmm-3.0 --cflags --libs`
THREADFLAGS = -lpthread -pthread

all: asio-1.12.2 $(SERVER) $(CLIENT) $(CHAT_SERVER) $(CHAT_CLIENT)

# unpack asio
asio-1.12.2:
	tar xzf asio-1.12.2.tar.gz

# build exectuables
$(SERVER): $(SERVER_OBJECT) $(OBJECTS)
	$(CC) $^ -o $@ $(GTKFLAGS) $(THREADFLAGS)

$(CLIENT): $(CLIENT_OBJECT) $(OBJECTS)
	$(CC) $^ -o $@ $(GTKFLAGS) $(THREADFLAGS)
	
$(CHAT_CLIENT): $(CHAT_CLIENT_OBJECT)
	$(CC) $^ -o $@ $(GTKFLAGS) $(THREADFLAGS)
	
$(CHAT_SERVER): $(CHAT_SERVER_OBJECT)
	$(CC) $^ -o $@ $(GTKFLAGS) $(THREADFLAGS)

# build objects from source files
.cpp.o: $(HEADERS)
	$(CC) $(HEADERS) $(CPPFLAGS) $(GTKFLAGS) $(THREADFLAGS) $< -o $@

# remove executables and asio lib
clean :
	-rm -rf asio-1.12.2
	-rm -f $(CLIENT)
	-rm -f $(SERVER)
	-rm -f $(CHAT_CLIENT)
	-rm -f $(CHAT_SERVER)
	-rm -f src/*.o
