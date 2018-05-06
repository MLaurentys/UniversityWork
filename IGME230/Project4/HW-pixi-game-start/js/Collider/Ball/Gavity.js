"use strict";
function applyGravity(ball){
    ball.vy -= 0.0055553333;//-10m/s^2 -> approximately -(10/60^2)m/s^2 --> 1m = 200px -> 0.55553333
}