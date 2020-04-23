#include "player.h"
#include "card.h"
#include "game.h"
#include <string>
#include <vector>
#include <iostream>

// ///////////////////////////////////////////////
// C O N S T R U C T O R S / D E S T R U C T O R S
// ///////////////////////////////////////////////

Game::Game()
{
  prize_pot = 0.0;
  current_bet = 1.0;
  game_stage = 0;
  playersNo=0;
  int i;
  	Suit S;
  	Card_value V;
  	i = 0;
  	for (S =Suit::HEART; S <= Suit::CLUB; ++S)
  		for (V =Card_value::TWO; V <= Card_value::ACE;++V)
  			{
                          deck[i] = {Card(V,S)};
  			i++;
  			}

  Player player;
  player.name = "Randall";
  player.wallet = 50;
  
  player.hand.push_back(Card(Card_value::TWO   , Suit::HEART));
  player.hand.push_back(Card(Card_value::THREE , Suit::HEART));
  player.hand.push_back(Card(Card_value::FOUR  , Suit::HEART));
  player.hand.push_back(Card(Card_value::SIX   , Suit::HEART));
  player.hand.push_back(Card(Card_value::FIVE  , Suit::HEART));
  
  player.calculate_handvalue();
  
  players[0] = player;
}

Game::~Game() { }

// /////////////
// M E T H O D S
// /////////////

void Game::player_join(Player player)
{
  //  int playersNo; //I have already described it in Game class, don't know why it asked me to initialized here
  std::vector<Player>players;
  players.push_back(player);
  playersNo++;
}

void Game::check()
{

}

void Game::bet(double amount)
{

}

void Game::call(double amount)
{

}

void Game::raise(double amount)
{

}

void Game::fold()
{

}

void Game::next_stage()
{

}

void Game::write_game_state(nlohmann::json& to_player)
{
  Player p = players[0];
  // write player total
  to_player["player_total"] = 1;
  
  // add player info to json
  for(unsigned int i = 0; i < 1; i++)
  {
    std::cout << "index " << i << std::endl;
    Player player = players[i];
    to_player["players"][i]["name"] = player.name;
    to_player["players"][i]["uuid"] = boost::lexical_cast<std::string>(player.UUID);
    to_player["players"][i]["wallet"] = player.wallet;
    to_player["players"][i]["bet_amount"] = player.bet_amount;
    to_player["players"][i]["has_bet"] = player.has_bet;
    
    // write player's hand
    for(int j = 0; j < NUM_CARDS; j++)
    {
      std::string hand_string = std::to_string( static_cast<int>(player.hand[j].suit) );
      hand_string += std::string(" ");
      hand_string += std::to_string(static_cast<int>(player.hand[j].value));
      to_player["players"][i]["cards"][j] = hand_string;
    }
  }
  
  // write the game attributes
  to_player["prize_pot"] = prize_pot;
  to_player["current_bet"] = current_bet;
  to_player["current_turn"] = current_turn;
  to_player["game_stage"] = game_stage;
  to_player["prize_pot"] = prize_pot;
}