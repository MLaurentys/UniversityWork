"use strict";
class Diamond extends PIXI.Sprite{
    constructor(x=SCENE_WIDTH/2,y=100){
        super(PIXI.loader.resources["Images/Movables/Diamond.png"].texture);
        this.anchor.set(.5,.5);
        this.scale.set(.8);
        /*
        this.x = x;
        this.y = y;
        this.vy = 2;
        this.vx = 0;
        this.maxSpeed = 2; //= vy + vx
        this.body = Matter.Bodies.fromVertices(this.x, this.y, [{x: this.x - 21.6, y: this.y - 22.4}, {x: this.x + 21.6, y: this.y - 22.4}, {x: this.x, y: this.y + 22.4}]);
        
        this.body.position.x = this.x;
        this.body.position.y = this.y;

        console.log("X: (body: " + this.body.position.x + ") (sprite: " + this.x + ")");
        console.log("y: (body: " + this.body.position.y + ") (sprite: " + this.y + ")");*/

        this.body = Matter.Bodies.fromVertices(x - this.width/2, y - this.height/2, [{x: x - 21.6, y: y - 22.4}, {x: x + 21.6, y: this.y - 22.4}, {x: x, y: y + 22.4}]);
        this.body.velocity.y = 2;
        this.x = this.body.position.x;
        this.y = this.body.position.y;
        this.vx = 0;
        this.vy = 0;
        this.maxSpeed = 4; //mas speed == vx^2 + vy^2
        console.log(this.body.velocity);
    }
    collidePlayer(player){
        if(this.y > player.y - this.height){
            if(this.x < player.x + player.width/2 && this.x > player.x - player.width/2){
                
                //CONVERTS VY into VX or VX into VY
                //if exceeding, ignores
                
                //depending on the zone of the player, throw the ball in a certain angle
                //__z1\z2|z3/z4__
                
                //z4 -- 30 degrees
                if(this.x > player.x + player.width/4){
                    //change angle
                    if(this.maxSpeed*Math.cos(Math.PI*1/6) > this.vx){
                        this.vx = Math.cos(Math.PI*1/6)*this.maxSpeed;
                        this.vy = this.maxSpeed - this.vx;
                        console.log("30");
                    }
                }
                //z3 --60 degrees
                else if(this.x > player.x){
                    if(this.maxSpeed*Math.cos(Math.PI*1/3) > this.vx){
                        this.vx = Math.cos(Math.PI*1/3)*this.maxSpeed;
                        this.vy = this.maxSpeed - this.vx;
                        console.log("60");
                    }
                }
                //z2 -- 120 degrees
                else if (this.x > player.x - player.width/4){
                    if(this.maxSpeed*Math.cos(Math.PI*2/3) < this.vx){
                        this.vx = Math.cos(Math.PI*2/3)*this.maxSpeed;
                        this.vy = this.maxSpeed + this.vx;
                        console.log("120");
                    }
                }
                //z1 -- 150 degrees
                else{
                    if(this.maxSpeed*Math.cos(Math.PI*5/6) < this.vx){
                        this.vx = Math.cos(Math.PI*5/6)*this.maxSpeed;
                        this.vy = this.maxSpeed + this.vx;
                        console.log("150");
                    }
                }
                
                Matter.Body.setPosition(this.body, Vector.create(this.body.position.x, player.y - player.height/2 - this.height/2 - 1));
                this.vy *= -1;
            }
        }
    }
    
    bounceWalls(){
        if(this.x < 0){
            this.vx *= -1;
            Matter.Body.setPosition(this.body, Vector.create(this.width/2 + 1, this.y));
        }
        else if(this.x > SCENE_WIDTH){
            this.x = 2*SCENE_WIDTH - this.x;
            this.vx *= -1;
            Matter.Body.setPosition(this.body, Vector.create(SCENE_WIDTH - this.width/2 - 1, this.y));
        }
        if(this.y - this.height/2< 0){
            this.vy *= -1;
            Matter.Body.setPosition(this.body, Vector.create(this.position.x, this.height - 1));
        }
        else if(this.y + this.height/2 > SCENE_HEIGHT){
            this.vy *= -1;
            //life = 0;
            //endGame();
        }
    }
    

    move(player, amt){
        this.vx = this.body.velocity.x;
        this.vy = this.body.velocity.y;

        this.collidePlayer(player);
        this.bounceWalls();
        
        Matter.Body.setVelocity(this.body, Matter.Vector.mult(Matter.Vector.normalise(Matter.Vector.create(this.vx,this.vy)), this.maxSpeed));
        
        //Matter.Body.setVelocity(this.body, this.maxSpeed);
        this.x = this.body.position.x;
        this.y = this.body.position.y;
        
        //console.log(this.body.velocity);
    }
    

}