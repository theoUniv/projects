export class Obstacle {
    constructor(x, y, width, height, imageUrl) {
        this.x = x;
        this.y = y;
        this.width = width;
        this.height = height;
        this.collision = false;
        this.collisionBox = new SAT.Box(new SAT.Vector(x, y), width, height);
        this.image = new Image();
        this.image.src = imageUrl;
    }

    draw(ctx) {
        ctx.drawImage(this.image, this.x, this.y, this.width, this.height);
    }

    getCollisionBox() {
        return this.collisionBox;
    }
    _toPolygon() {
        return this.collisionBox.toPolygon();
    }

    move_to_left(x){
        this.x -= x;
        this.collisionBox.pos.x -= x;
        //console.log("COlission :", this.getCollisionBox());
    }
}
