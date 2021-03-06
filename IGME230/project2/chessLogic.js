let piecesBlack = ['bR','bN','bB','bQ','bK','bB','bN','bR','bP','bP','bP','bP','bP','bP','bP','bP']
let piecesWhite = ['wP','wP','wP','wP','wP','wP','wP','wP','wR','wN','wB','wQ','wK','wB','wN','wR']
let boardCols = ['a','b','c','d','e','f','g','h']
let totalSecWhite = 0;
let totalSecBlack = 0;
let btimer = document.querySelector("#bTimer");
let bMin = document.querySelector("#bMinutes");
let bSec = document.querySelector("#bSeconds");
let wtimer = document.querySelector("#wTimer");
let wMin = document.querySelector("#wMinutes");
let wSec = document.querySelector("#wSeconds");
let time = setInterval(updateTimer, 1000);
//--------------------------
//-------Make_Board---------
//--------------------------
//1
"use strict"
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
let boardTop = Number(window.getComputedStyle(container, null).getPropertyValue("margin-top").split("px")[0]);
let boardLeft = Number(window.getComputedStyle(container, null).getPropertyValue("margin-left").split("px")[0]);
//5
for(let row = 0; row < NUM_ROWS; row++){
    cells.push([]);
    for(let col=0;col<NUM_COLS;col++){
        let cell = span.cloneNode();
        cell.style.left = boardLeft + col * (CELL_WIDTH + CELL_SPACING) + "px";
        cell.style.top = boardTop + row * (CELL_WIDTH + CELL_SPACING) + "px";
        cell.selected = false;
        if((row + col) % 2 == 0){
            cell.style.backgroundColor = "lightgray";
        } 
        else{
            cell.style.backgroundColor = "black";
        }
        container.appendChild(cell);
        cells[row][col] = cell;
    }
    container.style.width = 8*CELL_WIDTH;
    container.style.height = 8*CELL_WIDTH;
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
    switch(cells[num1][num2].piece){
        case "wR":
            cells[num1][num2].innerHTML = "<img class=\"piece\" src=\"pieces/wR.jpeg\" alt=\"White Rook\" >";
            break;
        case "wB":
            cells[num1][num2].innerHTML = "<img class=\"piece\" src=\"pieces/wB.jpeg\" alt=\"White Bishop\" >";
            break;
        case "wN":
            cells[num1][num2].innerHTML = "<img class=\"piece\" src=\"pieces/wN.jpeg\" alt=\"White Knight\" >";
            break;
        case "wK":
            cells[num1][num2].innerHTML = "<img class=\"piece\" src=\"pieces/wK.jpeg\" alt=\"White King\" >";
            break;
        case "wQ":
            cells[num1][num2].innerHTML = "<img class=\"piece\" src=\"pieces/wQ.jpeg\" alt=\"White Queen\" >";
            break;
        case "wP":
            cells[num1][num2].innerHTML = "<img class=\"piece\" src=\"pieces/wP.jpeg\" alt=\"White Pawn\" >";
            break;
        case "bR":
            cells[num1][num2].innerHTML = "<img class=\"piece\" src=\"pieces/bR.jpeg\" alt=\"Black Rook\" >";
            break;
        case "bB":
            cells[num1][num2].innerHTML = "<img class=\"piece\" src=\"pieces/bB.jpeg\" alt=\"Black Bishop\" >";
            break;
        case "bN":
            cells[num1][num2].innerHTML = "<img class=\"piece\" src=\"pieces/bN.jpeg\" alt=\"Black Knight\" >";
            break;
        case "bK":
            cells[num1][num2].innerHTML = "<img class=\"piece\" src=\"pieces/bK.jpeg\" alt=\"Black King\" >";
            break;
        case "bQ":
            cells[num1][num2].innerHTML = "<img class=\"piece\" src=\"pieces/bQ.jpeg\" alt=\"Black Queen\" >";
            break;
        case "bP":
            cells[num1][num2].innerHTML = "<img class=\"piece\" src=\"pieces/bP.jpeg\" alt=\"Black Pawn\" >";
            break;
        default:
            cells[num1][num2].innerHTML = cells[num1][num2].piece;
    }
    
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
let killedKing = "";
//7
container.onclick = selectCell;
let toPlay = 'w';

let pieceGrabbed = null;
let grabRow = null;
let grabCol = null;
//8

function canMove(num1,num2){
    let retVal = false;
    let clean = true;
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
                        if(lastMove == ("P" + boardCols[num2] + (8-(num1+1)) + boardCols[num2] + (8-(num1-1)))){
                            removePiece(num1-1,num2);
                            enPassant = true;
                            retVal = true;
                        }
                    }
                }
            }
            if(retVal){
                if(num1 == 0 || num1 == 7){
                    cells[grabRow][grabCol].piece = cells[grabRow][grabCol].piece[0] + 'Q';
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
            if((Math.abs(num1 - grabRow) == 2 && Math.abs(num2-grabCol) == 1) || (Math.abs(num1 - grabRow) == 1 && Math.abs(num2-grabCol) == 2)){
                if(cells[num1][num2].piece != null){
                    if(cells[num1][num2].piece[0] != pieceGrabbed[0]){
                        retVal = true;
                    }
                }
                else{
                    retVal = true;
                }
            }
            

            break;
        case 'R':
            //check if in line
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
            if(Math.abs(num1-grabRow) < 2 && Math.abs(num2-grabCol) < 2){
                if(cells[num1][num2].piece != null){
                    if(cells[num1][num2].piece[0] != pieceGrabbed[0]){
                        retVal = true;
                    }
                }
                else{
                    retVal = true;
                }
            }
            break;
        case 'Q':
            //check if diagonal
            if(Math.abs(num1-grabRow) == Math.abs(num2-grabCol)){
                //checks if path is clean
                let rowDir = (num1-grabRow)/Math.abs(num1-grabRow);
                let colDir = (num2-grabCol)/Math.abs(num2-grabCol);
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
            //check if in line
            else if((num1-grabRow == 0) && (num2-grabCol != 0)){
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
    }
    if(cells[num1][num2].piece != null){
        if(cells[num1][num2].piece[1] == 'K'){
            killedKing = cells[num1][num2].piece[0];
        }
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
    let mouseX = e.clientX - cells[0][0].offsetLeft;
    let mouseY = e.clientY - cells[0][0].offsetTop;
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

wtimer.style.marginTop=boardTop + 3*CELL_WIDTH + "px";
wtimer.style.marginLeft=boardLeft + 8*CELL_WIDTH + "px";
btimer.style.marginTop=boardTop - 15 + "px";
btimer.style.marginLeft=boardLeft + 8*CELL_WIDTH + "px";
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
let ol = document.querySelector("#log ol");
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
    if(killed){
        lastMove += 'x';
    }
    lastMove += boardCols[num2];
    lastMove += 8-num1;
    if(enPassant){lastMove += '!'}
    currLi.innerHTML += lastMove;
    currLi.innerHTML += "  ";
    if(killedKing){
        currLi.innerHTML += "<br>" ;
        currLi.innerHTML += "GAME ENDED" ;
        currLi.innerHTML += "<br/>";
        currLi.innerHTML += killedKing=='w'? "Black":"White" + " Won!";
        endGame();
    }
}

//--------------------------
//------Ends_The_Game-------
//--------------------------

function endGame(){
    clearInterval(time);
    container.onclick = null;
}





