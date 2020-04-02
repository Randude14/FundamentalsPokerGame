#include "client_communicator.h"
#include "player.h"
#include <stdlib.h>

client_communicator::client_communicator()
{
  // init info
  info = new client_info();
  info->player = new Player();
  for(int i = 0; i < MAX_OPPONENTS; i++)
    info->opponents[i] = new Player();
}

client_communicator::~client_communicator() { }

void client_communicator::close()
{
  // free memory
  delete info->player;
  for(int i = 0; i < MAX_OPPONENTS; i++)
    delete info->opponents[i];
  delete info;
}

client_info* client_communicator::get_info()
{
  return info;
}