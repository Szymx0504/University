package com.game;

import javax.swing.*;
import java.awt.*;
import java.awt.geom.AffineTransform;
import java.awt.image.AffineTransformOp;
import java.awt.image.BufferedImage;

public class AnimatedSprite extends Sprite {
    private Rectangle frame;
    private int frameMax;
    private long fps;
    private long time;
    private boolean flipped;

    AnimatedSprite(String name, Dimension frameSize, int frameMax, long fps) {
        super(name);
        this.frame = new Rectangle(0, 0, frameSize.width, frameSize.height);
        this.frameMax = frameMax;
        this.fps = fps;
        this.time = 0;
        this.flipped = false;
        setFrame(0, 0);
    }

    public void update(long elapsedTime) {
        time += elapsedTime;
        long timeUpdate = 1000 / fps;
        while (time > timeUpdate) {
            time -= timeUpdate;
            setFrame((frame.x + 1) % frameMax, frame.y);
        }
    }

    public void setFps(long fps) {
        this.fps = fps;
    }

    public void setFrame(int x, int y) {
        frame.x = x;
        frame.y = y;

        BufferedImage newImage = image.getSubimage(frame.x * frame.width,
                frame.y * frame.height, frame.width, frame.height);
        if (flipped) {
            AffineTransform at = AffineTransform.getScaleInstance(-1, 1);
            at.translate(-newImage.getWidth(), 0);
            AffineTransformOp op = new AffineTransformOp(at, AffineTransformOp.TYPE_BILINEAR);
            newImage = op.filter(newImage, null);
        }
        setIcon(new ImageIcon(newImage));
    }

    public void setFlipped(boolean flipped) {
        this.flipped = flipped;
        setFrame(getFrame().x, getFrame().y);
    }

    public Rectangle getFrame() {
        return this.frame;
    }
}
