#ifndef DEALER_H
#define DEALER_H

#pragma once

#include "player.h"
#include "game.h"
#include "card.h"
#include "json.hpp"
#include <string>
#include <vector>

/*
 * Dealer class which acts as server and manages gameplay
 * dealer.cpp to contain the main method that will act as the driver for the server
 */

class Game;

class Dealer
{
  public:
    Dealer();
    virtual ~Dealer();
           // Called when the exchange round is began, players may exchange unwanted cards here
    void process(nlohmann::json& to_dealer, nlohmann::json& to_player);
    
    bool is_game_idling(); // signal that the game is over
    void restart_game(nlohmann::json& to_player);

  private:
    Game game;
};

#endif // DEALER_H
