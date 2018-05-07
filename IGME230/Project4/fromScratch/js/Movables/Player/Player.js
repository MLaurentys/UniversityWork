"use strict";
class Player extends PIXI.Sprite{
        constructor(x=SCENE_WIDTH/2,y=SCENE_HEIGHT - 30){
        super(PIXI.loader.resources["Images/Movables/Player.png"].texture);
        this.anchor.set(.5,.5);
        this.scale.set(1.5);
        this.x = x;
        this.y = y;
        this.speed = 2;
    }
    
    move(amt){
        if(left.isDown){
            this.x -= this.speed;
        }
        if(right.isDown){
            this.x += this.speed;
        }
    }
}