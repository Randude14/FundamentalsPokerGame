#include <gtkmm.h>
#include <iostream>
#include <signal.h>

#include "poker_client.h"
#include "play_window.h"
#include "player.h"
#include "card.h"



poker_client *client = NULL;

// cntr-c handler
void HandleControlC(int s)
{
  if(client != NULL)
  {
    client->close();
    delete client;
    client = NULL;
    exit(1);
  }
}


// **************************** MAIN *****************************
int main(int argc, char* argv[])
{
  
  // catch a control-c force close
  struct sigaction control_c_handler;

  control_c_handler.sa_handler = HandleControlC;
  sigemptyset(&control_c_handler.sa_mask);
  control_c_handler.sa_flags = 0;

  sigaction(SIGINT, &control_c_handler, NULL);
  
  // client object and run
  client = new poker_client();
  int status = client->run();
  
  delete client;
  client = NULL;

  return status;
}



// ///////////////////////////////////////////////
// C O N S T R U C T O R S / D E S T R U C T O R S
// ///////////////////////////////////////////////

// Don't need to do anything for constructors currently
poker_client::poker_client() { }

poker_client::~poker_client() { }


// Called by main
int poker_client::run()
{
  
  auto login_app = Gtk::Application::create("poker.login.window");
  
  play_window* play_quit_window = new play_window();
   
  login_app->run(*play_quit_window);
  // grab info from play_quit_window before freeing memory
  std::string playername = play_quit_window->username;
  std::string host = play_quit_window->host;
  std::string port = play_quit_window->port;
  bool should_quit = play_quit_window->should_quit;
  
  delete play_quit_window;
   
  // user clicked quit
  if(should_quit)
  {
    return 0;
  }
   
  // if play was clicked, continue as is....
  
  std::cout << "Starting the " << APP_TITLE << " client for version " << VERSION << "...." << std::endl;
  
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
  
  // get the widget at ID for variable VAR
  // if widget was not found, return 1 and alert system
  #define GET_WIDGET(ID, VAR)             \
    refBuilder->get_widget(ID, VAR);      \
    if(VAR == nullptr) { std::cerr << "Failed to find widget for " << VAR << std::endl; return 1; }
  
  // Hook in widgets
  // macro finds widget with ID and sets to VAR while connecting its callback to FUNC
  #define GET_AND_CONNECT(ID, VAR, FUNC)  \
    GET_WIDGET(ID, VAR)                          \
    VAR->signal_clicked().connect(sigc::mem_fun(*this, FUNC));
    
  // get the window object from the referrence builder
  GET_WIDGET("Poker++", main_window);
  
  // set title....the builder apparently changes the title from the glade file. No biggie
  main_window->set_title(APP_TITLE);  
  
  // create communicator
  comm = new client_communicator(this, host, port);
 
  comm->current_bet = 10;
  comm->pot = 5;
  
  // get main player
  Player* player = comm->players[comm->main_player];
  player->name = playername;
  player->wallet = 100;
    
  // connect poker_client control buttons
  GET_AND_CONNECT( "check",     check_button,   &poker_client::on_check_click )
  GET_AND_CONNECT( "bet",       bet_button,     &poker_client::on_bet_click )
  GET_AND_CONNECT( "fold",      fold_button,    &poker_client::on_fold_click )
  GET_AND_CONNECT( "discard",   discard_button, &poker_client::on_discard_click )
  
  // get range and connect the value changed method
  GET_WIDGET("bet_slider", bet_value_slider)
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
    GET_WIDGET(std::string("card") + std::to_string(i+1), cards[i])
    card_buttons[i]->set_sensitive(false);
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
  GET_WIDGET("username", username)
  GET_WIDGET("turn_status", turn_status)
  GET_WIDGET("pot_label", pot_label)
  GET_WIDGET("current_bet_label", current_bet_label)
  GET_WIDGET("wallet_label", wallet_label)
  
  // get opponent widgets
  for(int i = 0; i < MAX_OPPONENTS; i++)
  {
    std::string player = std::string("p") + std::to_string(i+1) + std::string("_");
    opp_displays[i] = new opponent_display();
    auto opp_display = opp_displays[i];
    
    // get username label
    GET_WIDGET(player + std::string("name"), opp_display->username)
    
    // get username label
    GET_WIDGET(player + std::string("wallet"), opp_display->wallet)
    
    // get card images
    for(int j = 1; j <= NUM_CARDS; j++)
    {
      GET_WIDGET(player + std::string("card") + std::to_string(j), opp_display->cards[j-1])
      opp_display->cards[j-1]->set(card_down_file);
    }
      
    // get last action label
    GET_WIDGET(player + std::string("last_action"), opp_display->last_action)
  }
  
  // TODO: disable poker_client control buttons
  update_client();
  
  // Run the window
  auto main_app = Gtk::Application::create(APP_NAME);
  int ret = main_app->run(*main_window);
  
  this->close();

  // return exit status
  return ret;
}


// ***** CLOSE CLIENT **********
void poker_client::close()
{
  comm->close();
  
  for(int i = 0; i < MAX_OPPONENTS; i++)
    delete opp_displays[i];
  
  delete comm;
  comm = NULL;
  
  // free memory and return the exit status of the client
  std::cout << std::endl << "Client has been closed." << std::endl;
}

// called by the communication thread when a connection failed
// close the main window, this will trigger the app->run() to terminate and clean exit
void poker_client::connection_failed()
{
  main_window->close();
}


std::string poker_client::get_card_file(Card card)
{
  std::string image_file = card_directory;
  image_file.push_back( values[ static_cast<int>(card.value) ] );
  image_file.push_back( suits[ static_cast<int>(card.suit) ] );
  image_file += ".png";   
  
  return image_file;
}


// updatet the client GUI....eventually to be called by client_communicator
void poker_client::update_client(bool showcards)
{
  int od_index = 0;
  char buffer[50]; // used to format labels...
  
  for(int i = 0; i < MAX_PLAYERS; i++) // update opponents info
  {
    // update main player's info
    if(i == comm->main_player)
    {
      Player* player = comm->players[ comm->main_player ];
      
      username->set_label(player->name); // update username
  
      for(int i = 0; i < NUM_CARDS; i++) // update cards
      {
        std::string image_file = get_card_file(player->hand[i]);
        cards[i]->set(image_file);
        card_buttons[i]->set_sensitive(true);
      } 
    }
    
    // update opponent's info
    else if(i < comm->num_players)
    {
      auto od = opp_displays[od_index++];
      Player* opp = comm->players[i];

      od->username->set_label(opp->name);
      
      
      
      for(int j = 0; j < NUM_CARDS; j++) // update cards
      {
        std::string image_file = (showcards) ? get_card_file(opp->hand[j]) : card_down_file;
        od->cards[i]->set(image_file);
      }   
    }
    
    // this means we are beyond amount of players...fill in with blank info
    else
    {
      auto od = opp_displays[od_index++];
      
      od->username->set_label(blank_name);
      od->last_action->set_label("");
      od->wallet->set_label("");
      for(int j = 0; j < NUM_CARDS; j++) // update cards
      {
        od->cards[i]->set(card_down_file);
      }  
    }
  }
  
  Player* main_player = comm->players[ comm->main_player ];
  
  // update wallet label
  sprintf(buffer, wallet_label_format, main_player->wallet);
  wallet_label->set_label(buffer);
  
  // update wallet label
  sprintf(buffer, pot_label_format, comm->pot);
  pot_label->set_label(buffer);
  
  // update wallet label
  sprintf(buffer, current_bet_label_format, comm->current_bet);
  current_bet_label->set_label(buffer);
  
  bet_value_slider->set_range(comm->current_bet, main_player->wallet);
  
  
  // enable check button based on the current bet
  check_button->set_sensitive( comm->current_bet == 0 );
}


// Allows players to flip over cards. This tells the client they want to discard those files

// Use macros to shorten the need for multiple methods 
// that do the same thing, minus the number....
#define HAND_CLICK_METHODS(NUM)                                 \
void  poker_client::on_hand_click_##NUM()                       \
{                                                               \
  std::cout << "Hand " << NUM << " was clicked!" << std::endl;  \
  if(cards[NUM-1]->property_file() == card_down_file)           \
  {                                                             \
    Player* player = comm->players[ comm->main_player ];        \
    Card card = player->hand[NUM-1];                            \
    std::string image = get_card_file(card);                    \
    cards[NUM-1]->set(image);                                   \
  }                                                             \
  else                                                          \
  {                                                             \
    cards[NUM-1]->set(card_down_file);                          \
  }                                                             \
}              

HAND_CLICK_METHODS(1)
HAND_CLICK_METHODS(2)
HAND_CLICK_METHODS(3)
HAND_CLICK_METHODS(4)
HAND_CLICK_METHODS(5)


void poker_client::on_bet_value_changed()
{
  
  int bet_value = bet_value_slider->get_value();
  
  assert(bet_value >= comm->current_bet); // can't bet less than the current bet
  
  check_button->set_sensitive( bet_value == 0 );
}

// TODO: maybe the functions below would serve better in the client_communicator?

void poker_client::on_play_click()
{
  comm->write_message("play");
}

void poker_client::on_quit_click()
{
  comm->write_message("quit");
}

void poker_client::on_check_click()
{
  comm->write_message("check");
}

void poker_client::on_bet_click()
{
  comm->write_message("bet");
}

void poker_client::on_fold_click()
{
  comm->write_message("fold");
}

void poker_client::on_discard_click()
{
  comm->write_message("discard");
}
