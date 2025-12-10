package com.game;

import java.awt.*;

public class Fox extends Animal {
    private long biteTime = 0;

    Fox() {
        super(new AnimatedSprite("fox", new Dimension(32, 32), 2, 5),
                new Rectangle(8, 12, 16, 12), 0.12f);
    }

    @Override
    public void update(long elapsedTime) {
        updateSprite(elapsedTime);
        if (sprite.getFrame().y == 0) {
            return;
        }

        biteTime += elapsedTime;
        if (biteTime > 400) {
            biteTime = 0;
            sprite.setFrame(0, 0);
        }
    }

    public void bite() {
        if (sprite.getFrame().y == 0) {
            sprite.setFrame(0, 1);
        }
    }
}
