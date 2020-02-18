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
float maximumRange = 300.0; // Maximum distance in cm.
float minimumRange = 0.2; // Minimum distance in cm.
unsigned long duration = 0; // Time of voice to go there and back (in microseconds).
float distance = 0.0; // Distance (cm) to the target
unsigned long time = 0; // Time stamp (ms) in long type (int gives maximum of only 32 sec)

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
 time = millis();


 //The speed of voice at room temperature is 343m/s. This leads to eqn.
 
 distance =duration * 0.016854; //in cm

 if (distance >= maximumRange || distance <= minimumRange){
 // Test for the quality of the distance
 Serial.print(time);
 Serial.print(" ");
 Serial.println(-5);
 }
 else {

 // print out "good results":
 Serial.print(time);          // time(ms)
 Serial.print(" ");           // space for division
 Serial.print(distance);      // Distance (cm)
 Serial.println(" ");           
 }
 
 //Delay... NOTE THAT IF WE CALCULATE THE SPEED FROM THE DATA, IT IS BETTER NOT TO MEASURE TOO QUICKLY!
 delay(200);
}