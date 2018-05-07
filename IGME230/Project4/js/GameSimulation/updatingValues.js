"use strict";

function increaseScoreBy(value){
    score += value;
    scoreLabel.text = scoreLabel.text.replace(/[0-9]/g, '');
    scoreLabel.text += score;
}

function updateLocalMaxScore(){
    localMaxScoreLabel.text = localMaxScoreLabel.text.replace(/[0-9]/g, '');
    localMaxScoreLabel.text += localMaxScore;
}