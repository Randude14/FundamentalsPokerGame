#include "dealer.h"
#include "game.h"
#include "player.h"
#include "card.h"

#include <iostream>

void print_game(Game& game)
{
  std::cout << "Prize Pot: " << game.get_prize_pot() << std::endl;
  
  auto players = game.get_players();
  
  std::cout << "Player size: " << players.size() << std::endl << "---------" << std::endl;
  for(unsigned int i = 0; i < players.size(); i++)
  {
    Player* player = &players.at(i);
    std::cout << "Name: " << player->get_name() << std::endl;
    std::cout << "Wallet: " << player->get_wallet() << std::endl;
    std::cout << "Folded: " << player->has_folded() << std::endl;
    std::cout << "Current Bet : " << player->get_current_bet() << std::endl;
    std::cout << "Total Bet: " << player->get_total_bet() << std::endl;
    std::cout << "Cards: ";
    
    auto hand = player->get_hand();
    for(unsigned int j = 0; j < hand.size(); j++)
    {
      Card card = hand.at(j);
      std::cout << "(" << card.get_int_suit() << "," << card.get_int_value() << "), ";
    }
    std::cout << std::endl << std::endl;
  }
  
  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << std::endl;
}

int main(int argc, char* argv[])
{
  Game game;
  Player player1;
  player1.set_name("Randall");
  Player player2;
  player2.set_name("Robbie");
  Player player3;
  player3.set_name("Bud");
  Player player4;
  player4.set_name("Shishir");
  
  game.player_join(player1);
  game.player_join(player2);
  game.player_join(player3);
  game.player_join(player4);
  
  for(int i = 0; i < 3; i++)
  {

    
    if(game.min_players())
    {
      game.start_game();
    }
    
    print_game(game);
  
    game.check();
    game.fold();
    game.check();
    game.check();
    
    
    assert( game.round_over() );
    
    print_game(game);
    
    game.next_stage();
    
    bool e1[NUM_CARDS] = {false, false, true, false, true};
    bool e2[NUM_CARDS] = {false, true, false, true, true};
    
    game.exchange(e1);
    game.exchange(e2);
    game.exchange(e1);
    
    
    print_game(game);
    
    assert( game.round_over() );
    
    game.next_stage();
    game.check();
    game.bet(10);
    game.call();
    game.call();
    
    assert( game.round_over() );
    
    print_game(game);
    
    game.next_stage();
    assert( game.game_over() );
    
    game.end_game();
  }
  
  print_game( game );
  
  return 0;
}