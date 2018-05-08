//AABB collision with bounds of the screen
function bounceWalls(diamond){
        if(diamond.x < 0){
            diamond.vx *= -1;
            Matter.Body.setPosition(diamond.body, Vector.create(diamond.width/2, diamond.y));
        }
        else if(diamond.x > SCENE_WIDTH){
            diamond.x = 2*SCENE_WIDTH - diamond.x;
            diamond.vx *= -1;
            Matter.Body.setPosition(diamond.body, Vector.create(SCENE_WIDTH - diamond.width/2, diamond.y));
        }
        if(diamond.y - diamond.height/2< 0){
            diamond.vy *= -1;
            Matter.Body.setPosition(diamond.body, Vector.create(diamond.position.x, diamond.height));
        }
        else if(diamond.y + diamond.height/2 > SCENE_HEIGHT){
            diamond.vy *= -1;
            //life = 0;
            //endGame();
        }
    }