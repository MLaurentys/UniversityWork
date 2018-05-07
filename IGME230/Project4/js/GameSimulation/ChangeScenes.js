"use strict";
function startGame(){
    startScene.visible = false;
    gameOverScene.visible = false;
    gameScene.visible = true;
    score = 0;
    placeTargets();
    increaseScoreBy(0);
    updateLocalMaxScore();
    resetPlayerAndDiamond();
    //loadLevel();
    console.log("ok");
}

function resetPlayerAndDiamond(){

    if (diamond != null){
        Matter.Composite.remove(world, diamond);
        gameScene.removeChild(diamond);
    }
    if(player != null){gameScene.removeChild(player);}
    player = new Player();
    gameScene.addChild(player);
    diamond = new Diamond();
    gameScene.addChild(diamond);
    World.add(world, diamond.body);
    ready = true;
}

function placeTargets(){
    targets = [];

    for(let i = 0; i < Math.floor(3.5*currLevel); i++){
        console.log("adding");
        targets[i] = (new Target(50*(i%11), 40*(Math.floor(i/11))));
        gameScene.addChild(targets[i]);
    }
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