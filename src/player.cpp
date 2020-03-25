#include <gtkmm.h>
#include <iostream>

#include "player.h"
#include "json.hpp"


// ///////////////////////////////////////////////
// C O N S T R U C T O R S / D E S T R U C T O R S
// ///////////////////////////////////////////////

Player::Player()
{
  wallet = 100.0;
  bet_amount = 0.0;
}

Player::~Player() { }


// Called by main
int Player::run(int argc, char* argv[])
{
  auto app = Gtk::Application::create(argc, argv, APP_NAME);
  
  // create window object pointer
  Gtk::Window* win;
  
  //Load the GtkBuilder file and instantiate its widgets:
  auto refBuilder = Gtk::Builder::create();
  
  try
  {
  	// load glade file
  	refBuilder->add_from_file("interface.glade");
  }
  catch(const std::exception& ex)
  {
    std::cerr << "Error: " << ex.what() << std::endl;
    return 1;
  }
  
  // get the window object from the referrence builder
  refBuilder->get_widget("Poker++", win);
  
  // set title....gtk apparently changes the title. No biggie
  win->set_title(APP_TITLE);  
  
  // Hook in widgets
  // macro finds widget with ID and sets to VAR while connecting the button with FUNC
  #define GET_AND_CONNECT(ID, VAR, FUNC)  \
    refBuilder->get_widget(ID, VAR);      \
	VAR->signal_clicked().connect(sigc::mem_fun(*this, FUNC));
	
  // connect player control buttons
  GET_AND_CONNECT( "check",     check_button,   &Player::on_check_click )
  GET_AND_CONNECT( "bet_raise", bet_button,     &Player::on_bet_click )
  GET_AND_CONNECT( "call",      call_button,    &Player::on_call_click )
  GET_AND_CONNECT( "fold",      fold_button,    &Player::on_fold_click )
  GET_AND_CONNECT( "discard",   discard_button, &Player::on_discard_click )
  
  refBuilder->get_widget("bet_slider", bet_value_slider);
  bet_value_slider->signal_value_changed().connect( sigc::mem_fun(*this, &Player::on_bet_value_changed) );
  
  // images don't need connecting to a signal handler
  for(int i = 0; i < NUM_CARDS; i++)
  {
    std::string id = "card" + std::to_string(i+1);
	refBuilder->get_widget(id, card_images[i]);
  }
  
  // connect hand buttons
  GET_AND_CONNECT( "hand1", card_buttons[0], &Player::on_hand_click_1 )
  GET_AND_CONNECT( "hand2", card_buttons[1], &Player::on_hand_click_2 )
  GET_AND_CONNECT( "hand3", card_buttons[2], &Player::on_hand_click_3 )
  GET_AND_CONNECT( "hand4", card_buttons[3], &Player::on_hand_click_4 )
  GET_AND_CONNECT( "hand5", card_buttons[4], &Player::on_hand_click_5 )
  
  
  // TODO: disable player control buttons
  
  // Run the window and return the exit status
  return app->run(*win);
}

// Use macros to shorten the need for multiple methods 
// that do the same thing, minus the number....
#define HAND_CLICK_METHODS(NUM)                                   \
void  Player::on_hand_click_##NUM()                               \
{                                                                 \
  std::cout << "Hand " << NUM << " was clicked!" << std::endl;    \
}              

/*
   TODO: Eventual idea is to make the cards flip when their buttons are clicked
     flipped cards on their back will indicate to the system that the player wants to
     trade those cards in
*/
HAND_CLICK_METHODS(1)
HAND_CLICK_METHODS(2)
HAND_CLICK_METHODS(3)
HAND_CLICK_METHODS(4)
HAND_CLICK_METHODS(5)

void Player::on_play_click()
{
  std::cout << "Check button clicked!" << std::endl;
}

void Player::on_quit_click()
{
  std::cout << "Check button clicked!" << std::endl;
}

void Player::on_bet_value_changed()
{
  std::cout << "Player changed their bet to " << bet_value_slider->get_value() << "!" << std::endl;
}

void Player::on_check_click()
{
  std::cout << "Check button clicked!" << std::endl;
}

void Player::on_bet_click()
{
  std::cout << "Bet button clicked!" << std::endl;
}

void Player::on_call_click()
{
  std::cout << "Call button clicked!" << std::endl;
}

void Player::on_fold_click()
{
  std::cout << "Fold button clicked!" << std::endl;
}

void Player::on_discard_click()
{
  std::cout << "Discard button clicked!" << std::endl;
}
