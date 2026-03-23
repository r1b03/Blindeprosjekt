#include <Wire.h>
#include <LIS3MDL.h>

#define CS_PIN 5

#define NORTH_PIN 9
#define WEST_PIN 10
#define SOUTH_PIN 17
#define EAST_PIN 18


LIS3MDL mag;
float angle;



void setup()
{
  pinMode(CS_PIN, OUTPUT);
  pinMode(NORTH_PIN, OUTPUT);
  pinMode(WEST_PIN, OUTPUT);
  pinMode(SOUTH_PIN, OUTPUT);
  pinMode(EAST_PIN,OUTPUT);

  digitalWrite(CS_PIN, HIGH);


  digitalWrite(NORTH_PIN, LOW);
  digitalWrite(WEST_PIN, LOW);
  digitalWrite(SOUTH_PIN, LOW);
  digitalWrite(EAST_PIN, LOW);
  
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Starting up");
  
  
  Wire.begin();

  if (!mag.init())
  {
    Serial.println("Failed to detect LIS3MDL!");
    while (1);
  }

  mag.enableDefault();
}

void loop()
{
  mag.read();

  angle = atan2(mag.m.y,mag.m.x);
  angle =  angle* 180/PI;

  if (angle < 0){
    angle += 360;
  }
  
  if (angle <= 314 && angle >= 225){ //Vestover
        Serial.print(angle);
        Serial.println("Pointing westwards, vibrating RIGHT");
        while(angle <= 314 && angle >= 225){
           digitalWrite(WEST_PIN, HIGH);
           delay(10);
           
          //regne ny vink el
          mag.read();
          angle = atan2(mag.m.y,mag.m.x);
          angle =  angle* 180/PI;
          if (angle < 0){
            angle += 360;}
        }
        //Slutt å vibrere
          digitalWrite(WEST_PIN, LOW);
        if ((angle >= 314 && angle <= 360) || (angle <= 45 && angle >= 0)){ // Nordover
          Serial.println("Pointing north, vibrating FRONT");
          digitalWrite(NORTH_PIN, HIGH);
          delay(500);
        }
        digitalWrite(NORTH_PIN, LOW);
  }

  if (angle <= 224 && angle >= 135){ //Sør
        Serial.print(angle);
        Serial.println("Pointing south, vibrating BEHIND");
        while(angle <= 224 && angle >= 135){
           digitalWrite(SOUTH_PIN, HIGH);
           delay(10);
           
          //regne ny vinkel
          mag.read();
          angle = atan2(mag.m.y,mag.m.x);
          angle =  angle* 180/PI;
          if (angle < 0){
            angle += 360;}
        }
        //Slutt å vibrere
         digitalWrite(SOUTH_PIN, LOW);
        if ((angle >= 314 && angle <= 360) || (angle <= 45 && angle >= 0)){ // Nord
          Serial.println("Pointing north, vibrating FRONT");
          digitalWrite(NORTH_PIN, HIGH);
          delay(500);
        }
        digitalWrite(NORTH_PIN, LOW);
  }

   if (angle <= 134 && angle >= 46){ //Øst
        Serial.print(angle);
        Serial.println("Pointing eastwards, vibrating LEFT");
        while(angle <= 134 && angle >= 46){
           digitalWrite(EAST_PIN, HIGH);
           delay(10);
           
          //regne ny vinkel
          mag.read();
          angle = atan2(mag.m.y,mag.m.x);
          angle =  angle* 180/PI;
          if (angle < 0){
            angle += 360;}
        }
        digitalWrite(EAST_PIN, LOW);
        //Slutt å vibrere
        if ((angle >= 314 && angle <= 360) || (angle <= 45 && angle >= 0)){ // Nordover
          Serial.println("Pointing north, vibrating FRONT");
          // Vibrer rett frem
          digitalWrite(NORTH_PIN, HIGH);
          delay(500);
        }
        digitalWrite(NORTH_PIN, HIGH);
  }





  Serial.println(angle);

  delay(1000);
}


