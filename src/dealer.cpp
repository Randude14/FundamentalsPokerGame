#include "dealer.h"
#include <iostream>

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

void Dealer::shuffle()
{

}

void Dealer::deal()
{

}

void Dealer::exchange()
{

}

void Dealer::determine_winner()
{

}

int current_bet = 0;

void Dealer::process(nlohmann::json& to_dealer, nlohmann::json& to_player)
{
  std::string action = to_dealer["action"];
  std::cout << "Player invoked " << action << std::endl;
  
  current_bet = to_dealer["current_bet"];
  
  to_player["current_bet"] = current_bet;
  
  game->write_game_state( to_player );
}