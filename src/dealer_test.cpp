#include "dealer.h"
#include "game.h"
#include "player.h"
#include "card.h"

#include <iostream>

void print_game(Game& game)
{
  std::cout << "Prize Pot: " << game.prize_pot << std::endl;
  std::cout << "Player size: " << game.num_players << std::endl;
  for(int i = 0; i < game.num_players; i++)
  {
    Player* player = &game.players[i];
    std::cout << "Name: " << player->name << std::endl;
    std::cout << "Wallet: " << player->wallet << std::endl;
    std::cout << "Cards: ";
    for(unsigned int j = 0; j < player->hand.size(); j++)
    {
      Card card = player->hand.at(j);
      std::cout << "(" << card.get_int_suit() << "," << card.get_int_value() << "), ";
    }
    std::cout << std::endl;
  }
  
  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << std::endl;
}

int main(int argc, char* argv[])
{
  Game game;
  Player player1;
  Player player2;
  Player player3;
  Player player4;
  
  game.player_join(player1);
  game.player_join(player2);
  game.player_join(player3);
  game.player_join(player4);
  
  if(game.min_players())
  {
    game.start_game();
  }
  
  print_game(game);
  
  for(int i = 0; i < game.num_players; i++)
  {
    game.check();
    game.next_player();
  }
  
  assert( game.round_over() );
  
  print_game(game);
  
  game.next_stage();
  
  bool e1[NUM_CARDS] = {false, false, true, false, true};
  bool e2[NUM_CARDS] = {false, true, false, true, true};
  
  game.exchange(e1);
  game.next_player();
  game.exchange(e2);
  game.next_player();
  game.exchange(e2);
  game.next_player();
  game.exchange(e1);
  game.next_player();
  
  assert( game.round_over() );
  
  game.next_stage();
  
  game.bet(10);
  game.next_player();
  game.bet(20);
  game.next_player();
  game.call();
  game.next_player();
  game.call();
  game.next_player();
  game.bet(30);
  game.next_player();
  game.fold();
  game.next_player();
  game.call();
  game.next_player();
  game.fold();
  game.next_player();

  
  assert( game.round_over() );
  
  print_game(game);
  
  game.next_stage();
  assert( game.is_game_over() );
  
  return 0;
}