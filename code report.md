# code report
## The State Function

state function 用來評估目前盤面的值，我的實做完全以白方來考量，若評估的值為正，表示對白方有利，反知則對黑方有利。\
最主要的考量因素是pieces，也就是有哪些棋子還在棋盤上，沒有被吃掉。每個棋子的值如下：\
兵：1\
城堡：5\
騎士：3\
主教：3\
皇后：8\
國王：20\
評估時，這些值各乘100。\
另外，我也加入了一個簡單的 piece-square table，以產生更精準的判斷。\
兵：勇往直前，升變方修，皇后前的兵更鼓勵前進，而國王前的兵則應保護國王\
城堡：想盡辦法衝進敵人陣營\
騎士：盡量往棋盤中央移動\
主教：也盡量往棋盤中央移動\
皇后：無特殊限制\
國王：無特殊限制
```
int State::evaluate(){
  // [TODO] design your own evaluation function

  int res_state = 0; //result state
  auto w_board = board.board[0];
  auto b_board = board.board[1];
  for(int i=0; i<BOARD_H; i++){
    for(int j=0; j<BOARD_W; j++){
      int wpiece = w_board[i][j];
      int bpiece = b_board[i][j];
      res_state += 100 * piece_value[wpiece];
      res_state -= 100 * piece_value[bpiece];
      if(wpiece) res_state += pst[wpiece-1][0][i][j];
      if(bpiece) res_state -= pst[bpiece-1][1][i][j];
    }
  }
  return res_state;
}
```

## Minimax

我設定白方是maximizer，黑方是minimizer，所以在get_move function裡也有相應的呼叫。\
若搜尋的深度為0，直接return當下state的評估值。\
若深度不為0，maximizer會呼叫minimizer，且return評估直最大的move，也就是說，maximizer會return對自己助益最大，或在下一回合損害最小的move。minimizer則反之。
```
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
```

## alpha-beta pruning

alpha是maximizer可以確定的最小值，beta則是minimizer可以確定的最大值。如果搜尋的過程中，有個node使得alpha>=beta，那剩下的node就不必搜尋了，因為它們永遠不會被選到。
```
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
```
get_move與minimax大同小異，只在function call時多了alpha與beta兩個參數，而alpha初始設為-100000，beta則為100000。