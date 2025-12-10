package com.game;

import java.awt.*;
import java.util.Random;

public class Chicken extends Animal {
    private long time = 0;
    private long actionTime = 0;
    private boolean isDead = false;

    Chicken() {
        super(new AnimatedSprite("chicken", new Dimension(32, 32), 2, 5),
                new Rectangle(10, 8, 13, 14), 0.07f);
    }

    @Override
    public void update(long elapsedTime) {
        updateSprite(elapsedTime);
        time += elapsedTime;
        if (sprite.getFrame().y == 1) {
            if (time > actionTime) {
                sprite.setVisible(false);
                isDead = true;
            }
            return;
        }

        if (getPosition().x < 80) {
            run(Direction.RIGHT);
            Random rand = new Random();
            actionTime = rand.nextInt(6000) + 1000;
            return;
        }

        if (getPosition().x > 500) {
            run(Direction.LEFT);
            Random rand = new Random();
            actionTime = rand.nextInt(6000) + 1000;
            return;
        }

        if (getPosition().y < 60) {
            run(Direction.DOWN);
            Random rand = new Random();
            actionTime = rand.nextInt(6000) + 1000;
            return;
        }

        if (getPosition().y > 330) {
            run(Direction.UP);
            Random rand = new Random();
            actionTime = rand.nextInt(6000) + 1000;
            return;
        }

        if (time > actionTime) {
            time = 0;
            Random rand = new Random();
            actionTime = rand.nextInt(8000) + 1000;
            int action = rand.nextInt(9);
            if (action == 0) {
                run(Direction.UP);
            } else if (action == 1) {
                run(Direction.DOWN);
            } else if (action == 2) {
                run(Direction.LEFT);
            } else if (action == 3) {
                run(Direction.RIGHT);
            } else if (action == 4) {
                run(Direction.DOWN);
                stop(Direction.LEFT);
                stop(Direction.RIGHT);
            } else if (action == 5) {
                run(Direction.LEFT);
                stop(Direction.UP);
                stop(Direction.DOWN);
            } else if (action == 6) {
                run(Direction.RIGHT);
                stop(Direction.UP);
                stop(Direction.DOWN);
            } else if (action == 7){
                run(Direction.UP);
                stop(Direction.LEFT);
                stop(Direction.RIGHT);
            } else {
                stop(Direction.UP);
                stop(Direction.DOWN);
                stop(Direction.LEFT);
                stop(Direction.RIGHT);
            }
        }
    }

    public void die() {
        sprite.setFrame(0, 1);
        stop(Direction.UP);
        stop(Direction.DOWN);
        stop(Direction.LEFT);
        stop(Direction.RIGHT);
        time = 0;
        actionTime = 500;
    }

    public boolean isDead() {
        return isDead;
    }
}
