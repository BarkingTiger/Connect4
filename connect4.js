
var playerRed = "R";
var playerYellow = "Y";
var currPlayer = playerRed;
var playerChoice = "";
var gameOver = true;
var board;

var rows = 6;
var columns = 7;
var currColumns = []; //keeps track of which row each column is at.
var moves = 0;

window.onload = function() {
    setGame();
}

function enableChoice() {
    red = document.getElementById("red").style.visibility="visible";
    yellow = document.getElementById("yellow").style.visibility="visible";
}

function disableChoice() {
    red = document.getElementById("red").style.visibility="hidden";
    yellow = document.getElementById("yellow").style.visibility="hidden";
}

function clickRed() {
    playerChoice = playerRed;
    gameOver = false;
    disableChoice();
}

function clickYellow() {
    playerChoice = playerYellow;
    gameOver = false;
    disableChoice();
    test();
}

/*
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
*/

function setGame() {
    board = [];
    currColumns = [5, 5, 5, 5, 5, 5, 5];
    document.getElementById("button").style.visibility="hidden";

    for (let r = 0; r < rows; r++) {
        let row = [];
        for (let c = 0; c < columns; c++) {
            // JS
            row.push(' ');
            // HTML
            let tile = document.createElement("div");
            tile.id = r.toString() + "-" + c.toString();
            tile.classList.add("tile");
            tile.addEventListener("click", setPiece);
            tile.addEventListener("click", test);
            document.getElementById("board").append(tile);
        }
        board.push(row);
    }
}

function resetGame() {
    enableChoice();
    clearGame();
    setGame();
    let winner = document.getElementById("winner");
    winner.innerText = "";
    currPlayer = playerRed;
}

function clearGame() {
    for (let r = 0; r < rows; r += 1) {
        for (let c = 0; c < columns; c += 1) {
            let tile = document.getElementById(r.toString() + "-" + c.toString());
            tile.remove();
        }
    }
}

function setPiece() {
    if (gameOver) {
        return;
    }

    //get coords of that tile clicked
    let coords = this.id.split("-");
    let r = parseInt(coords[0]);
    let c = parseInt(coords[1]);

    // figure out which row the current column should be on
    r = currColumns[c]; 

    if (r < 0) { // board[r][c] != ' '
        return;
    }

    board[r][c] = currPlayer; //update JS board
    let tile = document.getElementById(r.toString() + "-" + c.toString());
    if (currPlayer == playerRed) {
        tile.classList.add("red-piece");
        currPlayer = playerYellow;
        moves += 1;
    }
    else {
        tile.classList.add("yellow-piece");
        currPlayer = playerRed;
        moves += 1;
    }
    r -= 1; //update the row height for that column
    currColumns[c] = r; //update the array

    checkWinner();    
    test();
    console.log(moves);
}

function checkWinner() {
     // horizontal
     for (let r = 0; r < rows; r++) {
         for (let c = 0; c < columns - 3; c++){
            if (board[r][c] != ' ') {
                if (board[r][c] == board[r][c+1] && board[r][c+1] == board[r][c+2] && board[r][c+2] == board[r][c+3]) {
                    setWinner(r, c);
                    return;
                }
            }
         }
    }

    // vertical
    for (let c = 0; c < columns; c++) {
        for (let r = 0; r < rows - 3; r++) {
            if (board[r][c] != ' ') {
                if (board[r][c] == board[r+1][c] && board[r+1][c] == board[r+2][c] && board[r+2][c] == board[r+3][c]) {
                    setWinner(r, c);
                    return;
                }
            }
        }
    }

    // anti diagonal
    for (let r = 0; r < rows - 3; r++) {
        for (let c = 0; c < columns - 3; c++) {
            if (board[r][c] != ' ') {
                if (board[r][c] == board[r+1][c+1] && board[r+1][c+1] == board[r+2][c+2] && board[r+2][c+2] == board[r+3][c+3]) {
                    setWinner(r, c);
                    return;
                }
            }
        }
    }

    // diagonal
    for (let r = 3; r < rows; r++) {
        for (let c = 0; c < columns - 3; c++) {
            if (board[r][c] != ' ') {
                if (board[r][c] == board[r-1][c+1] && board[r-1][c+1] == board[r-2][c+2] && board[r-2][c+2] == board[r-3][c+3]) {
                    setWinner(r, c);
                    return;
                }
            }
        }
    }
}

function setWinner(r, c) {
    let winner = document.getElementById("winner");
    if (board[r][c] == playerRed) {
        winner.innerText = "Red Wins";             
    } else {
        winner.innerText = "Yellow Wins";
    }
    document.getElementById("button").style.visibility="visible";
    gameOver = true;
}