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
  this->table_position = other.table_position;
  this->hand_ranking = other.hand_ranking;
  for(unsigned int i = 0; i < other.hand.size(); i++)
  {
    this->hand.push_back(other.hand[i]);
  }
}

void Player::operator=(const Player& other)
{
  this->wallet = other.wallet;
  this->bet_amount = other.bet_amount;
  this->name = other.name;
  this->has_bet = other.has_bet;
  this->table_position = other.table_position;
  this->hand_ranking = other.hand_ranking;
  this->hand.clear();
  for(unsigned int i = 0; i < NUM_CARDS; i++)
  {
    this->hand.push_back(other.hand[i]);
  }
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


int Player::compare_hands(Player& other)
{
  if(hand_ranking != other.hand_ranking)
    return hand_ranking < other.hand_ranking;
  
  for(int i = 0; i < NUM_CARDS; i++)
  {
    if(hand[i].value < other.hand[i].value)
    {
      return -1;
    }
    else if(hand[i].value > other.hand[i].value)
    {
      return 1;
    }
  }
  
  return 0;
}


// this map counts the number of cards per value
// this lets us determine pairs or whether we're dealing with 5 uinique cards
// EX: Ace, 3, Ace, 7, 3 - This would store {Ace:2, 3:2, 7:1}
std::map<int, int> counts;

// sort the rankings by their number first, then by their values
bool sort_rankings(Card c1, Card c2)
{
  int value1 = c1.get_int_value();
  int value2 = c2.get_int_value();
  int count1 = counts[value1];
  int count2 = counts[value2];
  
  
  if(count1 == count2)
  {
    return value1 > value2;
  }
  else
  {
    return count1 > count2;
  }
}

void Player::calculate_handvalue()
{
  counts.clear();
  hand_ranking = Hand_rankings::HIGH_CARD;
  
  // rank whether this hand is a flush
  bool flush = true;
  
  for(int i = 0; i < NUM_CARDS; i++)
  {
    Card card = hand[i];
    int value = card.get_int_value();
    counts[ value ]++; // let's us see if we have pairs
    
    // find whether all cards have the same suit...
    if(i < NUM_CARDS-1 && flush)
      flush = (flush) ? hand[i].suit == hand[i+1].suit : false;
  }
  
  std::sort(hand.begin(), hand.end(), sort_rankings);
  
  bool straight = true;
  
  // this means we have 5 unique cards
  if(counts.size() == 5)
  {
    
    for(int i = 0; i < NUM_CARDS-1; i++)
      if(hand[i].get_int_value() != hand[i+1].get_int_value()+1)
      {
        straight = false;
        break;
      }
      
      
    if(! straight)
    {
      // special case for five-high straight....no good clean work around other than checking manually
      if(hand[0].value == Card_value::ACE && hand[1].value == Card_value::FIVE && 
         hand[2].value == Card_value::FOUR && hand[3].value == Card_value::THREE && 
         hand[4].value == Card_value::TWO)
      {
        // place ace at the end
        Card temp = hand[0];
        for(int i = 1; i < NUM_CARDS; i++)
          hand[i-1] = hand[i]; 
          
        hand[4] = temp;
        straight = true;
      }
      
    }
  }
  else
  {
    // if we don't have 5 unique cards...that means we have pair(s)
    straight = false;
  }
  
  
  if(flush)
  {
    if(straight)
    { // card at 0 will have the highest value...if it's an ACE, it's a royal flush
      hand_ranking = (hand[0].value == Card_value::ACE) ? 
                      Hand_rankings::ROYAL_FLUSH : Hand_rankings::STRAIGHT_FLUSH;
    }
    else
    { // no straight involved...just a flush
      hand_ranking = Hand_rankings::FLUSH;
    }
  }
  else
  {
    if(straight)
    {
      hand_ranking = Hand_rankings::STRAIGHT;
    }
    else
    {    
      for(int i = 0; i < NUM_CARDS; )
      {
        // goes through counts and checks for multiples of values
        if(counts[ hand[i].get_int_value() ] == 4)
        {
          hand_ranking = Hand_rankings::FOUR_OF_KIND;
          break;
        }    
        else if(counts[ hand[i].get_int_value() ] == 3)
        {
          hand_ranking = Hand_rankings::THREE_OF_KIND;
          i += 3; // jump 3 ahead based on count, maybe a pair afterwards
        }
        else if(counts[ hand[i].get_int_value() ] == 2)
        {
          if(hand_ranking == Hand_rankings::THREE_OF_KIND)
          {
            // three of a kind and a two pair is a full house
            hand_ranking = Hand_rankings::FULL_HOUSE;
          }
          else if(hand_ranking == Hand_rankings::PAIR)
          { // pair before...with this pair becomes two pair
            hand_ranking = Hand_rankings::TWO_PAIR;
          }
          else
          { // first pair we found
            hand_ranking = Hand_rankings::PAIR;
          }
          i += 2; // jump by count 2
        }
        else
        { // otherwise skip ahead
          i++;
        }
      }
    }
  }
}
