#pragma once
#include "../state/state.hpp"


/**
 * policy class for minimax policy, 
 * your policy class should have get_move method
 */
class Minimax{
public:
  static Move get_move(State *state, int depth);
};