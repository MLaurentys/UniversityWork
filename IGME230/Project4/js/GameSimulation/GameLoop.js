"use strict";

function gameLoop(){
    if(ready){
	let dt = 1/app.ticker.FPS;
    if (dt > 1/12) dt=1/12;
    player.move(dt);
    diamond.move(player, dt);
    //Engine.update(engine, dt);
    //score
    if(scoreTimer >= 1){
        scoreTimer = 0;
        increaseScoreBy(1);
    }
        scoreTimer += dt;
    }
}