#ifndef PLAYER_H
#define PLAYER_H

#include "card.h"
#include <gtkmm.h>
#include <string>
#include <vector>

/*
 * Player class which acts as client and is driven by user input
 */

class Player
{
  public:
    Player();
    virtual ~Player();
  protected:
    void on_play_click();               // Play a new game
    void on_quit_click();               // Quit the game
    void on_bet_value_changed();        // Player->Bet Amount Slider
    void on_check_click();              // Player->Check (no bet)
    void on_bet_click();                // Player->Bet
    void on_call_click();               // Player->Call (match current bet)
    void on_raise_click();              // Player->Raise (increase current bet)
    void on_fold_click();               // Player->Fold (drop out of hand)
    void on_discard_click();            // Player->Discard (Exchange 0-3 Cards)
  private:
    int UUID;
    int table_position;
    std::string name;
    double wallet;
    std::vector< Card * > hand;
    double bet_amount;
    bool has_bet;
    Gtk::ToolButton play_button;        // Play a new game
    Gtk::ToolButton quit_button;        // Quit the game
    Gtk::Range *bet_value_slider;       // Player->Bet Amount Slider
    Gtk::ToolButton *check_button;      // Player->Check
    Gtk::ToolButton *bet_button;        // Player->Bet
    Gtk::ToolButton *call_button;       // Player->Call
    Gtk::ToolButton *raise_button;      // Player->Raise
    Gtk::ToolButton *fold_button;       // Player->Fold
    Gtk::ToolButton *discard_button;    // Player->Discard (Exchange Cards)
    void reset_sensitivity();           // Reset button sensitivity based on game state};

};

#endif // PLAYER_H
