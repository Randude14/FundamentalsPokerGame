#ifndef PLAY_WINDOW_H
#define PLAY_WINDOW_H
#include <gtkmm.h>
#include <cstdlib>

using namespace std;

// initial window that is opened before running the client
// gets the player name, host, and port to connect to
class play_window: public Gtk::Window
{
public:
  play_window();
  virtual ~play_window();
  
private:
  // **************** widgets
  Gtk::Button* play;
  Gtk::Button* quit;
  // main vbox
  Gtk::VBox* vbox;
  // hboxes for the entry fields
  Gtk::HBox* textbox;
  Gtk::HBox* hostbox;
  Gtk::HBox* portbox;
  // entry and label for player name
  Gtk::Entry* nameE;
  Gtk::Label* nameL;
  // entry and label for host
  Gtk::Entry* hostE;
  Gtk::Label* hostL;
  // entry and label for port
  Gtk::Entry* portE;
  Gtk::Label* portL;
  // empty labels...
  Gtk::Label* L;
  Gtk::Label* L2;
  Gtk::Label* L3;
  // image to display
  Gtk::Image* image;
  // end widgets ***************

public:
  void play_clicked(); // called by play and also by hitting enter
  void quit_clicked();
  bool text_changed(const Cairo::RefPtr< ::Cairo::Context>& cr);
  
  bool should_quit;
  std::string username;
  std::string host;
  std::string port;
};
#endif
