#ifndef DEALER_H
#define DEALER_H

#include "game.h"
#include <string>

/*
 * Dealer class which acts as server and manages gameplay
 * dealer.cpp to contain the main method that will act as the driver for the server
 */

class Dealer
{
  public:
    Dealer();
    virtual ~Dealer();
    void exchange();           // Called when the exchange round is began, players may exchange unwanted cards here
  private:
    Game* game;                // Referrence for the game that will handle main game functionality 
    void shuffle();            // Called to shuffle cards between rounds
    void deal();               // Called to deal out cards to players
    void determine_winner();   // Dealer then needs to determine winner based on the player's hands
};

#endif // DEALER_H
