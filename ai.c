#include <stdint.h>
#include <stdbool.h>

#define HEIGHT 6
#define WIDTH 7

static uint64_t top_mask(int col) {
	return ((uint64_t)(1) << (HEIGHT - 1)) << col * (HEIGHT + 1);
}

static uint64_t bottom_mask(int col) {
        return (uint64_t)(1) << col * (HEIGHT + 1);
}

static uint64_t column_mask(int col) {
	return (((uint64_t)(1) << HEIGHT) - 1) << col * (HEIGHT + 1);
}

bool canPlay(uint64_t mask, int col) {
        return (mask & top_mask(col)) == 0;
}


void play(uint64_t *current_position, uint64_t *mask, int col) {
	*current_position ^= *mask;
	*mask |= *mask + bottom_mask(col);
	//moves += 1;
}

static bool alignment(uint64_t pos) {
	//horizontal
	uint64_t m = pos & (pos >> (HEIGHT + 1));
	if (m & (m >> (2 * (HEIGHT + 1)))) {
		return true;
	}

	//diagnonal 1
	m = pos & (pos >> HEIGHT);
	if (m & (m >> (2 * HEIGHT))) {
		return true;
	}

	//diagonal 2
	m = pos & (pos << (HEIGHT + 2));
	if (m & (m >> (2 * (HEIGHT + 2)))) {
		return true;
	}

	//vertical
	m = pos & (pos >> 1);
	if (m & (m >> 2)) {
		return true;
	}

	return false;
}

bool isWinningMove(uint64_t current_position, uint64_t mask, int col) {
	uint64_t pos = current_position;
	pos |= (mask & bottom_mask(col)) & column_mask(col);
	return alignment(pos);
}

int negamax(uint64_t current_position, uint64_t mask, int moves, int alpha, int beta) {
	if (moves == WIDTH * HEIGHT) {
	return 0;
	}
	
	for (int x = 0; x < WIDTH; x += 1) {
		if (canPlay(mask, x) && isWinningMove(current_position, mask, x)) {
			return (WIDTH * HEIGHT + 1 - moves) / 2;
		}
	}

	int max = (WIDTH * HEIGHT - 1 - moves) / 2;
	
	if (beta > max) {
		beta = max;
		if (alpha >= beta) {
			return beta;
		}
	}

	for (int x = 0; x < WIDTH; x += 1) {
		if (canPlay(mask, x)) {
			//need to make position and mask pointers so they retain the play
			uint64_t copy_position = current_position;
			uint64_t copy_mask = mask;

			play(&copy_position, &copy_mask, x);

			int score = -negamax(copy_position, copy_mask, moves + 1, -beta, -alpha);

			if (score >= beta) {
				return score;
			}
			if (score > alpha) {
				alpha = score;
			}
		}
	}
	return alpha;
}

int playOptimalMove(char board[6][7], char side) {
	board[0][0] = 'r';
	char test2 = side;
	if (test2 == 'y') {
		return 0;
	}
	return 1;
}
