#ifndef DEALER_H
#define DEALER_H

#include"player.h"
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
 class Player;

#define MAX_PLAYERS 5

class Dealer
{
  public:
    Dealer();
    virtual ~Dealer();
    void exchange(std::vector<Card>& deck, Player& p);
           // Called when the exchange round is began, players may exchange unwanted cards here
    void process(nlohmann::json& to_dealer, nlohmann::json& to_player);

  private:
    Game* game;                // Reference for the game that will handle main game functionality
    std::vector<Card> shuffle(Game& g);            // Called to shuffle cards between rounds
    void deal(std::vector<Card>& temp,Game& g);               // Called to deal out cards to players
    void determine_winner(Game& g);   // Dealer then needs to determine winner based on the player's hands
};

#endif // DEALER_H
