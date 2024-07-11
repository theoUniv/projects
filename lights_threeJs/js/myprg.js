//python3 -m http.server 8000
//http://localhost:8000/

let cnv = document.querySelector('#myCanvas');
const scene = new THREE.Scene();
const camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000);
camera.position.z = -20;
camera.position.y = 5;
const renderTarget = new THREE.WebGLRenderTarget(window.innerWidth, window.innerHeight);
let renderer = new THREE.WebGLRenderer({canvas: cnv, antialiasing: true});
renderer.setSize(window.innerWidth, window.innerHeight);
document.body.appendChild(renderer.domElement);

// Matériau pour les balles
const bulbMeshMaterial = new THREE.MeshStandardMaterial({
  color: 0xffffff,
  metalness: 0.2,
  roughness: 0.1,
  emissiveIntensity: 1,
  side: THREE.DoubleSide,
  receiveShadow: true,
  transparent: true
});
// -----------------------------------------------------

// Materiau métallique --------------------------------
const metal_material = new THREE.MeshStandardMaterial({
  color: 0x333333,
  metalness: 0.5,
  roughness: 0.1,
  receiveShadow: true,
  transparent: true
});
// -----------------------------------------------------

function getRandomInt(min, max) {
  min = Math.ceil(min);
  max = Math.floor(max);
  return Math.floor(Math.random() * (max - min + 1)) + min;
}

function getRandomHexColor() {
  const r = Math.floor(Math.random() * 256);
  const g = Math.floor(Math.random() * 256);
  const b = Math.floor(Math.random() * 256);
  const hexR = r.toString(16).padStart(2, '0');
  const hexG = g.toString(16).padStart(2, '0');
  const hexB = b.toString(16).padStart(2, '0');
  const hexColor = `#${hexR}${hexG}${hexB}`;
  return hexColor;
}

document.addEventListener('keydown', function(event) {
  if (event.key === 'ArrowRight') {
    camera.position.z +=1;
  }
  if (event.key === 'ArrowLeft') {
    camera.position.z -=1;
  }
  if (event.key === 'ArrowUp') {
    camera.position.y +=1;
  }
  if (event.key === 'ArrowDown') {
    camera.position.y -=1;
  }
});

// Création d'un dedo coloré pour l'exemple
const geometryDedo = new THREE.DodecahedronGeometry(1, 2);
const materialDedo = new THREE.MeshStandardMaterial({ color: 0xFFFFFF }); // Rouge
const dedo = new THREE.Mesh(geometryDedo, materialDedo);
dedo.castShadow = false;
dedo.receiveShadow = false;
dedo.position.set(0, 5, 0);
// --------------------------------------

// Ajout du sol ---------------
let color_base = 0x535963;
const geometry_sol = new THREE.PlaneGeometry(100, 100 );
const plane = new THREE.Mesh( geometry_sol, metal_material ); plane.lookAt(0, 10, 0);
scene.add( plane );
plane.position.set(0, -1, 0);
// -----------------------------

// CREATION DE L'AMPOULE
const bulbGeometry = new THREE.SphereGeometry(1, 8, 8);
const neckGeometry = new THREE.CylinderGeometry(0.5, 0.5, 1.3, 32);
const neckGeometry_small1 = new THREE.CylinderGeometry(0.6, 0.6, 0.1); 
const neckGeometry_small2 = new THREE.CylinderGeometry(0.6, 0.6, 0.1); 
const neckGeometry_small3 = new THREE.CylinderGeometry(0.6, 0.6, 0.1); 
const baseGeometry = new THREE.CylinderGeometry(0.7, 0.7, 0.2, 32); 
// -----------------------------

// -----------------------------
let couleur_light = 0xfaec9b;
const bulbMesh = new THREE.Mesh(bulbGeometry, new THREE.MeshStandardMaterial({ color: couleur_light,
  })); 
const neckMesh = new THREE.Mesh(neckGeometry, metal_material); // Matériau gris pour le cou
const baseMesh = new THREE.Mesh(baseGeometry, metal_material); // Matériau gris pour la base
const neckMesh1 = new THREE.Mesh(neckGeometry_small1, metal_material); // Matériau gris pour le cou
const neckMesh2 = new THREE.Mesh(neckGeometry_small2, metal_material); // Matériau gris pour le cou
const neckMesh3 = new THREE.Mesh(neckGeometry_small3, metal_material); // Matériau gris pour le cou
const geometry_string = new THREE.CylinderGeometry( 0.1, 0.1, 50, 32, 32, false, 0, 6.2 ); 
const material_string = new THREE.MeshBasicMaterial( {color: couleur_light} ); 
const cylinder_string = new THREE.Mesh( geometry_string, material_string ); scene.add( cylinder_string );
// -----------------------------

// 6 points lumineux de l'ampoule
let light_power = 100;
const spotLight_sun = new THREE.PointLight(couleur_light, light_power);
const spotLight_sun2 = new THREE.PointLight(couleur_light, light_power);
const spotLight_sun3 = new THREE.PointLight(couleur_light, light_power);
const spotLight_sun4 = new THREE.PointLight(couleur_light, light_power);
const spotLight_sun5 = new THREE.PointLight(couleur_light, light_power);
const spotLight_sun6 = new THREE.PointLight(couleur_light, light_power);
// -----------------------------

const bulbGroup = new THREE.Group();
bulbGroup.add(cylinder_string);
bulbGroup.add(bulbMesh);
bulbGroup.add(neckMesh);
bulbGroup.add(baseMesh);
bulbGroup.add(neckMesh1);
bulbGroup.add(neckMesh2);
bulbGroup.add(neckMesh3);
// -----------------------------

cylinder_string.position.set(0, 25, 0);
bulbMesh.position.set(0, 0.1, 0);
neckMesh.position.set(0, 1.3, 0);
baseMesh.position.set(0, 1, 0);
neckMesh1.position.set(0, 1.5, 0);
neckMesh2.position.set(0, 1.3, 0);
neckMesh3.position.set(0, 1.7, 0);
scene.add(bulbGroup);
scene.add(spotLight_sun, spotLight_sun2, spotLight_sun3, spotLight_sun4, spotLight_sun5, spotLight_sun6);

let positionY = 10; 
let velocityY = 0.1; 
const gravity = (Math.random() - 0.5) * 0.9; 
const restitution = -0.9;
let rebound_counter = 0;
const numberOfDedos = 10;
let to_go = 0;
let spot_rotation = 0;
let rotationAngle = 0;
let angle = 30;

const dedos = [];
for (let i = 0; i < numberOfDedos; i++) {
  const geometryDedo = new THREE.DodecahedronGeometry(1, 5);
  const materialDedo = new THREE.MeshStandardMaterial({ color: getRandomHexColor() });
  const dedo = new THREE.Mesh(geometryDedo, bulbMeshMaterial);
  dedo.position.set(
    Math.random() * 10 - 5, 
    Math.random() * 10, 
    Math.random() * 10 - 5 
  );
  dedo.userData = {
    positionY: Math.random() * 15, 
    positionX: Math.random() * 2, 
    velocityY: Math.random() * 0.2, 
    gravity: -0.005,
    restitution: -0.9,
    is_fallen: false,
    rebound_counter: 0
  };
  scene.add(dedo);
  dedos.push(dedo);
}

// Création de la géométrie pour le mur
const wallGeometry = new THREE.PlaneGeometry(20, 10); // Définis la taille de ton mur
// Création du matériau réfléchissant
const mirrorMaterial = new THREE.MeshStandardMaterial({
  color: 0xffffff,
  metalness: 1, // Augmente la réflexion métallique
  roughness: 0, // Réduit la rugosité pour une réflexion plus nette
  envMapIntensity: 1, // Contrôle l'intensité de la réflexion
  side: THREE.DoubleSide // Affiche le matériau des deux côtés de la géométrie
});

// Création du maillage pour le mur
const wallMesh = new THREE.Mesh(wallGeometry, mirrorMaterial);
wallMesh.position.set(10, 10, 10); // Positionne le mur derrière les "dedos"
//scene.add(wallMesh); // Ajoute le mur à la scène
wallMesh.lookAt(10, 0, 0);
const wallMesh2 = new THREE.Mesh(wallGeometry, mirrorMaterial);
wallMesh2.position.set(20, 5, 10); // Positionne le mur derrière les "dedos"
scene.add(wallMesh2); // Ajoute le mur à la scène
wallMesh2.lookAt(0, 0, 0);

const wallMesh3 = new THREE.Mesh(wallGeometry, mirrorMaterial);
wallMesh3.position.set(-20, 5, 10); // Positionne le mur derrière les "dedos"
scene.add(wallMesh3); // Ajoute le mur à la scène
wallMesh3.lookAt(0, 0, 0);



function update() {
  requestAnimationFrame(update);

  const radius = 10;
  const spotLightX = radius * Math.sin(spot_rotation);
  const spotLightZ = radius * Math.cos(spot_rotation);
  bulbGroup.position.set(spotLightZ, radius-5, spotLightX);
  // Déplacement des points lumineux de l'ampoule en fonction de la position de celui_ci.
  spotLight_sun.position.set(bulbGroup.position.x, bulbGroup.position.y-2, bulbGroup.position.z);
  spotLight_sun2.position.set(bulbGroup.position.x, bulbGroup.position.y+2, bulbGroup.position.z);
  spotLight_sun3.position.set(bulbGroup.position.x-2, bulbGroup.position.y, bulbGroup.position.z);
  spotLight_sun4.position.set(bulbGroup.position.x+2, bulbGroup.position.y, bulbGroup.position.z);
  spotLight_sun5.position.set(bulbGroup.position.x, bulbGroup.position.y, bulbGroup.position.z-2);
  spotLight_sun6.position.set(bulbGroup.position.x, bulbGroup.position.y, bulbGroup.position.z+2);
  spot_rotation += 0.01;
  // -----------------------------------------------------

  const swingAmplitude = 0.5;
  const swingSpeed = 0.01; 
  rotationAngle = Math.sin(swingSpeed * Date.now()) * swingAmplitude;
  bulbGroup.rotation.y = rotationAngle;
    //premiers rebons
    dedos.forEach((dedo) => {
      let { positionY, positionX, velocityY, gravity, restitution, is_fallen } = dedo.userData;
      const randomMovement = (Math.random() - 0.5) * 1;
      // Si un dedo n'a pas encore terminé sa chute.
      if(dedo.userData.is_fallen == false){
        dedo.position.x = dedo.userData.positionX;
        positionY += velocityY;
        velocityY += gravity;
      
        if (positionY <= 0 && dedo.userData.is_fallen == false) {
          velocityY *= restitution;
          positionY = 0;
          velocityY *= 0.9;
          dedo.userData.rebound_counter++;
        }
        if(dedo.position.y < 0.1 && dedo.userData.rebound_counter > 20){
          positionY = 0;
          dedo.userData.is_fallen = true;
          console.log("is fallen = "+ dedo.userData.is_fallen);
        }
      } 
      //----------------------------------------------

      // Si un dedo a fini sa chute.
      if(dedo.userData.is_fallen == true){
        dedo.userData.rebound_counter = 0;
        //console.log("dedo tombé ");
        if(dedo.userData.positionY<5){
          if(positionX > -10){
            dedo.position.z +=0.5;
          }
          if(dedo.position.z>60){

            const flashLight = new THREE.PointLight(0xffffff, 100, 100); // Couleur blanche, intensité élevée
            flashLight.position.set(dedo.position.x, 5, 60); // Positionner la source de lumière au "dedo"
            scene.add(flashLight); // Ajouter la source de lumière à la scène

            // Définir une durée pendant laquelle l'éclair sera visible avant d'être retiré
            setTimeout(() => {
              scene.remove(flashLight); // Retirer la source de lumière après un certain délai (par exemple, 200 ms)
            }, 200);
            positionX = Math.random() * 20 - 10;
            positionY = Math.random() * 20 + 5;
            dedo.position.z = 0;
            dedo.userData.is_fallen = false;

            console.log(spotLight_sun.intensity);
          }
          //console.log(dedo.position.x);
        } else {
          dedo.userData.is_fallen = false;
        }
      }
      //---------------------------------

      


      dedo.position.x = positionX;
      dedo.position.y = positionY;
      dedo.userData.positionY = positionY;
      dedo.userData.positionX = positionX;
      dedo.userData.velocityY = velocityY;
    });
  



  const cameraSpeed = 0.0005;
  const cameraX = radius * Math.sin(angle);
  const cameraZ = radius * Math.cos(angle);


  //camera.position.x = cameraX;
  //camera.position.z = cameraZ;

  angle += cameraSpeed;


  camera.lookAt(0, 0, 0);
  renderer.render(scene, camera);
}

update();