"use strict";

function gameLoop(){
	let dt = 1/app.ticker.FPS;
    if (dt > 1/12) dt=1/12;
    player.move(dt);
    diamond.move(player, dt);
    //Engine.update(engine, dt);
}