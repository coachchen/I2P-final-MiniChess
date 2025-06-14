#include <iostream>
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
int minimax(State state, int depth, int max_turn){
  if(depth == 0) return state.evaluate();
  if(!state.legal_actions.size())
    state.get_legal_actions();
  if(!state.legal_actions.size()) return state.evaluate();
  if(max_turn==1){
    int max = -1000000;
    auto actions = state.legal_actions;
    for(Move i:actions){
      State* newstate = state.next_state(i);
      max = std::max(max, minimax(*newstate, depth-1, 0));
    }
    return max;
  }
  else{
    int min = 1000000;
    auto actions = state.legal_actions;
    for(Move i:actions){
      State* newstate = state.next_state(i);
      min = std::min(min, minimax(*newstate, depth-1, 1));
    }
    return min;
  }
}
Move Minimax::get_move(State *state, int depth){
  if(!state->legal_actions.size())
    state->get_legal_actions();  
  auto actions = state->legal_actions;
  Move move;
  if(!state->player){
    int maxscore = -100000;
    for(Move i:actions){
      State* newstate = state->next_state(i);
      int score = minimax(*newstate, depth-1, 0);
      if(score>=maxscore){
        maxscore = score;
        move = i;
      }
    }
    return move;
  }
  else{
    int minscore = 100000;
    for(Move i:actions){
      State* newstate = state->next_state(i);
      int score = minimax(*newstate, depth-1, 1);
      if(score<=minscore){
        minscore = score;
        move = i;
      }
    }
    return move;
  }
}