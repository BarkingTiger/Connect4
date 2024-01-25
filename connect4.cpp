#include "solver.cpp"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <unordered_map>

//prints the Connect4 board
void print_board(char board[6][7]) {
        std::cout << "\n";
        for (int i = 0; i < 6; i += 1) {
                for (int j = 0; j < 7; j += 1) {
                        std::cout << board[i][j] << " ";
                }
                std::cout << "\n";
        }
        std::cout << "\n";
}

// insert existing board being played
void insert(char board[6][7], int top[7], char player, int col) {
  // if column is full
  if (top[col - 1] < 0) {
    return;
  }
  // set spot to player color
  board[top[col - 1]][col - 1] = player;
  // substract from top layer
  top[col - 1] -= 1;
  return;
}

//sets the player and opponent side
void side_selection(char board[6][7], int top[7], char *side, char *opponent) {
  bool selected = false;
  while (!selected) {
    // pick sides
    std::cout << "Which side are you on? (y/r)" << std::endl;
    scanf(" %c", side);
    if (*side == 'r' || *side == 'y') {
      selected = true;
    }
  }
  // set sides
  if (*side == 'r') {
    *opponent = 'y';
    std::cout << "You have selected the red side." << std::endl;
    print_board(board);
  }
  // if yellow, play opponent's turn first
  else {
    *opponent = 'r';
    std::cout << "You have selected the yellow side." << std::endl;
    print_board(board);
    // get input from user then insert to board
    int pick = -1;
    while (pick < 1 || pick > 7) {
      std::cout << "Enter opponent's move" << std::endl;
      scanf(" %d", &pick);
    }
    insert(board, top, 'r', pick);
  }
  return;
}

// may change this to be better but for now fuck it
bool check_winner(char board[6][7]) {

	//horizontal
  for (int i = 0; i < 6; i += 1) {
    for (int j = 0; j < 4; j += 1) {
      if (board[i][j] != '-') {
        if (board[i][j] == board[i][j + 1] &&
            board[i][j + 1] == board[i][j + 2] &&
            board[i][j + 2] == board[i][j + 3]) {
          return true;
        }
      }
    }
  }

  //vertical
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

  //diagonal
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

  //reverse diagonal
  for (int i = 3; i < 6; i += 1) {
    for (int j = 0; j < 4; j += 1) {
      if (board[i][j] != '-') {
        if (board[i][j] == board[i - 1][j + 1] &&
            board[i - 1][j + 1] == board[i - 2][j + 2] &&
            board[i - 2][j + 2] == board[i - 3][j + 3]) {
          return true;
        }
      }
    }
  }

  return false;
}

//empties the board
void clear_board(char board[6][7]) {
	for (int i = 0; i < 6; i += 1) {
		for (int j = 0; j < 7; j += 1) {
			board[i][j] = '-';
		}
	}
}

// prints usage
void usage() {
	std::cout << "CONNECT 4 AI" << std::endl;
	std::cout << "Type 'n' to make new game." << std::endl;
	std::cout << "Type 'q' to quit." <<std:: endl;
}

int main() {
	char board[6][7];
	int top[7] = {5, 5, 5, 5, 5, 5, 5};
	int pick = -1;
	char side;
	char opponent;
	char choice;
	char turn = 'r';
	std::unordered_map<uint64_t, int> table;
	std::ifstream infile("table.txt");

        if(!infile.is_open()) {
                std::cout << "table.txt is not found." << std::endl;
        }

	//caches early game move scores to speedup the program in the early game
        std::string line;
        while(std::getline(infile, line)) {
                std::istringstream iss(line);
                uint64_t a;
                int b;
                if(!(iss >> a >> b)) {
                        break;
                }
                table.insert({a, b});
        }

	// main loop
	  while (true) {
		  clear_board(board);
		  usage();
		  scanf(" %c", &choice);
		  switch (choice) {
			   // new board
			   case 'n':
				   side_selection(board, top, &side, &opponent);
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

    		 //main gameplay loop	 	
		 while (!check_winner(board)) {
			 if (turn == 'r') {
				 std::cout << "Red's Turn. Pick a column 1 to 7. Press 0 to find the best move." << std::endl;
			 }
			 else {
				 std::cout << "Yellow's Turn. Pick a column 1 to 7. Press 0 to find the best move." << std::endl;
			 }
	    
			 int input = scanf(" %d", &pick);
			 //input sanitization

	     		 if (input != 1 || pick < 0 || pick > 7) {
				 std::cout << "Invalid input." << std::endl;
				 while( getchar() != '\n');
				 continue;
			 }
	    
			 //if column is full
			 if (top[pick - 1] < 0) {
				 std::cout << "This column is full. Pick another column." << std::endl;
				 continue;
			 }

			 //run algorithm to find best move
			 if (pick == 0) {
				 int column = playOptimalMove(board, turn, table);
				 std::cout << "You should play in column " << column << "." << std::endl;
				 print_board(board);
				 continue;
			 }

			 std::cout << "You have picked column " << pick << "." << std::endl; 
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
			 std::cout << "Red side wins the game." << std::endl;
		 }
	
		 else {
		
			 std::cout << "Yellow side wins the game." << std::endl;
	
		 }

		 std::cout << "Play again? (y/n)" << std::endl;
		 scanf(" %c", &choice);
		 if (choice == 'n') {
		
			 break;
	
		 }
		 turn = 'r'; 
	  }

	  return 0;
}
