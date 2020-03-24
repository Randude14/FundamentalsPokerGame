#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "card.h"
#include <string>
#include <vector>

/*
 * Game class to maintain game state
 */

class Game
{
  public:
    Game(std::string name);
    virtual ~Game();
    void player_join(Player player);    // Called when each player joins the game
    void check();                       // Called when a player chooses to check (no bet)
    void bet(double amount);            // Called when a player chooses to bet
    void call(double amount);           // Called when a player chooses to call (match bet)
    void raise(double amount);          // Called when a player chooses to raise (increase bet)
    void fold();                        // Called when a player chooses to fold (forfeit)
    void next_stage();                  // Called to move from (0) Deal ->
                                        //                     (1) Betting round 1 ->
                                        //                     (2) Exchange cards ->
                                        //                     (3) Betting round 2 ->
                                        //                     (4) End
  private:
    std::vector< Card * > deck;
    std::vector< Card * > discard_pile;
    double prize_pot;
    double current_bet;
    std::vector< Player * > players;
    int current_turn;
    int game_stage;
    bool bets_agree;
};

#endif // GAME_H
