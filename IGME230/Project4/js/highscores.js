"use strict";
//get current scores
let storedScores = localStorage.getItem("highscore");
storedScores += "";
let highscoreTable = storedScores.split(',');

//get current players
let storedNames = localStorage.getItem("players");
for(let sc of highscoreTable){
    sc = Number(sc);
}
console.log(highscoreTable);

function addNewScore(sc, name){
    let index = highscoreTable.length-1;
    while(index >= 0 && highscoreTable[index] < sc){
        //highscore size = 10
        if(index != 9){
            highscoreTable[index + 1] = highscoreTable[index];
        }
        //highscoreTable[index] = 
    }
}