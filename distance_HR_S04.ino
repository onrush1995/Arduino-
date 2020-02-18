/*

Distance measurement with ultrasound sensor HC-SR04.
At first, upload this file into Arduino board.
Then you can check the connection pins from this code... and connect your sensor.
(If you connect the sensor first without uploading, you don't know the roles of the pins.
It can accidentally course a short circuit and break your sensor.)

 17.1.2019 -Jaakko Kaski-
 
 */

// Connection pins for HC-SR04
 const int GNDPin = 11; // Ground pin to the sensor
 const int echoPin = 10; // Echo pin shows the time from sending to get the reflection back.
 const int trigPin = 9; // Trigger Pin (1 pulse) wakes the ultrasound sending
 const int VccPin = 8; // 5V operating voltage to the sensor

// Variables
float maximumRange = 150.0; // Maximum distance in cm.
float minimumRange = 5.0; // Minimum distance in cm.
unsigned long duration = 0; // Time of voice to go there and back (in microseconds).
float distance = 0.0; // Distance (cm) to the target
unsigned long aika = 0; // Time stamp (ms) in long type (int gives maximum of only 32 sec)

//***** height et al.
float height1 = 0.0; //this measurement in centimeters
float height2 = 0.0; //previous measurement in centimeters
float heightDifference1 = 0.0; //delta for this measurement in centimeters
float heightDifference2 = 0.0; //delta for previous measurement in centimeters
bool ascending = false; //ascending = true / descending = false
bool directionChanged = false; //reached top or bottom
unsigned long pullUps = 0; //how many pull-ups
float lowPoint = 0.0; //Lowest point for this pull-up in centimeters
float highPoint = 0.0; //Highest point for this pull-up in centimeters
float bodyMass = 0.0; //Your body weight in kilograms
float g = 9.81; //kg*m/s*s
float energyConsumed = 0.0; //Energy consumed in this round of pull-ups in Joules
float totalEnergy = 0.0; //Total energy consumed in Joules

// Serial connection...
void setup() {
 Serial.begin (9600); // In Serial Monitor you can see the standard options for this
 
// Connection pins... setup for the sensor
 pinMode(GNDPin, OUTPUT); 
 pinMode(echoPin, INPUT);
 pinMode(trigPin, OUTPUT);
 pinMode(VccPin, OUTPUT); 

// Output pins to correct voltage levels. Note: 0V source is the same as GND
    digitalWrite(VccPin, HIGH);
    delayMicroseconds(2); 
    digitalWrite(GNDPin, LOW); 
    delayMicroseconds(2); 

    while(Serial.available() != 0)
     {
        // Wait for the serial connection before continuing...
     } 

}

void loop() {
     
// The pulse into trig pin wakes the ultrasound transmitter:
 digitalWrite(trigPin, LOW); 
 delayMicroseconds(2); 
 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10); 
 digitalWrite(trigPin, LOW);

 duration = pulseIn(echoPin, HIGH); // Echo pin is "HIGH" as long time as the sound is flying...

 // time stamp "aika" = "time" in finnish :)
 aika = millis();


 //The speed of voice at room temperature is 343m/s. This leads to eqn.
 distance =duration * 0.016854; //in cm

 //***** calculate diffence in height of this measurement and previous measurement
 height1 = distance;
 heightDifference1 = height1 - height2;

//***** if this measurement is higher than previous measurement, we are ascending, 
//***** else if this measurement is lower than previous measurement, we are descending
 if (heightDifference1 > 0) {
  ascending = true;
 } else {
  ascending = false;
 }

//***** if height difference is positive for this and previous measurement, the direction has not changed (going up)
//***** same if height differences are both negative, the direction has not changed (going down)
//***** However, if last time we were going up and this time we are going down, the direction has changed
//***** same if last time we were going down and this time we're going up, the direction has changed
 if (heightDifference1 > 0 && heightDifference2 > 0 || heightDifference1 < 0 && heightDifference2 < 0) {
  directionChanged = false;
  //***** if going up, the highest point needs to be updated
  if (ascending) {
    highPoint = distance;
  } else { //***** else we are going down, so update lowest point
    lowPoint = distance;
  }
 } else {
  directionChanged = true;
  //***** if last time we were going up and direction changed, so we reached the highest point
  //***** and we can calculate how much energy was consumed
  energyConsumed = bodyMass * g * (highPoint/100 - lowPoint/100); //E = mgh, h in meters (highPoint and lowPoint are in centimeters)
  totalEnergy = totalEnergy + energyConsumed;
 }

//***** if direction changed and we are going down, that means that we have done one pull-up, so add 1
if (directionChanged && ascending == false) {
  pullUps = pullUps + 1;
}

 if (distance >= maximumRange || distance <= minimumRange){
 // Test for the quality of the distance
 Serial.print(aika);
 Serial.print(" ");
 Serial.println(-5);
 }
 else {

 // print out "good results":
 Serial.print(aika);          // time(ms)
 Serial.print(" ");           // space for division
 Serial.print(distance);      // Distance (cm)
 Serial.println(" ");  
 
 //***** set some variables for next measurement
height2 = height1;
heightDifference2 = heightDifference1;
        
 }
 
 //Delay... NOTE THAT IF WE CALCULATE THE SPEED FROM THE DATA, IT IS BETTER NOT TO MEASURE TOO QUICKLY!
 delay(200);
}