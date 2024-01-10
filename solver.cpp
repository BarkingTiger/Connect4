#include "position.hpp"
#include "TranspositionTable.hpp"
#include "MoveSorter.hpp"

class Solver {
	private:
		int columnOrder[Position::WIDTH];

		TranspositionTable table;

		int negamax(const Position &P, int alpha, int beta) {
			uint64_t next = P.possibleNonLosingMoves();
			if (next == 0) {
				return -(Position::WIDTH * Position::HEIGHT - P.nMoves()) / 2;
			}

			if (P.nMoves() >= Position::WIDTH * Position::HEIGHT - 2) {
				return 0;
			}

			int min = -(Position::WIDTH * Position::HEIGHT - 2 - P.nMoves()) / 2;
			if (alpha < min) {
				alpha = min;
				if (alpha >= beta) {
					return alpha;
				}
			}

			int max = (Position::WIDTH * Position::HEIGHT - 1 - P.nMoves()) / 2;
			if (int val = table.get(P.key())) {
				max = val + Position::MIN_SCORE - 1;
			}

			if (beta > max) {
				beta = max;
				if (alpha >= beta) {
					return beta;
				}
			}

			MoveSorter moves;

			for (int i = Position::WIDTH; i -= 1; ) {
				if(uint64_t move = next & Position::column_mask(columnOrder[i])) {
					moves.add(move, P.moveScore(move));
				}
			}

			while(uint64_t next = moves.getNext()) {
				Position P2(P);
				P2.play(next);
				int score = -negamax(P2, -beta, -alpha);

				if (score >= beta) {
					return score;
				}
				if (score > alpha) {
					alpha = score;
				}
			}

			table.put(P.key(), alpha - Position::MIN_SCORE + 1);
			return alpha;
		}

	public:
		int solve(const Position &P) {
			P.print();
			if (P.canWinNext()) {
				return (Position::WIDTH * Position::HEIGHT - P.nMoves()) / 2;
			}
			int min = -(Position::WIDTH * Position::HEIGHT - P.nMoves()) / 2;
			int max = (Position::WIDTH * Position::HEIGHT + 1 - P.nMoves()) / 2;

			while (min < max) {
				int med = min + (max - min) / 2;
				if (med <= 0 && min / 2 < med) {
					med = min / 2;
				}
				else if (med >= 0 && max / 2 > med) {
					med = max / 2;
				}

				//int r = 1;
				int r = negamax(P, med, med + 1);
				if (r <= med) {
					max = r;
				}
				else {
					min = r;
				}
			}

			return min;
		}

		void reset() {
			table.reset();
		}

		Solver() : table(8388593) {
			reset();
			for (int i = 0; i < Position::WIDTH; i += 1) {
				columnOrder[i] = Position::WIDTH / 2 + (1 - 2 * (i % 2)) * (i + 1) / 2;
			}
		}

};

#include <iostream>
int main(int argc, char** argv) {

  Solver solver;

  bool weak = false;
  if(argc > 1 && argv[1][0] == '-' && argv[1][1] == 'w') weak = true;

  std::string line;

  Position P;
  P.colPlay(0);
  P.print();

  std::cout << solver.solve(P) << std::endl;

  return 0;
}


