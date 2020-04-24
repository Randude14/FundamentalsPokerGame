#include "player.h"
#include "card.h"
#include "game.h"
#include <string>
#include <vector>
#include <iostream>

// ///////////////////////////////////////////////
// C O N S T R U C T O R S / D E S T R U C T O R S
// ///////////////////////////////////////////////

Game::Game()
{
  prize_pot = 0.0;
  current_bet = 0.0;
  game_stage = IDLE;
  agreed = 0;
  
  Suit S;
  Card_value V;
  for (S = Suit::HEART; S <= Suit::CLUB; ++S)
  {
  	for (V = Card_value::TWO; V <= Card_value::ACE;++V)
  	{
      deck.push_back( Card(V,S) );
    }
  }
  
  rng = std::default_random_engine( std::time(0) );
}

Game::~Game() { }

// /////////////
// M E T H O D S
// /////////////


// ********** DECK METHODS *******************************

void Game::shuffle_deck()
{
  // retrieve cards from players and discard pile (if any)
  for(unsigned int i = 0; i < players.size(); i++)
  {
    Player* player = &players[i];
    
    for(auto &it : player->get_hand())
    {
      deck.push_back(it);
    }
    
   player->clear_hand();
  }
  
  for(auto &it : discard_pile)
  {
    deck.push_back(it);
  }
  
  discard_pile.clear();
  
  assert( validate_deck() );
  
  // shuffle 3 times
  std::shuffle(deck.begin(), deck.end(), rng);
  std::shuffle(deck.begin(), deck.end(), rng);
  std::shuffle(deck.begin(), deck.end(), rng);
}

bool Game::validate_deck()
{
  if(deck.size() > NORMAL_DECK_SIZE)
  {
    std::cout << "Deck size is too large." << std::endl;
    return false;
  }
    if(deck.size() < NORMAL_DECK_SIZE)
  {
    std::cout << "Deck size is too small." << std::endl;
    return false;
  }
  // use a 2d grid (Suits X Values) to validate the deck
  int grid[4][13] = {0};
  
  for(unsigned int i = 0; i < deck.size(); i++)
  {
    Card card = deck.at(i);
    int suit = card.get_int_suit();
    int value = card.get_int_value()-2; // account for offset of 2
    grid[suit][value]++;
    
    // we have repeats...
    if(grid[suit][value] >= 2)
    {
      std::cout << "Deck has repeats." << std::endl;
      return false;
    }
  }
  
  for(int i = 0; i < 4; i++)
  {
    for(int j = 0; j < 13; j++)
    {
      // card missing
      if(grid[i][j] != 1)
      {
        std::cout << "Deck is missing cards." << std::endl;
        return false;
      }
    }
  }
  
  return true;
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

void Game::calculate_handvalue(Player* player)
{
  counts.clear();
  Hand_rankings hand_ranking = Hand_rankings::HIGH_CARD;
  auto hand = player->get_hand();
  
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
  
  player->set_hand(hand);
  player->set_hand_ranking( hand_ranking );
}

// *********** PLAYER ACTIONS ********************************************************

bool Game::player_join(Player player)
{
  if(players.size() >= MAX_PLAYERS)
  {
    return false;
  }
  
  // treat player as a folded player
  // player will be ignored until next round
  player.set_folded(true);
  players.push_back( player );
  
  // middle of game...increase agreed so that round over is triggered
  if(game_stage != IDLE)
  {
    agreed++;
    folded++;
  }
  
  return true;
}

void Game::check()
{
  assert(game_stage == BET_ROUND_1 || game_stage == BET_ROUND_2);
  agreed++;
  
  this->next_player();
}

void Game::bet(double amount)
{
  Player* current_player = &players[current_turn];
  assert(game_stage == BET_ROUND_1 || game_stage == BET_ROUND_2);
  assert( current_player->get_wallet() >= amount && current_player->get_current_bet() == 0);
  assert( current_bet == 0 );
  agreed = folded + 1;
  
  // get player attributes
  auto wallet = current_player->get_wallet();
  auto p_total_bet = current_player->get_total_bet();
  
  current_bet = amount;
  
  // only increase the pot by the difference in their last bet, if there was one
  prize_pot += amount;
  // only decrease player wallet by the difference in their last bet, if there was one
  wallet -= amount;
  p_total_bet += amount;
  double p_current_bet = amount;
  
  // set player attributes
  current_player->set_current_bet(p_current_bet);
  current_player->set_total_bet(p_total_bet);
  current_player->set_wallet(wallet);
  
  this->next_player();
}

void Game::raise(double amount)
{ 
  Player* current_player = &players[current_turn];
  // can't raise bet if bet was never raise to begin with...
  assert( current_bet != 0 );
  assert( current_player->get_wallet() >= current_bet + amount );
  assert(game_stage == BET_ROUND_1 || game_stage == BET_ROUND_2);
  agreed = folded + 1;
  
  // get player attributes
  auto p_current_bet = current_player->get_current_bet();
  auto p_total_bet = current_player->get_total_bet();
  auto wallet = current_player->get_wallet();
  
  current_bet += amount;
  
  // only increase the pot by the difference in their last bet, if there was one
  prize_pot += (current_bet - current_player->get_current_bet());
  // only decrease player wallet by the difference in their last bet, if there was one
  wallet -= (current_bet - p_current_bet);
  p_total_bet += (current_bet - p_current_bet);
  p_current_bet = current_bet;
  
  // set player attributes
  current_player->set_current_bet(p_current_bet);
  current_player->set_total_bet(p_total_bet);
  current_player->set_wallet(wallet);
  
  
  this->next_player();
}

void Game::call()
{
  Player* current_player = &players[current_turn];
  assert( ( current_player->get_wallet() - current_player->get_current_bet() ) >= current_bet );
  assert(game_stage == BET_ROUND_1 || game_stage == BET_ROUND_2);
  agreed++;
  
  // get player attributes
  auto p_current_bet = current_player->get_current_bet();
  auto p_total_bet = current_player->get_total_bet();
  auto wallet = current_player->get_wallet();
  
  // only increase the pot by the difference in their last bet, if there was one
  prize_pot += (current_bet - current_player->get_current_bet());
  // only decrease player wallet by the difference in their last bet, if there was one
  wallet -= (current_bet - p_current_bet);
  p_total_bet += (current_bet - p_current_bet);
  p_current_bet = current_bet;
  
  // set player attributes
  current_player->set_current_bet(p_current_bet);
  current_player->set_total_bet(p_total_bet);
  current_player->set_wallet(wallet);
  
  this->next_player();
}

void Game::fold()
{
  assert(game_stage == BET_ROUND_1 || game_stage == BET_ROUND_2);
  agreed++;
  folded++;
  Player* current_player = &players[current_turn];
  current_player->set_folded(true);
  
  this->next_player();
  // nothing to do with pot...whatever bet the player had was already added
}

void Game::exchange(bool to_exchange[NUM_CARDS])
{
  assert(game_stage == EXCHANGE_ROUND);
  Player* current_player = &players[current_turn];
  agreed++;
  
  auto hand = current_player->get_hand();
  assert(hand.size() == NUM_CARDS); // double check hand size
  assert( deck.size() >= NUM_CARDS );              // must have enough cards to exchange
  
  for(int i = 0; i < NUM_CARDS; i++)
  {
    // exchange this card
    if(to_exchange[i])
    {
      // discard this card and draw from the top of the deck
      Card discard = hand[i];
      discard_pile.push_back(discard);
      
      // draw from deck
      Card exchange = deck[0];
      deck.erase( deck.begin() );
      
      hand[i].value = exchange.value;
      hand[i].suit = exchange.suit;
    }
  }
  
  current_player->set_hand(hand);
  // recaulcate the hand value after exchange
  this->calculate_handvalue(current_player);
  this->next_player();
}



// ************ GAME STAGE LOGIC ***********************************************************

bool Game::min_players()
{
  return players.size() >= 2;
}

bool Game::is_idle()
{
  return game_stage == IDLE;
}

bool Game::is_current_player(std::string& UUID)
{
  Player* current_player = &players[current_turn];
  
  return ( UUID == current_player->get_UUID() );
}

void Game::start_game()
{
  // only start game when it was idling
  assert(game_stage == IDLE);
  assert( min_players() );
  
  this->shuffle_deck();
  prize_pot = 0;
  current_bet = 0;
  folded = 0;
  
  for(int i = 0; i < NUM_CARDS; i++)
  {
    for(unsigned int j = 0; j < players.size(); j++)
    {
      // draw card
      Card draw = deck[0];
      deck.erase( deck.begin() );
      
      Player* player = &players[j];
      player->set_current_bet(0);
      player->set_total_bet(0);
      player->set_folded(false);
      player->draw_card(draw);
      
      // calcualte hand values on last card
      if(i == NUM_CARDS-1)
      {
        calculate_handvalue(player);
        
        // add player anties if the first betting round
        double wallet = player->get_wallet();
        wallet--;
        player->set_wallet(wallet);
        prize_pot++;
      }
    }
  }
  
  this->next_stage();
}

void Game::end_game()
{
  assert( game_over() );
  
  game_stage = IDLE;
  determine_winners();
  
  if(winners.size() > 1)
  {
    std::cout << "We have a tie!" << std::endl;
  }
  
  // normally this will be divided by 1 but there may be a tie
  double earnings = prize_pot / winners.size();
  
  for(auto& player_index : winners)
  {
    Player* winner = &players[player_index];
    std::cout << winner->get_name() << " has won!" << std::endl;
    
    double wallet = winner->get_wallet();
    wallet += earnings;
    winner->set_wallet(wallet);
  }
  
  std::cout << std::endl << std::endl;
}

bool Game::round_over()
{
  return agreed == players.size();
}

bool Game::game_over()
{
  return game_stage == END;
}

void Game::next_stage()
{
  assert(players.size() >= 2);
  agreed = folded;
  game_stage++;
  current_turn = -1;
  current_bet = 0;
  
  // no stage after the end, stop here
  if(game_stage >= END)
  {
    game_stage = END;
    return;
  }
  
  // if there is only one player that has not folded...end game
  if(folded > players.size()-2)
  {
    std::cout << "Game end   "  << folded << std::endl;
    game_stage = END;
    return;
  }
  
  for(unsigned int i = 0; i < players.size(); i++)
  {
    Player* player = &players[i];
    player->set_current_bet(0);
  }
  
  // go to next player
  next_player();
}

void Game::next_player()
{
  // only go to next person if this round is not over
  if(! round_over() )
  {
    Player* player;
    
    do
    {
      current_turn++;
      if(current_turn == (int)players.size())
      {
        current_turn = 0;
      }
      player = &players[current_turn];
    }
    // ignore players that have folded
    while( player->has_folded() );
    
    
  }
}

int Game::compare_hands(Player* p1, Player* p2)
{
  auto hand1 = p1->get_hand();
  auto hand2 = p2->get_hand();
  
  assert( hand1.size() == NUM_CARDS );
  assert( hand2.size() == NUM_CARDS );

  auto ranking1 = p1->get_hand_ranking();
  auto ranking2 = p2->get_hand_ranking();
  
  // compare hand rankings first
  if(ranking1 < ranking2)
  {
    return -1;
  }
  else if(ranking1 > ranking2)
  {
    return 1;
  }
  
  // if rankings equal, compare cards in descending order
  for(int i = 0; i < NUM_CARDS; i++)
  {
    if(hand1[i].value < hand2[i].value)
    {
      return -1;
    }
    else if(hand1[i].value > hand2[i].value)
    {
      return 1;
    }
  }
  
  return 0;  
}

void Game::determine_winners()
{ 
  // this vector will store the players in descending order 
  // according their ranking against each other
  winners.clear();
  
  for(unsigned int i = 0; i < players.size(); i++)
  {
    Player* player = &players[i];
    
    if(! player->has_folded())
    {
      winners.push_back(i);
    }
  }
  
  std::sort(winners.begin(), winners.end(), winner_comparer(this) );
  int top_index = winners[0];
  Player* top = &players[top_index];
  
  for(unsigned int i = 1; i < winners.size(); i++)
  {
    int index = winners[i];
    Player* player = &players[index];
    
    int cmp = compare_hands(top, player);
    
    if(cmp > 0)
    {
      std::cout << player->get_name() << " lost." << std::endl;
      winners.erase( winners.begin() + i);
      i--;
    }
  }
  
  //winner[playersNo] will hold the highest rank
  //std::cout << winner->get_name() <<" is the winner" << std::endl;
}

void Game::write_game_state(nlohmann::json& to_player)
{
  // write player total
  to_player["player_total"] = 1;
  
  // add player info to json
  for(unsigned int i = 0; i < 1; i++)
  {
    std::cout << "index " << i << std::endl;
    Player* player = &players[i];
    to_player["players"][i]["name"] = player->get_name();
    to_player["players"][i]["uuid"] = player->get_UUID();
    to_player["players"][i]["wallet"] = player->get_wallet();
    to_player["players"][i]["this_bet"] = player->get_current_bet();
    
    auto hand = player->get_hand();
    
    // write player's hand
    for(int j = 0; j < NUM_CARDS; j++)
    {
      std::string hand_string = std::to_string( static_cast<int>(hand[j].suit) );
      hand_string += std::string(" ");
      hand_string += std::to_string(static_cast<int>(hand[j].value));
      to_player["players"][i]["cards"][j] = hand_string;
    }
  }
  
  // write the game attributes
  to_player["prize_pot"] = prize_pot;
  to_player["current_bet"] = current_bet;
  to_player["game_stage"] = game_stage;
}