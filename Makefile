#Randall Ferree 1000909515

#makefile for FundamentalsPokerGame
CLIENT = poker_client
SERVER = poker_server

# define sources and their object files
SOURCES = src/card.cpp src/game.cpp src/player.cpp
OBJECTS = $(SOURCES:.cpp=.o)

# define main files and their object files
CLIENT_MAIN = src/PokerGuiClient.cpp
SERVER_MAIN = src/dealer.cpp
CLIENT_OBJECT = $(CLIENT_MAIN:.cpp=.o)
SERVER_OBJECT = $(SERVER_MAIN:.cpp=.o)

# define flags
GTKFLAGS = `/usr/bin/pkg-config gtkmm-3.0 --cflags --libs`
CFLAGS = -c -g -std=c++11 -Wall
CC = g++

.PHONY: all $(SERVER) $(CLIENT)

all: $(SERVER) $(CLIENT)

$(SERVER): $(SERVER_OBJECT) $(OBJECTS)
	$(CC) $^ -o $@ $(GTKFLAGS) 

$(CLIENT): $(CLIENT_OBJECT) $(OBJECTS) 
	$(CC) $^ -o $@ $(GTKFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $(GTKFLAGS) $< -o $@
	
clean :
	rm -r src/*.o $(CLIENT) $(SERVER)
