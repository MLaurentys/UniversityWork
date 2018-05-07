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

//endgame scene
let gameOverScene;
let finalScore;
let backToStartButton;

//pre-load images
PIXI.loader.add(
["Images/Backgrounds/Game/GameBackground.png", "Images/Backgrounds/Menu/Menu.png", "Images/Movables/Player.png", "Images/Movables/Diamond.png"]
).on("progress",e=>{console.log(`progress=${e.progress}`)}).load(setup);

