#include <cstdlib>
#include "../state/state.hpp"
#include "./alphabeta.hpp"


/**
 * @brief Get a legal action by alpha-beta pruning
 * 
 * @param state Now state
 * @param depth this is the search depth
 * @return Move 
 */
int alphabeta(State state, int alpha, int beta, int depth, int max_turn){
  if(depth == 0) return state.evaluate();
  if(!state.legal_actions.size())
    state.get_legal_actions();
  if(!state.legal_actions.size()) return state.evaluate();
  if(max_turn==1){
    int max = -1000000;
    auto actions = state.legal_actions;
    for(Move i:actions){
      State* newstate = state.next_state(i);
      max = std::max(max, alphabeta(*newstate, alpha, beta, depth-1, 0));
      if(max>=beta) break;
      alpha = std::max(alpha, max);
    }
    return max;
  }
  else{
    int min = 1000000;
    auto actions = state.legal_actions;
    for(Move i:actions){
      State* newstate = state.next_state(i);
      min = std::min(min, alphabeta(*newstate, alpha, beta, depth-1, 1));
      if(min<=alpha) break;
      min = std::min(min, beta);
    }
    return min;
  }
}
Move Alphabeta::get_move(State *state, int depth){
  if(!state->legal_actions.size())
    state->get_legal_actions();  
  auto actions = state->legal_actions;
  Move move;
  if(!state->player){
    int maxscore = -100000;
    for(Move i:actions){
      State* newstate = state->next_state(i);
      int score = alphabeta(*newstate, -100000, 100000, depth-1, 0);
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
      int score = alphabeta(*newstate, -100000, 100000, depth-1, 1);
      if(score<=minscore){
        minscore = score;
        move = i;
      }
    }
    return move;
  }
}