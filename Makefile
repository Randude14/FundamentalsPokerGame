# Project Team 002-5
# Shishir Acharya
# Randall Ferree
# Robbie Frazier

CXXFLAGS += -DASIO_STANDALONE -Wall -O0 -g -std=c++11
CPPFLAGS += -I./include -I./asio-1.12.2/include
GTKFLAGS = `/usr/bin/pkg-config gtkmm-3.0 --cflags --libs`

all: asio-1.12.2 poker src/json

asio-1.12.2:
	tar xzf asio-1.12.2.tar.gz

poker: main.o mainwin.o dealer.o player.o game.o deck.o card.o *.h
	$(CXX) $(CXXFLAGS) main.o mainwin.o dealer.o player.o game.o deck.o card.o $(GTKFLAGS) -o build/poker

main.o: src/main.cpp include/*.h
	$(CXX) $(CXXFLAGS) -c src/main.cpp $(GTKFLAGS) -o build/main.o

mainwin.o: src/mainwin.cpp include/*.h
	$(CXX) $(CXXFLAGS) -c src/mainwin.cpp $(GTKFLAGS) -o build/mainwin.o

dealer.o: src/dealer.cpp include/*.h
	$(CXX) $(CXXFLAGS) -c src/dealer.cpp -o build/dealer.o

player.o: src/player.cpp include/*.h
	$(CXX) $(CXXFLAGS) -c src/player.cpp -o build/player.o

game.o: src/game.cpp include/*.h
	$(CXX) $(CXXFLAGS) -c src/game.cpp -o build/game.o

deck.o: src/deck.cpp include/*.h
	$(CXX) $(CXXFLAGS) -c src/deck.cpp -o build/deck.o

card.o: src/card.cpp include/*.h
	$(CXX) $(CXXFLAGS) -c src/card.cpp -o build/card.o

clean:
	-rm -rf asio-1.12.2 build/* src/json

