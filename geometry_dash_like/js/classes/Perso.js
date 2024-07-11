export class Perso {
    constructor(x, y, imageUrl, speedx, speedy) {
        this.spacePressed = false;
        document.addEventListener("keydown", this.handleKeyPress.bind(this));
        this.x = x;
        this.y = y;
        this.width = 80;
        this.height = 80;
        this.speedx = speedx;
        this.speedy = speedy;
        this.collision = false;
        this.is_alive = true;
        this.collisionPolygon = new SAT.Polygon(new SAT.Vector(x + 40, y + 40), [
            new SAT.Vector(-40, -40),
            new SAT.Vector(-40, +40),
            new SAT.Vector(+40, +40),
            new SAT.Vector(+40, -40),
        ]);
        // this.collisionPolygon = this.collisionPolygon.toPolygon();
        this.image = new Image();
        this.image.src = imageUrl;
        this.rotation = 0;

        console.log(this.collisionPolygon);
    }

    draw_SAT_polygon(ctx) {
        ctx.save();
        const polygon = this.collisionPolygon;

        ctx.translate(polygon.pos.x, polygon.pos.y);
        ctx.rotate(polygon.angle);
        ctx.beginPath();
        // ctx.rect(box.pos.x, box.pos.y, box.w, box.h);
        ctx.moveTo(polygon.points[0].x, polygon.points[0].y);
        for (let i = 1; i < polygon.points.length; i++) {
            const point = polygon.points[i];
            const x = point.x;
            const y = point.y;
            ctx.lineTo(x, y);
        }
        ctx.closePath();
        ctx.strokeStyle = "red";
        ctx.stroke();
        ctx.restore();
    }

    _movex(cnv) {
        this.x += this.speedx;
        if (this.x > cnv.width) {
            this.x = 0;
        }
        this.collisionPolygon.pos.x = this.x + 40;
    }

    place(cnv, x, y) {
        this.y = y;
        this.x = x;
        this.collisionPolygon.pos.x = this.x + this.width / 2; // Mettre à jour la position X du collisionPolygon
        this.collisionPolygon.pos.y = this.y + this.height / 2; // Mettre à jour la position Y du collisionPolygon
    }

    draw(ctx) {
        ctx.save(); 
        ctx.translate(this.x + this.width / 2, this.y + this.height / 2); //pivot de rotation
        ctx.rotate(this.rotation);
        // Dessine le perso
        ctx.drawImage(
            this.image,
            -this.width / 2,
            -this.height / 2,
            this.width,
            this.height
        );

        ctx.restore(); // Restaure le contexte précédent
    }

    getcollisionPolygon() {
        return this.collisionPolygon;
    }

    // _toPolygon() {
    //     return this.collisionPolygon.toPolygon();
    // }

    jump(cnv) {
        // Ajoute la gravité
        this.y += this.speedy;
        this.collisionPolygon.pos.y = this.y + 40;
        this.speedy += 0.3;

        if (this.speedy < 0.3) {
            this.rotation += Math.PI * 0.059;
            this.collisionPolygon.setAngle(this.rotation);
        }
        // Si le perso est au sol
        if (this.y > cnv.height - 130) {
            this.y = cnv.height - 130;
            this.collisionPolygon.pos.y = this.y + 40;
            this.speedy = 0; 
            this.rotation = 0;
            this.collisionPolygon.setAngle(this.rotation);
        }
    }

    GameOver(cnv, ctx) {
        this.is_alive = false;
        console.log("HEHO");
        if (this.collision) {
            this.image.src = "./assets/dead_hero.png";
            ctx.fillStyle = "Black";
            ctx.font = "bold 48px sans-serif";
            ctx.fillText("Game Over", cnv.width / 2 - 100, cnv.height / 2 - 20);
            this.speedx = 0;

            // return;
        } else {
            this.image.src = "./assets/cute_hero.png";
        }
    }

    handleKeyPress(event) {
        if (event.code === "Space" && !this.spacePressed) {
            this.spacePressed = true;
        } else {
            this.spacePressed = false;
        }
    }

    platform(cnv, obstacle) {
            if (this.y-this.height < obstacle.y) { // En dessous de la plateforme
                console.log("test");
                this.y += 50;
                this.collisionPolygon.pos.y = this.y + 40;
                this.speedy = 0;
                this.rotation = 0;
                this.collisionPolygon.setAngle(this.rotation);
            }

            if (this.y < obstacle.y + obstacle.height) { // Au dessus de la plateforme
                this.y = obstacle.y - this.height;
                this.collisionPolygon.pos.y = this.y + 40;
                this.speedy = 0; 
                this.rotation = 0;
                this.collisionPolygon.setAngle(this.rotation);
            }
            if(this.collision && this.spacePressed) { // Vérifier collision et si espace est pressée
                if (this.y - this.height < obstacle.y || this.y < obstacle.y + obstacle.height) {
                    this.y -= 10;
                    this.speedy -= 10;
                }
            }
            this.spacePressed = false;
        }

    }
