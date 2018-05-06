"use strict"; 
function setup() {
	stage = app.stage;
	// #1 - Create the `start` scene
	startScene = new PIXI.Container();
    stage.addChild(startScene);
	// #2 - Create the main `game` scene and make it invisible
    gameScene = new PIXI.Container();
    gameScene.visible = false;
    stage.addChild(gameScene);
    //Player Input
    left = keyboard(37);   
    right = keyboard(39);
    left.press = () => {
            player.position.x -= 10;
    }
    right.press = () =>{
            player.position.x += 10;
    }
	// #3 - Create the `gameOver` scene and make it invisible
    gameOverScene = new PIXI.Container();
    gameOverScene.visible = false;
    stage.addChild(gameOverScene);
	// #4 - Create labels for all 3 scenes
	createLabelsAndButtons();
	// #5 - Create player
	player = new Player();
    gameScene.addChild(player);
    // #5A - Create !BALL!
    ball = new Ball();
    gameScene.addChild(ball);
	// #6 - Load Sounds
	shootSound = new Howl({
	src: ['sounds/shoot.wav']
    });

    hitSound = new Howl({
        src: ['sounds/hit.mp3']
    });

    fireballSound = new Howl({
        src: ['sounds/fireball.mp3']
    });
	// #7 - Load sprite sheet
	explosionTextures = loadSpriteSheet();
    
	// #8 - Start update loop
	app.ticker.add(gameLoop);
	// #9 - Start listening for click events on the canvas
	//app.view.onclick = fireBullet;
	// Now our `startScene` is visible
	// Clicking the button calls startGame()
}
