#include "client_communicator.h"

#include <stdlib.h>
#include <chrono>

client_communicator::client_communicator(poker_client* c, char* argv[]) : client{c}
{
  for(int i = 0; i < MAX_PLAYERS; i++)
    players[i] = new Player();
  
  main_player = 0;
  num_players = 1;
  
  comm_t = new std::thread( &client_communicator::asio_run , this, argv );
}

client_communicator::~client_communicator() { }

void client_communicator::close()
{
  
  // close chat_client
  if(comm)
  {
    try
    {
      comm->close();
      comm_t->join();
    }
    catch(std::exception say)
    {
      std::cout << "Exception: " << say.what() << std::endl;
    }
    delete comm;
    delete comm_t;
    comm = nullptr;
    comm_t = nullptr;
  }
  
  for(int i = 0; i < MAX_PLAYERS; i++)
  {
    if(players[i] != nullptr)
    {
      delete players[i];
      players[i] = nullptr;
    }
  }
}

void client_communicator::asio_run(char* argv[])
{
  try
  {
    // create context and resolver
    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(argv[1], argv[2]);
    
    comm = new chat_client(this, io_context, endpoints);
    // run the io context
    io_context.run();
  }
  catch(std::exception say)
  {
    std::cout << "Exception: " << say.what() << std::endl;
  }
}

void client_communicator::message_readin(std::string message)
{
  std::cout << message << std::endl;
}

void client_communicator::write_message(std::string action)
{
  
  //Player* main_player = players[main_player];
  
  nlohmann::json to_dealer;
  to_dealer["action"] = action;
  
  // this field is important....this tells the dealer who sent this action
  to_dealer["table_pos"] = 0;//main_player->table_position;
  
  to_dealer["test"] = "Some message...";
  std::string t = to_dealer.dump();
  
  chat_message msg;
  msg.body_length(t.size());
  std::memcpy(msg.body(), t.c_str() , msg.body_length());
  msg.encode_header();
  
  try
  {
    comm->write(msg);
  }
  catch(std::exception say)
  {
    std::cout << "Exception: " << say.what() << std::endl;
  }
}