#include "dealer.h"
#include "player.h"
#include "card.h"
#include "game.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
#include <bits/stdc++.h>



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

int Dealer::compare_hands(Player* p1, Player* p2)
{
  for(int i = 0; i < NUM_CARDS; i++)
  {
    if(p1->hand[i].value < p2->hand[i].value)
    {
      return -1;
    }
    else if(p1->hand[i].value > p2->hand[i].value)
    {
      return 1;
    }
  }
  
  return 0;
}

void Dealer::determine_winner()
{ 
  Player* winner = game.players[0];
  
  for(int i = 1; i < game.num_players; i++)
  {
    Player* other = &game.players[i];
    // player @ i has a better hand than current winner
    if(compare_hands(winner, other) < 0)
    {
      winner = other;
    }
  }
  
  //winner[playersNo] will hold the highest rank
  std::cout << winner->name <<" is the winner" << std::endl;
}

void Dealer::process(nlohmann::json& to_dealer, nlohmann::json& to_player)
{
  std::string action = to_dealer["action"];
  std::cout << "Player invoked " << action << std::endl;
  
  game.write_game_state( to_player );
}
