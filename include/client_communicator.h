// header guard
#ifndef CLIENT_COMMUNICATOR_H
#define CLIENT_COMMUNICATOR_H

#include "player.h"
#include "dealer.h"
#include "poker_client.h"

#include <iostream>
#include <thread>

#include <cstdlib>
#include <deque>
#include <vector>
#include <iostream>
#include <thread>
#include "asio.hpp"
#include "chat_message.hpp"

#include <boost/uuid/uuid.hpp>
#include "json.hpp"


class poker_client;
class chat_client;

// poker_client will have a client_communicator object that will connect to the server
// and pull info to the client that will require input (if it's their turn) that the
// client_communicator will wait for and push to the server

class client_communicator
{
  public:
    friend class poker_client;
    client_communicator(poker_client* client, std::string host, std::string port);
    virtual ~client_communicator();
  
    void close();
    void client_run();                                 // runs async waiting for data to be read from dealer/server
    void asio_run(std::string host, std::string port); // runs the io context to read the data
    void message_readin(std::string);                  // message is read in from chat_client
    
    void send_message();
  
  private:
    
    std::string turn_status;           // message indicating who the dealer is waiting on
    long pot;                          // current pot of the game
    long current_bet;                  // current bet of the game, can be 0 for "check" 
    poker_client* client;              // client this communicator is tied to
    
    bool failed;                       // flag to signal connection failed
    chat_client* comm;                 // this will connect with the dealer/server
    asio::io_context io_context;
    std::thread* comm_t;               // thread to run the asio and chat_client
    void send_action(int cards,
                                      std::string chat,
                                      double curr_bet,
                                      std::string event,
                                      std::string uuid,
                                      std::string name,
                                      double tot_bet);   // write message to chat_client
};

using asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

class chat_client
{
public:
  chat_client(client_communicator* c ,asio::io_context& io_context,
      const tcp::resolver::results_type& endpoints)
    : io_context_(io_context),
      socket_(io_context),
      comm{c}
  {
    do_connect(endpoints);
  }

  void write(const chat_message& msg)
  {
    asio::post(io_context_,
        [this, msg]()
        {
          bool write_in_progress = !write_msgs_.empty();
          write_msgs_.push_back(msg);
          if (!write_in_progress)
          {
            do_write();
          }
        });
  }

  void close()
  {
    asio::post(io_context_, [this]() { socket_.close(); });
  }

private:
  void do_connect(const tcp::resolver::results_type& endpoints)
  {
    asio::async_connect(socket_, endpoints,
        [this](std::error_code ec, tcp::endpoint)
        {
          if (!ec)
          {
            do_read_header();
          }
        });
  }

  void do_read_header()
  {
    asio::async_read(socket_,
        asio::buffer(read_msg_.data(), chat_message::header_length),
        [this](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec && read_msg_.decode_header())
          {
            for (unsigned int i=0;i<chat_message::max_body_length;i++)
            {
                read_msg_.body() [i] = '\0';
            }
            
            do_read_body();
          }
          else
          {
            socket_.close();
          }
        });
  }

  void do_read_body()
  {
    asio::async_read(socket_,
        asio::buffer(read_msg_.body(), read_msg_.body_length()),
        [this](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            std::string str = read_msg_.body();
            
            try 
            {
              comm->message_readin(str);
            }
            catch(std::exception& say)
            {
              std::cout << "Error processing '" << read_msg_.body() << "'" << std::endl;
              std::cout << say.what() << std::endl;
            }
            
            do_read_header();
          }
          else
          {
            socket_.close();
          }
        });
  }

  void do_write()
  {
    asio::async_write(socket_,
        asio::buffer(write_msgs_.front().data(),
          write_msgs_.front().length()),
        [this](std::error_code ec, std::size_t /*length*/)
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
            socket_.close();
          }
        });
  }

private:
  asio::io_context& io_context_;
  tcp::socket socket_;
  chat_message read_msg_;
  chat_message_queue write_msgs_;
  client_communicator* comm;
};


/*
int main(int argc, char* argv[])
{
  try
  {
    if (argc != 3)
    {
      std::cerr << "Usage: chat_client <host> <port>\n";
      return 1;
    }

    asio::io_context io_context;

    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(argv[1], argv[2]);
    chat_client c(io_context, endpoints);

    std::thread t([&io_context](){ io_context.run(); });

    char line[chat_message::max_body_length + 1];
    while (std::cin.getline(line, chat_message::max_body_length + 1))
    {
      chat_message msg;


      nlohmann::json to_dealer;
      to_dealer["from"] = { {"uuid","3f96b414-9ac9-40b5-8007-90d0e771f0d0"} , {"name","Bud"} };
      to_dealer["event"] = "stand";        // "stand","hit","fold","raise","join","request_cards"
      to_dealer["cards_requested"] = 3;    // optional, number of cards requested, 1 to 5
      to_dealer["current_bet"] = 1.00;
      to_dealer["total_bet"] = 5.00;
      to_dealer["chat"] = std::string(line);
      //std::cout << "to dealer:" << std::endl;
      //std::cout << to_dealer.dump(2) << std::endl;

      std::string t = to_dealer.dump();

      msg.body_length(t.size());
      std::memcpy(msg.body(), t.c_str() , msg.body_length());
      msg.encode_header();
      c.write(msg);
    }

    c.close();
    t.join();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}*/

#endif