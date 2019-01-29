import apsync.*;
import processing.serial.*;

//ARDUINO TO PROCESSING SYNC
AP_Sync streamer;

public String Y;
public String G;
public String B;
public String P;
public String H;

// DATA
Table csv;
String[] columns = {"Y", "G", "B", "P", "H"};

// DRAWING
//float yellow = 500.0;
float prevYellowX = -1.0;
float prevYellowY = 501.0;

//float green = 500.0;
float prevGreenX = -1.0;
float prevGreenY = 501.0;

float x = 1.0;

PImage curve;
boolean expert;

void setup(){
  size(750,500);

  //println(Serial.list()[1]);
  String port = Serial.list()[1];
  streamer = new AP_Sync(this, port, 9600);
  
  csv = new Table();
  for(String column : columns){
    csv.addColumn(column);
  }
  
  
  strokeWeight(4);
  background(255);
  
  curve = loadImage("pullcurve_background.png");
  image(curve, 0, 0, 750, 500);
  
  expert = false;


  //noLoop();

}

void draw() {
  
    if(expert){
      float yellow = map(int(Y), 0.0, 1000.0, 500.0, 0.0);
      
      stroke(236, 299, 0);
      line(x, yellow, prevYellowX, prevYellowY);
      prevYellowX = x;
      prevYellowY = yellow;
      
      float green = map(int(G), 0.0, 1000.0, 500.0, 0.0);
      
      stroke(102, 204, 0);
      line(x, green, prevGreenX, prevGreenY);
      prevGreenX = x;
      prevGreenY = green;
      
    } else if(!expert){  
      
      float yellow = map(int(Y), 0.0, 1000.0, 500.0, 0.0);
      stroke(0, 0, 255);
      line(x, yellow, prevYellowX, prevYellowY);
      prevYellowX = x;
      prevYellowY = yellow;
      
    }
    
    //print(yellow); print(", "); println(green);

    x += 1.0;
    
    TableRow row = csv.addRow();
    row.setString("Y", Y);
    row.setString("G", G);
    row.setString("B", B);
    row.setString("P", P);
    row.setString("H", H);
    
    if(x > 750){
      
      background(255);
      image(curve, 0, 0, 750, 500);
      x = 0.0;    
      prevYellowX = 0.0;
      prevGreenX = 0.0;
      saveTable(csv, "pottery_sensors.csv");
      
    }

    print(Y); print(',');
    
    print(G); print(',');
    
    print(B); print(',');
    
    print(P); print(',');
    
    println(H);

}
