#include "play_window.h"

#include <iostream>
#include <gtkmm.h>
#include <cstdlib>
using namespace std;


play_window::play_window()
{
  // init widgets, Gtk::manage takes care of deletions
  play = Gtk::manage(new Gtk::Button());
  quit = Gtk::manage(new Gtk::Button());
  
  vbox = Gtk::manage(new Gtk::VBox());
  
  textbox = Gtk::manage(new Gtk::HBox(true));
  hostbox = Gtk::manage(new Gtk::HBox(true));
  portbox = Gtk::manage(new Gtk::HBox(true));
  
  nameE = Gtk::manage(new Gtk::Entry());
  hostE = Gtk::manage(new Gtk::Entry());
  portE = Gtk::manage(new Gtk::Entry());
  
  nameL = Gtk::manage(new Gtk::Label());
  hostL = Gtk::manage(new Gtk::Label());
  portL = Gtk::manage(new Gtk::Label());
  
  L = Gtk::manage(new Gtk::Label());
  L2 = Gtk::manage(new Gtk::Label());
  L3 = Gtk::manage(new Gtk::Label());
  
  image = Gtk::manage(new Gtk::Image());;
  
  // set window settings
  set_size_request(500,300);
  add(*vbox);
  set_title("Welcome to poker game");

  nameL->set_text("Full Name:");
  hostL->set_text("Host:");
  portL->set_text("Port:");

  textbox->set_spacing(12);
  hostbox->set_spacing(12);
  portbox->set_spacing(12);

  // add images
  image->set("gui/poker.jpg");
  vbox->pack_start(*image);
  L3->set_text(" ");
  vbox->pack_start(*L3);
  
  // add entry fields
  vbox->pack_start(*textbox);
  textbox->pack_start(*nameL);
  textbox->pack_start(*nameE);
  nameE->set_max_length(25);
  
  vbox->pack_start(*hostbox);
  hostbox->pack_start(*hostL);
  hostbox->pack_start(*hostE);
  hostE->set_text("127.0.0.1");
  hostE->set_max_length(25);
  
  vbox->pack_start(*portbox);
  portbox->pack_start(*portL);
  portbox->pack_start(*portE);
  portE->set_text("9000");
  portE->set_max_length(25);
  
  L->set_text(" ");
  vbox->pack_start(*L);
  
  L2->set_text(" ");
  vbox->pack_start(*L2);
  
  
  
  play->set_label("Play");
  quit->set_label("Quit");
  vbox->pack_start(*play);
  vbox->pack_start(*quit);
  
  // connect callbacks
  play->signal_clicked().connect( sigc::mem_fun(*this, &play_window::play_clicked) );
  play->set_sensitive(false);
  
  // work around for enabling and disabling play button based on entry length
  nameE->signal_draw().connect( sigc::mem_fun(*this, &play_window::text_changed) );
  hostE->signal_draw().connect( sigc::mem_fun(*this, &play_window::text_changed) );
  portE->signal_draw().connect( sigc::mem_fun(*this, &play_window::text_changed) );
  
  // invoke play button when enter is clicked
  nameE->signal_activate().connect( sigc::mem_fun(*this, &play_window::play_clicked) );
  hostE->signal_activate().connect( sigc::mem_fun(*this, &play_window::play_clicked) );
  portE->signal_activate().connect( sigc::mem_fun(*this, &play_window::play_clicked) );
  quit->signal_clicked().connect( sigc::mem_fun(*this,   &play_window::quit_clicked) );
  
  // show window
  show_all_children();
}

play_window::~play_window() {}

void play_window::play_clicked()
{
  // activate from entry can call this method...play button may not be sensitive
  if(! play->is_sensitive()) return;
  
  should_quit = false;
  username = nameE->get_text();
  host = hostE->get_text();
  port = portE->get_text();
  close();
}

bool play_window::text_changed(const Cairo::RefPtr< ::Cairo::Context>& cr)
{
  // enable play button only when all entries are not blank
  play->set_sensitive( nameE->get_text().size() > 0 
                    && hostE->get_text().size() > 0
                    && hostE->get_text().size() > 0);
  
  return false;
}

void play_window::quit_clicked()
{
  should_quit = true;
  close();
}

