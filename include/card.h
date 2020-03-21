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
  ACE,
  TWO,
  THREE,
  FOUR,
  FIVE,
  SIX,
  SEVEN,
  EIGHT,
  NINE,
  TEN,
  JACK,
  QUEEN,
  KING,
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

class Card
{
  public:
    Card(Card_value value, Suit suit);
    virtual ~Card();
  private:
    Card_value value;                   // A = 1, J = 11, Q = 12, K = 13
    Suit suit;
};

#endif // CARD_H
