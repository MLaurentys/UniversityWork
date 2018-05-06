"use strict";
function Move_Ball(ball, player){
    applyGravity(ball);
    ball.y -= ball.vy;
    CollidePlayer(ball, player);
    bounceOnBalls(ball);
}