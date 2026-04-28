#include <Wire.h>
#include <LIS3MDL.h>
#include <Adafruit_GPS.h>

#define CS_PIN 5

#define NORTH_PIN 9
#define WEST_PIN 10
#define SOUTH_PIN 17
#define EAST_PIN 18

// Connect GPS TX to Serial1 RX, GPS RX to Serial1 TX
#define GPSSerial Serial1
Adafruit_GPS GPS(&GPSSerial);




LIS3MDL mag;
float userPointingAngle;
//Nord = 0, West = 1, Sør = 2, Øst = 3
const char* dirNames[] = {"Nord", "West", "Sør", "Øst"};
const int motorPins[] = {NORTH_PIN, WEST_PIN, SOUTH_PIN, EAST_PIN};

float destinationLatitude = 0;
float destinationLongitude = 0;
float userLatitude = 0;
float userLongitude = 0;

void setup() {
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
  

  // GPS setup
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); // RMC and GGA sentences
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);    // 1Hz update rate
  GPS.sendCommand(PGCMD_ANTENNA);               // Request antenna status


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

  Serial.println("Enter latitude");
  while (Serial.available() == 0) {
    // Waits for input
  }
  destinationLatitude = Serial.parseFloat();

  Serial.println("Enter longitude");
  while (Serial.available() == 0) {
    // Waits for input
  }
  destinationLongitude = Serial.parseFloat();

 }

void loop() {

  

  // Read GPS data
  char c = GPS.read();
  
  // Parse complete NMEA sentence when available
  if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA())) return; // Failed to parse, wait for next
  }

  // Only navigate if we have a GPS fix
  if (!GPS.fix) {
    Serial.println("No GPS fix yet...");
    delay(1000);
    return;
  }

  float userLatitude  = GPS.latitudeDegrees;  // Automatically handles N/S
  float userLongitude = GPS.longitudeDegrees; // Automatically handles E/W

  Serial.print("Lat: "); Serial.println(userLatitude, 6);
  Serial.print("Lon: "); Serial.println(userLongitude, 6);
  Serial.print("Fix quality: "); Serial.println(GPS.fixquality);
  Serial.print("Satellites: "); Serial.println((int)GPS.satellites);


  //Read values from magnetometer
  mag.read();
  //Calculates value based on strength of x and y magnetic field
  userPointingAngle = atan2(mag.m.y, mag.m.x);
  //Radians to degrees
  userPointingAngle = userPointingAngle*180/PI;
  if (userPointingAngle < 0) userPointingAngle += 360;

  

  float latitudeVector  = destinationLatitude  - userLatitude;
  float longitudeVector = (destinationLongitude - userLongitude) * cos(userLatitude * PI / 180.0);

  float correctAngle = atan2(longitudeVector,latitudeVector) *180/PI;
  if (correctAngle < 0) correctAngle += 360;


  int angleDiff = (int)(fmod(userPointingAngle - correctAngle + 360.0, 360.0));

  if (angleDiff <= 22 || angleDiff >= 338) {
    digitalWrite(NORTH_PIN, HIGH);
    Serial.println("forward");
    } 
    else if (angleDiff >= 23 && angleDiff <= 112) {
    digitalWrite(WEST_PIN, HIGH);
    Serial.println("left");
    } 
    else if (angleDiff >= 113 && angleDiff <= 247) {
    digitalWrite(SOUTH_PIN, HIGH);
    Serial.println("backwards");
    }  
    else if (angleDiff >= 248 && angleDiff <= 337) {
    digitalWrite(EAST_PIN, HIGH);
    Serial.println("right");
    }

    delay(200);
    digitalWrite(NORTH_PIN, LOW);
    digitalWrite(WEST_PIN, LOW);
    digitalWrite(SOUTH_PIN, LOW);
    digitalWrite(EAST_PIN, LOW);
    // put your main code here, to run repeatedly:

  }
