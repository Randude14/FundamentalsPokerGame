#Randall Ferree 1000909515

#makefile for FundamentalsPokerGame
PLAYER = poker_client
SERVER = poker_server

# define sources and their object files
SOURCES = src/card.cpp src/game.cpp src/player.cpp
OBJECTS = $(SOURCES:.cpp=.o)

# define main files and their object files
PLAYER_MAIN = src/main.cpp
SERVER_MAIN = src/dealer.cpp
PLAYER_OBJECT = $(PLAYER_MAIN:.cpp=.o)
SERVER_OBJECT = $(SERVER_MAIN:.cpp=.o)

# define flags
GTKFLAGS = `/usr/bin/pkg-config gtkmm-3.0 --cflags --libs`
CFLAGS = -c -g -std=c++11 -Wall
CC = g++

.PHONY: all $(SERVER) $(PLAYER)

all: $(SERVER) $(PLAYER) $()

$(SERVER): $(SERVER_OBJECT) $(OBJECTS)
	$(CC) $^ -o $@ $(GTKFLAGS) 

$(PLAYER): $(PLAYER_OBJECT) $(OBJECTS) 
	$(CC) $^ -o $@ $(GTKFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $(GTKFLAGS) $< -o $@
	
clean :
	rm -r *.o $(PLAYER) $(SERVER)
