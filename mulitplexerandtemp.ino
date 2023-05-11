
#include <Wire.h>
#include <SparkFunHTU21D.h>

//Create an instance of the object
HTU21D myHumidity;

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

  Serial.println("HTU21D Example!");
  myHumidity.begin();

  while (!Serial);  // Leonardo: wait for Serial Monitor
  Serial.println("\nI2C Scanner");
  
  TCA9548A(7);

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
  delay(1000);
}



void loop() {

Itwoc();
// Serial.println("working function");
TCA9548A(7);
Tempcal();

}
