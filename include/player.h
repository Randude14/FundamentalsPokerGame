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
    Player& operator=(const Player& other);
    virtual ~Player();
    
    // Getters
    std::string get_UUID();
    std::string get_name();
    double get_wallet();
    std::string get_action();
    std::string get_message();
    double get_current_bet();
    double get_total_bet();
    bool has_folded();
    int get_numDiscards();
    Hand_rankings get_hand_ranking();
    std::vector<Card>& get_hand();
    Card get_card(int);
    bool get_bet_status();
    
    // Setters
    void set_UUID(std::string);
    void set_name(std::string);
    void set_action(std::string);
    void set_message(std::string);
    void set_current_bet(double);
    void set_total_bet(double);
    void set_wallet(double);
    void set_numDiscards(int);
    void incr_numDiscards();
    void decr_numDiscards();
    void set_folded(bool);
    void set_hand_ranking(Hand_rankings);
    void set_hand(std::vector<Card>);
    void add_to_hand(Card);
    void draw_card(Card);
    void calculate_handvalue();  // calculate the player's hand value
    void set_bet_status(bool);

  private:
    // Player attributes
    boost::uuids::uuid UUID;
    std::string name;
    double wallet;
    std::vector<Card> hand;
    bool folded;
    double this_bet;
    double total_bet;
    Hand_rankings hand_ranking{Hand_rankings::HIGH_CARD};
    std::string action;
    std::string message;
    int numDiscards;
    bool has_bet;
    
    // -1 if other player has a better hand, 1 if this player has the better hand and 0 for tie
    int compare_hands(Player& other);
};

#endif // PLAYER_H
