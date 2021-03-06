#ifndef GAME_H
#define GAME_H

#pragma once

#include <algorithm>
#include <random>
#include <boost/uuid/uuid.hpp>
#include <boost/lexical_cast.hpp>
#include "player.h"
#include "card.h"
#include "json.hpp"
#include <string>
#include <vector>

/*
 * Game class to maintain game state
 */

#define NORMAL_DECK_SIZE (4 * 13)
#define MAX_PLAYERS 5
#define MAX_OPPONENTS MAX_PLAYERS-1

class Player;

enum { IDLE           = 0,
       BET_ROUND_1    = 1, 
       EXCHANGE_ROUND = 2, 
       BET_ROUND_2    = 3, 
       END            = 4
       };

class Game
{
  public:
    Game();
    virtual ~Game();
    bool is_current_player(std::string&);  // Returns if the uuid belogns to the player that has the current turn
    bool player_join(Player player);    // Called when each player joins the game
    bool min_players();                 // returns whether there are minimum players
    bool game_over();                   // return whether the current game is over
    bool is_idle();                     // returns whether the game is idling...
    void start_game();                  // starts the game....game_stage must be IDLE
    void check();                       // Called when a player chooses to check (no bet)
    void bet(double amount);            // Called when a player chooses to bet or raise the bet
    void raise(double amount);          // Called when a player chooses to bet or raise the bet
    void end_game();                    // called by dealer to end game, NOTE: is_game_over() must be true
    void call();                        // Called when a player chooses to call (match bet)
    void exchange(bool to_exchange[NUM_CARDS]); // Called when player wants to exchange cards
    void fold();                        // Called when a player chooses to fold (forfeit)
    
    // used to write the game state to the json that goes
    // out to all players    
    void write_game_state(nlohmann::json& to_player);
    double get_prize_pot();
    std::vector<Player>& get_players();
    
    bool round_over();                  // Returns when the current round is over
    void next_stage();                  // Called to move from (0) 
                                        //                     (1)  Betting round 1 ->
                                        //                     (2)  Exchange cards ->
                                        //                     (3)  Betting round 2 ->
                                        //                     (4)  End
  
 private:
    void determine_winners();            // determine who the winner is...may be more than one player
    std::vector< Player > players;
    std::vector< int > winners;          // list of the winner indexes. Indexes are used to get the referrences from players
    std::vector< Card > deck;
    std::vector< Card > discard_pile;
    std::string game_comment;            // tells that status of the game
    double prize_pot;
    double current_bet;
    int current_turn;
    int game_stage;
    unsigned int agreed;
    unsigned int folded;
    bool showcards;
    
    // struct used to sort the winner array indexes
    // this is used so that class fields may be called withi
    struct winner_comparer
    {
      winner_comparer(Game* g) : game(g) { }
      Game* game;
      
      bool operator() (int p1_index, int p2_index)
      {
          Player* p1 = &game->players[p1_index];
          Player* p2 = &game->players[p2_index];
          return game->compare_hands(p1, p2) > 0;
      }
      
    } typedef winner_comparer;
  
    std::default_random_engine rng;   // random engine used to shuffle the deck
    void shuffle_deck();              // shuffle the deck with a random engine
    bool validate_deck();             // validate the deck to make sure it has all cards
    void calculate_handvalue(Player* player); // calculate hand_value
    /*
      Description: compares two hands of different players
      Returns: -1 if p2 hand ranking > p1 hand ranking, 1 for p1 > p2, and 0 for tie
    */
    int compare_hands(Player*, Player* ); // compare the hands and hand rankings of two players
    
    void next_player();               // moves index to next player
};

#endif // GAME_H
