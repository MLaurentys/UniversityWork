"use strict";
function startCreateInteractibles(){
    player = new Player();
    gameScene.addChild(player);
    
    diamond = new Diamond();
    gameScene.addChild(diamond);
    World.add(world, diamond.body);

}