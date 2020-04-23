#include "client_communicator.h"
#include <boost/lexical_cast.hpp>

#include <stdlib.h>
#include <chrono>

client_communicator::client_communicator(poker_client* c, 
  std::string host, std::string port) : client{c}
{
  failed = false;
  comm_t = new std::thread( &client_communicator::asio_run , this, host, port );
}

client_communicator::~client_communicator() { }

void client_communicator::close()
{
  
  // close chat_client
  if(comm)
  {
    try
    {
      // don't close io_context if connection failed
      comm->close();
      comm_t->join();
    }
    catch(std::exception& say)
    {
      std::cout << "Exception: " << say.what() << std::endl;
    }
    
    delete comm;
    delete comm_t;
    comm = nullptr;
    comm_t = nullptr;
  }
}

void client_communicator::asio_run(std::string host, std::string port)
{
  try
  {
    // create context and resolver
    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(host, port);
    
    comm = new chat_client(this, io_context, endpoints);
    // run the io context
    io_context.run();
  }
  catch(std::exception& say)
  {
    failed = true;
    std::cout << "Exception: " << say.what() << std::endl;
    std::cout << "Connection failed." <<  std::endl;
    
    client->connection_failed();
  }
}

void client_communicator::message_readin(std::string message)
{
  nlohmann::json to_player = nlohmann::json::parse( message );
  
  client->num_players = to_player["player_total"];
  auto players = client->players;
  
  for(int i = 0; i < client->num_players; i++)
  {
    Player* player = players[i];
    player->name =   to_player["players"][i]["name"];
    std::string uuid_string = to_player["players"][i]["uuid"];
    player->UUID =   boost::lexical_cast<boost::uuids::uuid>(uuid_string);
    player->wallet = to_player["players"][i]["wallet"];
    player->name =   to_player["players"][i]["name"];
    player->bet_amount =   to_player["players"][i]["bet_amount"];
    player->has_bet =      to_player["players"][i]["has_bet"];
    
    for(int j = 0; j < NUM_CARDS; j++)
    {
      std::string hand_string = to_player["players"][i]["cards"][j];
      unsigned int index = hand_string.find(' ');
      assert(index != std::string::npos);
      
      try 
      {
        // read in the card....suit and value are split by a string
        int suit_t = std::stoi( hand_string.substr(0, index) );
        int value_t = std::stoi( hand_string.substr(index+1) );
        auto suit = static_cast<Suit>(suit_t);
        auto value = static_cast<Card_value>(value_t);
        
        player->hand[j].suit =  suit;
        player->hand[j].value = value;
      }
      catch(std::exception& ex)
      {
        std::cout << "Error processing: " << hand_string << std::endl;
      }
    }
  }
  
  client->update_client();
}

void client_communicator::send_action(std::string action, int bet)
{
  current_bet = bet;
  
  //Player* main_player = players[main_player];
  
  nlohmann::json to_dealer;
  to_dealer["action"] = action;
  
  // this field is important....this tells the dealer who sent this action
  to_dealer["table_pos"] = 0;//main_player->table_position;
  
  to_dealer["current_bet"] = current_bet;
  std::string t = to_dealer.dump();
  
  chat_message msg;
  msg.body_length(t.size());
  std::memcpy(msg.body(), t.c_str() , msg.body_length());
  msg.encode_header();
  
  try
  {
    comm->write(msg);
  }
  catch(std::exception& say)
  {
    std::cout << "Exception: " << say.what() << std::endl;
  }
}