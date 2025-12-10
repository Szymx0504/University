package com.game;

import java.awt.*;

public abstract class Animal {
    protected AnimatedSprite sprite;
    protected Rectangle rect;
    protected float speed;
    protected Point direction;

    Animal(AnimatedSprite sprite, Rectangle rect, float speed) {
        this.sprite = sprite;
        this.rect = rect;
        this.speed = speed;
        this.direction = new Point(0, 0);
    }

    public void update(long elapsedTime) {
        updateSprite(elapsedTime);
    }

    public void updateSprite(long elapsedTime) {
        sprite.update(elapsedTime);
        sprite.setPosition(sprite.getPosition().x + direction.x * (int)(speed * (float)elapsedTime),
                sprite.getPosition().y + direction.y * (int)(speed * (float)elapsedTime));
    }

    public void run(Direction direction) {
        switch(direction) {
            case UP:
                this.direction.y = -1;
                break;
            case DOWN:
                this.direction.y = 1;
                break;
            case LEFT:
                this.direction.x = -1;
                sprite.setFlipped(true);
                break;
            case RIGHT:
                this.direction.x = 1;
                sprite.setFlipped(false);
                break;
        }
    }

    public void stop(Direction direction) {
        switch(direction) {
            case UP:
                if (this.direction.y == -1) {
                    this.direction.y = 0;
                }
                break;
            case DOWN:
                if (this.direction.y == 1) {
                    this.direction.y = 0;
                }
                break;
            case LEFT:
                if (this.direction.x == -1) {
                    this.direction.x = 0;
                }
                break;
            case RIGHT:
                if (this.direction.x == 1) {
                    this.direction.x = 0;
                }
                break;
        }
    }

    public void setPosition(Point position) {
        sprite.setPosition(position.x, position.y);
    }

    public AnimatedSprite getSprite() {
        return sprite;
    }

    public Rectangle getCollisionRect() {
        return new Rectangle(rect.x + sprite.getPosition().x,
                rect.y + sprite.getPosition().y, rect.width, rect.height);
    }

    public Point getPosition() {
        return sprite.getPosition();
    }

    public boolean isCollision(Animal other) {
        return getCollisionRect().intersects(other.getCollisionRect());
    }
}
