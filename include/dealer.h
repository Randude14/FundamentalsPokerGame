#ifndef DEALER_H
#define DEALER_H

#include "game.h"
#include "json.hpp"
#include <string>

/*
 * Dealer class which acts as server and manages gameplay
 * dealer.cpp to contain the main method that will act as the driver for the server
 */
 
 class Game;
 
#define MAX_PLAYERS 5

class Dealer
{
  public:
    Dealer();
    virtual ~Dealer();
    void exchange();           // Called when the exchange round is began, players may exchange unwanted cards here
    /* 
      to_dealer - This json came from a player client, it will contain the input or action
                      they pressed. (fold, discard, check, raise, etc...)
      to_player - This is the json that will go back to all players. This json should contain
                      the entire game state (hands, player bets)
                      NOTE: it should also indicate whose turn it is by applying the uuid
      */    
    void process(nlohmann::json& to_dealer, nlohmann::json& to_player);
    
  private:
    Game* game;                // Reference for the game that will handle main game functionality 
    void shuffle();            // Called to shuffle cards between rounds
    void deal();               // Called to deal out cards to players
    void determine_winner();   // Dealer then needs to determine winner based on the player's hands
    
};

#endif // DEALER_H
