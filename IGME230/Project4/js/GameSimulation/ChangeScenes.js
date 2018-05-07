"use strict";
function startGame(){
    startScene.visible = false;
    gameOverScene.visible = false;
    gameScene.visible = true;
    score = 0;
    increaseScoreBy(0);
    updateLocalMaxScore();
    //resetPlayerAndDiamond();
    //loadLevel();
    console.log("ok");
}

function resetPlayerAndDiamond(){
    player = new Player();
    gameScene.addChild(player);
    Matter.Composite.remove(world, diamond);
    gameScene.removeChild(diamond);
    diamond = new Diamond();
    gameScene.addChild(diamond);
    World.add(world, diamond);
}

function endGame() {
    paused = true;

    //updates Local Max Score!
    let tempScore = score;
    gameOverScore = tempScore > gameOverScore ? tempScore : gameOverScoreLabel; 
    gameOverScore = score;
    //gameOverScoreLabel.text = `Your final score: ${tempScore}`;
    // clear out level
    gameOverScene.visible = true;
    gameScene.visible = false;
    
}