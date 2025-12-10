package com.game;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.WindowEvent;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Random;

public class Game extends JFrame {
    private long gameTime = 0;
    private long timeLimit = 0;
    private int level;
    private int score = 0;
    private ArrayList<Chicken> chickens;
    private ArrayList<Rock> rocks;
    private ArrayList<Flower> flowers;
    private Fox fox;
    private JPanel panel;
    private JPanel world;
    private JLabel scoreLabel;
    private Sprite background;
    private long secondsPerFrame;

    Game(long fps, int chickens, long timeLimit) {
        super("FoxChicken");
        this.level = chickens;
        this.secondsPerFrame = 1000 / fps;
        this.background = new Sprite("background");
        this.panel = new JPanel();
        this.panel.setLayout(new BoxLayout(panel, BoxLayout.Y_AXIS));
        this.world = new JPanel();
        this.world.setLayout(null);
        this.world.setPreferredSize(new Dimension(640, 480));
        this.panel.add(world);
        this.scoreLabel = new JLabel(Integer.toString(this.score));
        panel.add(scoreLabel);
        add(panel);

        setSize(655, 540);
        setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        addKeyListener(new GameKeyListener());
        try {
            setIconImage(ImageIO.read(new File("images/icon.png")));
        } catch (IOException e) {
            System.err.println(e.getMessage());
        }

        resetGame(chickens, timeLimit);
    }

    public void run() {
        long start = System.currentTimeMillis();
        long end = 0;
        long elapsedTime = 0;
        while (true) {
            end = System.currentTimeMillis();
            elapsedTime += end - start;
            start = end;
            while (elapsedTime > secondsPerFrame) {
                elapsedTime -= secondsPerFrame;
                update();
                checkLose();
                checkWin();
            }
        }
    }

    private void update() {
        fox.update(secondsPerFrame);
        ArrayList<Chicken> toRemove = new ArrayList<Chicken>();
        for (Chicken chicken : chickens) {
            chicken.update(secondsPerFrame);
            if (fox.getSprite().getFrame().y == 1 && !chicken.isDead() &&
                    fox.isCollision(chicken)) {
                chicken.die();
            }

            if (chicken.isDead()) {
                world.remove(chicken.getSprite());
                toRemove.add(chicken);
            }
        }

        for (Chicken chicken : toRemove) {
            chickens.remove(chicken);

            ++score;
            scoreLabel.setText(Integer.toString(score));
        }
    }

    private void checkLose() {
        if (System.currentTimeMillis() - gameTime > this.timeLimit * 1000) {
            JOptionPane.showMessageDialog(null, "You lose! Your score: " + score);
            this.dispatchEvent(new WindowEvent(this, WindowEvent.WINDOW_CLOSING));
        }
    }

    private void checkWin() {
        if (chickens.isEmpty()) {
            timeLimit += 15;
            level += 10;
            JOptionPane.showMessageDialog(null,
                    "You won! Your time: " + ((System.currentTimeMillis() - gameTime) / 1000) + " seconds\n" +
                            "Next level: chickens " + level + " time: " + timeLimit);
            resetGame(level, timeLimit);
        }
    }

    private void resetGame(int chickens, long timeLimit) {
        if (fox != null) {
            this.world.remove(fox.getSprite());
        }
        if (rocks != null) {
            for (Rock rock : rocks) {
                this.world.remove(rock);
            }
        }
        if (flowers != null) {
            for (Flower flower : flowers) {
                this.world.remove(flower);
            }
        }
        
        // 1. Initialize lists
        this.timeLimit = timeLimit;
        this.chickens = new ArrayList<Chicken>();
        this.rocks = new ArrayList<Rock>();
        this.flowers = new ArrayList<Flower>();
        this.fox = new Fox();
        fox.setPosition(new Point(100, 100));
        
        // 2. Add background FIRST
        if (background.getParent() == null) {
            this.world.add(background);
        } else {
            // Ensure background is present to be managed by Z-order later
            this.world.remove(background); 
            this.world.add(background);
        }

        // 3. Add Rocks 
        for (int i = 0; i < 5; ++i) {
            addRockRandomPos(i % 2 == 0);
        }
        
        // 4. Add Flowers
        for (int i = 0; i < 20; ++i) {
            addFlowerRandomPos();
        }

        // 5. Add Chickens
        for (int i = 0; i < chickens; ++i) {
            addChickenRandomPos();
        }

        // 6. Add Fox (Topmost layer)
        this.world.add(fox.getSprite());
        this.world.setComponentZOrder(fox.getSprite(), 0); 
        
        // FINAL Z-ORDER FIX: Push the background to the bottom-most layer (highest index value).
        this.world.setComponentZOrder(background, this.world.getComponentCount() - 1); 

        this.gameTime = System.currentTimeMillis();
        setVisible(true);
    }

    private void addChickenRandomPos() {
        Random rand = new Random();
        int x = rand.nextInt(420) + 80;
        int y = rand.nextInt(270) + 60;
        Chicken chicken = new Chicken();
        chicken.setPosition(new Point(x, y));
        chickens.add(chicken);
        
        world.add(chicken.getSprite());
        
        // Z-Order 1: Below Fox (0) but above Decor (2). Since Chickens are added
        // after Flowers/Rocks in resetGame, we just ensure they get a higher priority.
        world.setComponentZOrder(chicken.getSprite(), 1); 
    }

    private void addRockRandomPos(boolean up) {
        Random rand = new Random();
        int x, y;

        if (up) { // Top edge (Y near 0)
            x = rand.nextInt(560) + 40;
            y = rand.nextInt(10) + 1;
        } else { // Bottom edge (Y near 480)
            x = rand.nextInt(560) + 40;
            y = rand.nextInt(10) + 470;
        }

        Rock rock = new Rock();
        rock.setPosition(x, y);
        rocks.add(rock);

        world.add(rock);
        // FIX: Use a reliable relative index to place the rock above the background 
        // without violating the count check. This is the lowest sprite layer (2).
        world.setComponentZOrder(rock, world.getComponentCount() - 2);
    }

    private void addFlowerRandomPos() {
        Random rand = new Random();
        int x = rand.nextInt(130) + 250;
        int y = rand.nextInt(280) + 100;

        Flower flower = new Flower();
        flower.setPosition(x, y);
        flowers.add(flower);

        world.add(flower);
        // FIX: Use a reliable relative index. This is the lowest sprite layer (2).
        world.setComponentZOrder(flower, world.getComponentCount() - 2);
    }

    class GameKeyListener extends KeyAdapter {
        public void keyPressed(KeyEvent e) {
            switch (e.getKeyCode()) {
                case KeyEvent.VK_UP:
                case KeyEvent.VK_W:
                    fox.run(Direction.UP);
                    break;
                case KeyEvent.VK_DOWN:
                case KeyEvent.VK_S:
                    fox.run(Direction.DOWN);
                    break;
                case KeyEvent.VK_LEFT:
                case KeyEvent.VK_A:
                    fox.run(Direction.LEFT);
                    break;
                case KeyEvent.VK_RIGHT:
                case KeyEvent.VK_D:
                    fox.run(Direction.RIGHT);
                    break;
            }
        }

        public void keyReleased(KeyEvent e) {
            switch (e.getKeyCode()) {
                case KeyEvent.VK_SPACE:
                    fox.bite();
                    break;
                case KeyEvent.VK_UP:
                case KeyEvent.VK_W:
                    fox.stop(Direction.UP);
                    break;
                case KeyEvent.VK_DOWN:
                case KeyEvent.VK_S:
                    fox.stop(Direction.DOWN);
                    break;
                case KeyEvent.VK_LEFT:
                case KeyEvent.VK_A:
                    fox.stop(Direction.LEFT);
                    break;
                case KeyEvent.VK_RIGHT:
                case KeyEvent.VK_D:
                    fox.stop(Direction.RIGHT);
                    break;
            }
        }
    }

    private class Rock extends Sprite {
        Rock() {
            super("rock");
        }
    }

    private class Flower extends Sprite {
        Flower() {
            super("flower");
        }
    }
}
