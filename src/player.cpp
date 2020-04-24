#include "player.h"
#include "card.h"
#include "dealer.h"
#include <vector>
#include <iostream>
#include <algorithm>

Player::Player()
{
  UUID = boost::uuids::random_generator()();
  wallet = 100.0;
  bet_amount = 0.0;
  name = "unknown";
  bet_amount = 0.f;
  has_bet = false;
  folded = true;
}

Player::Player(const Player& other)
{
  this->wallet = other.wallet;
  this->bet_amount = other.bet_amount;
  this->name = other.name;
  this->has_bet = other.has_bet;
  this->table_position = other.table_position;
  this->hand_ranking = other.hand_ranking;
  this->folded = other.folded;
  this->hand.clear();
  for(unsigned int i = 0; i < other.hand.size(); i++)
  {
    this->hand.push_back(other.hand.at(i));
  }
}

Player& Player::operator=(const Player& other)
{
  this->wallet = other.wallet;
  this->bet_amount = other.bet_amount;
  this->name = other.name;
  this->has_bet = other.has_bet;
  this->table_position = other.table_position;
  this->hand_ranking = other.hand_ranking;
  this->folded = other.folded;
  this->hand.clear();
  for(unsigned int i = 0; i < other.hand.size(); i++)
  {
    this->hand.push_back(other.hand.at(i));
  }
  
  return *this;
}

Player::~Player() { }
