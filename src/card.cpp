#include "card.h"

// ///////////////////////////////////////////////
// C O N S T R U C T O R S / D E S T R U C T O R S
// ///////////////////////////////////////////////

Card::Card(Card_value v, Suit s) : value{v}, suit{s}
{

}

Card::~Card() { }

Card_value Card::get_value()const
{
	return value;
}

Suit Card::get_suit() const
{
	return suit;
}