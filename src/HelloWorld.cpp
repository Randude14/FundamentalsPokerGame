#include <gtkmm.h>
#include <iostream>

extern "C" void hello_world(void* widget, gpointer user_data)
{
	std::cout << "Hello World!" << std::endl;
	
}

int main(int argc, char* argv[])
{
	auto app = Gtk::Application::create(argc, argv, "hello.world.example");
	
	//Load the GtkBuilder file and instantiate its widgets:
	auto refBuilder = Gtk::Builder::create();
	
	try
	{
		refBuilder->add_from_file("test.glade");
	}
	catch(const std::exception& ex)
	{
		std::cerr << "Error: " << ex.what() << std::endl;
		return 1;
	}
	
	// this connects all signal handles from glade
	gtk_builder_connect_signals(refBuilder->gobj(), NULL);
	
	Gtk::Window* window;
	
	refBuilder->get_widget("Poker++", window);
	
	app->run(*window);
	
	return 0;
}