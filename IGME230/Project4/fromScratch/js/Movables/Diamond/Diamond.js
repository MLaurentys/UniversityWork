"use strict";
class Diamond extends PIXI.Sprite{
    constructor(x=0,y=100){
        super(PIXI.loader.resources["Images/Movables/Player.png"].texture);
        this.anchor.set(.5,.5);
        this.scale.set(0.1);
        this.x = x;
        this.y = y;
    }
}