import { Obstacle } from "./classes/Obstacle.js";
import { Perso } from "./classes/Perso.js";
import { Pic } from "./classes/Pic.js";
import { Particle } from "./classes/Particle.js";

const windowWidth = window.innerWidth;
const windowHeight = window.innerHeight;

let game_started = false;

let cnv = document.getElementById('Game');
let ctx = cnv.getContext('2d');
cnv.height = windowHeight;
cnv.width = windowWidth;
ctx.imageSmoothingEnabled= false;


var audio = document.getElementById("mouse_over_song");
let one_time= false ;

//Chargement des images pour le play button, play_bool-> false = bouton non selectionné
let play_button_img = ["play_1","play_2"] ; 
let tab_play_button = [] ; 
let play_bool = false ; 

for(let i = 0; i < 2; i += 1){
    let img = new Image();
    img.src = "./assets/menu/images/"+play_button_img[i]+".png";
    tab_play_button.push(img); 
}

// Coordonnées de play_button
let playButtonX = 400;
let playButtonY = 270;
let playButtonWidth = tab_play_button[0].width;
let playButtonHeight = tab_play_button[0].height;

//Chargement des images pour le demo button, demo_bool-> false = bouton non selectionné
let demo_button_img = ["demo_1","demo_2"] ; 
let tab_demo_button = [] ; 
let demo_bool = false ; 

for(let i = 0; i < 2; i += 1){
    let img = new Image();
    img.src = "./assets/menu/images/"+demo_button_img[i]+".png";
    tab_demo_button.push(img); 
}

// Coordonnées de demo_button
let demoButtonX = 400;
let demoButtonY = 360;
let demoButtonWidth = tab_demo_button[0].width;
let demoButtonHeight = tab_demo_button[0].height;

//Chargement du background 
let background = new Image() ; 
background.src = "./assets/menu/images/geometry_dash_atm.png"; 

//Fonctions d'évènements 

//Fonction de survol de la souris 
function onMouseMove(event) {
    if (!game_started){
        update_menu();
        // Coordonnées de la souris 
        let mouseX = event.clientX - cnv.getBoundingClientRect().left;
        let mouseY = event.clientY - cnv.getBoundingClientRect().top;

        // Test si la souris est sur le bouton "Play"
        if (mouseX >= playButtonX &&
            mouseX <= playButtonX + playButtonWidth &&
            mouseY >= playButtonY &&
            mouseY <= playButtonY + playButtonHeight)
        {
            //Curseur de la souris est sur le bouton play
            play_bool = true;
            console.log(play_bool);
            if(one_time == false){
                audio.pause();
                audio.play();
                one_time = true ; 
            }
        }

        //Test si la souris est sur le bouton "Demo"
        else if (mouseX >= demoButtonX &&
                mouseX <= demoButtonX + demoButtonWidth &&
                mouseY >= demoButtonY &&
                mouseY <= demoButtonY + demoButtonHeight)
        {
            //Curseur de la souris est sur le bouton demo
            demo_bool = true; 
            if(one_time == false){
                audio.pause();
                audio.play();
                one_time = true ; 
            }
        }
        //On se trouve si sur le bouton play ni demo 
        else{
            one_time  = false ; 
            demo_bool = false ; 
            play_bool = false ; 
        }
    }
}

//Fonction déclenché au clic de la souris 
function onClick(event) {
    if (!game_started){
        // Coordonnées de la souris 
        let mouseX = event.clientX - cnv.getBoundingClientRect().left;
        let mouseY = event.clientY - cnv.getBoundingClientRect().top;

        // Test si la souris est sur le bouton "Play"
        if (mouseX >= playButtonX &&
            mouseX <= playButtonX + playButtonWidth &&
            mouseY >= playButtonY &&
            mouseY <= playButtonY + playButtonHeight) 
        {
           
            console.log('Bouton play cliqué.');
            //play_game() ; 
        } 

        else if (mouseX >= demoButtonX &&
                mouseX <= demoButtonX + demoButtonWidth &&
                mouseY >= demoButtonY &&
                mouseY <= demoButtonY + demoButtonHeight) 
        {
            //Mettre ici le script de la démo. 
            console.log('Bouton Demo cliqué.');
            ctx.fillRect(0,0, cnv.width, cnv.height);
            game_started = true;
            update();
            //play_demo()
        }
    }
}


function update_menu(){
        if(!game_started){
            ctx.beginPath();
            ctx.fillStyle = "#FFFFFF";
            ctx.fillRect(0,0, cnv.width, cnv.height);
            ctx.closePath();
            ctx.drawImage(background, 0, 0);
            if(play_bool == false){
                ctx.drawImage(tab_play_button[0], playButtonX, playButtonY);
            }
            else{
                ctx.drawImage(tab_play_button[1], playButtonX, playButtonY);
            }

            if(demo_bool == false){
                ctx.drawImage(tab_demo_button[0], demoButtonX, demoButtonY);
            }
            else{
                ctx.drawImage(tab_demo_button[1], demoButtonX, demoButtonY);
            }
            console.log("Started");
        }  
}
cnv.addEventListener('mousemove', onMouseMove);
cnv.addEventListener('click', onClick);

// -------------------- Partie Jeu ---------------------

let perso = new Perso(0, cnv.height - 130, "./assets/cute_hero.png", 4, 0);
let bg = new Image();
bg.src = "./assets/background.png";

let pic1 = new Pic(680, cnv.height-50, 40, 50, 25);
let pic2 = new Pic(100, 0, -400, 90, 400);  

// Paramètres des background.
let x_background = 0;
let x_background_2 = cnv.width;
let speed_defilement = 5;
perso.place(ctx, cnv.width/3, cnv.height-130);

// Création des obstacles.
let obstacle_actuel = 0;
let nb_obstacle = 10 ; 
let obstacleTable = [] ; 
function gen_obstacle(nb_obstacle , x , pos_x){
    let i;
    let obstacleTable = [];
    for (i = 0; i < nb_obstacle; i++) {
        let obstacle = new Obstacle(getRandomInt(1000, 1500),cnv.height - 130,80,80,"./assets/cube.png")
        obstacle.x = x ; 
        obstacle.collisionBox.pos.x = pos_x ; 
        obstacleTable.push(obstacle);
    }
    return obstacleTable ; 
}
obstacleTable = gen_obstacle(nb_obstacle,1300,1300)

// Création des platformes.
let i ; 
let platform_table = [];
let nb_platform = 2;
let platform_actuelle = 0;
for (i = 0; i < nb_obstacle; i++) {
    let obstacle = new Obstacle(1500,cnv.height - 250,200,30,"./assets/cube.png")
    platform_table.push(obstacle);
}

const particles = [];

document.addEventListener("keydown", function (event) {
    if (event.code == "Space" && perso.collision) {
        perso.speedy -= 10;
        console.log("JUMP");
    }
});

function getRandomInt(min, max) {
  min = Math.ceil(min);
  max = Math.floor(max);
  return Math.floor(Math.random() * (max - min + 1)) + min;
}

// Gestion du score. 
let score = 0;
// Fonction pour augmenter le score
function increaseScore() {
    score+=0.1;
    drawScore();
}

// Fonction pour réinitialiser le score
function resetScore() {
    score = 0;
    updateScore();
    drawScore();
}

// Fonction pour dessiner le score sur le cnv
function drawScore() {
    ctx.font = '30px Arial';
    ctx.fillStyle = 'white';
    ctx.fillText('Score: ' + parseInt(score), 10, 40);
}

function draw_SAT_box(ctx, box, fill, stroke, color) {
    ctx.beginPath();
    ctx.rect(box.pos.x, box.pos.y, box.w, box.h);
    if (fill) {
        ctx.fillStyle = color;
        ctx.fill();
    } else if (stroke) {
        ctx.strokeStyle = color;
        ctx.stroke();
    }
    ctx.closePath();
}

function init() {
    // Positionner les particules autour du personnage
    for (let i = 0; i < 10; i++) {
      const offsetX = getRandomInt(-20, 20);
      const offsetY = getRandomInt(-20, 20);
      const x = perso.x + offsetX;
      const y = perso.y + offsetY;
      const radius = 3;
      const color = 'white';
      const velocity = {x: (Math.random() - 5) * 2,
      y: (Math.random() - 0.5) * 2 };
      particles.push(new Particle(x, y, radius, color, velocity));
    }
  }
  
init();
    function update(time) { 
        if(game_started){
            console.log("GAME_STARTED");
            // Détéction de sortie de la scene de l'obstacle --> Actualisation de l'indice.
            if(obstacleTable[obstacle_actuel].x < -80){
                if(obstacle_actuel == nb_obstacle-1){
                    obstacle_actuel = 0;
                    obstacleTable = gen_obstacle(nb_obstacle, 1300, 1300) ; 
                } 
                else {
                    obstacle_actuel++;
                }
            }

            if(platform_table[platform_actuelle].x < (0-platform_table[platform_actuelle].width)){ // Si la plateforme sors.
                if(platform_actuelle >= nb_platform){ // On dépasse le nombre de plateforme  
                    let margin = 50 ; 
                    platform_actuelle = 0;
                    let next_obs = (obstacle_actuel+1)%nb_obstacle ; 
                    for(i=0; i<nb_platform; i++){
                        let futur_pos = getRandomInt(perso.x + obstacleTable[obstacle_actuel].x + 70 , obstacleTable[obstacle_actuel].x + 400 ) ; 
                        platform_table[i].x = obstacleTable[obstacle_actuel].x + futur_pos;
                        platform_table[i].collisionBox.pos.x = obstacleTable[obstacle_actuel].x + futur_pos;
                        if( (obstacleTable[next_obs].x + obstacleTable[next_obs].width >  platform_table[i].x 
                          && obstacleTable[next_obs].x + obstacleTable[next_obs].width <  platform_table[i].x + platform_table[i].width))   
                        {
                                platform_table[i].x += 100
                                platform_table[i].collisionBox.pos.x += 100 ; 
                        }
                    }
                } else {
                    platform_actuelle++;
                }
            }

            // Gestion defillement background & score.
            if(x_background<((-cnv.width)+speed_defilement)) 
                x_background = cnv.width;

            if(x_background_2<-(cnv.width)+speed_defilement)
                x_background_2 = cnv.width;
            x_background_2 -= speed_defilement;
            x_background -= speed_defilement;

            // Gestion graphique.

            ctx.clearRect(0, 0, cnv.width, cnv.height);
            ctx.drawImage(bg, x_background, 0, cnv.width, cnv.height);
            ctx.drawImage(bg, x_background_2, 0, cnv.width, cnv.height);
            if(perso.collision){
                particles.forEach(particle => {
                    const offsetX = getRandomInt(-20, 20);
                    const offsetY = getRandomInt(+80, 70 );
                    particle.x = perso.x + offsetX;
                    particle.y = perso.y + offsetY;
                    particle.update(ctx);
                  });
            }
            perso.draw(ctx);
            perso.draw_SAT_polygon(ctx);

            // Dessine les osbtacles.
            obstacleTable[obstacle_actuel].draw(ctx);
            draw_SAT_box(ctx, obstacleTable[obstacle_actuel].collisionBox, false, true, "blue");
            // Dessine les plateformes.
            platform_table[platform_actuelle].draw(ctx);
            draw_SAT_box(ctx, platform_table[platform_actuelle].collisionBox, false, true, "blue");

            pic1.move_to_left(speed_defilement, ctx)
            pic2.move_to_left(speed_defilement, ctx);
            drawScore();

            if(SAT.testPolygonPolygon(obstacleTable[obstacle_actuel]._toPolygon(), perso.collisionPolygon) || SAT.testPolygonPolygon(platform_table[platform_actuelle]._toPolygon(), perso.collisionPolygon)
            ) {
                console.log("COLLISION OBSTACLE");
                perso.collision = true;
            } else {
                perso.collision = false;
                //perso.jump(cnv);
            }

            // Collisions des pics.
            if(SAT.testPolygonPolygon(pic1.collisionPolygon, perso.collisionPolygon) || SAT.testPolygonPolygon(pic2.collisionPolygon, perso.collisionPolygon)
            ){
                console.log("COLLISION PIC");
                perso.collision = true;
                perso.GameOver(cnv, ctx);
            }

            if(perso.y == cnv.height-130){
                perso.collision = true;
            }

            if(perso.collision){
                if (perso.x >= obstacleTable[obstacle_actuel].x - obstacleTable[obstacle_actuel].width && perso.x <= obstacleTable[obstacle_actuel].x + obstacleTable[obstacle_actuel].width
                   && perso.y <= obstacleTable[obstacle_actuel].y + obstacleTable[obstacle_actuel].height && perso.y <= obstacleTable[obstacle_actuel].y) {
                    perso.platform(cnv, obstacleTable[obstacle_actuel]);
                    console.log("TEST_obstacleee");
                }
                // ---------------  Gestion de la collision. Plateforme -------------
                if (perso.x >= platform_table[platform_actuelle].x - platform_table[platform_actuelle].width && perso.x <= platform_table[platform_actuelle].x + platform_table[platform_actuelle].width
                    && perso.y <= platform_table[platform_actuelle].y + platform_table[platform_actuelle].height && perso.y <= platform_table[platform_actuelle].y) {
                    perso.platform(cnv, platform_table[platform_actuelle]);
                    console.log("TEST_platform");
                }
            }

            // ---------------  Gestion des surfaces. Box -------------

            if(perso.is_alive){
                x_background_2 -= speed_defilement;
                x_background -= speed_defilement;
                obstacleTable[obstacle_actuel].move_to_left(speed_defilement);
                platform_table[platform_actuelle].move_to_left(speed_defilement);
                increaseScore();
                perso.jump(cnv);
            }

            draw_SAT_box(ctx, obstacleTable[obstacle_actuel].collisionBox, false, true, "blue");
            draw_SAT_box(ctx, platform_table[platform_actuelle].collisionBox, false, true, "blue");

            window.requestAnimationFrame(update);
            //console.log(time);
        }
        // setInterval(update, 15);
}


