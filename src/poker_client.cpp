#include <gtkmm.h>
#include <iostream>
#include <signal.h>

#include "poker_client.h"
#include "play_window.h"
#include "player.h"
#include "card.h"
#include "game.h"



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
    if(VAR == nullptr) { std::cerr << "Failed to find widget for " << ID << std::endl; return 1; }
  
  // Hook in widgets
  // macro finds widget with ID and sets to VAR while connecting its callback to FUNC
  #define GET_AND_CONNECT(ID, VAR, FUNC)         \
    GET_WIDGET(ID, VAR)                          \
    VAR->signal_clicked().connect(sigc::mem_fun(*this, FUNC));
    
  // get the window object from the referrence builder
  GET_WIDGET("Poker++", main_window);
  
  // set title....the builder apparently changes the title from the glade file. No biggie
  std::string window_title = APP_TITLE;
  window_title += std::string("   ") + playername;
  main_window->set_title(window_title);  
  
  // create communicator
  comm = new client_communicator(this, host, port);
 
  comm->current_bet = 0;
  comm->pot = 5;
  
  
  // get main player
  for(int i = 0; i < MAX_PLAYERS; i++)
  {
    players[i] = new Player();
  }
  
  num_players = 1;
  main_player = 0;
  
  Player* player = players[0];
  player->set_name(playername);
  player->set_wallet(100);
  main_uuid = player->get_UUID();
  std::cout << "main uuid: " << main_uuid << std::endl;
  player->set_message("Player joined");
  player->set_action("join");
  
  // login to dealer
  make_json(player);
  // set default values for player's hand....these will be removed later
  Card c1{Card_value::TEN, Suit::CLUB};
  Card c2{Card_value::JACK, Suit::CLUB};
  Card c3{Card_value::QUEEN, Suit::CLUB};
  Card c4{Card_value::KING, Suit::CLUB};
  Card c5{Card_value::ACE, Suit::CLUB};
  player->add_to_hand(c1);
  player->add_to_hand(c2);
  player->add_to_hand(c3);
  player->add_to_hand(c4);
  player->add_to_hand(c5);
   
  // connect poker_client control buttons
  GET_AND_CONNECT( "check",     check_button,   &poker_client::on_check_click )
  GET_AND_CONNECT( "bet",       bet_button,     &poker_client::on_bet_click )
  GET_AND_CONNECT( "call",      call_button,    &poker_client::on_call_click )
  GET_AND_CONNECT( "fold",      fold_button,    &poker_client::on_fold_click )
  GET_AND_CONNECT( "discard",   discard_button, &poker_client::on_discard_click )
  
  // get range and connect the value changed method
  GET_WIDGET("bet_slider", bet_value_slider)
  bet_value_slider->signal_value_changed().connect( sigc::mem_fun(*this, &poker_client::on_bet_value_changed) );
  //bet_value_slider->set_range(0, player->wallet); // set range based on initial player wallet
  
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
    }
      
    // get last action label
    GET_WIDGET(player + std::string("last_action"), opp_display->last_action)
  }
  
  // Run the window
  std::string appname = APP_NAME;
  appname += std::string(".") + playername;
  auto main_app = Gtk::Application::create(appname);
  int ret = main_app->run(*main_window);
  
  this->close();

  // return exit status
  return ret;
}


// ***** CLOSE CLIENT **********
void poker_client::close()
{
  if(comm)
  {
    comm->close();
    delete comm;
    comm = NULL;
  }
  
  for(int i = 0; i < MAX_OPPONENTS; i++)
    if(opp_displays[i])
      delete opp_displays[i];
    
  for(int i = 0; i < MAX_PLAYERS; i++)
    if(players[i])
      delete players[i];
  
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


void poker_client::update_client(bool showcards)
{
  update_lock.lock();
  int od_index = 0;
  
  for(int i = 0; i <NUM_CARDS; i++)
  {
    discards[i] = false;
  }
  
  for(int i = 0; i < MAX_PLAYERS; i++) // update opponents info
  {
    // update main player's info
    if(i == main_player)
    {
      Player* player = players[ main_player ];
      
      username->set_label(player->get_name()); // update username
      
      auto hand = player->get_hand();
  
      for(unsigned int j = 0; j < NUM_CARDS; j++) // update cards
      {
        std::string image_file;
        if(j < hand.size())
        {
          image_file = get_card_file(hand[j]);
        }
        else
        {
          image_file = card_down_file;
        }
        std::cout << image_file << " at " << j << std::endl;
        card_buttons[j]->set_sensitive(true);
        
        if(cards[j]->property_file() != image_file)
        {
          cards[j]->set(image_file);
        }
      } 
    }
    
    // update opponent's info
    else if(i < num_players)
    {
      auto od = opp_displays[od_index];
      Player* opp = players[i];

      od->username->set_label(opp->get_name());
      od->last_action->set_label( opp->get_action() );
      
      auto hand = opp->get_hand();
      
      for(unsigned int j = 0; j < NUM_CARDS; j++) // update cards
      {
        std::string image_file;

        if(j < hand.size())
        {
          image_file = (showcards) ? get_card_file(hand[j]) : card_down_file;
        }
        else
        {
          image_file = card_down_file;
        }
        
        if(image_file != od->cards[j]->property_file())
        {
          od->cards[j]->set(image_file);
        }
      }

      od_index++;
    }
    
    // this means we are beyond amount of players...fill in with blank info
    else
    {
      auto od = opp_displays[od_index];
      
      std::cerr << "od_index " << od_index << std::endl;
      od->username->set_label(blank_name);
      od->last_action->set_label("");
      od->wallet->set_label("");
      std::string image_file = card_down_file;
      for(int j = 0; j < NUM_CARDS; j++) // update cards
      {
        if(image_file != od->cards[j]->property_file())
        {
          od->cards[j]->set(image_file);
        }
      }  
      
      od_index++;
    }
  }
  
  char buffer[50]; // used to format labels...
  Player* main = players[ main_player ];
  
  // update wallet label
  sprintf(buffer, wallet_label_format, main->get_wallet());
  wallet_label->set_label(buffer);
  
  // update pot label
  sprintf(buffer, pot_label_format, comm->pot);
  pot_label->set_label(buffer);
  
  // update bet label
  sprintf(buffer, current_bet_label_format, comm->current_bet);
  current_bet_label->set_label(buffer);
  
  // update turn status
  turn_status->set_label( comm->game_status );
  
  bet_value_slider->set_range(comm->current_bet, main->get_wallet());
  
  
  // enable check button based on the current bet
  check_button->set_sensitive( comm->current_bet == 0 );
  call_button->set_sensitive( comm->current_bet != 0 );
  bet_button->set_sensitive( 0 );
  
  main_window->queue_draw();
  update_lock.unlock();
}


// Allows players to flip over cards. This tells the client they want to discard those files

// Use macros to shorten the need for multiple methods 
// that do the same thing, minus the number....
#define HAND_CLICK_METHODS(NUM)                                 \
void  poker_client::on_hand_click_##NUM()                       \
{                                                               \
  std::cout << "Hand " << NUM << " was clicked!" << std::endl;  \
  Player* player = players[ main_player ];                      \
  if( player->get_hand().size() < NUM) { return; }              \
                                                                \
  if(cards[NUM-1]->property_file() == card_down_file)           \
  {                                                             \
    Card card = player->get_card(NUM-1);                        \
    std::string image = get_card_file(card);                    \
    cards[NUM-1]->set(image);                                   \
    player->discard(NUM-1, true);                               \
    player->incr_numDiscards();                                 \
  }                                                             \
  else                                                          \
  {                                                             \
    cards[NUM-1]->set(card_down_file);                          \
    player->discard(NUM-1, false);                              \
    player->decr_numDiscards();                                 \
  }                                                             \
  discards[NUM-1] = ! discards[NUM-1];                          \
}

HAND_CLICK_METHODS(1)
HAND_CLICK_METHODS(2)
HAND_CLICK_METHODS(3)
HAND_CLICK_METHODS(4)
HAND_CLICK_METHODS(5)


void poker_client::on_bet_value_changed()
{
  int bet_value = bet_value_slider->get_value();
  
  //assert(bet_value >= comm->current_bet); // can't bet less than the current bet
  bet_button->set_sensitive( bet_value != 0 );
}

void poker_client::on_check_click()
{
  // can't check if there is a bet
  //assert( comm->current_bet <= 0 );
  Player* player = players[ main_player ];
  player->set_numDiscards(0);
  player->set_message("Check!");
  player->set_current_bet(0.0);
  player->set_action("check");
  player->set_total_bet(0.0);
  make_json(player);
  update_client();
}

void poker_client::on_bet_click()
{
  // bet current amount
  //assert(bet >= comm->current_bet);
  Player* player = players[ main_player ];
  player->set_message("I bet !");
  player->set_current_bet(bet_value_slider->get_value() - player->get_total_bet());
  player->set_action("bet");
  player->set_total_bet(bet_value_slider->get_value());
  make_json(player);
  update_client();
}

void poker_client::on_call_click()
{
  // set total bet to bet value slider
  Player* player = players[ main_player ];
  player->set_message("I'll call!");
  player->set_current_bet(bet_value_slider->get_value() - player->get_total_bet());
  player->set_action("call");
  player->set_total_bet(bet_value_slider->get_value());
  make_json(player);
  update_client();
}

void poker_client::on_fold_click()
{
  Player* player = players[ main_player ];
  player->set_numDiscards(5);
  player->set_message("I'm out!");
  player->set_action("fold");
  player->set_folded(true);
  make_json(player);
  update_client();
}

void poker_client::on_discard_click()
{
  Player* player = players[ main_player ];
  player->set_message("Give me x cards");
  player->set_action("exchange");
  make_json(player);
  update_client();
}

void poker_client::bet_sensitivity()
{
  // Prevent player from flipping and exchanging cards
  for(int idx = 0; idx < NUM_CARDS; idx++)
  {
    card_buttons[idx]->set_sensitive(false);
  }
  
  discard_button->set_sensitive(false);
  
  // Set Check, Bet, Call, Fold button sensitivity
  if(comm->current_bet > 0.0)
  {
    check_button->set_sensitive(false);
  }
  
  bet_button->set_sensitive(true);
  
  if(players[main_player]->get_current_bet() < comm->current_bet)
  {
    call_button->set_sensitive(true);
  }
  
  fold_button->set_sensitive(true);

}

void poker_client::exchange_sensitivity()
{
  // Allow player to flip and exchange cards
  for(int idx = 0; idx < NUM_CARDS; idx++)
  {
    card_buttons[idx]->set_sensitive(true);
  }

  discard_button->set_sensitive(true);
  
  // Set Check, Bet, Call, Fold button sensitivity
  check_button->set_sensitive(false);
  bet_button->set_sensitive(false);
  call_button->set_sensitive(false);
  fold_button->set_sensitive(true);
}

void poker_client::make_json(Player* player)
{
  comm->send_action(player->get_numDiscards(),
                    player->get_message(),
                    player->get_current_bet(),
                    player->get_action(),
                    player->get_UUID(),
                    player->get_name(),
                    player->get_total_bet(),
                    discards);
}
