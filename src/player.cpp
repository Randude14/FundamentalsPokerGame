#include "player.h"
#include "json.hpp"

// ///////////////////////////////////////////////
// C O N S T R U C T O R S / D E S T R U C T O R S
// ///////////////////////////////////////////////

Player::Player()
{
  wallet = 100.0;
  bet_amount = 0.0;
}

Player::~Player() { }

