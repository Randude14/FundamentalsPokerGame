#include <gtkmm.h>
#include <iostream>

// const 
const static std::string APP_TITLE{"CSE3310 Poker++"};
const static std::string APP_NAME{"edu.uta.cse3310.poker.v0_1"};
const static std::string VERSION{"0.1"};


// used a a macro to link the card buttons functions that the player can discard
// my idea is that at the start of the round, the player will see the cards
// when they click on the card buttons they will "flip" over (i.e. we change the card image)
// this will indicate to the player that they want to turn these cards in, we can either check
// the card file name or use a bool array to check if they flipped a card over.

// NOTE: extern "C" must be included to connect the callback functions to the button signals
#define CARD_CLICK(NUM)   \
extern "C" void on_click_##NUM(void* widget, gpointer user_data)\
{\
	std::cout << "Button click on card " << NUM << "!" << std::endl;\
}

// function definitions...
CARD_CLICK(1)
CARD_CLICK(2)
CARD_CLICK(3)
CARD_CLICK(4)
CARD_CLICK(5)




int main (int argc, char *argv[]) {
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
		//std::cerr << "Error: " << ex.what() << std::endl;
		return 1;
	}
	
	// this connects all callback functions to the buttons from glade...
	gtk_builder_connect_signals(refBuilder->gobj(), NULL);
	
	// get the window object from the referrence builder
	refBuilder->get_widget("Poker++", win);
	
	// set title....gtk apparently changes the title. No biggie
	win->set_title(APP_TITLE);
	
	// run the window and return the exit status
    return app->run(*win);
}

