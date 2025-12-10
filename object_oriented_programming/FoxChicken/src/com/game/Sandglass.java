package com.game;
import java.awt.Dimension;

public class Sandglass extends AnimatedSprite {
    private static final int MAX_FRAME_INDEX = 1; 

    Sandglass() {
        super("sandglass", new Dimension(32, 32), 1, 1); 
        setFrame(0, 0);
    }

    public void updateVisual(float percentRemaining) {
        int step = (int) Math.floor((1.0f - percentRemaining) * MAX_FRAME_INDEX);
        int frameIndex = Math.min(Math.max(0, step), MAX_FRAME_INDEX);
        setFrame(0, frameIndex);
    }
}