#ifndef CARD_H
#define CARD_H

#include <string>

/* 
 * Acts as the cards the game will deal out and handle
 */

// The card values assigned
// Values may need to be changed later to reflect their actual game values...
enum class Card_value
{
  TWO =     2,
  THREE =   3,
  FOUR =    4,
  FIVE =    5,
  SIX =     6,
  SEVEN =   7,
  EIGHT =   8,
  NINE =    9,
  TEN =     10,
  JACK =    11,
  QUEEN =   12,
  KING =    13,
  ACE =     14,   // make ace highest card...only the five high straight is the only case for it being 1
};

// Suit values assigned
enum class Suit
{
  HEART,
  SPADE,
  DIAMOND,
  CLUB,
};


// hand rankings for the player's hands
enum class Hand_rankings
{
  HIGH_CARD,      // 0
  PAIR,           // 1
  TWO_PAIR,       // 2
  THREE_OF_KIND,  // 3
  STRAIGHT,       // 4
  FLUSH,          // 5
  FULL_HOUSE,     // 6
  FOUR_OF_KIND,   // 7
  STRAIGHT_FLUSH, // 8
  ROYAL_FLUSH,    // 9
};

/*
 * Each card contains a suit and card value
 */

class Card
{
  public:
    Card(Card_value value=Card_value::ACE, Suit suit=Suit::HEART);
    virtual ~Card();
    
    // used to get the int value from the enums
    int get_int_value();
    int get_int_suit();
    
    Card_value value;
    Suit suit;
};

#endif // CARD_H
