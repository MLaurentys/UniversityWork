"use strict";
//called every second
//for now: +1 per second
function increaseScoreBy(value){
    score += value;
    scoreLabel.text = scoreLabel.text.replace(/[0-9]/g, '');
    scoreLabel.text += score;
}

function updateFinalScore(){
    gameOverScoreLabel.text = gameOverScoreLabel.text.replace(/[0-9]/g, '');
    gameOverScoreLabel.text += score;
}
//saves maximum score of the section
//allows firends competing locally
function updateLocalMaxScore(){
    if(score > localMaxScore){
        localMaxScore = score;
        localMaxScoreLabel.text = localMaxScoreLabel.text.replace(/[0-9]/g, '');
        localMaxScoreLabel.text += localMaxScore;
    }

}