//python3 -m http.server 8000
//http://localhost:8000/
let cnv = document.getElementById('Menu');
let ctx = cnv.getContext('2d');
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


function update(){
        

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
            
}
setInterval(update, 10);
cnv.addEventListener('mousemove', onMouseMove);
cnv.addEventListener('click', onClick);

//Fonctions d'évènements 

//Fonction de survol de la souris 
function onMouseMove(event) {
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

//Fonction déclenché au clic de la souris 
function onClick(event) {
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
        //play_demo()
    }
}

function playAudio() {
  // Obtenez l'élément audio
  var audio = document.getElementById("myAudio");

  // Vérifiez si le son est en pause
  if (audio.paused) {
    // Si le son est en pause, jouez-le
    audio.play();
  } else {
    // Si le son est en cours de lecture, mettez-le en pause
    audio.pause();
  }
}
