#include "player.h"
#include "card.h"
#include <string>
#include <vector>
#include <iostream>

// ///////////////////////////////////////////////
// C O N S T R U C T O R S / D E S T R U C T O R S
// ///////////////////////////////////////////////
class Game;
Game::Game(std::string name)
{
  prize_pot = 0.0;
  current_bet = 1.0;
  game_stage = 0;
  playersNo=0;
  std::vector<Player>players;
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

void next_stage()
{

}
