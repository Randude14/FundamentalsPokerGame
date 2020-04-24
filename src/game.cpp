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
  num_players = 0;
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
  for(int i = 0; i < num_players; i++)
  {
    Player* player = &players[i];
    
    for(auto &it : player->hand)
    {
      deck.push_back(it);
    }
    
   player->hand.clear();
  }
  
  for(auto &it : discard_pile)
  {
    deck.push_back(it);
  }
  
  assert( validate_deck() );
  
  // shuffle 3 times
  std::shuffle(deck.begin(), deck.end(), rng);
  std::shuffle(deck.begin(), deck.end(), rng);
  std::shuffle(deck.begin(), deck.end(), rng);
}

bool Game::validate_deck()
{
  if(deck.size() != NORMAL_DECK_SIZE)
  {
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
  auto hand = player->hand;
  
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
  
  player->hand_ranking = hand_ranking;
}

// *********** PLAYER ACTIONS ********************************************************

bool Game::player_join(Player player)
{
  if(num_players >= MAX_PLAYERS)
  {
    return false;
  }
  
  // treat player as a folded player
  // player will be ignored until next round
  player.folded = true;
  player.table_position = num_players + 1;
  players[num_players] = player;
  num_players++;
  
  return true;
}

void Game::check()
{
  agreed++;
}

void Game::bet(double amount)
{
  // reset the agreed amount, all players must agree to this bet or fold
  agreed = 1;
  
  Player* current_player = &players[current_turn];
  
  // only increase the pot by the difference in their last bet, if there was one
  prize_pot += (amount - current_player->bet_amount);
  // only decrease player wallet from the different on their last bet, if there was one
  current_player->wallet -= (amount - current_player->bet_amount);
  current_player->bet_amount = amount;
  current_bet = amount;
}

void Game::raise(double amount)
{
  // reset the agreed amount, all players must agree to this bet or fold
  agreed = 1;
  
  Player* current_player = &players[current_turn];
  
  // only increase the pot by the difference in their last bet, if there was one
  prize_pot += (amount - current_player->bet_amount);
  // only decrease player wallet from the different on their last bet, if there was one
  current_player->wallet -= (amount - current_player->bet_amount);
  current_player->bet_amount = amount;
  current_bet = amount;
}

void Game::call()
{
  agreed++;
  Player* current_player = &players[current_turn];
  
  // only increase the pot by the difference in their last bet, if there was one
  prize_pot += (current_bet - current_player->bet_amount);
  // only decrease player wallet by the difference in their last bet, if there was one
  current_player->wallet -= (current_bet - current_player->bet_amount);
  current_player->bet_amount = current_bet;
}

void Game::fold()
{
  agreed++;
  Player* current_player = &players[current_turn];
  current_player->folded = true;
  
  // nothing to do with pot...whatever bet the player had was already added
}

void Game::exchange(bool to_exchange[NUM_CARDS])
{
  Player* current_player = &players[current_turn];
  agreed++;
  
  assert(current_player->hand.size() == NUM_CARDS); // double check hand size
  assert( deck.size() >= NUM_CARDS );              // must have enough cards to exchange
  
  for(int i = 0; i < NUM_CARDS; i++)
  {
    // exchange this card
    if(to_exchange[i])
    {
      // discard this card and draw from the top of the deck
      Card discard = current_player->hand[i];
      discard_pile.push_back(discard);
      
      // draw from deck
      Card exchange = deck[0];
      deck.erase( deck.begin() );
      
      current_player->hand[i].value = exchange.value;
      current_player->hand[i].suit = exchange.suit;
    }
  }
  
  // recaulcate the hand value after exchange
  this->calculate_handvalue(current_player);
}

// ************ GAME STAGE LOGIC ***********************************************************

bool Game::min_players()
{
  return num_players >= 2;
}

bool Game::is_player_current_turn(boost::uuids::uuid& UUID)
{
  Player current_player = players[current_turn];
  
  return (current_player.UUID == UUID);
}

void Game::start_game()
{
  // only start game when it was idling
  assert(game_stage == IDLE);
  
  this->shuffle_deck();
  this->next_stage();
  
  for(int i = 0; i < NUM_CARDS; i++)
  {
    for(int j = 0; j < num_players; j++)
    {
      // draw card
      Card draw = deck[0];
      deck.erase( deck.begin() );
      
      Player* player = &players[j];
      player->hand.push_back(draw);
      
      // calcualte hand values on last card
      if(i == NUM_CARDS-1)
      {
        calculate_handvalue(player);
      }
    }
  }
}

bool Game::round_over()
{
  return agreed == num_players;
}

bool Game::is_game_over()
{
  return game_stage == END;
}

void Game::next_stage()
{
  assert(num_players >= 2);
  agreed = 0;
  game_stage++;
  current_turn = 0;
  
  // no stage after the end, stop here
  if(game_stage == END)
  {
    return;
  }
  
  
  for(int i = 0; i < num_players; i++)
  {
    Player* player = &players[i];
    player->bet_amount = 0;
    player->folded = false;
    
    // add player anties if the first betting round
    if(game_stage == BET_ROUND_1)
    {
      player->wallet--;
      prize_pot++;
    }
  }
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
      if(current_turn == num_players)
      {
        current_turn = 0;
      }
      player = &players[current_turn];
      
      if(player->folded)
      {
        agreed++;
      }
      else
      {
        break;
      }
    }
    // ignore players that have folded
    while(player->folded == true || agreed != num_players);
    
    
  }
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
    to_player["players"][i]["name"] = player->name;
    to_player["players"][i]["uuid"] = boost::lexical_cast<std::string>(player->UUID);
    to_player["players"][i]["wallet"] = player->wallet;
    to_player["players"][i]["bet_amount"] = player->bet_amount;
    to_player["players"][i]["has_bet"] = player->has_bet;
    
    // write player's hand
    for(int j = 0; j < NUM_CARDS; j++)
    {
      std::string hand_string = std::to_string( static_cast<int>(player->hand[j].suit) );
      hand_string += std::string(" ");
      hand_string += std::to_string(static_cast<int>(player->hand[j].value));
      to_player["players"][i]["cards"][j] = hand_string;
    }
  }
  
  // write the game attributes
  to_player["prize_pot"] = prize_pot;
  to_player["current_bet"] = current_bet;
  to_player["current_turn"] = current_turn;
  to_player["game_stage"] = game_stage;
  to_player["prize_pot"] = prize_pot;
}