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
  ACE =    1,
  TWO =   ACE + 1,
  THREE = ACE + 2,
  FOUR =  ACE + 3,
  FIVE =  ACE + 4,
  SIX =   ACE + 5,
  SEVEN = ACE + 6,
  EIGHT = ACE + 7,
  NINE =  ACE + 8,
  TEN =   ACE + 9,
  JACK =  ACE + 10,
  QUEEN = ACE + 11,
  KING =  ACE + 12
};

// Suit values assigned
enum class Suit
{
  HEART,
  SPADE,
  DIAMOND,
  CLUB,
};

/*
 * Each card contains a suit and card value
 */

// definitions used to identify cards by characters
#define HEART_ 'H'
#define SPADE_ 'S'
#define DIAMOND_ 'D'
#define CLUB_ 'C'

class Card
{
  public:
    Card(Card_value value=Card_value::ACE, Suit suit=Suit::HEART);
    virtual ~Card();
    Card_value value;                   // A = 1, J = 11, Q = 12, K = 13
    Suit suit;
};

#endif // CARD_H
