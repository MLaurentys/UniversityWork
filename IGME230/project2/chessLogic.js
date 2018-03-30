let piecesBlack = ['bR','bN','bB','bQ','bK','bB','bN','bR','bP','bP','bP','bP','bP','bP','bP','bP']
let piecesWhite = ['wP','wP','wP','wP','wP','wP','wP','wP','wR','wN','wB','wQ','wK','wB','wN','wR']
let boardCols = ['a','b','c','d','e','f','g','h']
let totalSecWhite = 0;
let totalSecBlack = 0;
let bMin = document.querySelector("#bMinutes");
let bSec = document.querySelector("#bSeconds");
let wMin = document.querySelector("#wMinutes");
let wSec = document.querySelector("#wSeconds");
setInterval(updateTimer, 1000);
//--------------------------
//-------Make_Board---------
//--------------------------
//1
const NUM_COLS = 8;
const NUM_ROWS = 8;
const CELL_WIDTH = 90;
const CELL_SPACING = 0;

//2
const container = document.querySelector("#gridContainer");
//3
const span = document.createElement('span');
span.className = 'cell';
//4
const cells = [];

//5
for(let row = 0; row < NUM_ROWS; row++){
    cells.push([]);
    for(let col=0;col<NUM_COLS;col++){
        let cell = span.cloneNode();
        cell.style.left = `${col * (CELL_WIDTH + CELL_SPACING)}px`;
        cell.style.top = `${row * (CELL_WIDTH + CELL_SPACING)}px`;
        cell.selected = false;
        if((row + col) % 2 == 0){
            cell.style.backgroundColor = "white";
        } 
        else{
            cell.style.backgroundColor = "black";
        }
        container.appendChild(cell);
        cells[row][col] = cell;
    }
}

//--------------------------
//------Place_pieces--------
//--------------------------
for(let i = 0; i < cells.length; i++){
    for(let j = 0; j < cells[i].length; j++){
        if((8*i+j) < 16){
            //cells[i][j].piece = piecesBlack[i*8+j];
            placePiece(i,j,piecesBlack[i*8+j]);
            //placeImage(i,j);
        }
        else if((8*i+j) > 47){
            placePiece(i,j,piecesWhite[i*8+j - 48]);
        }
        else{
            cells.piece = null;
        }
    }
}

function placeImage(num1, num2){
    cells[num1][num2].innerHTML = cells[num1][num2].piece;
}

function placePiece(num1, num2, p){
    cells[num1][num2].piece = p;
    placeImage(num1,num2);
}
function removePiece(num1, num2){
    let retVal = cells[num1][num2].piece;
    cells[num1][num2].innerHTML = "";
    cells[num1][num2].piece = null;
    return retVal;
}

//--------------------------
//-------Move_Pieces--------
//--------------------------
let killed = false;
let enPassant = false;
//7
container.onclick = selectCell;
let toPlay = 'w';

let pieceGrabbed = null;
let grabRow = null;
let grabCol = null;
//8

function canMove(num1,num2){
    let retVal = false;
    switch(pieceGrabbed[1]){
        case 'P':
            if(pieceGrabbed[0] == 'w'){
                //move
                if(num2 - grabCol == 0){
                    //regular move forward
                    if(num1 - grabRow == -1){
                        if(cells[grabRow-1][grabCol].piece == null){
                            retVal = true;
                        }
                    }
                    //double forward move
                    if(num1 - grabRow == -2){
                        if(cells[grabRow-2][grabCol].piece == null){
                            if(grabRow == 6){
                                retVal = true;
                            }
                        }
                    }
                }
                //capture
                else if((num2 - grabCol == 1 || num2 - grabCol == -1) && (num1 - grabRow == -1)){
                    //regular capture
                    if(cells[num1][num2].piece != null){
                        if(cells[num1][num2].piece[0] == 'b'){
                            killed = true;
                            retVal = true;
                        }
                    }
                    //capture en-passant
                    else{
                    console.log("P" + boardCols[num2] + 8-(num1-1) + boardCols[num2] + 8-(num1+1));
                        if(lastMove == ("P" + boardCols[num2] + (8-(num1-1)) + boardCols[num2] + (8-(num1+1)))){
                            removePiece(num1+1,num2);
                            enPassant = true;
                            retVal = true;
                        }
                    }
                }
            }
            else{
                if(num2 - grabCol == 0){
                    //regular move forward
                    if(num1 - grabRow == 1){
                        if(cells[grabRow+1][grabCol].piece == null){
                            retVal = true;
                        }
                    }
                    //double forward move
                    if(num1 - grabRow == 2){
                        if(cells[grabRow+2][grabCol].piece == null){
                            if(grabRow == 1){
                                retVal = true;
                            }
                        }
                    }
                }
                //capture
                else if((num2 - grabCol == 1 || num2 - grabCol == -1) && (num1 - grabRow == 1)){
                    //regular capture
                    if(cells[num1][num2].piece != null){
                        if(cells[num1][num2].piece[0] == 'w'){
                            killed = true;
                            retVal = true;
                        }
                    }
                    //capture en-passant
                    else{
                    console.log("P" + boardCols[num2] + 8-(num1-1) + boardCols[num2] + 8-(num1+1));
                        if(lastMove == ("P" + boardCols[num2] + (8-(num1+1)) + boardCols[num2] + (8-(num1-1)))){
                            removePiece(num1-1,num2);
                            enPassant = true;
                            retVal = true;
                        }
                    }
                }
            }
            break;
        case 'B':
            //check if diagonal
            if(Math.abs(num1-grabRow) == Math.abs(num2-grabCol)){
                //checks if path is clean
                let rowDir = (num1-grabRow)/Math.abs(num1-grabRow);
                let colDir = (num2-grabCol)/Math.abs(num2-grabCol);
                let clean = true;
                for(let i = 1; i < Math.abs(num1-grabRow); i++){
                    if(cells[grabRow + (i*rowDir)][grabCol + (i*colDir)].piece != null){
                        clean = false;
                    }
                }
                if(clean){
                    //check if not alied piece on destination
                    if(cells[num1][num2].piece != null){
                        if(cells[num1][num2].piece[0] != pieceGrabbed[0]){
                            retVal = true;
                        }
                    }
                    else{
                        retVal = true;
                    }
                }
            }
            break;
        case 'N':
            break;
        case 'R':
            //check if in line
            let clean = true;
            if((num1-grabRow == 0) && (num2-grabCol != 0)){
                let dir = (num2-grabCol)/Math.abs(num2-grabCol);
                //check if clean
                for(let i = 1; i < Math.abs(num2-grabCol); i++){
                    if(cells[num1][grabCol+i*dir].piece != null){
                        clean = false;
                    }
                }
                if(clean){
                    if(cells[num1][num2].piece != null){
                        if(cells[num1][num2].piece[0] != pieceGrabbed[0]){
                            retVal = true;
                        }
                    }
                    else{
                        retVal = true;
                    }
                }
            }
            else if((num2-grabCol == 0) && (num1-grabRow != 0)){
                let dir = (num1-grabRow)/Math.abs(num1-grabRow);
                //check if clean
                for(let i = 1; i < Math.abs(num2-grabCol); i++){
                    if(cells[num1 + 1*dir][grabCol].piece != null){
                        clean = false;
                    }
                }
                if(clean){
                    if(cells[num1][num2].piece != null){
                        if(cells[num1][num2].piece[0] != pieceGrabbed[0]){
                            retVal = true;
                        }
                    }
                    else{
                        retVal = true;
                    }
                }
            }
            break;
        case 'K':
            break;
        case 'Q':
            break;
    }
    return retVal;
}
    

    
    
function select(row,col){
        tile = cells[row][col];
        if((pieceGrabbed = tile.piece) != null && pieceGrabbed[0] == toPlay){
            grabRow = row;
            grabCol = col;
            pieceGrabbed.selected = true;
            tile.style.borderColor = "green";
        }
        else{
            pieceGrabbed = null;
        }
}
function deSelect(){
    cells[grabRow][grabCol].style.borderColor = "darkGray";
    grabRow = null; 
    grabCol = null;
    pieceGrabbed.selected = false;
    pieceGrabbed = null;
    killed = false;
    enPassant = false;
}
function selectCell(e){
    let rect = container.getBoundingClientRect();
    let mouseX = e.clientX - rect.left;
    let mouseY = e.clientY - rect.top;
    let columnWidth = CELL_WIDTH+CELL_SPACING;
    let col = Math.floor(mouseX/columnWidth);
    let row = Math.floor(mouseY/columnWidth);
    let selectedCell = cells[row][col];
    if(pieceGrabbed != null){
        if(canMove(row,col)){
            makeLog(row, col);
            let aux = removePiece(grabRow, grabCol);
            placePiece(row,col,aux);
            if(toPlay == 'w'){toPlay = 'b';}
            else if(toPlay == 'b'){toPlay = 'w';}
            deSelect();
            
        }
        else{
            deSelect();
        }
    }
    else{
        select(row, col);
    }
}



//--------------------------
//------Updates_Clock-------
//--------------------------
function pad(val) {
  var valString = val + "";
  if (valString.length < 2) {
    return "0" + valString;
  } else {
    return valString;
  }
}
function updateTimer(){
    if(toPlay == 'w'){
        totalSecWhite++;
        wSec.innerHTML = pad(totalSecWhite%60);
        wMin.innerHTML = pad(parseInt(totalSecWhite/60));
    }
    else if(toPlay == 'b'){
        totalSecBlack++;
        bSec.innerHTML = pad(totalSecBlack%60);
        bMin.innerHTML = pad(parseInt(totalSecBlack/60));
    }
}
//--------------------------
//-----Checks_"Check"-------
//--------------------------

//--------------------------
//------Logs_The_Game-------
//--------------------------
let ol = document.querySelector("#log");
let currLi;
let lastMove = "";
function makeLog(num1, num2){
    lastMove = "";
    if(toPlay == 'w'){
        let li = document.createElement("LI");
        currLi = li;
        ol.appendChild(li);
    }
    lastMove += cells[grabRow][grabCol].piece[1];
    lastMove += boardCols[grabCol];
    lastMove += 8-grabRow;
    if(killed){lastMove += 'x'}
    lastMove += boardCols[num2];
    lastMove += 8-num1;
    if(enPassant){lastMove += '!'}
    currLi.innerHTML += lastMove;
    currLi.innerHTML += "  ";
}
            
        