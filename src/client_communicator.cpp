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
  game_status = to_player["game_comment"];
  auto players = client->players;
  
  for(int i = 0; i < client->num_players; i++)
  {
    std::string i_str = std::to_string(i);
    Player* player = players[i];
    player->set_name(to_player["players"][i_str]["name"]);
    std::string uuid_string = to_player["players"][i_str]["uuid"];
    player->set_UUID(uuid_string);
    player->set_wallet(to_player["players"][i_str]["wallet"]);
    player->set_total_bet(to_player["players"][i_str]["bet_amount"]);
    player->set_current_bet(to_player["players"][i_str]["current_bet"]);
    player->set_bet_status(to_player["players"][i_str]["has_bet"]);
    player->set_action(to_player["players"][i_str]["action"]);
    
    // set main player in table
    if(client->main_uuid == uuid_string)
    {
      client->main_player = i;
      std::cout << "You are now " << i << " player." << std::endl;
    }
    else
    {
      std::cout << client->main_uuid << " and " << uuid_string << " do not match." << std::endl;
    }
    
    int total = to_player["players"][i_str]["cards"]["total"];
    player->clear_hand();
    
    for(int j = 0; j < total; j++)
    {
      std::string j_str = std::to_string(j);
      std::string hand_string = to_player["players"][i_str]["cards"][j_str];
      unsigned int index = hand_string.find(' ');
      assert(index != std::string::npos);
      
      try 
      {
        // read in the card....suit and value are split by a string
        int suit_t = std::stoi( hand_string.substr(0, index) );
        int value_t = std::stoi( hand_string.substr(index+1) );
        auto suit = static_cast<Suit>(suit_t);
        auto value = static_cast<Card_value>(value_t);
        Card card(value, suit);
        player->add_to_hand(card);
      }
      catch(std::exception& ex)
      {
        std::cout << "Error processing: " << hand_string << std::endl;
      }
    }
  }
  
  
  /// ********************* ROBBIE HERE ******************************
  
  
  if(to_player.contains("current_turn") && to_player.contains("turn_uuid"))
  {
    int current_turn = to_player["current_turn"];
    
    // it's this players turn to do something...
    if(current_turn == client->main_player)
    {
      int game_stage = to_player["game_stage"];
      
      // Betting round
      if(game_stage == BET_ROUND_1 || game_stage == BET_ROUND_2)
      {
        client->bet_sensitivity();
      }
      // exchange round
      else if(game_stage == EXCHANGE_ROUND)
      {
        client->exchange_sensitivity();
      }
    }
  }
  
  client->update_client();
}

void client_communicator::send_action(int cards,
                                      std::string chat,
                                      double curr_bet,
                                      std::string event,
                                      std::string uuid,
                                      std::string name,
                                      double tot_bet,
                                      bool discards[NUM_CARDS])
{
  nlohmann::json to_dealer;
  to_dealer["cards_requested"] = cards;
  to_dealer["chat"] = chat;
  to_dealer["current_bet"] = curr_bet;
  to_dealer["event"] = event;
  to_dealer["from"] = { {"uuid",uuid} , {"name",name} };
  to_dealer["total_bet"] = tot_bet;
  
  for(unsigned int i = 0; i < NUM_CARDS; i++)
  {
    std::string i_str = std::to_string(i);
    to_dealer["discards"][i_str] = discards[i];
  }

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
