"use strict";
//resetStorage();
//get current scores
let storedScores = localStorage.getItem("highscore");
storedScores += "";

let highscoreTable = storedScores.split(',');

//get current players
for(let sc of highscoreTable){
    sc = Number(sc);
}
let storedNames = localStorage.getItem("players");
storedNames += "";
let namesTable = storedNames.split(',');
//"accessor"
document.querySelector("#submitName").onclick = function(){
    addNewScore(score, document.querySelector("#playerName input").value);
}

//gets if will be placed on the table
//requests name if placing on table
set();
function register(sc){
    let enterHigh;
    enterHigh = sc > highscoreTable[9];
    if(enterHigh){
        document.querySelector("#playerName").style.visibility = "visible";
        document.querySelector("#playerName *").style.visibility = "visible";
        document.querySelector("#submitName").style.visibility = "visible";
    }
    
    return enterHigh;
}
//save current scores to storage
function addNewScore(sc, name){
    if (name == ""){
        name = "Not Assigned";
    }
    let index = highscoreTable.length - 1;
    while(index >= 0 && highscoreTable[index] < sc){
        //highscore size = 10
        if(index != 9){
            highscoreTable[index + 1] = highscoreTable[index];
            namesTable[index+1] = namesTable[index];
        }
        namesTable[index] = name;
        highscoreTable[index] = sc;
        index--;
    }
    set();
    save();
    hideHS();
}
//hide highscore selector
function hideHS(){
        document.querySelector("#playerName").style.visibility = "hidden";
        document.querySelector("#playerName *").style.visibility = "hidden";
        document.querySelector("#submitName").style.visibility = "hidden";
}

//stores on localStorage
function save(){
    let newNames = "";
    let newScores = "";
    for(let sc of highscoreTable){
        newScores += sc;
        newScores += ',';
    }
    for(let nm of namesTable){
        newNames += nm;
        newNames += ',';
    }
    localStorage.setItem("highscore", newScores);
    localStorage.setItem("players", newNames);

}

//set ranking
function set(){
    let list = document.querySelector("#ranking");
    list.innerHTML = "";
    for(let i = 0; i < 10 ; i++){
        ranking.innerHTML += "<li>" + namesTable[i] + "  -->  " + highscoreTable[i] + "</li>";
    }
}
//aux
function resetStorage(){
    localStorage.setItem("highscore", "0,0,0,0,0,0,0,0,0,0");
    localStorage.setItem("players", "tba,tba,tba,tba,tba,tba,tba,tba,tba,tba");
}

