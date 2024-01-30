#include "position.hpp"
#include "TranspositionTable.hpp"
#include "MoveSorter.hpp"
#include <iostream>
#include <unordered_map>

class Solver {
  private:
    int columnOrder[Position::WIDTH];

    TranspositionTable transTable;

    int negamax(const Position &P, int alpha, int beta) {

      uint64_t next = P.possibleNonLosingMoves();
      if(next == 0) { //if you are guarenteed to lose
	      return -(Position::WIDTH * Position::HEIGHT - P.nMoves()) / 2;
      }

      if(P.nMoves() >= Position::WIDTH * Position::HEIGHT - 2) { //if board is full
	      return 0; 
      }

      int min = -(Position::WIDTH * Position::HEIGHT - 2 - P.nMoves()) / 2;	// lower bound of score
      if(alpha < min) {
        alpha = min;                     
        if(alpha >= beta) return alpha;  // prune the exploration if the [alpha;beta] window is empty.
      }

      int max = (Position::WIDTH * Position::HEIGHT - 1 - P.nMoves()) / 2;	// upper bound of our score
      if(int val = transTable.get(P.key())) {
	      max = val + Position::MIN_SCORE - 1;
      }

      if(beta > max) {
        beta = max;                     
        if(alpha >= beta) return beta;  // prune the exploration if the [alpha;beta] window is empty.
      }


      MoveSorter moves;

      for(int i = Position::WIDTH; i--; ) { //scoring moves
	      if(uint64_t move = next & Position::column_mask(columnOrder[i])) {
		      moves.add(move, P.moveScore(move));
	      }
      }

      while(uint64_t next = moves.getNext()) {
          Position P2(P);
          P2.play(next);  // It's opponent turn in P2 position after current player plays x column.
          int score = -negamax(P2, -beta, -alpha); // explore opponent's score within [-beta;-alpha] windows:
          // no need to have good precision for score better than beta (opponent's score worse than -beta)
          // no need to check for score worse than alpha (opponent's score worse better than -alpha)

          if(score >= beta) {
		  return score;  
	  }// prune the exploration if we find a possible move better than what we were looking for.
          if(score > alpha) {
		  alpha = score; 
	  }// reduce the [alpha;beta] window for next exploration, as we only 
          // need to search for a position that is better than the best so far.
        }

      transTable.put(P.key(), alpha - Position::MIN_SCORE + 1); // save the upper bound of the position
      return alpha;
    }

  public:

    int solve(const Position &P, int *score) {
      if(P.canWinNext()) { //if you can win int the next move
	      *score = -(Position::WIDTH * Position:: HEIGHT + 1 - P.nMoves()) / 2;
	      return (Position::WIDTH * Position::HEIGHT + 1 - P.nMoves()) / 2;
      }
      int min = -(Position::WIDTH * Position::HEIGHT - P.nMoves())/2;
      int max = (Position::WIDTH * Position::HEIGHT + 1 - P.nMoves())/2;

      while(min < max) {                    
        int med = min + (max - min) / 2;
        if(med <= 0 && min / 2 < med) med = min / 2;
        else if(med >= 0 && max / 2 > med) med = max / 2;
        int r = negamax(P, med, med + 1);   // use a null depth window to know if the actual score is greater or smaller than med
        if(r <= med) max = r;
        else min = r;
      }

      *score = -min;
      return min;
    }

    void reset() 
    {
      transTable.reset();
    }

    // Constructor
    Solver() : transTable(8388593) { //8388593 prime = 64MB of transposition table
      reset();
      for(int i = 0; i < Position::WIDTH; i++)
        columnOrder[i] = Position::WIDTH / 2 + (1 - 2 * (i % 2)) * (i + 1) / 2;   
      // initialize the column exploration order, starting with center columns
      // example for WIDTH=7: columnOrder = {3, 4, 2, 5, 1, 6, 0}
    }

  };

int playOptimalMove(char board[6][7], char side, std::unordered_map<uint64_t, int> table) {
	Solver solver;
        uint64_t position = 0;
        uint64_t mask = 0;
        int counter = 0;
	int scores[7] = {0};
	int score = -99;
        unsigned int moves = 0;

	std::cout << "Calculating best move..." << std::endl;
	//translating board into bitboard
        for (int i = 0; i < 7; i += 1) {
                for (int j = 5; j > -1; j -= 1) {
                        if (board[j][i] == side) {
                                position |= 1UL << counter;
                        }
                        else {
                                position |= 0 << counter;
                        }
                        if (board[j][i] != '-') {
                                mask |= 1UL << counter;
                                moves += 1;
                        }
                        else {
                                mask |= 0 << counter;
                        }
                        counter += 1;
                }
                counter += 1;
        }

	Position P[7];
	int column = 0;

	//play each column and get the scores
	for (int i = 0; i < 7; i += 1) {
		P[i].setCurrent(position);
		P[i].setMask(mask);
		P[i].setMoves(moves);
		//if next move wins you the game, return max score
		if (P[i].isWinningMove(i)) {
			scores[i] = (Position::WIDTH * Position::HEIGHT + 1 - P[i].nMoves()) / 2;
			continue;
		}
		P[i].colPlay(i);
		//if the move is stored, return the score asscioated with it
		std::unordered_map<uint64_t, int>::const_iterator s = table.find(P[i].getCurrent() + P[i].getMask());
		if (s != table.end()) {
                        scores[i] = -s->second;
			continue;
                }
		//else run the algorithm
		solver.solve(P[i], &scores[i]);
	}

	//finds the highest score
	for (int i = 0; i < 7; i += 1) {
		std::cout << "SCORE: " << scores[i] << std::endl;
                std::cout << "COL:   " << i + 1 << std::endl << std::endl;
		if ((scores[i]) > score) {
			score = scores[i];
			column = i + 1;
		}
	}

	return column;
}
