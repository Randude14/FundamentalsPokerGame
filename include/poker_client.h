#ifndef POKER_GUI_CLIENT_H
#define POKER_GUI_CLIENT_H

#include "card.h"
#include "player.h"
#include <gtkmm.h>
#include <string>
#include <vector>

/*
 * Player class which acts as client and is driven by user input
 * main.cpp acts as the driver for this class
 */
 
 /*
 * Macros for application metadata
 */
const std::string APP_TITLE{"CSE3310 Poker++"};
const std::string APP_NAME{"edu.uta.cse3310.poker.v0_1"};
const std::string VERSION{"0.1"};

// max players that the client will play against
#define MAX_OPPONENTS 4

class poker_client
{
  public:
    poker_client();             // Set player defaults
    virtual ~poker_client();
	
  struct opponent_display
  {
    Gtk::Label *username;
    Gtk::Label *last_action;
    Gtk::Image *cards[NUM_CARDS];
  };
	
	// run the player client given the arguments
	// returns the exit status of the client window
	int run(int argc, char* argv[]);
	
  protected:
  
    // player that this gui is taking input from
    Player *player;
	
	// array for the other players in the game
	Player *opponents; 
	
	// vector for the widgets of the other players
	// will get updated from time to time
	opponent_display *opp_displays;
	
	const std::string card_directory = "cards/";
	const std::string card_down_file = card_directory + "card_down.png";
    const std::string SUITS = "HSDC";
    const std::string VALUES = " A23456789TJQK"; // use space as offset
  
    // Callbacks for the UI widgets for the player
	
	                                    // "widget name"            "widget action"
    void on_play_click();               // Player->play_button      (play a new game)
    void on_quit_click();               // Player->quit_button      (quit the game)
    void on_bet_value_changed();        // Player->bet_value_slider (player is adjusting bet)
    void on_check_click();              // Player->check_button     (no bet)
    void on_bet_click();                // Player->bet_button       (player wants to bet)
    void on_call_click();               // Player->call_button      (match current bet)
    void on_raise_click();              // Player->raise_button     (increase current bet)
    void on_fold_click();               // Player->fold_button      (drop out of hand)
    void on_discard_click();            // Player->discard_button   (exchange 0-3 Cards)
	
	void on_hand_click_1();             // Player->card_images      (buttons that hold the player cards)
	void on_hand_click_2();
	void on_hand_click_3();
	void on_hand_click_4();
	void on_hand_click_5();
	
	// End callbacks
	
	const std::string facedown = "cards/card_down.png";
	
  private:
	
	// References for the UI widgets
    Gtk::Button *play_button;
    Gtk::Button *quit_button;
    Gtk::Range *bet_value_slider;
    Gtk::Button *check_button;    
    Gtk::Button *bet_button;
    Gtk::Button *call_button;
    Gtk::Button *fold_button;
    Gtk::Button *discard_button;
	Gtk::Label *username;
	Gtk::Label *turn_status;
	
	// formats for the pot and current_labels
	std::string pot_label_format = "Pot: %.0f";
	std::string current_bet_label_format = "Current Bet: %.0f";
	
	// pot and current_bet labels
	Gtk::Label *pot_label;
	Gtk::Label *current_bet_label;
	
	Gtk::Button *card_buttons[NUM_CARDS]; // buttons for the player cards
	Gtk::Image *cards[NUM_CARDS];         // images for the player cards
	// End referrences
	
	
    void reset_sensitivity();           // Reset button sensitivity based on game state

};

#endif // POKER_GUI_CLIENT_H
