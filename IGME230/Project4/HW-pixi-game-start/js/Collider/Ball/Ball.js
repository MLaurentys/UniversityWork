class Ball extends PIXI.Sprite{
    constructor(x=sceneWidth/2,y=10){
        super(PIXI.loader.resources["images/Spaceship.png"].texture);
        this.anchor.set(.5,.5);
        this.scale.set(0.1);
        
        this.x = x;
        this.y = y;
        this.vy = 0;
    }
}