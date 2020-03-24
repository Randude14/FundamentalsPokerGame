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
    exit(1);
  }
  
  // this connects all callback functions to the buttons from glade...
  gtk_builder_connect_signals(refBuilder->gobj(), NULL);
  
  // get the window object from the referrence builder
  refBuilder->get_widget("Poker++", win);
  
  // set title....gtk apparently changes the title. No biggie
  win->set_title(APP_TITLE);
  
  
  // hook in widgets
  refBuilder->get_widget("bet_slider", bet_value_slider);
  refBuilder->get_widget("check",      check_button);
  refBuilder->get_widget("bet",        check_button);
  refBuilder->get_widget("call",       call_button);
  refBuilder->get_widget("raise",      raise_button);
  refBuilder->get_widget("fold",       fold_button);
  refBuilder->get_widget("discard",    discard_button);
  
  for(int i = 0; i < NUM_CARDS; i++)
  {
    std::string id = "card" + std::to_string(i+1);
	refBuilder->get_widget(id, card_images[i]);
  }
  
  // run the window and return the exit status
  return app->run(*win);
}

void Player::on_play_click()
{

}

void Player::on_quit_click()
{

}

void Player::on_bet_value_changed()
{

}

void Player::on_check_click()
{

}

void Player::on_bet_click()
{

}

void Player::on_call_click()
{

}

void Player::on_raise_click()
{

}

void Player::on_fold_click()
{

}

void Player::on_discard_click()
{

}
