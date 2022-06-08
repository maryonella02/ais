#include<LiquidCrystal.h>
#include <DHT.h>

#define echo 9
#define trigger 10
#define tank_pump 4
#define watering_pump 13
#define moisture_sensor A0
#define tempPin A1
#define dhtPin A2
#define DHTTYPE DHT11

DHT dht(dhtPin, DHTTYPE);

long duration;
int distance;
int moistureValue;
int distancePercent;
int moistPercent;
const float speedOfSound = 0.034;

float temp;
float humidity;
const float tempResolution = 1024.0;
const float voltage = 5000.0;

LiquidCrystal lcd(12,11,8,7,6,5);
const int lcdRows = 4;
const int lcdCols = 20;


void setup1()
{
  pinMode(tempPin, INPUT);
  Serial.begin(9600);
  dht.begin();
}

void readTemperature() 
{
temp = analogRead(tempPin);
humidity = dht.readHumidity() * 2;
temp = dht.readTemperature() * 2;
Serial.print("Humidity = ");
Serial.print(humidity);
Serial.println("%  ");
Serial.print("Temperature: ");
Serial.print(temp);
Serial.println(" *C ");

delay(1000);
}

void setup () 
{
lcd.begin(lcdCols, lcdRows);
Serial.begin(9600);
pinMode(echo,INPUT);
pinMode(moisture_sensor,INPUT);
pinMode(trigger,OUTPUT);
digitalWrite(trigger,LOW);
pinMode(watering_pump,OUTPUT);
pinMode(tank_pump,OUTPUT);
digitalWrite(watering_pump,LOW);
digitalWrite(tank_pump,LOW);

lcd.setCursor(0,0);
lcd.print("AUTOMATED IRRIGATION");
lcd.setCursor(0,1);
lcd.print("SYSTEM");
delay(1000);
lcd.clear();
}

void loop()
{
digitalWrite(trigger,LOW);
delayMicroseconds(2);
digitalWrite(trigger,HIGH);
delayMicroseconds(10); 
digitalWrite(trigger,LOW);
duration=pulseIn(echo,HIGH);
distance=duration*speedOfSound/2; 
distancePercent=map(distance,0,1023,0,100);
moistureValue= analogRead(moisture_sensor);
moistPercent=map(moistureValue,0,1023,0,100);
LCD();
readTemperature();
}

void LCD()
{
    if (humidity <= 70) {
        lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("TANK LEVEL= ");
    lcd.print(distancePercent);
    lcd.print("%");
    lcd.setCursor(0,1);
    lcd.print("MOISTURE STATUS= ");
    lcd.print(moistPercent);
    lcd.print("%");
    if (distancePercent>50 && moistPercent<70)
    {
    lcd.setCursor(0,2);
    lcd.print("WATER PUMP:");
    lcd.print("ON");
    lcd.setCursor(0,3);
    lcd.print("TANK PUMP:");
    lcd.print("OFF");
    digitalWrite(tank_pump,LOW);
    digitalWrite(watering_pump,HIGH);
    delay(1000);
    }
    else if (distancePercent<65 &&moistPercent>85)
    {
    lcd.setCursor(0,2);
    lcd.print("WATER PUMP:");
    lcd.print("OFF");
    lcd.setCursor(0,3);
    lcd.print("TANK PUMP:");
    lcd.print("ON");
    digitalWrite(tank_pump,HIGH);
    digitalWrite(watering_pump,LOW);
    delay(1000);
    }
    else if (distancePercent>65 &&moistPercent>85)
    {
    lcd.setCursor(0,2);
    lcd.print("WATER PUMP:");
    lcd.print("OFF");
    lcd.setCursor(0,3);
    lcd.print("TANK PUMP:");
    lcd.print("OFF");
    digitalWrite(tank_pump,LOW);
    digitalWrite(watering_pump,LOW);
    delay(1000);
    }
    else if (distancePercent<65 &&moistPercent<85)
    {
    lcd.setCursor(0,2);
    lcd.print("WATER PUMP:");
    lcd.print("ON");
    lcd.setCursor(0,3);
    lcd.print("TANK PUMP:");
    lcd.print("ON");
    digitalWrite(tank_pump,HIGH);
    digitalWrite(watering_pump,HIGH);
    delay(1000);
    }
    } else {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Air Humidity High");
        lcd.setCursor(0,2);
        lcd.print("WATER PUMP:");
        lcd.print("OFF");
        lcd.setCursor(0,3);
        lcd.print("TANK PUMP:");
        lcd.print("OFF");
        digitalWrite(tank_pump,LOW);
        digitalWrite(watering_pump,LOW);
        delay(1000);
    }
}
