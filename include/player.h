#ifndef PLAYER_H
#define PLAYER_H

#include "card.h"
#include <string>
#include <vector>

/*
 * Player class which acts as client and is driven by user input
 */

class Player
{
  public:
    Player();
    virtual ~Player();
  private:
    int UUID;
    int table_position;
    std::string name;
    double wallet;
    std::vector< Card * > hand;
    double bet_amount;
    bool has_bet;
};

#endif // PLAYER_H
