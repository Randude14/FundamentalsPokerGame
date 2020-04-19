#makefile for FundamentalsPokerGame

# exectuable names
CLIENT = poker_client
SERVER = poker_server

DEALER_TEST = dealer_test

# define sources and their object files
# NOTE: these source files should NOT have a main or make will fail on compile
SOURCES = src/card.cpp src/player.cpp src/game.cpp src/dealer.cpp
OBJECTS = $(SOURCES:.cpp=.o) 
HEADERS = -I./include/ -I./asio-1.12.2/include

CLIENT_DEPEND = src/play_window.cpp src/client_communicator.cpp $(SOURCES)
CLIENT_DEPEND_OBJECTS = $(CLIENT_DEPEND:.cpp=.o) 

# define main files and their object files
CLIENT_MAIN = src/poker_client.cpp
SERVER_MAIN = src/chat_server.cpp
CLIENT_OBJECT = $(CLIENT_MAIN:.cpp=.o)
SERVER_OBJECT = $(SERVER_MAIN:.cpp=.o)

DEALER_TEST_MAIN = src/dealer_test.cpp
DEALER_MAIN_OBJECT = $(DEALER_TEST_MAIN:.cpp=.o)

TEST_MAIN = src/hand_comp_test.cpp
TEST_OBJECT = $(TEST_MAIN:.cpp=.o)

# define flags
CC = g++
CPPFLAGS += -c -g -std=c++11 -Wall
CXXFLAGS += -Wall -O0 -g -std=c++11
GTKFLAGS = `/usr/bin/pkg-config gtkmm-3.0 --cflags --libs`
THREADFLAGS = -lpthread -pthread

all: asio-1.12.2 $(SERVER) $(CLIENT) $(DEALER_TEST)

# unpack asio
asio-1.12.2:
	tar xzf asio-1.12.2.tar.gz

$(DEALER_TEST): $(DEALER_MAIN_OBJECT) $(OBJECTS)
	$(CC) $^ -o $@ $(GTKFLAGS) $(THREADFLAGS)
  
# build exectuables
$(SERVER): $(SERVER_OBJECT) $(OBJECTS)
	$(CC) $^ -o $@ $(GTKFLAGS) $(THREADFLAGS)

$(CLIENT): $(CLIENT_OBJECT) $(CLIENT_DEPEND_OBJECTS)
	$(CC) $^ -o $@ $(GTKFLAGS) $(THREADFLAGS)
  
  
#run commands
server: $(SERVER)
	./$(SERVER) 9000
    
client: $(CLIENT)
	./$(CLIENT)
  

# build objects from source files
.cpp.o: $(HEADERS)
	$(CC) $(HEADERS) $(CPPFLAGS) $(GTKFLAGS) $(THREADFLAGS) $< -o $@
  
  
# remove executables and asio lib
clean :
	-rm -rf asio-1.12.2
	-rm -f $(CLIENT)
	-rm -f $(SERVER)
	-rm -f $(CHAT_SERVER)
	-rm -f src/*.o
