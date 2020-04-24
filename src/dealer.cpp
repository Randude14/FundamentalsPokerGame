#include "dealer.h"
#include "player.h"
#include "card.h"
#include "game.h"
#include <iostream>
#include <vector>



// ///////////////////////////////////////////////
// C O N S T R U C T O R S / D E S T R U C T O R S
// ///////////////////////////////////////////////

Dealer::Dealer()
{
  
}

Dealer::~Dealer() { }



// /////////////
// M E T H O D S
// /////////////

bool Dealer::is_game_idling()
{
  return game.is_idle();
}

void Dealer::restart_game(nlohmann::json& to_player)
{
  game.start_game(); // start game again
  game.write_game_state(to_player);
}

void Dealer::process(nlohmann::json& to_dealer, nlohmann::json& to_player)
{
  std::string event = to_dealer["event"];
  std::string uuid = to_dealer["from"]["uuid"];
  std::string name = to_dealer["from"]["name"];
  
  std::cout << name << " invoked " << event << std::endl;
  // used to flag that only the game state info should be sent
  // should only be set to true when a player joins
  
  // new player has joined the game
  if(event == "join")
  {
    Player player;

    player.set_UUID(uuid);
    player.set_name(name);
    
    if(game.player_join(player))
    {
      std::cout << "Player " << name << " joined." << std::endl;
    }
  }
  // player inputs from a player currently in the game
  else
  {
    // check this is the correct player
    assert( game.is_current_player(uuid) );
  }
  
  
  // player actions
  if(event == "check")
  {
    game.check();
  }
  else if(event == "bet")
  {
    double amount = to_dealer["current_bet"];
    game.bet(amount);
  }
  else if(event == "raise")
  {
    double amount = to_dealer["current_bet"];
    game.raise(amount);
  }
  else if(event == "fold")
  {
    game.fold();
  }
  else if(event == "exchange")
  {
    // TODO: replace later with json info
    bool to_exchange[NUM_CARDS] = {false};
    game.exchange(to_exchange);
  }  
  
  // advance to next round
  if( game.round_over() )
  {
    game.next_stage();
    std::cout << "Advancing to next stage" << std::endl;
  }
 
  // game is over
  if( game.game_over() )
  {
    std::cout << "Game is over."  << std::endl; 
    game.end_game();      // call end game
  }
  
  game.write_game_state( to_player );
}
