export class Pic {
    constructor(x, y, scale, width, pic_pos_x) {
        this.x = x;
        this.y = y;
        this.pic_pos_x = pic_pos_x;
        this.scale = scale;
        this.width = width;
        this.collision = false;
        this.collisionPolygon = new SAT.Polygon(new SAT.Vector(this.x, this.y), 
            [
              new SAT.Vector(0, 0),
              new SAT.Vector(this.width, 0),
              new SAT.Vector(this.pic_pos_x,-this.scale)
            ]);
        console.log("Instance de pic créer en x:"+this.x+", y:"+this.y);
    }

    draw_SAT_pic(ctx) {
        ctx.save();
        ctx.beginPath();
        ctx.moveTo(this.x, this.y); // On déplace le curseur.
        ctx.lineTo(this.x + this.pic_pos_x, this.y - this.scale); // Ligne bas gauche vers haut.
        ctx.lineTo(this.x + this.width, this.y); // Ligne haut vers bas droite.
        ctx.closePath();
        ctx.fillStyle = "black";
        ctx.fill();
        ctx.strokeStyle = "white ";
        ctx.stroke();
    }

    getcollisionPolygon() {
        return this.collisionPolygon;
    }

    move_to_left(x, ctx){
        this.x -= x;
        this.collisionPolygon.pos.x -= x;
        this.draw_SAT_pic(ctx)
        //console.log("COlission :", this.getCollisionBox());
    }
}