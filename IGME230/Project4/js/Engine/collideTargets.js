//AABB collision with targets
function collideTargets(diamond){
    let index = targets.length-1;
    //efficiency concerns
    while(targets[index] == null){
        index--;
        if(index == -1){
            //nextLevel
        }
    }
    
    //console.log(index);
    
    //efficiency
    if(diamond.y - diamond.height/2 < targets[index].y+targets[index].height/2){
        //console.log("aqui");
        for(; index >= 0; index--){
            if(targets[index]==null){
                //console.log("deleted");
                continue;
            }
            //AABB
            if(diamond.y - diamond.height/2 < targets[index].y + targets[index].height/2  &&
              diamond.y + diamond.height/2 > targets[index].y - targets[index].height/2 &&
               diamond.x - diamond.width/2 < targets[index].x + targets[index].width/2 &&
               diamond.x + diamond.width/2 > targets[index].x - targets[index].width/2)
                {
                    gameScene.removeChild(targets[index]);
                    //console.log("colided with target");
                    //changes direction
                    if(diamond.x + diamond.width/2 - 5 > targets[index].x-targets[index].width/2 && diamond.x - diamond.width/2 + 5 < targets[index].x + targets[index].width/2)
                        diamond.vy *= -1;
                    if(diamond.y + diamond.height/2 - 2> targets[index].y-targets[index].height/2 && diamond.y- diamond.height/2 + 2< targets[index].y + targets[index].height/2)
                        diamond.vx *= -1;
                    targets[index] = null;
                    remainingTargets--;
                    player.speed *= 1.1;
                    diamond.maxSpeed *= 1.1;
                    collideAudio.play();
                }
        }
    }
}