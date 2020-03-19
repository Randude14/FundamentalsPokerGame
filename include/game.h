#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "card.h"
#include <string>
#include <vector>

const std::string APP_TITLE{"CSE3310 Poker++"};
const std::string APP_NAME{"edu.uta.cse3310.poker.v0_1"};
const std::string VERSION{"0.1"};

/*
 * Game class to maintain game state
 */

class Game
{
  public:
    Game(std::string name);
    virtual ~Game();
    void player_join(Player player);
    void check();
    void bet(double amount);
    void call(double amount);
    void raise(double amount);
    void fold();
    void next_stage();
  private:
//    Dealer dealer;
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
