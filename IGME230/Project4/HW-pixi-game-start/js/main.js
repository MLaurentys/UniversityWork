// We will use `strict mode`, which helps us by having the browser catch many common JS mistakes
// https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Strict_mode
"use strict";
const app = new PIXI.Application(600,800);
document.body.appendChild(app.view);

// constants
const sceneWidth = app.view.width;
const sceneHeight = app.view.height;	

// pre-load the images
PIXI.loader.
add(["images/Spaceship.png","images/explosions.png"]).
on("progress",e=>{console.log(`progress=${e.progress}`)}).
load(setup);

// aliases
let stage;

// game variables
let startScene;
let gameScene,player,scoreLabel,lifeLabel,shootSound,hitSound,fireballSound;
let gameOverScene;
let right;
let left;
let ball;
let circles = [];
let bullets = [];
let aliens = [];
let explosions = [];
let explosionTextures;
let score = 0;
let life = 100;
let levelNum = 1;
let paused = true;

//endgame
let finalScore;
let gameOverScoreLabel;





function startGame(){
    startScene.visible = false;
    gameOverScene.visible = false;
    gameScene.visib;e = true;
}

function increaseScoreBy(value){
    score += value;
    scoreLabel.text = `Score ${score}`
}

function decreaseLifeBy(value){
    life -= value;
    life = parseInt(life);
    lifeLabel.text = `Life ${life}%`;
}

function createCircles(numCircles){
    for(let i=0;i<numCircles;i++){
        let c = new Circle(10,0xFFFF00);
        c.x = Math.random() * (sceneWidth - 50) + 25;
        c.y = Math.random() * (sceneHeight - 400) + 25;
        circles.push(c);
        gameScene.addChild(c);
    }
}

function loadLevel(){
	createCircles(levelNum * 5);
	paused = false;
}

function fireBullet(e){
    // let rect = app.view.getBoundingClientRect();
    // let mouseX = e.clientX - rect.x;
    // let mouseY = e.clientY - rect.y;
    // console.log(`${mouseX},${mouseY}`);
    if (paused) return;
    
    let b = new Bullet(0xFFFFFF,player.x,player.y);
    if (levelNum >= 2){
        let b2 = new Bullet(0xFFFFFF,player.x+7,player.y);
        bullets.push(b2)
        gameScene.addChild(b2);
        
        let b3 = new Bullet(0xFFFFFF,player.x-7,player.y);
        bullets.push(b3)
        gameScene.addChild(b3);
    } 
    bullets.push(b)
    gameScene.addChild(b);
    shootSound.play();
}


function startGame(){
    startScene.visible = false;
    gameOverScene.visible = false;
    gameScene.visible = true;
    levelNum = 1;
    score = 0;
    life = 100;
    increaseScoreBy(0);
    decreaseLifeBy(0);
    player.x = sceneWidth/2;
    player.y = sceneHeight - 30;
    loadLevel();
}



function gameLoop(){
	// if (paused) return; // keep this commented out for now
	
	// #1 - Calculate "delta time"
	let dt = 1/app.ticker.FPS;
    if (dt > 1/12) dt=1/12;
	
	// #2 - Move player
/*	
	let mousePosition = app.renderer.plugins.interaction.mouse.global;
    //player.position = mousePosition;
    
    let amt = 6 * dt;
    
    //lerp (linear interpolate) the x & y values with lerp()
    let newX = lerp(player.x, mousePosition.x, amt);
    let newY = lerp(player.y, mousePosition.y, amt);
    
    //keep the player on the screen with clamp()
    let w2 = player.width/2;
    let h2 = player.heigth/2
    player.x = clamp(newX,0+w2,sceneWidth-w2);
    player.y = clamp(newY,0+h2,sceneHeight-h2);
*/
    //REFER TO UTILITIES -> FUNCTION
    //#2A MOVE BALL
    Move_Ball(ball, player);

	// #7 - Is game over?
	if (life <= 0 ){
        end();
        return; // return here so we skip #8 below
    }
	// #8 - Load next level
    if (circles.length == 0){
	levelNum ++;
	loadLevel();
    }
}



function end() {
    gameOverScoreLabel.text = `Your final score: ${score}`;

    paused = true;
    // clear out level
    circles.forEach(c=>gameScene.removeChild(c)); // concise arrow function with no brackets and no return
    circles = [];
    
    bullets.forEach(b=>gameScene.removeChild(b)); // ditto
    bullets = [];
    
    explosions.forEach(e=>gameScene.removeChild(e)); // ditto
    explosions = [];
    
    gameOverScene.visible = true;
    gameScene.visible = false;
    
}


















