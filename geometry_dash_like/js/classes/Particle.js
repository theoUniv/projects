export class Particle {
    constructor(x, y, radius, color, velocity) {
      this.x = x;
      this.y = y;
      this.radius = radius;
      this.color = color;
      this.velocity = velocity;
    }
  
    draw(context) {
      context.beginPath();
      context.arc(this.x, this.y, this.radius, 0, Math.PI * 2, false);
      context.fillStyle = this.color;
      context.fill();
      context.closePath();
    }
  
    update(context) {
      this.draw(context);
      this.x += this.velocity.x;
      this.y += this.velocity.y;
      // Ajoutez ici d'autres conditions de mise à jour/destruction des particules
    }
  }