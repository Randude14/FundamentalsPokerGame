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
//This functions returns a shuffled deck of 52 cards
template<typename T>
void pop_front(std::vector<T> &v)
{
    if (v.size() > 0) {
        v.erase(v.begin());
    }
}

std::vector<Card> Dealer::shuffle(Game& g)
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

  std::shuffle (g.deck.begin(), g.deck.end(), std::default_random_engine(seed));
return g.deck;
}
//This function takes the shuffled deck as a parameter which it uses to deal the card to the player
//and it will update the deck after the function is called
void Dealer::deal(std::vector<Card>& temp,Game& g)
{
	int Noofcards=5, i;
	unsigned j;
	for(i=0;i<Noofcards;i++)
	{
    for(j=0;j<g.players.size();j++)
    {
      g.players[j].hand[i]=temp[0];
      pop_front(temp);
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
std::cout<<"Enter the card no. you would like to exchange"<<std::endl;
std::cin>>y;
//hand.erase(std::find(hand.begin(),hand.end(),y-1));
p.hand[y-1]=deck[0]; //gets the first card from the Deck
                   /*we don't need the discard pile because the card in the hands
                   are already been popped out from the deck so while exchanging they
                   will never get the card that has already been served*/
        pop_front(deck);
}
}
void Dealer::determine_winner(Game& g)
{
std::vector<int>winner;
//inputting all the rank of each player in a winner vector
unsigned i;
for(i=0;i<g.players.size();i++)
{
  winner.push_back((int)g.players[i].hand_ranking);
}
 std::sort(winner.begin(), winner.end());
//winner[playersNo] will hold the highest rank
for(i=0;i<g.players.size();i++)
{
if(winner[g.playersNo]==(int)g.players[i].hand_ranking)
{
std::cout<<"Player: "<<i+1<<" is the winner"<<std::endl;
}
}
}

int current_bet = 0;

void Dealer::process(nlohmann::json& to_dealer, nlohmann::json& to_player)
{
  std::string action = to_dealer["action"];
  std::cout << "Player invoked " << action << std::endl;
  
  // process player input
  to_player["main_player"] = 0;
  to_player["num_players"] = 0;
  
  game->write_game_state( to_player );
}
