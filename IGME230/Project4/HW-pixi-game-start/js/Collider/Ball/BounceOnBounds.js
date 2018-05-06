"use strict";
function bounceOnBalls(ball){
    if(ball.x < 0){
        ball.x *= -1;
    }
    else if(ball.x > sceneWidth){
        ball.x = 2*sceneWidth - ball.x;
    }
    if(ball.y < 0){
        ball.vy *= -1;

    }
    else if(ball.y > sceneHeight){
        ball.vy *= -1;
        life = 0;
    }
}