//
// chat_server.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#define ASIO_STANDALONE

#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include "asio.hpp"
#include "json.hpp"
#include "chat_message.hpp"

#include "dealer.h"

class Dealer;

using asio::ip::tcp;

//----------------------------------------------------------------------

typedef std::deque<chat_message> chat_message_queue;

//----------------------------------------------------------------------

class chat_participant
{
public:
  virtual ~chat_participant() {}
  virtual void deliver(const chat_message& msg) = 0;
};

typedef std::shared_ptr<chat_participant> chat_participant_ptr;

//----------------------------------------------------------------------

class chat_room
{
public:
  void join(chat_participant_ptr participant)
  {
    participants_.insert(participant);
  }

  void leave(chat_participant_ptr participant)
  {
    participants_.erase(participant);
  }

  void deliver(const chat_message& msg)
  {

    for (auto participant: participants_)
      participant->deliver(msg);
  }

private:
  std::set<chat_participant_ptr> participants_;
};

//----------------------------------------------------------------------

class chat_session
  : public chat_participant,
    public std::enable_shared_from_this<chat_session>
{
public:
  chat_session(tcp::socket socket, chat_room& room, Dealer* d)
    : socket_(std::move(socket)),
      room_(room),
      dealer{d}
  {
  }

  void start()
  {
    room_.join(shared_from_this());
    do_read_header();
  }

  void deliver(const chat_message& msg)
  {
    bool write_in_progress = !write_msgs_.empty();
    write_msgs_.push_back(msg);
    if (!write_in_progress)
    {
      do_write();
    }
  }

private:
  void do_read_header()
  {
    auto self(shared_from_this());
    asio::async_read(socket_,
        asio::buffer(read_msg_.data(), chat_message::header_length),
        [this, self](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec && read_msg_.decode_header())
          {
            // clear out the old buffer from the last read
            // a '\0' is a good value to make sure a string
            // is terminated
            for (unsigned int i=0;i<chat_message::max_body_length;i++)
            {
                read_msg_.body() [i] = '\0';
            }

            do_read_body();
          }
          else
          {
            room_.leave(shared_from_this());
          }
        });
  }

  void do_read_body()
  {
    auto self(shared_from_this());
    asio::async_read(socket_,
        asio::buffer(read_msg_.body(), read_msg_.body_length()),
        [this, self](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
        
            nlohmann::json to_dealer = nlohmann::json::parse(std::string(read_msg_.body()));
            nlohmann::json to_player;  // represents the entire game state.  sent to all players
            /*
            to_player["turn"] = "3f96b414-9ac9-40b5-8007-90d0e771f0d0";   // UUID of the current player. 
            to_player["chat"] = to_dealer["chat"];
            to_player["dealer_comment"] = "fred has raised and received 2 new cards";
            to_player["recommended_play"] = "you should fold";
            to_player["hand"] = { 
              {{"bet",1},{"current_bet",10}, {"uuid","3f96b414-9ac9-40b5-8007-90d0e771f0d0"} , {"name","Bud"} ,{"cards",{"acespades","10hearts","9clubs","2diamonds","kinghearts"}}},
              {{"bet",2},{"current_bet",1}, {"uuid","3f96b414-9ac9-40b5-8007-20d0e771f0d0"} , {"name","Donald"} ,{"cards",{"acehearts","10spades","9clubs","2clubs","jackhearts"}}},
              {{"bet",5},{"current_bet",5}, {"uuid","3f96b414-9ac9-40b5-8007-30d0e771f0d0"} , {"name","Ann"} ,{"cards",{"aceclubs","10diamonds","9clubs","2hearts","queenhearts"}}},
              {{"bet",10},{"current_bet",0}, {"uuid","3f96b414-9ac9-40b5-8007-40d0e771f0d0"} , {"name","Melania"} ,{"cards",{"acediamonds","10clubs","9clubs","2spades","kinghearts"}}}
                       };
   
            //std::cout << "to player:" << std::endl;
            //std::cout << to_player.dump(2) << std::endl;*/
            
            try 
            {
              dealer->process( to_dealer, to_player );
            }
            catch(std::exception& say)
            {
              std::cout << "Error processing '" << read_msg_.body() << "'" << std::endl;
              std::cout << say.what() << std::endl;
            }
            
            std::string t = to_player.dump();
            chat_message sending;
            if (t.size() < chat_message::max_body_length)
            {
              std::cout << "Sending '" << t << "' to players" << std::endl;
              memcpy( sending.body(), t.c_str(), t.size() );
	            sending.body_length(t.size());
	            sending.encode_header();
               room_.deliver(sending);
            }
            do_read_header();
          }
          else
          {
            room_.leave(shared_from_this());
          }
        });
  }

  void do_write()
  {
    auto self(shared_from_this());
    asio::async_write(socket_,
        asio::buffer(write_msgs_.front().data(),
          write_msgs_.front().length()),
        [this, self](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            write_msgs_.pop_front();
            if (!write_msgs_.empty())
            {
              do_write();
            }
          }
          else
          {
            room_.leave(shared_from_this());
          }
        });
  }

  tcp::socket socket_;
  chat_room& room_;
  Dealer* dealer;
  chat_message read_msg_;
  chat_message_queue write_msgs_;
};

//----------------------------------------------------------------------

class chat_server
{
public:
  chat_server(asio::io_context& io_context,
      const tcp::endpoint& endpoint, Dealer& d)
    : acceptor_(io_context, endpoint), dealer{&d}
  {
    do_accept();
  }

private:
  void do_accept()
  {
    acceptor_.async_accept(
        [this](std::error_code ec, tcp::socket socket)
        {
          if (!ec)
          {
            std::make_shared<chat_session>(std::move(socket), room_, dealer)->start();
          }

          do_accept();
        });
  }

  tcp::acceptor acceptor_;
  Dealer* dealer;
  chat_room room_;
};


//----------------------------------------------------------------------

int main(int argc, char* argv[])
{
  try
  {
    if (argc < 2)
    {
      std::cerr << "Usage: chat_server <port>\n";
      return 1;
    }
    
    std::cout << "Starting dealer server..." << std::endl;
    
    // create dealer object
    Dealer dealer;

    // only need one server
    asio::io_context io_context;
    tcp::endpoint endpoint(tcp::v4(), std::atoi("9000"));
    
    chat_server server( io_context, endpoint, dealer );
    std::cout << "Dealer started." << std::endl;
    
    io_context.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
