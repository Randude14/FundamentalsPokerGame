#include "client_communicator.h"
#include "player.h"
#include "poker_client.h"
#include <stdlib.h>

client_communicator::client_communicator(poker_client* c, char* argv[]) : client{c}
{
  for(int i = 0; i < MAX_PLAYERS; i++)
    players[i] = new Player();
  
  main_player = 0;
  num_players = 1;
}

client_communicator::~client_communicator() { close(); }

void client_communicator::close()
{
  for(int i = 0; i < MAX_PLAYERS; i++)
    if(players[i] != NULL)
    {
      delete players[i];
      players[i] = NULL;
    }
}