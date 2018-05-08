"use strict";
const app = new PIXI.Application(525,700);
document.querySelector("#gameItself").appendChild(app.view);

// global constants
const SCENE_WIDTH = app.view.width;
const SCENE_HEIGHT = app.view.height;

//game variables
let score;
let localMaxScore;
let scoreTimer;

//start scene
let startScene;
let playButton;
let gameTitle;

//game keyboard
let left;
let right;

// aliases
let stage;
let Engine = Matter.Engine;
let World = Matter.World;
let Bodies = Matter.Bodies;
let Vector = Matter.Vector;
let engine;
let world;

//game scene
let gameScene;
let player;
let diamond;
let scoreLabel;
let localMaxScoreLabel;
let currLevel;
let targets;
let ready;
let remainingTargets;
let instructionsActive;
let instructions;

//endgame scene
let gameOverScene;
let gameOverScoreLabel;
let finalScore;
let backToStartButton;
let space;
let enterHighScore;
let paused = true;

//pre-load images
PIXI.loader.add(
["Images/Backgrounds/Game/GameBackground.png", "Images/Backgrounds/Menu/Menu.png", "Images/Movables/Player.png", "Images/Movables/Diamond.png",
"Images/Targets/target.png", "Images/Backgrounds/Game/Instructions.png", "Images/Backgrounds/EndGame/endScreen.png"]
).on("progress",e=>{console.log(`progress=${e.progress}`)}).load(setup);

//audio
let backSong = new Howl({
        src: ['audio/backSong.mp3'],
        loop: 1,
        volume:0.7,
        rate:0.5
    });
let endSong = new Howl({
        src: ['audio/endGame.mp3'],
        loop: 1,
        volume:0.7,
        rate:0.5
});
endSong.play();
endSong.pause();
backSong.play();
let collideAudio = new Howl({
        src: ['audio/collide.mp3'],
        volume:1,
        rate:1
});
