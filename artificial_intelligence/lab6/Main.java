import java.awt.*;
import java.awt.event.*;
import java.util.ArrayList;
import java.util.List;
import java.util.Collections;
import javax.swing.*;

class Main {
    
    private static final int BOARD_SIZE = 4; 
    
    private static final int NUM_CARDS = BOARD_SIZE * BOARD_SIZE;
    private static final int NUM_PAIRS = NUM_CARDS / 2;
    
    private static final Dimension BUTTON_SIZE = new Dimension(80, 80); 
    
    private static final String[] UNIQUE_ANIMALS = {
        "Lion", "Tiger", "Bear", "Panda", "Koala", "Monkey", "Gorilla", "Fox", "Wolf", "Dog",
        "Horse", "Unicorn", "Deer", "Cow", "Pig", "Boar", "Sheep", "Goat", "Camel", "Giraffe",
        "Elephant", "Rhino", "Mouse", "Hamster", "Rabbit", "Squirrel", "Bat", "Owl", "Eagle", "Duck",
        "Swan", "Parrot", "Frog", "Croc", "Turtle", "Snake", "Whale", "Dolphin", "Fish", "Shark",
        "Octopus", "Crab", "Shrimp", "Snail", "Butterfly", "Worm", "Ant", "Bee", "Ladybug", "Spider" 
    };

    private static memoButton firstCard = null;
    private static memoButton secondCard = null;
    private static boolean isBusy = false; 
    private static boolean gameWon = false; 
    private static int score = 0;
    private static int matchesFound = 0;
    private static JLabel scoreLabel; 
    private static JPanel gamePanel; 
    private static ArrayList<memoButton> allButtons; 

    public static void main(String[] args) {
        JFrame frame = new JFrame("Memory Game (" + BOARD_SIZE + "x" + BOARD_SIZE + ")");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        
        int frameSize = (BOARD_SIZE * 90) + 150; 
        frame.setSize(frameSize, frameSize);

        frame.setLayout(new BorderLayout()); 

        gamePanel = new JPanel();
        gamePanel.setLayout(new GridLayout(BOARD_SIZE, BOARD_SIZE, 5, 5));
        frame.add(gamePanel, BorderLayout.CENTER); 

        initializeBoard(); 

        JPanel scorePanel = new JPanel();
        scorePanel.setLayout(new FlowLayout(FlowLayout.CENTER, 50, 10)); 

        scoreLabel = new JLabel("Score: 0"); 
        scoreLabel.setFont(new Font("Arial", Font.BOLD, 24));
        scorePanel.add(scoreLabel);

        JButton newGameButton = new JButton("New Game");
        newGameButton.setFont(new Font("Arial", Font.BOLD, 16));
        newGameButton.addActionListener(e -> resetGame()); 
        scorePanel.add(newGameButton);
        
        frame.add(scorePanel, BorderLayout.SOUTH); 
        frame.setVisible(true);
    }
    
    private static void initializeBoard() {
        if (gamePanel != null) {
            gamePanel.removeAll();
        }
        allButtons = new ArrayList<>();
        
        List<String> shuffledValues = generateShuffledPairs();
        
        for(int i = 0; i < NUM_CARDS; i++)
        {
            String animalValue = shuffledValues.get(i);
            memoButton button = new memoButton(animalValue, "?"); 
            button.addActionListener(new CardClickListener()); 
            allButtons.add(button); 
            gamePanel.add(button);
        }
        if (gamePanel != null) {
            gamePanel.revalidate();
            gamePanel.repaint();
        }
    }
    
    public static void resetGame() {
        score = 0;
        matchesFound = 0;
        firstCard = null;
        secondCard = null;
        isBusy = false;
        gameWon = false; 
        
        updateScoreDisplay(); 
        
        initializeBoard();
    }
    
    public static void cardClicked(memoButton clickedCard) {
        if (gameWon || isBusy || clickedCard.isMatched || clickedCard.isRevealed) {
            return; 
        }

        clickedCard.showText();

        if (firstCard == null) {
            firstCard = clickedCard;
        } else {
            secondCard = clickedCard;
            isBusy = true; 
            checkForMatch();
        }
    }
    
    private static void checkForMatch() {
        if (firstCard.value.equals(secondCard.value)) {
            matchesFound++;
            score += 10;
            
            firstCard.setMatched();
            secondCard.setMatched();

            resetTurn(false);
            
            if (matchesFound == NUM_PAIRS) {
                gameWon = true;
                updateScoreDisplay(); 
            }

        } else {
            score = Math.max(0, score - 1); 
            
            Timer timer = new Timer(1000, new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    firstCard.hideText();
                    secondCard.hideText();
                    resetTurn(true);
                    ((Timer)e.getSource()).stop();
                }
            });
            timer.setRepeats(false);
            timer.start();
        }
        updateScoreDisplay();
    }
    
    private static void resetTurn(boolean delayed) {
        firstCard = null;
        secondCard = null;
        isBusy = false; 
    }
    
    private static void updateScoreDisplay() {
        if (gameWon) {
            scoreLabel.setText("!!! YOU WON !!! (score: " + score + ")");
        } else {
            scoreLabel.setText("Score: " + score);
        }
    }

    public static List<String> generateShuffledPairs() {
        if (UNIQUE_ANIMALS.length < NUM_PAIRS) {
            throw new IllegalStateException("Board size (" + BOARD_SIZE + "x" + BOARD_SIZE + ") requires " + NUM_PAIRS + 
                                            " unique pairs, but only " + UNIQUE_ANIMALS.length + " are available!");
        }
        List<String> values = new ArrayList<>();
        for (int i = 0; i < NUM_PAIRS; i++) {
            values.add(UNIQUE_ANIMALS[i]);
            values.add(UNIQUE_ANIMALS[i]);
        }
        Collections.shuffle(values);
        return values;
    }

    public static Dimension getButtonSize() {
        return BUTTON_SIZE;
    }
}

class CardClickListener implements ActionListener {
    @Override
    public void actionPerformed(ActionEvent e) {
        Main.cardClicked((memoButton) e.getSource());
    }
}

class memoButton extends JButton
{
    public String value;
    public boolean isRevealed = false;
    public boolean isMatched = false; 

    public memoButton(String cardValue, String defaultText)
    {
        super(defaultText);
        this.value = cardValue;

        this.setPreferredSize(Main.getButtonSize());
        this.setHorizontalAlignment(SwingConstants.CENTER);
        this.setFont(new Font("Arial", Font.BOLD, 14));
        this.setBackground(Color.LIGHT_GRAY);

        this.hideText();
    }
    
    public void setMatched() {
        this.isMatched = true;
        this.setEnabled(false); 
        this.setBackground(Color.GREEN); 
    }

    public void hideText(){
        this.setText("?");
        this.isRevealed = false;
        this.setBackground(Color.LIGHT_GRAY);
    }

    public void showText(){
        this.setText(this.value); 
        this.isRevealed = true;
        this.setBackground(Color.WHITE);
    }
}