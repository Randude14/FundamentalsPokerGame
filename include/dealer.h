#ifndef DEALER_H
#define DEALER_H

#include "game.h"
#include <string>

/*
 * Dealer class which acts as server and manages gameplay
 */

class Dealer
{
  public:
    Dealer();
    virtual ~Dealer();
    void exchange();
  private:
    Game* game;
    void shuffle();
    void deal();
    void determine_winner();
};

#endif // DEALER_H
