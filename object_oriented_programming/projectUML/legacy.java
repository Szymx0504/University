import javafx.application.Application;
import javafx.collections.ObservableList;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.ListCell;
import javafx.scene.control.TextField;
import javafx.scene.control.Slider;
import javafx.scene.control.CheckBox;
import javafx.scene.control.MultipleSelectionModel;
import javafx.scene.control.SelectionMode;
import javafx.scene.layout.VBox;
import javafx.scene.layout.HBox;
import javafx.scene.layout.GridPane;
import javafx.stage.Stage;

import java.util.ArrayList;
import java.util.Map;
import java.util.List;

public class legacy extends Application {
    private static final List<Map<String, Object>> HOMES = new ArrayList<>();
    static {
        HOMES.add(Map.of("name", "Modern Apartment", "price", 200000, "city", "New York", "bedrooms", 2));
        HOMES.add(Map.of("name", "Family House", "price", 350000, "city", "Los Angeles", "bedrooms", 4));
        HOMES.add(Map.of("name", "Cozy Studio", "price", 180000, "city", "Chicago", "bedrooms", 1));
        HOMES.add(Map.of("name", "Beach Villa", "price", 275000, "city", "Miami", "bedrooms", 3));
    }

    private Object[] createSliderGroup(int min, int max, int initial, String unit, int tickUnit, int increment){
        Slider slider = new Slider(min, max, initial);
        Label label = new Label(initial + unit);

        slider.setShowTickLabels(true);
        slider.setShowTickMarks(true);
        slider.setMajorTickUnit(tickUnit);
        slider.setBlockIncrement(increment);

        slider.valueProperty().addListener((obs, oldVal, newVal) -> {
            label.setText(newVal.intValue() + unit);
        });

        return new Object[]{slider, label};
    }

    private void connectSliders(Slider minSlider, Slider maxSlider){
        minSlider.valueProperty().addListener((obs, oldVal, newVal) -> {
            if(newVal.intValue() > maxSlider.getValue()){
                maxSlider.setValue(newVal.intValue());
            }
        });
        
        maxSlider.valueProperty().addListener((obs, oldVal, newVal) -> {
            if(newVal.intValue() < minSlider.getValue()){
                minSlider.setValue(newVal.intValue());
            }
        });
    }

    private CheckBox createCheckBox(String text){
        CheckBox checkBox = new CheckBox(text);

        return checkBox;
    }

    public void start(Stage stage){
        GridPane criteriasGrid = new GridPane();

        Object[] minPriceGroup = createSliderGroup(0, 10000000, 100000, "€", 200000, 50000);
        Slider minPriceSlider = (Slider) minPriceGroup[0];
        Label minPriceLabel = (Label) minPriceGroup[1];
        
        Object[] maxPriceGroup = createSliderGroup(0, 10000000, 500000, "€", 200000, 50000);
        Slider maxPriceSlider = (Slider) maxPriceGroup[0];
        Label maxPriceLabel = (Label) maxPriceGroup[1];

        Object[] minAreaGroup = createSliderGroup(0, 500, 50, "sqm", 100, 1);
        Slider minAreaSlider = (Slider) minAreaGroup[0];
        Label minAreaLabel = (Label) minAreaGroup[1];

        Object[] maxAreaGroup = createSliderGroup(0, 500, 50, "sqm", 100, 1);
        Slider maxAreaSlider = (Slider) maxAreaGroup[0];
        Label maxAreaLabel = (Label) maxAreaGroup[1];

        Object[] minNofRoomsGroup = createSliderGroup(0, 10, 2, " rooms", 5, 1);
        Slider minNofRoomsSlider = (Slider) minNofRoomsGroup[0];
        Label minNofRoomsLabel = (Label) minNofRoomsGroup[1];

        Object[] maxRoomsGroup = createSliderGroup(0, 10, 4, " rooms", 5, 1);
        Slider maxRoomsSlider = (Slider) maxRoomsGroup[0];
        Label maxRoomsLabel = (Label) maxRoomsGroup[1];

        CheckBox kitchenAnnex = createCheckBox("Kitchen annex");

        Object[] minConstructionYearGroup = createSliderGroup(1950, 2026, 2000, "", 5, 1);
        Slider minConstructionYearSlider= (Slider) minConstructionYearGroup[0];
        Label minConstructionYearLabel = (Label) minConstructionYearGroup[1];

        Object[] maxConstructionYearGroup = createSliderGroup(1950, 2026, 2026, "", 5, 1);
        Slider maxConstructionYearSlider = (Slider) maxConstructionYearGroup[0];
        Label maxConstructionYearLabel = (Label) maxConstructionYearGroup[1];

        Object[] minFloorGroup = createSliderGroup(0, 25, 0, " floor", 5, 1);
        Slider minFloorSlider = (Slider) minFloorGroup[0];
        Label minFloorLabel = (Label) minFloorGroup[1];

        Object[] maxFloorGroup = createSliderGroup(0, 25, 10, " floor", 5, 1);
        Slider maxFloorSlider = (Slider) maxFloorGroup[0];
        Label maxFloorLabel = (Label) maxFloorGroup[1];

        Object[] maxTransportDistGroup = createSliderGroup(0, 10, 1, " floor", 5, 1); // kilometers (?)
        Slider maxTransportDistSlider = (Slider) maxTransportDistGroup[0];
        Label maxTransportDistLabel = (Label) maxTransportDistGroup[1];

        Object[] minParkingSpaceGroup = createSliderGroup(0, 5, 1, " slots", 1, 1);
        Slider minParkingSpaceSlider = (Slider) minParkingSpaceGroup[0];
        Label minParkingSpaceLabel = (Label) minParkingSpaceGroup[1];

        Object[] maxParkingSpaceGroup = createSliderGroup(0, 5, 1, " slots", 1, 1);
        Slider maxParkingSpaceSlider = (Slider) maxParkingSpaceGroup[0];
        Label maxParkingSpaceLabel = (Label) maxParkingSpaceGroup[1];

        CheckBox forSale = createCheckBox("Prefere for sale");

        CheckBox primaryMarket = createCheckBox("Prefer primary market");

        VBox neighbourhoodBox = new VBox(5);


        //////////////

        Button searchButton = new Button("Search");
        HBox optionsLayout = new HBox(10);

        /////////////

        VBox resultsLayout = new VBox(10);

        /////////////

        VBox mainLayout = new VBox(10); // 10px spacing

        /////////// COMPONENTS

        criteriasGrid.setHgap(10);
        criteriasGrid.setVgap(10);

        criteriasGrid.add(new Label("Price range:"), 0, 0); // col, row
        criteriasGrid.add(minPriceSlider, 1, 0);
        criteriasGrid.add(minPriceLabel, 2, 0);
        criteriasGrid.add(new Label("to"), 0, 1);
        criteriasGrid.add(maxPriceSlider, 1, 1);
        criteriasGrid.add(maxPriceLabel, 2, 1);
        connectSliders(minPriceSlider, maxPriceSlider);

        criteriasGrid.add(new Label("Area range:"), 0, 2);
        criteriasGrid.add(minAreaSlider, 1, 2);
        criteriasGrid.add(minAreaLabel, 2, 2);
        criteriasGrid.add(new Label("to"), 0, 3);
        criteriasGrid.add(maxAreaSlider, 1, 3);
        criteriasGrid.add(maxAreaLabel, 2, 3);
        connectSliders(minAreaSlider, maxAreaSlider);

        criteriasGrid.add(new Label("Rooms range:"), 0, 4);
        criteriasGrid.add(minNofRoomsSlider, 1, 4);
        criteriasGrid.add(minNofRoomsLabel, 2, 4);
        criteriasGrid.add(new Label("to:"), 0, 5);
        criteriasGrid.add(maxRoomsSlider, 1, 5);
        criteriasGrid.add(maxRoomsLabel, 2, 5);
        connectSliders(minNofRoomsSlider, maxRoomsSlider);

        // kitchenAnnex.setSelected(false); // default is false already
        criteriasGrid.add(kitchenAnnex, 0, 6); // .isSelected() for future use-cases

        criteriasGrid.add(new Label("Construction year:"), 0, 7);
        criteriasGrid.add(minConstructionYearSlider, 1, 7);
        criteriasGrid.add(minConstructionYearLabel, 2, 7);
        criteriasGrid.add(new Label("to:"), 0, 8);
        criteriasGrid.add(maxConstructionYearSlider, 1, 8);
        criteriasGrid.add(maxConstructionYearLabel, 2, 8);
        connectSliders(minConstructionYearSlider, maxConstructionYearSlider);

        criteriasGrid.add(new Label("Floor:"), 0, 9);
        criteriasGrid.add(minFloorSlider, 1, 9);
        criteriasGrid.add(minFloorLabel, 2, 9);
        criteriasGrid.add(new Label("to:"), 0, 10);
        criteriasGrid.add(maxFloorSlider, 1, 10);
        criteriasGrid.add(maxFloorLabel, 2, 10);
        connectSliders(minFloorSlider, maxFloorSlider);

        criteriasGrid.add(new Label("Max transport distance:"), 0, 11);
        criteriasGrid.add(maxTransportDistSlider, 1, 11);
        criteriasGrid.add(maxTransportDistLabel, 2, 11);

        criteriasGrid.add(new Label("Parking space:"), 0, 12);
        criteriasGrid.add(minParkingSpaceSlider, 1, 12);
        criteriasGrid.add(minParkingSpaceLabel, 2, 12);
        criteriasGrid.add(new Label("to:"), 0, 13);
        criteriasGrid.add(maxParkingSpaceSlider, 1, 13);
        criteriasGrid.add(maxParkingSpaceLabel, 2, 13);
        connectSliders(minParkingSpaceSlider, maxParkingSpaceSlider);

        criteriasGrid.add(forSale, 0, 14);

        criteriasGrid.add(primaryMarket, 0, 15);

        criteriasGrid.add(new Label("Neighbourhoods:"), 0, 16);
        // these will be in array (also add scrolling)
        CheckBox cb1 = new CheckBox("Wielkopolska");
        CheckBox cb2 = new CheckBox("Zachodniopomorskie");
        CheckBox cb3 = new CheckBox("Lubuskie");
        neighbourhoodBox.getChildren().addAll(cb1, cb2, cb3);
        criteriasGrid.add(neighbourhoodBox, 1, 16);


        ///////////

        
        searchButton.setOnAction(e -> {
            resultsLayout.getChildren().clear();
            // System.out.println("search...for price < " + priceField.getText());
            for(Map<String, Object> home : HOMES){
                if((Integer) home.get("price") <= (int) minPriceSlider.getValue()){
                    resultsLayout.getChildren().add(new Label((String) home.get("name") + " " + home.get("price") + " " + home.get("city")));
                }
            }


            // will be used to search by neighbourhood
            // List<String> selected = new ArrayList<>();
            // if (cb1.isSelected()) selected.add("Wielkopolska");
            // if (cb2.isSelected()) selected.add("Zachodniopomorskie");
            // if (cb3.isSelected()) selected.add("Lubuskie");
        });

        
        optionsLayout.getChildren().addAll(
            new Label("Max nof offers: inf"),
            searchButton
        );

        ////////////

        
        resultsLayout.getChildren().addAll(
            new Label("Home 1"),
            new Label("Home 2")
        );

        
        mainLayout.getChildren().addAll(criteriasGrid, optionsLayout, resultsLayout);

        // Scene scene = new Scene(mainLayout, 400, 300);
        Scene scene = new Scene(mainLayout, 1000, 1000);
        stage.setScene(scene);
        stage.setTitle("Home seeker");
        stage.show();
    }

    public static void main(String[] args){
        launch(args);
    }
}
