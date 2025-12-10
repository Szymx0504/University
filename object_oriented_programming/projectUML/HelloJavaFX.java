import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;

public class HelloJavaFX extends Application {
    public void start(Stage stage) {
        Label label = new Label("Hello JavaFX!");
        Button button = new Button("Click me!");
        
        button.setOnAction(e -> {
            label.setText("Button was clicked!");
        });

        
        
        VBox layout = new VBox(10); // Vertical box, 10px spacing
        layout.getChildren().addAll(label, button);
        
        Scene scene = new Scene(layout, 400, 300);
        stage.setScene(scene);
        stage.setTitle("JavaFX Basics");
        stage.show();
    }
    
    public static void main(String[] args) { 
        launch(args); 
    }
}