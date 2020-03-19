#ifndef GAME_H
#define GAME_H

#include "dealer.h"
#include "player.h"
#include "deck.h"
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
    void player_join(Player player);
    void check();
    void bet(double amount);
    void call(double amount);
    void raise(double amount);
    void fold();
    void next_stage();
  private:
    Dealer dealer;
    Deck deck;
    std::vector< Card * > discard_pile;
    double prize_pot;
    double current_bet;
    std::vector< Player * > players;
    int current_turn;
    int game_stage;
    bool bets_agree;
};

#endif // GAME_H
