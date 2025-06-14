#pragma once
#include "../state/state.hpp"


/**
 * policy class for alpha-beta pruning, 
 * your policy class should have get_move method
 */
class Alphabeta{
public:
  static Move get_move(State *state, int depth);
};