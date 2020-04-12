#define WITH_DEALER

#include "player.h"
#include "card.h"
#include <iostream>

int main()
{
  Player p, p1;
  Card hand[NUM_CARDS] = {Card(Card_value::ACE, Suit::CLUB), 
                          Card(Card_value::THREE, Suit::CLUB), 
                          Card(Card_value::TWO, Suit::DIAMOND), 
                          Card(Card_value::TWO, Suit::CLUB), 
                          Card(Card_value::FIVE, Suit::CLUB)};
  
  for(int i = 0; i < NUM_CARDS; i++)
    p.hand[i] = hand[i];
  
  p.calculate_handvalue();
  for(unsigned int i = 0; i < p.hand.size(); i++)
  {
    std::cout << "Value - " << p.hand[i].get_int_value() << ", Suit - " << p.hand[i].get_int_suit() << std::endl;
  }
  std::cout << std::endl << std::endl;
  
  Card hand1[NUM_CARDS] = {Card(Card_value::TWO, Suit::CLUB), 
                          Card(Card_value::TWO, Suit::DIAMOND), 
                          Card(Card_value::SIX, Suit::CLUB), 
                          Card(Card_value::TWO, Suit::CLUB), 
                          Card(Card_value::TWO, Suit::CLUB)};  
                          
  for(int i = 0; i < NUM_CARDS; i++)
    p1.hand[i] = hand1[i];
  
  p1.calculate_handvalue();
  for(unsigned int i = 0; i < p1.hand.size(); i++)
  {
    std::cout << "Value - " << p1.hand[i].get_int_value() << ", Suit - " << p1.hand[i].get_int_suit() << std::endl;
  }
  std::cout << std::endl << std::endl;
  std::cout << "Hand ranking: " << (int)p.hand_ranking << std::endl;
  std::cout << "Hand ranking: " << (int)p1.hand_ranking << std::endl;
  std::cout << "Player 1 and Player 2 (-1 for player 2, 1 for player 1, and 0 for tie): " << p.compare_hands(p1) << std::endl;
  
  return 0;
}