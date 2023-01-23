#include <Wire.h>
#include <Mouse.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd (0x27 , 16 , 2);
const int trigpin= 10;
const int echopin= 13;
long duration;
int distance;
int water=A0;
const int buzzer_led = 11 ;
volatile int flow_frequency; // Measures flow sensor pulses
unsigned int l_hour; // Calculated litres/hour
unsigned char flowsensor = 2; // Sensor Input
unsigned long currentTime;
unsigned long cloopTime;
void flow () // Interrupt function
{
   flow_frequency++;
}
void setup()
{
pinMode(buzzer_led, OUTPUT);  
pinMode(trigpin,OUTPUT);
pinMode(echopin,INPUT);
pinMode(flowsensor, INPUT);
digitalWrite(flowsensor, HIGH);
Serial.begin(9600);
attachInterrupt(0, flow, RISING);
sei(); // Enable interrupts
currentTime = millis();
cloopTime = currentTime;
lcd.init ();
lcd.backlight();
}
void loop()
{
  digitalWrite(trigpin,HIGH);
  delayMicroseconds(3000);
  digitalWrite(trigpin,LOW);
  duration=pulseIn(echopin,HIGH);
  distance = duration*0.034/2;
  int a=analogRead(water);
  currentTime = millis();
  if(currentTime >= (cloopTime + 1000)){
     cloopTime = currentTime;
     l_hour = (flow_frequency * 60 / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
     flow_frequency = 0; // Reset Counter
  }  
  
  Serial.print("The water is :  ");
  Serial.print(distance);
  Serial.print(" CM Away");
  
  Serial.print(" ||  Water level is: ");
  Serial.print(a);
  
  Serial.print(" ||  Water flow is: ");
  Serial.print(l_hour, DEC);
  Serial.println(" Litter/hour");
 
  if(l_hour >= 50|| distance <= 3 || a >= 400){
    digitalWrite(buzzer_led, HIGH);  
    delay(1000);                     
    digitalWrite(buzzer_led, LOW);  
    delay(100);
  }

  lcd.setCursor (0,0);
  lcd.print("  Early Flood");
  lcd.setCursor(0,1);
  lcd.print("Detection System");
  delay (3000); 
  lcd.clear();
  
  lcd.setCursor (0,0);
  lcd.print("The water is :");
  lcd.setCursor(0,1);
  lcd.print(distance);
  lcd.print(" CM away");
  delay (3000);
  lcd.clear();
  if(distance<=3){
  lcd.setCursor(0,0);
  lcd.print("     DANGER  ");
  delay (3000);
  lcd.clear();  
  }
  
  lcd.setCursor (0,0);
  lcd.print("Water level is:");
  lcd.setCursor(0,1);
  lcd.print(a);
  delay (3000);
  lcd.clear();
  if(a>=400){
  lcd.setCursor(0,0);
  lcd.print("     DANGER  ");
  delay (3000);
  lcd.clear();  
  }
  lcd.setCursor (0,0);
  lcd.print("Water flow is:");
  lcd.setCursor(0,1);
  lcd.print(l_hour);
  lcd.print(" L/Hour");
  delay (3000);
  lcd.clear();
  if(l_hour>=50){
  lcd.setCursor(0,0);
  lcd.print("     DANGER  ");
  delay (3000);
  lcd.clear();  
  }

}
