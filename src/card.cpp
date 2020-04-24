#include "card.h"
#include <iostream>

// ///////////////////////////////////////////////
// C O N S T R U C T O R S / D E S T R U C T O R S
// ///////////////////////////////////////////////

Card::Card(Card_value v, Suit s) : value{v}, suit{s}  { }

Card::Card(const Card& other) 
{ 
  this->value = other.value;
  this->suit = other.suit;
}

Card& Card::operator=(const Card& other) 
{ 
  this->value = other.value;
  this->suit = other.suit;
  return *this;
}

Card::~Card() { }

int Card::get_int_value()
{
  return static_cast<int>(value);
}

int Card::get_int_suit()
{
  return static_cast<int>(suit);
}