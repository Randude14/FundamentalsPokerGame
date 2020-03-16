#ifndef DECK_H
#define DECK_H

#include "card.h"
#include <string>
#include <vector>

/*
 * Deck class to manage cards in play
 */

class Deck
{
  public:
    Deck();
    virtual ~Deck();
  private:
    std::vector< Card * > cards;
};

#endif // DECK_H
