#ifndef POSITION_HPP
#define POSITION_HPP

#include <iostream>
#include <string>
#include <cstdint>

constexpr static uint64_t bottom(int width, int height) {
	return width == 0 ? 0 : bottom(width - 1, height) | 1LL << (width - 1) * (height + 1);
}

class Position {
	public:
		static constexpr uint64_t column_mask(int col) {
			return ((UINT64_C(1) << HEIGHT) - 1) << col * (HEIGHT + 1);
		}

		static const int WIDTH = 7;
		static const int HEIGHT = 6;
		static const int MIN_SCORE = -(WIDTH * HEIGHT) / 2 + 3;
		static const int MAX_SCORE = (WIDTH * HEIGHT + 1) / 2 - 3;

		void play(uint64_t move) {
			current_position ^= mask;
			mask |= move;
			moves += 1;
		}

	  	void colPlay(int col) {
			playCol(col);	
		}

		bool canWinNext() const {
			return winning_position() & possible();
		}

		int nMoves() const {
			return moves;
		}

		uint64_t key() const {
			return current_position + mask;
		}

		uint64_t possibleNonLosingMoves() const {
			uint64_t possible_mask = possible();
			uint64_t opponent_win = opponent_winning_position();
			uint64_t forced_moves = possible_mask & opponent_win;
			if(forced_moves) {
				if(forced_moves & (forced_moves - 1)) {
					return 0;
				}
				else {
					possible_mask = forced_moves;
				}
			}
			
			return possible_mask & ~(opponent_win >> 1);
		}

		int moveScore(uint64_t move) const {
			return popcount(compute_winning_position(current_position | move, mask));
		}

		void print() const {
			std::cout << this->current_position << std::endl;
			std::cout << this->mask << std::endl;
		}

		void setCurrent(uint64_t position) {
			current_position = position;
		}

		void setMask(uint64_t mask) {
			this->mask = mask;
		}

		void setMoves(unsigned int moves) {
			this->moves = moves;
		}

		uint64_t getCurrent() {
			return this->current_position;
		}

		uint64_t getMask() {
			return this->mask;
		}

		bool canPlay(int col) const {
			return (mask & top_mask_col(col)) == 0;
		}

		bool isWinningMove(int col) const {
			return winning_position() & possible() & column_mask(col);
		}

		Position() : current_position{0}, mask{0}, moves{0} {}

	private:
                uint64_t current_position;
                uint64_t mask;
                unsigned int moves;

                void playCol(int col) {
                        play((mask + bottom_mask_col(col)) & column_mask(col));
                }

                uint64_t winning_position() const {
                        return compute_winning_position(current_position, mask);
                }

                uint64_t opponent_winning_position() const {
                        return compute_winning_position(current_position ^ mask, mask);
                }

                uint64_t possible() const {
                        return (mask + bottom_mask) & board_mask;
                }

                static unsigned int popcount(uint64_t m) {
                        unsigned int c = 0;
                        for (c = 0; m; c += 1) {
                                m &= m - 1;
                        }
                        return c;
                }

                static uint64_t compute_winning_position(uint64_t position, uint64_t mask) {
                        //vertical
                        uint64_t r = (position << 1) & (position << 2) & (position << 3);

                        //horizontal
                        uint64_t p = (position << (HEIGHT + 1)) & (position << 2 * (HEIGHT + 1));
                        r |= p & (position << 3*(HEIGHT+1));
                        r |= p & (position >> (HEIGHT+1));
                        p = (position >> (HEIGHT+1)) & (position >> 2*(HEIGHT+1));
                        r |= p & (position << (HEIGHT+1));
                        r |= p & (position >> 3*(HEIGHT+1));

                        //diagonal 1
                        p = (position << HEIGHT) & (position << 2*HEIGHT);
                        r |= p & (position << 3*HEIGHT);
                        r |= p & (position >> HEIGHT);
                        p = (position >> HEIGHT) & (position >> 2*HEIGHT);
                        r |= p & (position << HEIGHT);
                        r |= p & (position >> 3*HEIGHT);

                        //diagonal 2
                        p = (position << (HEIGHT+2)) & (position << 2*(HEIGHT+2));
                        r |= p & (position << 3*(HEIGHT+2));
                        r |= p & (position >> (HEIGHT+2));
                        p = (position >> (HEIGHT+2)) & (position >> 2*(HEIGHT+2));
                        r |= p & (position << (HEIGHT+2));
                        r |= p & (position >> 3*(HEIGHT+2));

                        return r & (board_mask ^ mask);
                }

                const static uint64_t bottom_mask = bottom(WIDTH, HEIGHT);
                const static uint64_t board_mask = bottom_mask * ((1LL << HEIGHT) - 1);

                static constexpr uint64_t top_mask_col(int col) {
                        return UINT64_C(1) << ((HEIGHT - 1) + col * (HEIGHT + 1));
                }

                static constexpr uint64_t bottom_mask_col(int col) {
                        return UINT64_C(1) << col * (HEIGHT + 1);
                }


};

#endif
