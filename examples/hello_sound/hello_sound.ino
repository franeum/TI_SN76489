#include <TI_SN76489.h>

TI_SN76489 sn(2, 3, 4, 5, 6, 7, 8, 10, 13, 9, _4MHz);

void setup()
{
  Serial.begin(115200);
  sn.begin();
  sn.attenuation(0, 0);
  sn.frequency(0, 440);
}

void loop()
{
  // put your main code here, to run repeatedly:
  sn.attenuation(0, 0);
  sn.frequency(0, 200);
  delay(200);
  sn.frequency(0, 400);
  delay(200);
  sn.frequency(0, 600);
  delay(200);
  sn.frequency(0, 800);
  delay(200);
}
