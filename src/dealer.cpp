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

void Dealer::shuffle()
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

  std::shuffle (game.deck.begin(), game.deck.end(), std::default_random_engine(seed));
}

//This function takes the shuffled deck as a parameter which it uses to deal the card to the player
//and it will update the deck after the function is called
void Dealer::deal(std::vector<Card>& deck)
{
  assert(deck.size() == 52);
  
	int Noofcards=5, i;
	unsigned j;
	for(i = 0 ; i < Noofcards; i++)
	{
    for(j = 0 ; j < game.players.size(); j++)
    {
      // player gets top of deck, then remove the top card
      game.players[j].hand[i] = deck[0];
      deck.erase( deck.begin() );
    }
	}
}

void Dealer::exchange(std::vector<Card>& deck, Player& p)
{
  int x,y;
  //Should be the message box in GTK
  std::cout<<"How many card you want to exchange?(limit is 3)"<<std::endl;
  //Should take the input from GTK
  std::cin>>x;
  for(int i=0;i<x;i++)
  {
    std::cout << "Enter the card no. you would like to exchange"<<std::endl;
    std::cin >> y;
    //hand.erase(std::find(hand.begin(),hand.end(),y-1));
    p.hand[y-1]=deck[0]; //gets the first card from the Deck
                      /*we don't need the discard pile because the card in the hands
                      are already been popped out from the deck so while exchanging they
                      will never get the card that has already been served*/
    deck.erase( deck.begin() );
  }
}
void Dealer::determine_winner()
{
  assert(game.players.size() >= 2);
  
  //inputting all the rank of each player in a winner vector
  unsigned int i;
  Player winner = game.players[0];
  
  for(i=1; i < game.players.size(); i++)
  {
    // player @ i has a better hand than current winner
    if(winner.compare_hands(game.players[i]) < 0)
    {
      winner = game.players[i];
    }
  }
  
  //winner[playersNo] will hold the highest rank
  std::cout << winner.name <<" is the winner" << std::endl;
}

int current_bet = 0;

void Dealer::process(nlohmann::json& to_dealer, nlohmann::json& to_player)
{
  std::string action = to_dealer["action"];
  std::cout << "Player invoked " << action << std::endl;
  
  game.write_game_state( to_player );
}
