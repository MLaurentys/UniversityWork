"use strict";
function CollidePlayer(ball, player){
    if(ball.y > player.y - ball.height){
        if(ball.x < player.x + player.width/2 && ball.x > player.x - player.width/2){
            ball.vy *= -1;
        }
    }
}