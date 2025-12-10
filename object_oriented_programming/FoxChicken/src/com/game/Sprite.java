package com.game;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

public class Sprite extends JLabel {
    protected BufferedImage image;

    Sprite(String name) {
        try {
            image = ImageIO.read(new File("images/" + name + ".png"));
            setIcon(new ImageIcon(image));
            setPosition(0, 0);
        } catch (IOException e) {
            System.err.println(e.getMessage());
        }
    }

    public void setPosition(int x, int y) {
        setBounds(x, y, getPreferredSize().width, getPreferredSize().height);
    }

    public Point getPosition() {
        return getBounds().getLocation();
    }
}
