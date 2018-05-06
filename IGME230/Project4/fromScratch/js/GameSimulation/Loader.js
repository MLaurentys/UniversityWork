"use strict";
const app = new PIXI.Application(600,800);
document.body.appendChild(app.view);

// global constants
const SCENE_WIDTH = app.view.width;
const SCENE_HEIGHT = app.view.height;

//game variables
let score;
let paused = true;

//start scene
let startScene;
let playButton;
let gameTitle;


//game scene
let gameScene;
let player;
let diamond;

//endgame scene
let gameOverScene;
let finalScore;
let backToStartButton;

function startGame(){
    startScene.visible = false;
    gameOverScene.visible = false;
    gameScene.visible = true;
    score = 0;
    increaseScoreBy(0);
    player.x = SCENE_WIDTH/2;
    player.y = SCENE_HEIGHT - 30;
    loadLevel();
}

function endGame() {
    gameOverScoreLabel.text = `Your final score: ${score}`;
    paused = true;
    // clear out level
    gameOverScene.visible = true;
    gameScene.visible = false;
    
}