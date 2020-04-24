#include "player.h"
#include "card.h"
#include "dealer.h"
#include <vector>
#include <iostream>
#include <algorithm>

Player::Player()
{
  UUID = boost::uuids::random_generator()();
  name = "unknown";
  bet_amount = 0.f;
  has_bet = false;
  folded = true;
  wallet = 100.0;
  action = "";
  message = "";
  this_bet = 0.0;
  total_bet = 0.0;
  numDiscards = 0;
}

Player::Player(const Player& other)
{
  this->wallet = other.wallet;
  this->bet_amount = other.bet_amount;
  this->name = other.name;
  this->has_bet = other.has_bet;
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
  this->hand_ranking = other.hand_ranking;
  this->folded = other.folded;
  this->hand.clear();
  for(unsigned int i = 0; i < other.hand.size(); i++)
  {
    this->hand.push_back(other.hand[i]);
  }
  
  return *this;
}

Player::~Player() { }

// Getters
std::string Player::get_UUID()
{
  return boost::uuids::to_string(UUID);
}

std::string Player::get_name()
{
  return name;
}

double Player::get_wallet()
{
  return wallet;
}

std::string Player::get_action()
{
  return action;
}

std::string Player::get_message()
{
  return message;
}

double Player::get_current_bet()
{
  return this_bet;
}

double Player::get_total_bet()
{
  return total_bet;
}

int Player::get_numDiscards()
{
  return numDiscards;
}

// Setters
void Player::set_UUID(std::string uuid)
{
  Player::UUID = boost::uuids::string_generator(uuid);
}

void Player::set_name(std::string username)
{
  name = username;
}

void Player::set_action(std::string act)
{
  action = act;
}

void Player::set_message(std::string msg)
{
  message = msg;
}

void Player::set_current_bet(double curr_bet)
{
  this_bet = curr_bet;
}

void Player::set_total_bet(double tot_bet)
{
  total_bet = tot_bet;
}

void Player::set_numDiscards(int cards)
{
  numDiscards = cards;
}

Player::~Player() { }
