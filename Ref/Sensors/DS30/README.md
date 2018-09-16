# This sketch is an example of the read CO2 sensor connected Via I2C

### Method example

```///////////////////////////////////////////////////////////////////
// Function : int readCO2()
// Returns : CO2 Value upon success, 0 upon checksum failure
// Assumes : - Wire library has been imported successfully.
// - LED is connected to IO pin 13
// - CO2 sensor address is defined in co2_addr
///////////////////////////////////////////////////////////////////
int readCO2()
{
  int co2_value = 0;
  Wire.beginTransmission(0x68);
  Wire.write(0x22);
  Wire.write(0x00);
  Wire.write(0x08);
  Wire.write(0x2A);

  if (Wire.endTransmission() == 0) {
    delay(10);
    Wire.requestFrom(0x68, 4);
    byte ii = 0;
    byte buffer[4] = {0, 0, 0, 0};
    delay(10);
    while(Wire.available() and ii < 500)
    {
      buffer[ii] = Wire.read();
      ii++;
    }

    co2_value = 0;
    co2_value |= buffer[1] & 0xFF;
    co2_value = co2_value << 8;
    co2_value |= buffer[2] & 0xFF;

    byte sum = 0;
    sum = buffer[0] + buffer[1] + buffer[2];

    if(sum == buffer[3])
    {
      return co2_value;
    }
    else
    {
      return 0;
    }
  } else {
    srPrintln("Fault wire transmission Co2");
    return(FALSE);
  }
}```