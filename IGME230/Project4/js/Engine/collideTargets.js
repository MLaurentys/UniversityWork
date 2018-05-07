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
    if(diamond.y - diamond.height/2 < targets[index].y+targets[index].height/2){
        console.log("aqui");
        for(tf of targets){
            if(tf==null){
                console.log("error");
                continue;
            }
            if(diamond.y - diamond.height/2 < tf.y + tf.height/2  &&
              diamond.y + diamond.height/2 > tf.y - tf.height/2 &&
               diamond.x - diamond.width/2 < tf.x + tf.width/2 &&
               diamond.x + diamond.width/2 > tf.x - tf.width/2)
                {
                    gameScene.removeChild(tf);
                    tf = null;
                    diamond.vy *= -1;
                    diamond.vx *= -1;
                }
        }
    }
}