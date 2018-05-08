"use strict";
//creates all static labels and button across the whole game
function createLabelsAndButtons(){
    mainMenuButtonsAndLabels();
    gameButtonsAndLabels();
    endMenuButtonsAndLabels();
}
//exclusive for main screen
function mainMenuButtonsAndLabels(){
    //background
    startScene.addChild(PIXI.Sprite.fromImage("Images/Backgrounds/Menu/Menu.png"));
    //play button
    let startButton = new PIXI.Text();
    startButton.x = 102;
    startButton.y = 623;
    startButton.width = 204;
    startButton.height = 83;
    startButton.interactive = true;
    startButton.buttonMode = true;
    startButton.on("pointerup", startGame);
    startButton.on('pointerover', e=>e.target.alpha = 0.7);
    startButton.on('pointerout', e=>e.currentTarget.alpha = 1.0);
    startScene.addChild(startButton);
}
//exclusive for game scene
function gameButtonsAndLabels(){
    //Create Background
    gameScene.addChild(PIXI.Sprite.fromImage("Images/Backgrounds/Game/GameBackground.png"));
    //create Score Label
    scoreLabel = new PIXI.Text("Score: ");
    scoreLabel.style = new PIXI.TextStyle({
        fill: 0xFFFFFF,
        fontSize: 32,
        fontFamily: 'Verdana',
        stroke:0xFF0000,
        strokeThickness:6,
        alpha: 0.8
    })
    scoreLabel.x = 15;
    scoreLabel.y = 15;
    
    //create Local Max Score Label
    localMaxScoreLabel = new PIXI.Text("Local Max Score: ");
    localMaxScoreLabel.style = new PIXI.TextStyle({
        fill: 0xFFFFFF,
        fontSize: 32,
        fontFamily: 'Verdana',
        stroke:0xFF0000,
        strokeThickness:6,
        alpha: 0.8
    })
    localMaxScoreLabel.x = 220;
    localMaxScoreLabel.y = 15;
    
    
    gameScene.addChild(scoreLabel);
    gameScene.addChild(localMaxScoreLabel);


}
//exclusive for endGame
function endMenuButtonsAndLabels(){
    //background
    gameOverScene.addChild(PIXI.Sprite.fromImage("Images/Backgrounds/EndGame/endScreen.png"));
    gameOverScoreLabel = new PIXI.Text("Final Score: ");
    gameOverScoreLabel.style = new PIXI.TextStyle({
        fill: 0xFFFFFF,
        fontSize: 32,
        fontFamily: 'Verdana',
        stroke:0xFF0000,
        strokeThickness:6,
        alpha: 0.8
    })
    gameOverScoreLabel.x = 100;
    gameOverScoreLabel.y = 350;
    gameOverScene.addChild(gameOverScoreLabel);
}




