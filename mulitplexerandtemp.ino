
#include <Wire.h>
#include <SparkFunHTU21D.h>
#include <LiquidCrystal.h>

//lcd display
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);


//Create an instance of the object
HTU21D myHumidity;

// variable declaration for ultrasonic
int trigPin = 11;    // Trigger
int echoPin = 12;    // Echo
long duration, cm, inches;

//soil moisture sensor variable declaration
#define AOUT_PIN A0 // Arduino pin that connects to AOUT pin of moisture sensor 

// Select I2C BUS
void TCA9548A(uint8_t bus){
  Wire.beginTransmission(0x70);  // TCA9548A address
  Wire.write(1 << bus);          // send byte to select bus
  Wire.endTransmission();
  Serial.print(bus);
}

void setup() {
  Wire.begin();

  Serial.begin(9600);

  //Define inputs and outputs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.println("HTU21D Example!");
  myHumidity.begin();

  while (!Serial);  // Leonardo: wait for Serial Monitor
  Serial.println("\nI2C Scanner");
  
  TCA9548A(7);

  //lcd display
  lcd.begin(16, 2);
  lcd.print("Data Display");
}

// I2c Scan Function
void Itwoc() {
  int nDevices = 0;

  Serial.println("Scanning...");

  for (byte address = 1; address < 127; ++address) {
    // The i2c_scanner uses the return value of
    // the Wire.endTransmission to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.print(address, HEX);
      Serial.println("  !");

      ++nDevices;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  } else {
    Serial.println("done\n");
  }
  delay(5000);  // Wait 5 seconds for next scan
}


void Tempcal(){
    float humd = myHumidity.readHumidity();
  float temp = myHumidity.readTemperature();

  Serial.print("Time:");
  Serial.print(millis());
  Serial.print(" Temperature:");
  Serial.print(temp, 1);
  Serial.print("C");
  Serial.print(" Humidity:");
  Serial.print(humd, 1);
  Serial.print("%");

  Serial.println();

  //lcd preview 
  Lcd("Temprature" , temp);
  Lcd("Humidity" , humd);
}

void Ultrasonic(){
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
 
  // Convert the time into a distance
  cm = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
  inches = (duration/2) / 74;   // Divide by 74 or multiply by 0.0135
  
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  
}


//soil moisture function
void Moisture(){
  int value = analogRead(AOUT_PIN); // read the analog value from sensor
 
  Serial.print("The soil Moisture is : ");
  Serial.print(value);

  //lcd preview data
  Lcd("Mositure" , value);

}

//lcd display function
void Lcd(char myString[], int data){
  
  lcd.clear();
  lcd.write(myString);
  lcd.write(" : ");
  lcd.print(data);
  delay(2000);
  lcd.clear();
}

void loop() {

// Itwoc();
// Serial.println("working function");
TCA9548A(7);
Tempcal();
Serial.println(" ");
Ultrasonic();
Serial.println(" ");
Moisture();
Serial.println(" ");

delay(5000);

}
