"use strict";
class Target extends PIXI.Sprite{
    constructor(x, y){
        super(PIXI.loader.resources["Images/Targets/target.png"].texture);
        this.x = 5 + x + this.width/2;
        this.y = 300 + y + this.height/2;
        this.anchor.set(.5,.5);
        this.scale.set(1);
    }
}