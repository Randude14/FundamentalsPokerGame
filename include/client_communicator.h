// header guard
#ifndef CLIENT_COMMUNICATOR_H
#define CLIENT_COMMUNICATOR_H

#include "player.h"
#include <iostream>


// max players that the client will play against
#define MAX_OPPONENTS 4


// this is used to communicate info to the client
class client_info
{
  public:
    Player* player;
    Player* opponents[MAX_OPPONENTS];
    std::string turn_status;
    long pot;
    long current_bet;
};

// poker_client will have a client_communicator object that will connect to the server
// and pull info to the client that will require input (if it's their turn) that the
// client_communicator will wait for and push to the server

class client_communicator
{
  public:
    client_communicator();
    
    ~client_communicator();
  
    void close();
    
    client_info* get_info();
  
  private:
    client_info *info; // info that will be updated
    
  
};

#endif