#include <gtkmm.h>
#include <iostream>

#include "poker_client.h"
#include "player.h"
#include "card.h"
#include "json.hpp"

// **************************** MAIN *****************************
int main(int argc, char* argv[])
{
	std::cout << "Starting the " << APP_TITLE << " client for version " << VERSION << "...." << std::endl;
	
	// client object and run
	poker_client *client = new poker_client();
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
poker_client::poker_client() { }

poker_client::~poker_client() { }


// Called by main
// TODO: using the args, connect to dealer
int poker_client::run(int argc, char* argv[])
{
  auto app = Gtk::Application::create(argc, argv, APP_NAME);
  
  // create window object pointer
  Gtk::Window* win;
  
  // Create the builder
  auto refBuilder = Gtk::Builder::create();
  
  try
  {
  	// load glade file using the builder
  	refBuilder->add_from_file("interface.glade");
  }
  catch(const std::exception& ex)
  {
    std::cerr << "Error: " << ex.what() << std::endl;
    return 1;
  }
  
  // get the window object from the referrence builder
  refBuilder->get_widget("Poker++", win);
  
  // set title....the builder apparently changes the title from the glade file. No biggie
  win->set_title(APP_TITLE);  
  
  player = new Player();
  
  opp_displays = (opponent_display*) malloc(sizeof(opponent_display) * MAX_OPPONENTS);
  opponents = (Player*) malloc(sizeof(Player) * MAX_OPPONENTS);
  
  // Hook in widgets
  // macro finds widget with ID and sets to VAR while connecting its callback to FUNC
  #define GET_AND_CONNECT(ID, VAR, FUNC)  \
    refBuilder->get_widget(ID, VAR);      \
	VAR->signal_clicked().connect(sigc::mem_fun(*this, FUNC));
	
  // connect poker_client control buttons
  GET_AND_CONNECT( "check",     check_button,   &poker_client::on_check_click )
  GET_AND_CONNECT( "bet_raise", bet_button,     &poker_client::on_bet_click )
  GET_AND_CONNECT( "call",      call_button,    &poker_client::on_call_click )
  GET_AND_CONNECT( "fold",      fold_button,    &poker_client::on_fold_click )
  GET_AND_CONNECT( "discard",   discard_button, &poker_client::on_discard_click )
  
  // get range and connect the value changed method
  refBuilder->get_widget("bet_slider", bet_value_slider);
  bet_value_slider->signal_value_changed().connect( sigc::mem_fun(*this, &poker_client::on_bet_value_changed) );
  bet_value_slider->set_range(0, player->wallet); // set range based on initial player wallet
  
  // connect hand buttons
  GET_AND_CONNECT( "hand1", card_buttons[0], &poker_client::on_hand_click_1 )
  GET_AND_CONNECT( "hand2", card_buttons[1], &poker_client::on_hand_click_2 )
  GET_AND_CONNECT( "hand3", card_buttons[2], &poker_client::on_hand_click_3 )
  GET_AND_CONNECT( "hand4", card_buttons[3], &poker_client::on_hand_click_4 )
  GET_AND_CONNECT( "hand5", card_buttons[4], &poker_client::on_hand_click_5 )
  
  // find card images
  for(int i = 0; i < NUM_CARDS; i++)
  {
    refBuilder->get_widget(std::string("card") + std::to_string(i+1), cards[i]);
	cards[i]->set(card_down_file);
  }
  
  // set default values for player's hand....these will be removed later
  Card c1{Card_value::TEN, Suit::CLUB};
  Card c2{Card_value::JACK, Suit::CLUB};
  Card c3{Card_value::QUEEN, Suit::CLUB};
  Card c4{Card_value::KING, Suit::CLUB};
  Card c5{Card_value::ACE, Suit::CLUB};
  player->hand[0] = c1;
  player->hand[1] = c2;
  player->hand[2] = c3;
  player->hand[3] = c4;
  player->hand[4] = c5;
  
  // grab labels
  refBuilder->get_widget("username", username);
  refBuilder->get_widget("turn_status", turn_status);
  refBuilder->get_widget("pot_label", pot_label);
  refBuilder->get_widget("current_bet_label", current_bet_label);
  
  
  
  
  for(int i = 1; i <= MAX_OPPONENTS; i++)
  {
	std::string player = std::string("p") + std::to_string(i) + std::string("_");
	
    refBuilder->get_widget(player + std::string("name"), opp_displays[i-1].username);
	
    for(int j = 1; j <= NUM_CARDS; j++)
    {
      refBuilder->get_widget(player + std::string("card") + std::to_string(j), opp_displays[i-1].cards[j-1]);
	  opp_displays[i-1].cards[j-1]->set(card_down_file);
    }
	  
	refBuilder->get_widget(player + std::string("last_action"), opp_displays[i-1].last_action);
  }
  
  // TODO: disable poker_client control buttons
  
  // Run the window
  int ret = app->run(*win);
  
  for(int i = 0; i < MAX_OPPONENTS; i++)
  {
      opp_displays[i].username = NULL;
      opp_displays[i].last_action = NULL;  
	  
      for(int j = 0; j < NUM_CARDS; j++)
        opp_displays[i].cards[j] = NULL;
  }
  
  // free memory
  delete player;
  free(opp_displays);
  free(opponents);

  // return exit status
  return ret;
}

// Allows

// Use macros to shorten the need for multiple methods 
// that do the same thing, minus the number....
#define HAND_CLICK_METHODS(NUM)                                 \
void  poker_client::on_hand_click_##NUM()                       \
{                                                               \
  std::cout << "Hand " << NUM << " was clicked!" << std::endl;  \
  if(cards[NUM-1]->property_file() == card_down_file)           \
  {                                                             \
    Card card = player->hand[NUM-1];                            \
	int suit = (int)card.get_suit();                            \
	int value = (int)card.get_value();                          \
    std::string image = "cards/";                               \
    image.push_back( VALUES[value] );	                        \
    image.push_back( SUITS[suit] );	                            \
	image += ".png";                                            \
	cards[NUM-1]->set(image);                                   \
  }                                                             \
  else                                                          \
  {                                                             \
    cards[NUM-1]->set(card_down_file);                          \
  }                                                             \
}              

/*
   TODO: Eventual idea is to make the cards flip when their buttons are clicked
     flipped cards on their back will indicate to the system that the poker_client wants to
     trade those cards in
*/
HAND_CLICK_METHODS(1)
HAND_CLICK_METHODS(2)
HAND_CLICK_METHODS(3)
HAND_CLICK_METHODS(4)
HAND_CLICK_METHODS(5)

void poker_client::on_play_click()
{
  std::cout << "Check button clicked!" << std::endl;
}

void poker_client::on_quit_click()
{
  std::cout << "Check button clicked!" << std::endl;
}

void poker_client::on_bet_value_changed()
{
  std::cout << "poker_client changed their bet to " << bet_value_slider->get_value() << "!" << std::endl;
}

void poker_client::on_check_click()
{
  std::cout << "Check button clicked!" << std::endl;
}

void poker_client::on_bet_click()
{
  std::cout << "Bet button clicked!" << std::endl;
}

void poker_client::on_call_click()
{
  std::cout << "Call button clicked!" << std::endl;
}

void poker_client::on_fold_click()
{
  std::cout << "Fold button clicked!" << std::endl;
}

void poker_client::on_discard_click()
{
  std::cout << "Discard button clicked!" << std::endl;
}
