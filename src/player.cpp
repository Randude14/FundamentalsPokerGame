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
  folded = true;
  wallet = 100.0;
  action = "";
  message = "";
  this_bet = 0.0;
  total_bet = 0.0;
  numDiscards = 0;
}

/*
    boost::uuids::uuid UUID;
    std::string name;
    double wallet;
    std::vector<Card> hand;
    bool folded;
    
    Hand_rankings hand_ranking{Hand_rankings::HIGH_CARD};
    std::string action;
    std::string message;
    double this_bet;
    double total_bet;
    int numDiscards;
*/

Player::Player(const Player& other)
{
  this->wallet = other.wallet;
  this->name = other.name;
  this->hand_ranking = other.hand_ranking;
  this->folded = other.folded;
  this->action = other.action;
  this->message = other.message;
  this->this_bet = other.this_bet;
  this->total_bet = other.total_bet;
  this->hand.clear();
  for(unsigned int i = 0; i < other.hand.size(); i++)
  {
    this->hand.push_back(other.hand.at(i));
  }
}

Player& Player::operator=(const Player& other)
{
  this->wallet = other.wallet;
  this->name = other.name;
  this->hand_ranking = other.hand_ranking;
  this->folded = other.folded;
  this->action = other.action;
  this->message = other.message;
  this->this_bet = other.this_bet;
  this->total_bet = other.total_bet;
  this->hand.clear();
  for(unsigned int i = 0; i < other.hand.size(); i++)
  {
    this->hand.push_back(other.hand[i]);
  }
  
  return *this;
}

// Getters
std::string Player::get_UUID()
{
  return boost::lexical_cast<std::string>(UUID);
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

bool Player::has_folded()
{
  return folded;
}

int Player::get_numDiscards()
{
  return numDiscards;
}

Hand_rankings Player::get_hand_ranking()
{
  return hand_ranking;
}

std::vector<Card>& Player::get_hand()
{
  return hand;
}

Card Player::get_card(int index)
{
  return hand[index];
}

void Player::draw_card(Card card)
{
  hand.push_back(card);
  assert(hand.size() <= NUM_CARDS);
}


// Setters
void Player::set_UUID(std::string uuid)
{
  UUID = boost::lexical_cast<boost::uuids::uuid>(uuid);
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

void Player::set_folded(bool flag)
{
  folded = flag;
}

void Player::set_wallet(double w)
{
  wallet = w;
}

void Player::set_numDiscards(int cards)
{
  numDiscards = cards;
}

void Player::incr_numDiscards()
{
  numDiscards++;
}

void Player::decr_numDiscards()
{
  numDiscards--;
}

void Player::set_hand_ranking(Hand_rankings r)
{
  hand_ranking = r;
}

void Player::set_hand(std::vector<Card> h)
{
  hand.clear();
  hand.insert(hand.end(), h.begin(), h.end());
}

void Player::add_to_hand(Card c)
{
  hand.push_back(c);
}

void Player::set_bet_status(bool status)
{
  has_bet = status;
}

Player::~Player() { }
