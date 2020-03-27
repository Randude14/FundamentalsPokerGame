#include <gtkmm.h>
#include <iostream>

#include "PokerGuiClient.h"
#include "player.h"
#include "json.hpp"

// **************************** MAIN *****************************
int main(int argc, char* argv[])
{
	std::cout << "Starting the " << APP_TITLE << " client for version " << VERSION << "...." << std::endl;
	
	// client object and run
	PokerGuiClient *client = new PokerGuiClient();
	int status = client->run(argc, argv);
	
	// free memory and return the exit status of the client
	std::cout << "Client has been closed." << std::endl;
	delete client;
	return status;
}




// ///////////////////////////////////////////////
// C O N S T R U C T O R S / D E S T R U C T O R S
// ///////////////////////////////////////////////

// Don't need to do anything for constructors currently
PokerGuiClient::PokerGuiClient() { }

PokerGuiClient::~PokerGuiClient() { }


// Called by main
// TODO: using the args, connect to dealer
int PokerGuiClient::run(int argc, char* argv[])
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
	
  // connect PokerGuiClient control buttons
  GET_AND_CONNECT( "check",     check_button,   &PokerGuiClient::on_check_click )
  GET_AND_CONNECT( "bet_raise", bet_button,     &PokerGuiClient::on_bet_click )
  GET_AND_CONNECT( "call",      call_button,    &PokerGuiClient::on_call_click )
  GET_AND_CONNECT( "fold",      fold_button,    &PokerGuiClient::on_fold_click )
  GET_AND_CONNECT( "discard",   discard_button, &PokerGuiClient::on_discard_click )
  
  // get range and connect the value changed method
  refBuilder->get_widget("bet_slider", bet_value_slider);
  bet_value_slider->signal_value_changed().connect( sigc::mem_fun(*this, &PokerGuiClient::on_bet_value_changed) );
  bet_value_slider->set_range(0, player.wallet); // set range based on initial player wallet
  
  // connect hand buttons
  GET_AND_CONNECT( "hand1", card_buttons[0], &PokerGuiClient::on_hand_click_1 )
  GET_AND_CONNECT( "hand2", card_buttons[1], &PokerGuiClient::on_hand_click_2 )
  GET_AND_CONNECT( "hand3", card_buttons[2], &PokerGuiClient::on_hand_click_3 )
  GET_AND_CONNECT( "hand4", card_buttons[3], &PokerGuiClient::on_hand_click_4 )
  GET_AND_CONNECT( "hand5", card_buttons[4], &PokerGuiClient::on_hand_click_5 )
  
  
  // TODO: disable PokerGuiClient control buttons
  
  // Run the window and return the exit status
  return app->run(*win);
}

// Use macros to shorten the need for multiple methods 
// that do the same thing, minus the number....
#define HAND_CLICK_METHODS(NUM)                                   \
void  PokerGuiClient::on_hand_click_##NUM()                       \
{                                                                 \
  std::cout << "Hand " << NUM << " was clicked!" << std::endl;    \
}              

/*
   TODO: Eventual idea is to make the cards flip when their buttons are clicked
     flipped cards on their back will indicate to the system that the PokerGuiClient wants to
     trade those cards in
*/
HAND_CLICK_METHODS(1)
HAND_CLICK_METHODS(2)
HAND_CLICK_METHODS(3)
HAND_CLICK_METHODS(4)
HAND_CLICK_METHODS(5)

void PokerGuiClient::on_play_click()
{
  std::cout << "Check button clicked!" << std::endl;
}

void PokerGuiClient::on_quit_click()
{
  std::cout << "Check button clicked!" << std::endl;
}

void PokerGuiClient::on_bet_value_changed()
{
  std::cout << "PokerGuiClient changed their bet to " << bet_value_slider->get_value() << "!" << std::endl;
}

void PokerGuiClient::on_check_click()
{
  std::cout << "Check button clicked!" << std::endl;
}

void PokerGuiClient::on_bet_click()
{
  std::cout << "Bet button clicked!" << std::endl;
}

void PokerGuiClient::on_call_click()
{
  std::cout << "Call button clicked!" << std::endl;
}

void PokerGuiClient::on_fold_click()
{
  std::cout << "Fold button clicked!" << std::endl;
}

void PokerGuiClient::on_discard_click()
{
  std::cout << "Discard button clicked!" << std::endl;
}
