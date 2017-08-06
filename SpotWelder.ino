/*  _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _
   / \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \
   \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/

   Spot welder made from an old microwave

     for more information visit
      www.ideahex.com

   Written by Damian Kleiss
    _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _
   / \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \
   \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \*/

/*

  To read Power switch
  enable pull up on switch pin shared with other 3 switches
  set switch wantinted to read to HIGH all otherd to LOW example below

  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);

  digitalWrite(15,HIGH);
  digitalWrite(16,LOW);
  digitalWrite(12,LOW);
  digitalWrite(11,LOW);


  pinMode(8,INPUT_PULLUP);
  pinMode(7,OUTPUT);
  digitalWrite(7,HIGH);
*/

/* Encoder Library - Basic Example
   http://www.pjrc.com/teensy/td_libs_Encoder.html

   This example code is in the public domain.
*/

#include <Encoder.h>

///////////////// LED Display ///////////////////////
#define DIGIT0  18  //1 Active High
#define DIGIT1  19  //2
#define DIGIT2  14
#define DIGIT3  6
#define DIGIT4  4
#define NUM_DIGITS 5

#define SEGMENTA    7   // 14 Active Low
#define SEGMENTB    8   // 13    
#define SEGMENTC    9   // 12  ALSO power switch
#define SEGMENTD    10  // 11
#define SEGMENTE    11  // 9
#define SEGMENTF    16  // 7  ALSO CLOCK SWITCH
#define SEGMENTG    12  // 8
#define SEGMENT1        // 10
#define SEGMENT2        // 

#define BLANK 10
#define COLON 1

///////////////// Switch matrix ///////////////////////
#define SW_PIN_PWR_CLK          15
#define SW_PIN_START_C1         16//SEGMENTF//16
#define SW_PIN_GRL_WEI          12//SEGMENTG//12
#define SW_PIN_STOP_C2          11//SEGMENTE//11
#define SW_PIN_PWR_STOP_C1_GRL  8//SEGMENTB//8
#define SW_PIN_START_C2_WEI_CLK 7//SEGMENTF//7
// Rows
#define SW_PWR_CLK          0
#define SW_START_C1         1
#define SW_GRL_WEI          2
#define SW_STOP_C2          3
// Cols
#define SW_PWR_STOP_C1_GRL  0
#define SW_START_C2_WEI_CLK 1

// SWITCHES
#define SW_PWR    0
#define SW_STOP   1
#define SW_C1     2
#define SW_GRL    3
#define SW_START  4
#define SW_C2     5
#define SW_WEI    6
#define SW_CLK    7

#define NUM_SWITCHES 8
#define NUM_ROWS  4
#define NUM_COLS 2


///////////////// ENCODER ///////////////////////
#define ENCODERA  2 // Use ext interupts
#define ENCODERB  3 // Use ext interupts

///////////////// BUZZER ///////////////////////
#define BUZZER 5

#define FRED 1

///////////////// State machine stuff ///////////////////////
#define STATE_IDLE    0
#define STATE_PREWELD 2
#define STATE_DWELL   3
#define STATE_WELD    4

///////////////// Switch variables ///////////////////////
bool switchState[NUM_SWITCHES];
unsigned int switchRows[NUM_ROWS] = {SW_PIN_PWR_CLK, SW_PIN_START_C1, SW_PIN_GRL_WEI, SW_PIN_STOP_C2};
unsigned int switchCols[NUM_COLS] = {SW_PIN_PWR_STOP_C1_GRL, SW_PIN_START_C2_WEI_CLK};

///////////////// Display Variables ///////////////////////
unsigned int period = 0;
bool updateDisplay = 0;
unsigned int displayHolder[NUM_DIGITS] = {BLANK, BLANK, BLANK, BLANK, BLANK};

//                      zero      one       two         three   four      five      six         seven   eight     nine        blank
const int numeral[] = {B1000000, B1111001, B0100100, B0110000, B0011001, B0010010, B0000010, B1111000, B0000000, B0010000, B1111111};//0b011000000, 0b110110010};
int currentDigit = 1;

///////////////// State machine variables ///////////////////////
unsigned int state = STATE_IDLE;

///////////////// Other variables ///////////////////////

// Weld times 
// As the SSR is zero switching times will need to be in multiples of 1/100 HZ = 10 milliseconds
unsigned int preWeldTime = 50; //milliseconds // 50ms is a good place to start
unsigned int dwellTime = 20;    // Don't know
unsigned int weldTime = 50;   // Between 50 and 250ms

long encoderOldPosition  = -999;
long encoderNewPosition = 0;

// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder myEnc(3, 2);


// the setup function runs once when you press reset or power the board
void setup()
{
  Serial.begin(9600);
  Serial.println("");
  Serial.println("Spot Welder by ideaHex");
  Serial.println("");

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(DIGIT0, OUTPUT);
  pinMode(DIGIT1, OUTPUT);
  pinMode(DIGIT2, OUTPUT);
  pinMode(DIGIT3, OUTPUT);
  pinMode(DIGIT4, OUTPUT);

  pinMode(SEGMENTA, OUTPUT);
  pinMode(SEGMENTB, OUTPUT);
  pinMode(SEGMENTC, OUTPUT);
  pinMode(SEGMENTD, OUTPUT);
  pinMode(SEGMENTE, OUTPUT);
  pinMode(SEGMENTF, OUTPUT);
  pinMode(SEGMENTG, OUTPUT);

  pinMode(SW_PIN_PWR_CLK, OUTPUT);


  // Timer0 is already used for millis() - we'll just interrupt somewhere
  // in the middle and call the "Compare A" function below
  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);
}


void UpdateSwitches()
{
  for (int row = 0; row < NUM_ROWS; row++)
  {
    // Set all rows low except the one we are wanting to check
    for (int i = 0; i < NUM_ROWS; i++)
    {
      if (i == row) digitalWrite(switchRows[i], HIGH);
      else  digitalWrite(switchRows[i], LOW);
    }
    pinMode(SW_PIN_START_C2_WEI_CLK, INPUT);
    pinMode(SW_PIN_PWR_STOP_C1_GRL, INPUT);
    switchState[2 * row] = digitalRead(SW_PIN_START_C2_WEI_CLK);
    switchState[(2 * row) + 1] = digitalRead(SW_PIN_PWR_STOP_C1_GRL);
    pinMode(SW_PIN_START_C2_WEI_CLK, OUTPUT);
    pinMode(SW_PIN_PWR_STOP_C1_GRL, OUTPUT);
  }

}
// Interrupt is called once a millisecond
SIGNAL(TIMER0_COMPA_vect)
{
  if (period++ > FRED)
  {
    period = 0;
    enableDigit(currentDigit);
    writeNumeral(displayHolder[currentDigit]);
    if (currentDigit++ >= NUM_DIGITS)
    {
      UpdateSwitches();
      currentDigit = 0;
    }
    updateDisplay = 0;
  }
}

// Pass it a digit value 0 - 4
void enableDigit(int digit)
{
  digitalWrite(DIGIT0, digit == 0);
  digitalWrite(DIGIT1, digit == 1);
  digitalWrite(DIGIT2, digit == 2);
  digitalWrite(DIGIT3, digit == 3);
  digitalWrite(DIGIT4, digit == 4);
}

void writeNumeral(int num)
{
  if (num > 9) num = 10; // Make it blank
  int seg = 0;
  digitalWrite(SEGMENTA, numeral[num] & 1 << seg++);
  digitalWrite(SEGMENTB, numeral[num] & 1 << seg++);
  digitalWrite(SEGMENTC, numeral[num] & 1 << seg++);
  digitalWrite(SEGMENTD, numeral[num] & 1 << seg++);
  digitalWrite(SEGMENTE, numeral[num] & 1 << seg++);
  digitalWrite(SEGMENTF, numeral[num] & 1 << seg++);
  digitalWrite(SEGMENTG, numeral[num] & 1 << seg++);
}

void UpdateEncoder()
{
  encoderNewPosition = myEnc.read();
  if (encoderNewPosition != encoderOldPosition)
  {
    if (encoderNewPosition > encoderOldPosition) tone(BUZZER, 1200, 80);
    else tone(BUZZER, 600, 80);
    if (encoderNewPosition > 399)
    {
      myEnc.write(399);
      encoderNewPosition = 399;
    }
    if (encoderNewPosition < 0)
    {
      myEnc.write(0);
      encoderNewPosition = 0;
    }
    encoderOldPosition = encoderNewPosition;
    //Serial.println(newPosition);
  }
}
// the loop function runs over and over again forever
void loop()
{
  UpdateEncoder();
  switch (state)
  {
    case STATE_IDLE:
      //Serial.println("IDLE");
      if (displayHolder[4]  == COLON) displayHolder[4] = BLANK;
      else displayHolder[4] = COLON;
      //displayHolder[4]=8;
      preWeldTime = encoderNewPosition / 4;
      if (preWeldTime / 10 < 1) displayHolder[0] = BLANK;
      else displayHolder[0] = preWeldTime / 10;
      displayHolder[1] = preWeldTime - ((preWeldTime / 10) * 10);
      //if (preWeldTime >22) preWeldTime = 0;

      if (weldTime / 10 < 1) displayHolder[2] = BLANK;
      else displayHolder[2] = weldTime / 10;
      displayHolder[3] = weldTime - ((weldTime / 10) * 10);
      if (weldTime++ > 99) weldTime = 0;

      for (int j = 0; j < NUM_SWITCHES; j++)
      {
        Serial.print(switchState[j]);
        Serial.print(",");
      }
      Serial.println("");
      delay(200);

      //Serial.println(powerSwitchState);

      //delay(200);
      break;

    case STATE_PREWELD:
      Serial.println("Preweld");
      break;

    case STATE_DWELL:
      Serial.println("Preweld");
      break;

    case STATE_WELD:
      Serial.println("Weld");
      break;
  }


}
