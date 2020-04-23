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
    Player(const Player& other);  // Copy constructor
    void operator=(const Player& other);
    virtual ~Player();
    
    // Getters
    std::string get_UUID();
    std::string get_name();
    double get_wallet();
    std::string get_action();
    std::string get_message();
    double get_current_bet();
    double get_total_bet();
    int get_numDiscards();
    
    // Setters
    void set_UUID();
    void set_name();
    void set_action();
    void set_message();
    void set_current_bet();
    void set_total_bet();
    void set_numDiscards();

    void calculate_handvalue();  // calculate the player's hand value

  private:
    // Player attributes
    boost::uuids::uuid UUID;
    std::string name;
    double wallet;
    std::vector<Card> hand;
    std::string action;
    std::string message;
    double this_bet;
    double total_bet;
    int numDiscards;
    
    Hand_rankings hand_ranking{Hand_rankings::HIGH_CARD};
    
    // -1 if other player has a better hand, 1 if this player has the better hand and 0 for tie
    int compare_hands(Player& other);
};

#endif // PLAYER_H
