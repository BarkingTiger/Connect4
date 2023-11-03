#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// insert existing board being played
void insert(char board[6][7], int top[7], char player, int col) {
  // if column is full
  if (top[col - 1] < 0) {
    return;
  }
  printf("%c\n", board[top[col - 1]][col - 1]);
  printf("%c\n", player);
  // set spot to player color
  board[top[col - 1]][col - 1] = player;
  // substract from top layer
  top[col - 1] -= 1;
  return;
}

void side_selection(char board[6][7], int top[7], char *side, char *opponent) {
  bool selected = false;
  while (!selected) {
    // pick sides
    printf("Which side are you on?\n");
    scanf(" %c", side);
    if (*side == 'r' || *side == 'y') {
      selected = true;
    }
  }
  // set sides
  if (*side == 'r') {
    *opponent = 'y';
  }
  // if yellow, play opponent's turn first
  else {
    *opponent = 'r';
    // get input from user then insert to board
    int pick = -1;
    while (pick < 1 || pick > 7) {
      printf("Enter opponent's move\n");
      scanf(" %d", &pick);
    }
    insert(board, top, 'r', pick);
  }
  return;
}

// may change this to be better but for now fuck it
bool check_winner(char board[6][7]) {
  for (int i = 0; i < 6; i += 1) {
    for (int j = 0; j < 7; j += 1) {
      if (board[i][j] != '-') {
        if (board[i][j] == board[i][j + 1] &&
            board[i][j + 1] == board[i][j + 2] &&
            board[i][j + 2] == board[i][j + 3]) {
          return true;
        }
      }
    }
  }

  for (int j = 0; j < 7; j += 1) {
    for (int i = 0; i < 3; i += 1) {
      if (board[i][j] != '-') {
        if (board[i][j] == board[i + 1][j] &&
            board[i + 1][j] == board[i + 2][j] &&
            board[i + 2][j] == board[i + 3][j]) {
          return true;
        }
      }
    }
  }

  for (int i = 0; i < 3; i += 1) {
    for (int j = 0; j < 4; j += 1) {
      if (board[i][j] != '-') {
        if (board[i][j] == board[i + 1][j + 1] &&
            board[i + 1][j + 1] == board[i + 2][j + 2] &&
            board[i + 2][j + 2] == board[i + 3][j + 3]) {
          return true;
        }
      }
    }
  }

  for (int i = 3; i < 6; i += 1) {
    for (int j = 0; j < 4; j += 1) {
      if (board[i][j] != '-') {
        if (board[i][j] == board[i - 1][j + 1] &&
            board[i - 1][j + 1] == board[i - 2][j + 2] &&
            board[i - 2][j - 2] == board[i - 3][j + 3]) {
          return true;
        }
      }
    }
  }

  return false;
}

void print_board(char board[6][7]) {
	for (int i = 0; i < 6; i += 1) {
		for (int j = 0; j < 7; j += 1) {
			printf("%c ", board[i][j]);
		}
		printf("\n");
	}
}

// prints usage
void usage() { printf("BLAH BLAH BLAH INSERT GUIDE HERE\n"); }

int main() {
  char board[6][7];
  int top[7] = {5, 5, 5, 5, 5, 5, 5};
  int pick = -1;
  char side;
  char opponent;
  char choice;
  char turn = 'r';
  //bool run = true;

  // initalize board
  for (int i = 0; i < 6; i += 1) {
    for (int j = 0; j < 7; j += 1) {
      board[i][j] = '-';
    }
  }

  // main loop
  while (true) {
    usage();
    scanf(" %c", &choice);
    // characters = getline(&buffer, &size, stdin);
    printf("You have typed in %c\n", choice);
    switch (choice) {
      // new board
    case 'n':
      side_selection(board, top, &side, &opponent);
      break;
      // existing board
    case 'e':
      insert(board, top, 'r', 7);
      break;
      // help text
    case 'h':
      usage();
      break;
      // quit program
    case 'q':
      return 0;
      break;
      // help text on anything else
    default:
      usage();
      break;
    }

    if (side == 'y') {
	    turn = 'y';
    }

    //player vs player
    //make this into function so i can add the AI
    while (!check_winner(board)) {
	    if (turn == 'r') {
		    printf("RED TURN PICK COLUMN\n");
	    }
	    else {
		    printf("YELLOW TURN PICK COLUMN\n");
	    }
	    int input = scanf(" %d", &pick);
	    //input sanitization
	    if (input != 1 || pick < 1 || pick > 7) {
		    printf("INVALID INPUT\n");
		    while( getchar() != '\n');
		    continue;
	    }
	    //if column is full
	    if (top[pick] < 0) {
		    printf("COLUMN FULL\n");
		    continue;
	    }
	    insert(board, top, turn, pick);
	    print_board(board);
	    if (turn == 'r') {
		    turn = 'y';
	    }
	    else {
		    turn = 'r';
	    }
    }

    //find who is the winner
    if (turn == 'y') {
	    printf("RED WON THE GAME\n");
    }
    else {
	    printf("YELLOW WON THE GAME\n");
    }

    //play again
    //probably need better input sanitization
    printf("PLAY AGAIN y/n\n");
    scanf(" %c", &choice);
    if (choice == 'n') {
	    break;
    }

    /*
    printf("TESTING SIDES %c\n", side);
    printf("TESTING OPPONENT %c\n", opponent);
    for (int i = 0; i < 6; i += 1) {
      for (int j = 0; j < 7; j += 1) {
        printf("%c ", board[i][j]);
      }
      printf("\n");
    }
    */
  }

  return 0;
}
