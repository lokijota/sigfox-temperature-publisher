#include <SoftwareSerial.h>

SoftwareSerial sigfox(5, 4);

// setup code, run once
void setup()
{
  Serial.begin(9600);
  sigfox.begin(9600);

  Serial.write("I'M READY\n");
}

// main code here, run repeatedly
void loop()
{
  String temperature = "";
  String hexTemperature = "";

  // sigfox.write("ATE0\n"); -- sometimes I get back strange binary codes from the board, this seemed to avoid that
  sigfox.write("ATI26\n");

  // .available() returns the number of bytes available to read from a serial port. this is data that's already arrived and stored in the serial receive buffer
  // https://www.arduino.cc/en/Reference/SoftwareSerialAvailable
  while (sigfox.available())
  {
    char c = sigfox.read();
    temperature = temperature + c;
  }

  // if we got any reading back from the chip in this execution loop...
  if (temperature.length() > 0)
  {
    temperature.trim(); // there are newlines before and after the temperature information
    Serial.println("Temperature: " + temperature + "C");

    // convert temperature to hex string
    hexTemperature =  String(temperature.toInt(), HEX);
    // DEBUG: Serial.print(hexTemperature);

    // send message to sigfox network
    sigfox.write("AT$SF=");
    sigfox.print(hexTemperature);
    sigfox.write("\n");
  }

  delay(60000); // beware of the 140msg/day limit! don't leave your board publishing!
  return;
}
