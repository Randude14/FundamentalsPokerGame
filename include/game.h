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
    bool is_player_current_turn(boost::uuids::uuid&);  // Returns if the uuid belogns to the player that has the current turn
    bool player_join(Player player);    // Called when each player joins the game
    bool min_players();
    bool is_game_over();
    void start_game();
    void check();                       // Called when a player chooses to check (no bet)
    void bet(double amount);            // Called when a player chooses to bet or raise the bet
    void raise(double amount);            // Called when a player chooses to bet or raise the bet
    void call();                        // Called when a player chooses to call (match bet)
    void exchange(bool to_exchange[NUM_CARDS]); // Called when player wants to exchange cards
    void fold();                        // Called when a player chooses to fold (forfeit)
    bool round_over();                  // Returns when the current round is over
    void next_player();
    void next_stage();                  // Called to move from (0) 
                                        //                     (1)  Betting round 1 ->
                                        //                     (2)  Exchange cards ->
                                        //                     (3)  Betting round 2 ->
                                        //                     (4)  End
                                        
    // used to write the game state to the json that goes
    // out to all players    
    void write_game_state(nlohmann::json& to_player);
    std::vector< Card > deck;
    std::vector< Card > discard_pile;
    double prize_pot;
    double current_bet;
    Player players[5];
    int current_turn;
    int game_stage;
    int agreed;
    int num_players;
    
  private:
    std::default_random_engine rng;   // random engine used to shuffle the deck
    void shuffle_deck();              // shuffle the deck with a random engine
    bool validate_deck();             // validate the deck to make sure it has all cards
    void calculate_handvalue(Player* player); // calculate hand_value
};

#endif // GAME_H
