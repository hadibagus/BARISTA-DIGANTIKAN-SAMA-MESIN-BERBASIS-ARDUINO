/*
 Setup your scale and start the sketch WITHOUT a weight on the scale
 Once readings are displayed place the weight on the scale
 Press +/- or a/z to adjust the calibration_factor until the output readings match the known weight
 Arduino pin 6 -> HX711 CLK
 Arduino pin 5 -> HX711 DOUT
 Arduino pin 5V -> HX711 VCC
 Arduino pin GND -> HX711 GND 
*/

#include "HX711.h"
#include <Servo.h>        //Servo library


Servo servo_test;      //initialize a servo object for the connected servo
HX711 scale(5, 6);

//Button Variables pin D1
int button = 1;
int valButton = 0;
int old_valButton = 0;
int stateButton = 0;

//Weight Variables
float gr;
float ml;

//Servo Variables
float angle = 0;

//Temperature Variables pin A0
int ThermistorPin = 0;
int Vo;
float R1 = 10000;
float logR2, R2, T, Tc, Tf;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

//Relay Variables pin D12
int relay = 12;
volatile byte relayState = LOW;

//Program State
int programState = 0;

void setup() {
  servo_test.attach(9);   // attach the signal pin of servo to pin9 of arduino
  Serial.begin(9600);
  Serial.println("HX711 calibration sketch");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press + or a to increase calibration factor");
  Serial.println("Press - or z to decrease calibration factor");
  
  
  scale.set_scale();
  scale.tare();  //Reset the scale to 0

  long zero_factor = scale.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);

  pinMode(relay, OUTPUT);
  pinMode(button, INPUT);
}

void temperature(){
  /*
   * untuk sensing temperatir lalu untuk trigger relay heater element
  */
  Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  Tc = T - 273.15;
  Tf = (Tc * 9.0)/ 5.0 + 32.0; 
  }

void closeRelay(){
  /*
   *diperuntukan untuk mengaktifkan/menonaktifkan heater element relay berdasarkan temperatur
  */
  if(Tc <= 87){
  digitalWrite(relay, HIGH);
  }
}

void buttonState(){
  valButton=digitalRead(button);
if( (valButton==HIGH) && (old_valButton==LOW)) {
  stateButton=1-stateButton;}
  old_valButton=valButton;
  // Trigger if (stateButton==1) { digitalWrite(led, HIGH);}
  //          else {digitalWrite (led,LOW);}
  }

float weight(){
      float calibration_factor = 2230; // this calibration factor is adjusted according to my load cell
      float units;
      float ounces;
  scale.set_scale(calibration_factor); //Adjust to this calibration factor

  
  units = scale.get_units(), 10;
  if (units < 0)
  {
    units = 0.00;
  }
  ounces = units * 0.035274;
  ml = (units*15)/10;
  ml = round(ml);
  ml = ml*10;
  return units;
  
  }


void timing(){
  /*
   * mengatur seberapa lama bukaan air yang dipengaruhi oleh units dari weight
   * dari ml yang di dapat per detiknya, lalu angka tersebut digunakan untuk mencari berapa lama waktu yang dibutuhkan untuk mendapatkan air sebanyak itu.
   * 
  */
  }

  
/*  if(Serial.available())
  {
    char temp = Serial.read();
    if(temp == '+' || temp == 'a')
      calibration_factor += 1;
    else if(temp == '-' || temp == 'z')
      calibration_factor -= 1;
  }
  
 */

void loop() {
  
temperature();
buttonState();
 
 if(stateButton==1){
  if(programState == 0){
   closeRelay();
   programState++;
    }
  }
 else {
    /*
      * function Display dimasukkan kedalam sini
     */
  }

Serial.print("Water Needed: ");
  Serial.print(weight());
  Serial.print(" gr"); 
  Serial.println();
  delay(10);
}
