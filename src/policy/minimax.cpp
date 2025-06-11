#include <cstdlib>
#include "../state/state.hpp"
#include "./minimax.hpp"


/**
 * @brief Get a legal action by minimax
 * 
 * @param state Now state
 * @param depth this is the search depth
 * @return Move 
 */
int minimax(State state, int depth, int my_turn){
  if(depth == 0) return state.evaluate();
  if(!state.legal_actions.size())
    state.get_legal_actions();
  if(!state.legal_actions.size()) return state.evaluate();
  if(my_turn==1){
    int min = 1000000;
    auto actions = state.legal_actions;
    for(Move i:actions){
      State* newstate = state.next_state(i);
      int score = minimax(*newstate, depth-1, 0);
      if(score<min) min = score;
      delete newstate;
    }
    return min;
  }
  else{
    int max = -1000000;
    auto actions = state.legal_actions;
    for(Move i:actions){
      State* newstate = state.next_state(i);
      int score = minimax(*newstate, depth-1, 1);
      if(score>max) max = score;
      delete newstate;
    }
    return max;
  }
}
Move Minimax::get_move(State *state, int depth){
  if(!state->legal_actions.size())
    state->get_legal_actions();  
  auto actions = state->legal_actions;
  int maxscore = -100000;
  Move move;
  for(Move i:actions){
    State* newstate = state->next_state(i);
    int score = minimax(*newstate, depth-1, 0);
    if(score>maxscore){
      maxscore = score;
      move = i;
    }
    delete newstate;
  }
  return move;
}