#include "TranspositionTable.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define HEIGHT 6
#define WIDTH 7
#define MIN_SCORE -(WIDTH * HEIGHT) / 2 + 3
#define MAX_SCORE (WIDTH * HEIGHT + 1) / 2 - 3

int columnOrder[WIDTH] = {3, 4, 2, 5, 1, 6, 0};

struct Move {
	uint64_t move;
	int score;
};

typedef struct Move Move;

struct Moves {
	int size;
	Move *moves;
};

typedef struct Moves Moves;

Moves *create_moves(int size) {
	Moves *moves = (Moves *)malloc(sizeof(Moves));
	if (moves) {
		moves->moves = (Move *)malloc(sizeof(Move) * size); //width
		moves->size = 0;
		return moves;
	}
	return NULL;
}

void delete_moves(Moves **m) {
	free((*m)->moves);
	free(*m);
}

extern Table *table;

void add (Moves *moves, uint64_t move, int score) {
	int pos = moves->size++;
	for(; pos && moves->moves[pos - 1].score > score; --pos) moves->moves[pos] = moves->moves[pos - 1];
	moves->moves[pos].move = move;
	moves->moves[pos].score = score;
}

uint64_t getNext(Moves *moves) {
	if (moves->size) {
		return moves->moves[--moves->size].move;
	}
	else {
		return 0;
	}
}

void resetMoves(Moves *moves) {
	moves->size = 0;
}

/*
void printMoves(Moves *moves) {
	for int(i = 0; i < WIDTH; i += 1) {
		printf("%d\n", i);
	}
}
*/

uint64_t bottom(int width, int height) {
        return width == 0 ? 0 : bottom(width - 1, height) | 1LL << (width - 1) * (height + 1);
}

static uint64_t bottom_mask() {
        return bottom(WIDTH, HEIGHT);
}

static uint64_t board_mask() {
        return bottom_mask() * ((1LL << HEIGHT) - 1);
}

static uint64_t column_mask(int col) {
        return (((uint64_t)(1) << HEIGHT) - 1) << col * (HEIGHT + 1);
}

static uint64_t top_mask_col(int col) {
        return ((uint64_t)(1) << (HEIGHT - 1)) << col * (HEIGHT + 1);
}

static uint64_t bottom_mask_col(int col) {
        return (uint64_t)(1) << col * (HEIGHT + 1);
}

/*
static uint64_t column_mask_col(int col) {
        return (((uint64_t)(1) << HEIGHT) - 1) << col * (HEIGHT + 1);
}
*/

bool canPlay(uint64_t mask, int col) {
        return (mask & top_mask_col(col)) == 0;
}

void play(uint64_t *current_position, uint64_t *mask, uint64_t move) {
	*current_position ^= *mask;
	*mask |= move;
}

void playCol(uint64_t *current_position, uint64_t *mask, int col) {
        *current_position ^= *mask;
        *mask |= *mask + bottom_mask_col(col);
        //moves += 1;
}

/*
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
*/

/*
bool isWinningMove(uint64_t current_position, uint64_t mask, int col) {
        uint64_t pos = current_position;
        pos |= (mask & bottom_mask_col(col)) & column_mask_col(col);
        return alignment(pos);
}
*/

uint64_t compute_winning_position(uint64_t position, uint64_t mask) {

        //vertical
        uint64_t r = (position << 1) & (position << 2) & (position << 3);

        //horizontal
        uint64_t p = (position << (HEIGHT + 1)) & (position << 2 * (HEIGHT + 1));
        r |= p & (position << 3 * (HEIGHT + 1));
        r |= p & (position >> (HEIGHT + 1));
        p >>= 3 * (HEIGHT + 1);
        r |= p & (position << HEIGHT);
        r |= p & (position >> 3 * (HEIGHT + 1));

        //diagonal 1
        p = (position << HEIGHT) & (position << 2*HEIGHT);
        r |= p & (position << 3 * HEIGHT);
        r |= p & (position >> HEIGHT);
        p >>= 3 * HEIGHT;
        r |= p & (position << HEIGHT);
        r |= p & (position >> 3 * HEIGHT);

        //diagonal 2
        p = (position << (HEIGHT + 2)) & (position << 2 * (HEIGHT + 2));
        r |= p & (position << 3*(HEIGHT + 2));
        r |= p & (position >> (HEIGHT + 2));
        p >>= 3 * (HEIGHT + 2);
        r |= p & (position << (HEIGHT + 2));
        r |= p & (position >> 3*(HEIGHT + 2));

        return r & (board_mask() ^ mask);
}

uint64_t possible(uint64_t mask) {
        return (mask + bottom_mask()) & board_mask();
}

uint64_t opponent_winning_position(uint64_t current_position, uint64_t mask) {
        return compute_winning_position(current_position ^ mask, mask);
}

uint64_t possibleNonLosingMoves(uint64_t current_position, uint64_t mask) {
        uint64_t possible_mask = possible(mask);
        uint64_t opponent_win = opponent_winning_position(current_position, mask);
        uint64_t forced_moves = possible_mask & opponent_win;
        if (forced_moves) {
                if (forced_moves & (forced_moves - 1)) {
                        return 0;
                }
                else {
                        possible_mask = forced_moves;
                }
        }

        return possible_mask & ~(opponent_win >> 1);
}

bool canWinNext(uint64_t current_position, uint64_t mask) {
        return compute_winning_position(current_position, mask) & possible(mask);
}

bool isWinningMove(uint64_t current_position, uint64_t mask, int col) {
        return compute_winning_position(current_position, mask) & possible(mask) & column_mask(col);
}

unsigned int popcount(uint64_t m) {
        unsigned int c = 0;
        for (c = 0; m; c += 1) {
                m &= m - 1;
        }
        return c;
}

int moveScore(uint64_t move, uint64_t current_position, uint64_t mask) {
        return popcount(compute_winning_position(current_position | move, mask));
}

int negamax(uint64_t current_position, uint64_t mask, int moves, int alpha, int beta) {
	/*
	if (alpha < beta) {
		printf("Something went wrong.\n");
	}
	*/
	//printf("ALPHA %d\n BETA %d\n", alpha, beta);
	uint64_t next = possibleNonLosingMoves(current_position, mask);
	if (next == 0) {
		//printf("next == 0\n");
		return -(WIDTH * HEIGHT - moves) / 2;
	}

        if (moves >= WIDTH * HEIGHT - 2) {
               	//printf("END 1\n");
                return 0;
        }

	/*
        for (int x = 0; x < WIDTH; x += 1) {
                if (canPlay(mask, x) && isWinningMove(current_position, mask, x)) {
                        //printf("END 2 %d\n", x);
                        return (WIDTH * HEIGHT + 1 - moves) / 2;
                }
        }
	*/

	int min = -(WIDTH * HEIGHT - 2 - moves) / 2;
        if (alpha < min) {
                alpha = min;
                if (alpha >= beta) {
			//printf("alpha >= beta\n");
                        return alpha;
                }
        }

        int max = (WIDTH * HEIGHT - 1 - moves) / 2;
        int val = get(table, tableIndex(table, current_position + mask));
        if (val) {
                //printf("VAL %d\n", val);
                max = val + MIN_SCORE - 1;
        }

        if (beta > max) {
                beta = max;
                if (alpha >= beta) {
                        //printf("END 3\n");
			//printf("alpha >= beta 2\n");
                        return beta;
                }
        }

	Moves *movesArray = create_moves(WIDTH);

	/*
	for (int i = 0; i < WIDTH; i += 1) {
                printf("MOVE %" PRIu64 " ", movesArray->moves[i].move);
                printf("SCORE %d ", movesArray->moves[i].score);
		printf("\n");
        }
        //printf("\n");
	*/
	
	

	/*	
	if (moves == 1) {
		printf("ALPHA %d\n", alpha);
		return 1;
	}
	*/
	
	

	for (int i = WIDTH; i--; ) {
		uint64_t m = next & column_mask(columnOrder[i]);
		//printf("COLUMN %d ", columnOrder[i]);
		//printf("NEXT %" PRIu64 " ", next);
		//printf("MASK %" PRIu64 " ", column_mask(columnOrder[i]));
		if (m) {
			add(movesArray, m, moveScore(m, current_position, mask));
		}
		//printf("\n");
	}
	//printf("\n");
	
	/*	
	for (int i = 0; i < WIDTH; i += 1) {
                printf("MOVE %" PRIu64 " ", movesArray->moves[i].move);
                printf("SCORE %d ", movesArray->moves[i].score);
                printf("\n");
        }
        printf("\n");
	*/


	uint64_t nextCol = getNext(movesArray);

	while(nextCol) {

		uint64_t copy_position = current_position;
		uint64_t copy_mask = mask;

		play(&copy_position, &copy_mask, nextCol);
		int score = -negamax(copy_position, copy_mask, moves + 1, -beta, -alpha);
		//printf("TEST\n");
		if (score >= beta) {
			delete_moves(&movesArray);
			return score;
		}
		if (score > alpha) {
			alpha = score;
		}

		nextCol = getNext(movesArray);
		if (nextCol == 0) {
			break;
		}
	}

	/*
        for (int x = 0; x < WIDTH; x += 1) {
                if (canPlay(mask, x)) {
                        //need to make position and mask pointers so they retain the play
                        uint64_t copy_position = current_position;
                        uint64_t copy_mask = mask;

                        play(&copy_position, &copy_mask, x);

                        int score = -negamax(copy_position, copy_mask, moves + 1, -beta, -alpha);

                        if (score >= beta) {
                                //printf("END 4\n");
                                return score;
                        }
                        if (score > alpha) {
                                alpha = score;
                        }
                }
        }
	*/

        put(table, current_position + mask, alpha - MIN_SCORE + 1);
	delete_moves(&movesArray);
        //printf("AFTER PUT\n");
        return alpha;
}

/*
int solve(uint64_t position, uint64_t mask, int moves, int col) {
        if (canWinNext(position, mask)) {
                return (WIDTH * HEIGHT + 1 - moves) / 2;
        }
        uint64_t copy_position = position;
        uint64_t copy_mask = mask;
        playCol(&copy_position, &copy_mask, col);
        //int min = -(WIDTH * HEIGHT - moves) / 2;
        //int max = (WIDTH * HEIGHT + 1 - moves) / 2;
	int min = -1;
	int max = 1;
	//printf("MIN %d\n MAX %d\n", min, max);
        while (min < max) {
                int med = min + (max - min) / 2;
		//printf("med %d\n", med);
                if (med <= 0 && min / 2 < med) {
                        med = min / 2;
                }
                else if (med >= 0 && max / 2 > med) {
                        med = max / 2;
                }
                int r = negamax(copy_position, copy_mask, moves, med, med + 1);
		//printf("R %d\n", r);

                if (r <= med) {
                        max = r;
                }
                else {
                        min = r;
                }
		//printf("%d %d\n", min, max);
        }

	//int r = negamax(copy_position, copy_mask, moves, max, min);
	//min = r;
	printf("DONE\n");
        return min;
}*/

int solve(uint64_t position, uint64_t mask, int moves, int col) {
	if (canWinNext(position, mask)) {
		return (WIDTH * HEIGHT + 1 - moves) / 2;
	}
	uint64_t copy_position = position;
	uint64_t copy_mask = mask;
	playCol(&copy_position, &copy_mask, col);
	
	int min = -(WIDTH * HEIGHT - moves) / 2;
	int max = (WIDTH * HEIGHT + 1 - moves) / 2;

	while (min < max) {
		int med = min + (max - min) / 2;
		if (med <= 0 && min / 2 > med) {
			med = min / 2;
		}
		else if (med >= 0 && max / 2 > med) {
			med = max / 2;
		}
		
		int r = negamax(copy_position, copy_mask, moves, med, med - 1);
		if (r <= med) {
			max = r;
		}
		else {
			min = r;
		}
	}

	return min;
}

int playOptimalMove(char board[6][7], char side) {
        uint64_t position = 0;
        uint64_t mask = 0;
        int counter = 0;
        int moves = 0;
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
	position ^= mask;
        printf("POSITION %" PRIu64 "\n", position);
        printf("MASK %" PRIu64 "\n", mask);



	/*	
        int test = -99;
	
        for (int i = 6; i >= 0; i -= 1) {
                if (canPlay(mask , i)) {
                        int score = solve(position, mask, moves, i);
			if (score > test) {
			       test = score;
			}
			reset(table);
			printf("SCORE %d\n", score);
			printf("DONE %d\n", i);	
                }
        }
	*/
	
	
	int test = solve(position, mask, moves, 1);
	//printAll(table);
	return test;

        //return solve(position, mask, moves, 3);
        //return negamax(position, mask, moves, -21, 21);
}



                                                                                                                    
