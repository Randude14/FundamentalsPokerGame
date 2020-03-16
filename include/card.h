#ifndef CARD_H
#define CARD_H

#include <string>

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

enum class Suit
{
  HEART,
  SPADE,
  DIAMOND,
  CLUB,
};

/*
 * Card class to contain face value and suit
 */

class Card
{
  public:
    Card();
    virtual ~Card();
  private:
    Card_value value;                   // A = 1, J = 11, Q = 12, K = 13
    Suit suit;
};

#endif // CARD_H
