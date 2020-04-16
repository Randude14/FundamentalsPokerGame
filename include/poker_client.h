#ifndef POKER_GUI_CLIENT_H
#define POKER_GUI_CLIENT_H

#include "card.h"
#include "player.h"
#include "client_communicator.h"
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
 
 
#define MAX_OPPONENTS MAX_PLAYERS-1

class client_communicator;
 
 // used to store the widgets for opposing players
class opponent_display
{
  public:
    Gtk::Label *username;
    Gtk::Label *last_action;
    Gtk::Label *wallet;
    Gtk::Image *cards[NUM_CARDS];
};

class poker_client
{
  public:
    poker_client();             // Set player defaults
    virtual ~poker_client();
    
    const std::string APP_TITLE{"CSE3310 Poker++"};
    const std::string APP_NAME{"edu.uta.cse3310.poker.v0_1"};
    const std::string VERSION{"0.1"};
    
    // run the player client given the arguments
    // returns the exit status of the client window
    int run();
    
    void close(); // called when client window closes
    void connection_failed();
	
  protected:
  
    client_communicator *comm;
    
    // vector for the widgets of the other players
    // will get updated from time to time
    opponent_display* opp_displays[MAX_OPPONENTS];
    
    const std::string card_directory = "cards/";
    const std::string card_down_file = card_directory + "card_down.png";
    const std::string suits = "HSDC";
    const std::string values = " A23456789TJQKA"; // use space as offset
    
    const std::string blank_name = "Empty seat"; // for seats with no players
    
    const std::string call_action = "Call\n";    // used in switching the call/raise button
    const std::string raise_action = "Raise\n";
    
      // Callbacks for the UI widgets for the player
    
                                        // "widget name"            "widget action"
    void on_play_click();               // Player->play_button      (play a new game)
    void on_quit_click();               // Player->quit_button      (quit the game)
    void on_bet_value_changed();        // Player->bet_value_slider (player is adjusting bet)
    void on_check_click();              // Player->check_button     (no bet)
    void on_bet_click();                // Player->bet_button       (player wants to bet)
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
    Gtk::Window *main_window;
    Gtk::Button *play_button;
    Gtk::Button *quit_button;
    Gtk::Range *bet_value_slider;
    Gtk::Button *check_button;    
    Gtk::Button *bet_button;
    Gtk::Button *fold_button;
    Gtk::Button *discard_button;
    Gtk::Label *username;
    Gtk::Label *turn_status;
    
    // formats for the pot and current_labels
    const char* pot_label_format = "Pot: %ld";
    const char* current_bet_label_format = "Current Bet: %ld";
    const char* wallet_label_format = "Wallet: %ld";
    
    // pot and current_bet labels
    Gtk::Label *pot_label;
    Gtk::Label *current_bet_label;
    Gtk::Label *wallet_label;
    
    Gtk::Button *card_buttons[NUM_CARDS]; // buttons for the player cards
    Gtk::Image *cards[NUM_CARDS];         // images for the player cards
    // End referrences
    
    std::string get_card_file(Card card);      // get card image file from card
    void update_client(bool showcards=false);  // called when new info is read in from server
    void reset_sensitivity();                  // Reset button sensitivity based on game state

};

#endif // POKER_GUI_CLIENT_H
