
function test() {
    if (gameOver || currPlayer == playerChoice) {
        return;
    }
    console.log(board);
    let choice = Math.floor(Math.random() * 7);
    while (currColumns[choice] < 0) {
        choice = Math.floor(Math.random() * 7);
    }
    let tile = document.getElementById("0-" + choice.toString());
    tile.click();
}

function negamax(board, depth, alpha, beta) {
    //check for draw game
    //if number of moves is equal to 42 return 0
    if (moves == 42) {
        return 0;
    }

    //checks if the curent player can win the next move
    for (let x = 0; x < 7; x += 1) {
        if (currColumns[xc] < 0 && checkWinner()) {
            return (49 - moves) / 2
        }
    }

    //initialize the best possible score with a lower bound score
    var bestScore = -42; 

    //compute the score of all possible moves and keep the best one
    for (let x = 0; x < 7; x += 1) {
        //if can play then play it
        //if current player plays col x, his score will be the opposite of opponent's score after
        let score = -negamax(board, depth -1 , -beta, -alpha);
        if (score > bestScore) {
            bestScore = score;
        }
    }

    return bestScore;
}