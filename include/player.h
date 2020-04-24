#ifndef PLAYER_H
#define PLAYER_H

#pragma once

#include "card.h"
#include "json.hpp"
#include <string>
#include <vector>
#include <map>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

/*
 * Player class which contains the data of a player in the game
 * May become just a data structure later on...
 */
 
 // number of cards each player gets
#define NUM_CARDS 5

class Player
{
  public:
    Player();                     // Set player defaults
    Player(const Player& other); // Copy constructor
    Player& operator=(const Player& other);
    virtual ~Player();

    // Player attributes
    boost::uuids::uuid UUID;
    int table_position;
    std::string name;
    double wallet;
    std::vector<Card> hand;
    double bet_amount;
    bool has_bet;
    bool folded;
    
    Hand_rankings hand_ranking{Hand_rankings::HIGH_CARD};
};

#endif // PLAYER_H
