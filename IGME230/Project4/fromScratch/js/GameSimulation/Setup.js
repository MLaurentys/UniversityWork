"use strict"; 
function setup() {
	stage = app.stage;
    
    setupStart();
    setupGame();
    setupEnd();
    app.ticker.add(gameLoop);
}

function setupStart(){
    startScene = new PIXI.Container();
    stage.addChild(startScene);
    
}

function setupGame(){
    gameScene = new PIXI.Container();
    gameScene.visible = false;
    stage.addChild(gameScene);
    player = new Player();
    gameScene.addChild(player);
    diamond = new Diamond();
    gameScene.addChild(diamond);
    
}

function setupEnd(){
    gameOverScene = new PIXI.Container();
    gameOverScene.visible = false;
    stage.addChild(gameOverScene);
    
}