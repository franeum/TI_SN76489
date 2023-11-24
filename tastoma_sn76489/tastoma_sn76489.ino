/***************************************************************************
***** SN76489 ATARI 800XL style self test                              *****
***** by Kobi Tyrkel                                                   *****
***** Demostrates how to play sound with the SN76489 library           *****
***** Created 16 July 2019                                             *****
***************************************************************************/
#include <SN76489.h>
#include "Pitches.h"

#define PIN_NotWE 13

// Define FREQUENCY according to the frequency in your hardware setup
#define FREQUENCY 4000000.0

// Either directly interface with SN76489 with 9 pins (8bit + NotWE) or use a 595 Shift Register with 4 pins (3 for 595 + NotWE)
// Comment / Uncomment the relevant setup below to choose your HW setup

/***************************************************************************
***** Directly interface SN76489 IC with the following PIN definitions *****
***** and by calling 8-bit constractor                                 *****
***** The SN76489 pinout considered for this library is as follows:    *****
*****                                                                  *****
*****                        ========                                  *****
*****        D2       --> [ 1]  ()  [16] <-- VCC                       *****
*****        D1       --> [ 2]      [15] <-- D3                        *****
*****        D0       --> [ 3]  7   [14] <-- CLOCK OSC                 *****
*****     READY       <-- [ 4]  6   [13] <-- D4                        *****
*****    NOT WE       --> [ 5]  4   [12] <-- D5                        *****
*****    NOT CE       --> [ 6]  8   [11] <-- D6                        *****
***** AUDIO OUT       <-- [ 7]  9   [10] <-- D7                        *****
*****       GND       --> [ 8]      [ 9] --- N.C.                      *****
*****                        ========                                  *****
***************************************************************************/
#define PIN_D0 2
#define PIN_D1 3
#define PIN_D2 4
#define PIN_D3 5
#define PIN_D4 6
#define PIN_D5 7
#define PIN_D6 8
#define PIN_D7 10
SN76489 mySN76489 = SN76489(PIN_NotWE, PIN_D0, PIN_D1, PIN_D2, PIN_D3, PIN_D4, PIN_D5, PIN_D6, PIN_D7, FREQUENCY);


/***************************************************************************
***** Interface SN76489 IC with 595 Shift Register and PIN definitions *****
***** and by calling 3 PIN constractor                                 *****
***** 595 Shift Register pins connection with SN76489 is as follows:   *****
*****                                                                  *****
*****                        ========                                  *****
*****       D6 <-- Q1 <-- [ 1]  ()  [16] <-- VCC                       *****
*****       D5 <-- Q2 <-- [ 2]      [15] --> Q0     --> D7             *****
*****       D4 <-- Q3 <-- [ 3]  5   [14] <-- SER    <-- PIN_SER        *****
*****       D3 <-- Q4 <-- [ 4]  9   [13] <-- Not CE <-- GND            *****
*****       D2 <-- Q5 <-- [ 5]  5   [12] <-- CLOCK  <-- PIN_CLK        *****
*****       D1 <-- Q6 <-- [ 6]      [11] <-- LATCH  <-- PIN_LATCH      *****
*****       D0 <-- Q7 <-- [ 7]      [10] <-- Not MR <-- VCC            *****
*****             GND --> [ 8]      [ 9] --- Q7'    --- Not used       *****
*****                        ========                                  *****
***************************************************************************/
/*
#define PIN_SER   7
#define PIN_LATCH 6
#define PIN_CLK   5

SN76489 mySN76489 = SN76489(PIN_NotWE, PIN_SER, PIN_LATCH, PIN_CLK, FREQUENCY);
*/

/***************************************************************************
***** Melody and tempo data                                            *****
***************************************************************************/

uint16_t melody[] = {NOTE_G4, NOTE_F4, NOTE_AS4, NOTE_C5, NOTE_F5, NOTE_D5};
uint8_t tempo[] = {4, 4, 4, 8, 8, 4};

/***************************************************************************
***** Setup                                                            *****
***** Set 3 sound generators and noise to silence                      *****
***************************************************************************/
void setup() {
  TCNT1 = 0;
  TCCR1B = B00001001;
  TCCR1A = B01000000;
  OCR1A = 1;              // CLK frequency = 4 MHz
  pinMode(9, OUTPUT);
  delay(1000);
  
  mySN76489.setAttenuation(0, 3);
  mySN76489.setAttenuation(1, 3);
  mySN76489.setAttenuation(2, 0xF);
  mySN76489.setAttenuation(3, 0xF);
  mySN76489.setFrequency(0, 4000);
  mySN76489.setFrequency(1, 3600);
  mySN76489.setFrequency(2, 3200);
  Serial.begin(115200);
}

int mtof(int m) {
  return (int)(pow(2, (m - 69)/12.0) * 440.0);
}

/***************************************************************************
***** Loop                                                             *****
***** Play melody on each of the 3 tone generators                     *****
***** and then play all noises on the noise generator                  *****
***************************************************************************/

uint16_t notes[] = {84, 72, 67, 63, 60, 63, 67, 72};
uint16_t notes2[] = {86, 72, 67, 63, 60, 63, 67, 72};
uint16_t counter = 0;
long t1 = 0;

void loop()
{
  //static uint8_t toneGenerator = 0;
  static uint8_t idx = 0;
  static uint8_t idx2 = 0;
  static uint8_t idx3 = 0;
  static byte flag = 0;

  //int vol = analogRead(A0);
  //vol = map(vol,0,1024,0,30);

  //mySN76489.setAttenuation(0, 0x3);
  //mySN76489.setAttenuation(1, 0x3);
  //mySN76489.setAttenuation(2, 0x3);
  //mySN76489.setFrequency(0, notes[noteIndex]);
  //mySN76489.setFrequency(1, notes[(noteIndex+1)%4]);
  //mySN76489.setFrequency(2, notes[(noteIndex+2)%4]);
  /*
  for (int i=0; i<28; i++) {
    mySN76489.setAttenuation(0, i);
    mySN76489.setFrequency(0, 125);
    delay(25);
  }
  */
  //mySN76489.setAttenuation(0, 3);
  //mySN76489.setFrequency(0, notes[idx1]);
  //mySN76489.setFrequency(1, notes[idx2]);

  
    //mySN76489.setFrequency(0, 4000);
    //mySN76489.setFrequency(1, 3600);
    //mySN76489.setFrequency(2, 3200);
    

  

/*
  if (t1 % 1200 == 0) {
    
    idx2++;  
    if (idx2 > 8) idx2 = 0; 
  }
  */

/*
  int no = (int)random(125, 300);
  byte token = (byte)random(0,3);

  switch (token) {
    case 0:
      mySN76489.setFrequency(0, no);
      break;
    case 1:
      mySN76489.setFrequency(1, no);
      break;
    default:
      mySN76489.setFrequency(2, no);
  }
  */

  mySN76489.setFrequency(1, mtof(notes[idx2+6]-1));
  idx2++;
  if (idx2>1) idx2=0;
  
  if (!flag)
    mySN76489.setFrequency(0, mtof(notes[idx]-1));
  else
    mySN76489.setFrequency(0, mtof(notes2[idx]-1));
    Serial.println(mtof(notes[idx]+12));

  int token = (int)random(0,100);

  if (token > 90) {
    mySN76489.setAttenuation(2, 20);
    mySN76489.setFrequency(2, mtof(96));
  } else 
    mySN76489.setAttenuation(2, 0xF);

  idx++;
  if (idx>7) {
    flag = 1 - flag;
    idx=0;
  }

  delay(100);

}
