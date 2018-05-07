"use strict"; 
function setup() {
	stage = app.stage;

    
    setupStart();
    setupGame();
    setupEnd();
    createLabelsAndButtons();
    startCreateInteractibles();
    app.ticker.add(gameLoop);
}

function setupStart(){
    startScene = new PIXI.Container();
    stage.addChild(startScene);
    
}

function setupGame(){
    //matter
    engine = Engine.create();
    world = engine.world;
    Engine.run(engine);
    world.gravity.y = 0;
    world.friction = 0;
    world.frictionAir = 0;
    world.bounds = {min: {x:0,y:0}, max: {x:600, y: 800}};
    //scenes
    gameScene = new PIXI.Container();
    gameScene.visible = false;
    stage.addChild(gameScene);
    startCreateInteractibles();
    gameOverScore = 0;
    scoreTimer = 0;
    localMaxScore = 0;
    currLevel = 1;
    ready = false;
    diamond = null;
    //game keyboard input
    left = keyboard(37);
    right = keyboard(39);

    
}

function setupEnd(){
    gameOverScene = new PIXI.Container();
    gameOverScene.visible = false;
    stage.addChild(gameOverScene);
    
}