// header guard
#ifndef CLIENT_COMMUNICATOR_H
#define CLIENT_COMMUNICATOR_H

#include "player.h"
#include "dealer.h"
#include "poker_client.h"
#include <iostream>


class poker_client;

// poker_client will have a client_communicator object that will connect to the server
// and pull info to the client that will require input (if it's their turn) that the
// client_communicator will wait for and push to the server

class client_communicator
{
  public:
    friend class poker_client;
    client_communicator(poker_client* client, char* argv[]);
    virtual ~client_communicator();
  
    void close();
  
  private:
    Player* players[MAX_PLAYERS];      // keep track of player info
    int num_players;
    int main_player;                   // index of the player this client is tied to
    
    std::string turn_status;           // message indicating who the dealer is waiting on
    long pot;                          // current pot of the game
    long current_bet;                  // current bet of the game, can be 0 for "check"    
    poker_client* client;              // client this communicator is tied to
};

#endif