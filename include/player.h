#ifndef PLAYER_H
#define PLAYER_H

#include "card.h"
#include <gtkmm.h>
#include <string>
#include <vector>

/*
 * Player class which acts as client and is driven by user input
 * main.cpp acts as the driver for this class
 */

class Player
{
  public:
    Player();             // Set player defaults
    virtual ~Player();
  protected:
  
    // Callbacks for the UI widgets for the player
	
	                                    // "widget name"            "widget action"
    void on_play_click();               // Player->player_button    (play a new game)
    void on_quit_click();               // Player->quit_button      (quit the game)
    void on_bet_value_changed();        // Player->bet_value_slider (player is adjusting bet)
    void on_check_click();              // Player->check_button     (no bet)
    void on_bet_click();                // Player->bet_button       (player wants to bet)
    void on_call_click();               // Player->call_button      (match current bet)
    void on_raise_click();              // Player->raise_button     (increase current bet)
    void on_fold_click();               // Player->fold_button      (drop out of hand)
    void on_discard_click();            // Player->discard_button   (exchange 0-3 Cards)
	// End callbacks
	
  private:
    int UUID;
    int table_position;
    std::string name;
    double wallet;
    std::vector< Card * > hand;
    double bet_amount;
    bool has_bet;
	
	// Referrences for the UI widgets
    Gtk::ToolButton play_button;
    Gtk::ToolButton quit_button;
    Gtk::Range *bet_value_slider;
    Gtk::ToolButton *check_button;    
    Gtk::ToolButton *bet_button;
    Gtk::ToolButton *call_button;
    Gtk::ToolButton *raise_button;
    Gtk::ToolButton *fold_button;
    Gtk::ToolButton *discard_button;
	// End referrences
	
    void reset_sensitivity();           // Reset button sensitivity based on game state

};

#endif // PLAYER_H
