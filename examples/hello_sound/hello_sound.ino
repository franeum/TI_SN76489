#include <TI_SN76489.h>

TI_SN76489 sn(9, 2,3,4,5,6,7,8,10,13);

void setup() {
  sn.begin();
  sn.attenuation(0, 0);
  sn.frequency(0, 440);
}

void loop() {
  // put your main code here, to run repeatedly:

}
