#ifndef PLAYER_H
#define PLAYER_H

#include "card.h"
#include "dealer.h"
#include <string>
#include <vector>
#include <map>

/*
 * Player class which contains the data of a player in the game
 * May become just a data structure later on...
 */
 
 // number of cards each player gets
#define NUM_CARDS 5

class Player
{
  public:
    Player();             // Set player defaults
    virtual ~Player();

    void calculate_handvalue();  // calculate the player's hand value

    int UUID;             // Player attributes
    int table_position;
    std::string name;
    long wallet;
    std::vector<Card> hand{ std::vector<Card>( NUM_CARDS, Card())  };
    long bet_amount;
    bool has_bet;
    
    Hand_rankings hand_ranking{Hand_rankings::HIGH_CARD};
    
    // -1 if other player has a better hand, 1 if this player has the better hand and 0 for tie
    int compare_hands(Player& other);     
};

#endif // PLAYER_H
