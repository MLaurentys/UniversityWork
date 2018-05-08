"use strict";


//Modularizing for startGame()
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
    remainingTargets = 0;
    for(let i = 0; i < Math.floor(3.5*currLevel); i++){
        console.log("adding");
        targets[i] = (new Target(50*(i%11), 40*(Math.floor(i/11))));
        gameScene.addChild(targets[i]);
        remainingTargets++;
    }
}
//Start -> Game
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

//Game -> gameOver
function endGame() {
    paused = true;
    ready = false;
    //updates Local Max Score!
    updateLocalMaxScore();
    updateFinalScore();
    //gameOverScoreLabel.text = `Your final score: ${tempScore}`;
    // clear out level
    gameOverScene.visible = true;
    gameScene.visible = false;
    
}

//gameOver -> Start
function Restart(){
    gameOverScene.visible = false;
    startScene.visible = true;
    for(let tg of targets){
        if(tg != null){
            gameScene.removeChild(tg);
            console.log("removing");
        }
        else{
            console.log("not"); 
        }
    }
    currLevel = 1;
}