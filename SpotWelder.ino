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

// TO DO
// Debounce switches



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

//#define SEGMENTA
//#define SEGMENTA
//#define SEGMENTA
//#define SEGMENTA
//#define SEGMENTA


#define BLANK 10
#define COLON 1
#define P     11
#define R     12
#define D     13
#define W_1   14
#define W_2   15
#define C     16
#define NUMERAL_MAX   16

///////////////// Switches ///////////////////////
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
#define SW_PWR    1
#define SW_STOP   7
#define SW_C1     3
#define SW_GRL    5
#define SW_START  2
#define SW_C2     6
#define SW_WEI    4
#define SW_CLK    0
#define SW_FOOT   8   // Seperate switch

#define NUM_SWITCHES 8  // In Matrix
#define NUM_ROWS  4
#define NUM_COLS 2

#define SW_PIN_FOOT 17

#define SW_DOWN  1// held down
#define SW_UP    0// Not held down
#define SW_REL   2// Has bee released but not activated on


///////////////// ENCODER ///////////////////////
#define ENCODERA  2 // Use ext interupts
#define ENCODERB  3 // Use ext interupts
#define ENCODER_DIV 4

///////////////// BUZZER ///////////////////////
#define BUZZER        5
#define TONE_WELD     0//1000
#define TONE_UP       0//800
#define TONE_DOWN     0//600
#define TONE_DURATION 0//60

///////////////// SSR ///////////////////////
#define SSR_PIN 13

#define FRED 1

///////////////// State machine stuff ///////////////////////
#define STATE_IDLE            0
#define STATE_CHANGE_PREWELD  1
#define STATE_CHANGE_DWELL    2
#define STATE_CHANGE_WELD     3
#define STATE_ACTIVE          4
#define STATE_PREWELD         5
#define STATE_DWELL           6
#define STATE_WELD            7
#define STATE_WELD_COUNT      8

///////////////// Switch variables ///////////////////////
unsigned int switchState[NUM_SWITCHES];
unsigned int switchRows[NUM_ROWS] = {SW_PIN_PWR_CLK, SW_PIN_START_C1, SW_PIN_GRL_WEI, SW_PIN_STOP_C2};
unsigned int switchCols[NUM_COLS] = {SW_PIN_PWR_STOP_C1_GRL, SW_PIN_START_C2_WEI_CLK};

///////////////// Display Variables ///////////////////////
unsigned int period = 0;
bool updateDisplay = 0;
unsigned int displayHolder[NUM_DIGITS] = {BLANK, BLANK, BLANK, BLANK, BLANK};

//                      zero      one       two         three   four      five      six         seven   eight     nine        blank   P R E W_1 W_2
const int numeral[] = {B1000000, B1111001, B0100100, B0110000, B0011001, B0010010, B0000010, B1111000, B0000000, B0010000,
                       // blank        P         R         D         W_1     W_2      C
                       B1111111, B0001100, B1001100, B0100001, B1000011, B1100001, B1000110
                      };
int currentDigit = 1;

///////////////// State machine variables ///////////////////////
unsigned int state = STATE_IDLE;
unsigned int prevoiusState = STATE_IDLE;

///////////////// Other variables ///////////////////////

// Weld times
// As the SSR is zero switching times will need to be in multiples of 1/100 HZ = 10 milliseconds
unsigned int preWeldTime = 5; // 10 x milliseconds 5 = 50 milliseconds// 50ms is a good place to start
unsigned int dwellTime = 2;    // Don't know
unsigned int weldTime = 5;   // Between 50 and 250ms

unsigned int elapsedTime = 0; //milliseconds
unsigned int preWeldElapsedTime = 0;
unsigned int dwellElapsedTime = 0;
unsigned int weldElapsedTime = 0;

unsigned long weldCount = 0;

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

  pinMode(SSR_PIN, OUTPUT);
  digitalWrite(SSR_PIN, LOW);


  // Timer0 is already used for millis() - we'll just interrupt somewhere
  // in the middle and call the "Compare A" function below
  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);
}


void UpdateSwitches()
{
  int sw;

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

    sw = digitalRead(SW_PIN_START_C2_WEI_CLK);
    if (sw == SW_DOWN)  switchState[2 * row] = SW_DOWN;
    else if ( (switchState[2 * row] == SW_UP) && (sw == SW_UP) ) switchState[2 * row] = SW_UP;
    else if ( (switchState[2 * row] == SW_REL) && (sw == SW_UP) ) switchState[2 * row] = SW_UP;
    else switchState[2 * row] = SW_REL;

    sw = digitalRead(SW_PIN_PWR_STOP_C1_GRL);
    if (sw == SW_DOWN)  switchState[(2 * row) + 1] = SW_DOWN;
    else if ( (switchState[(2 * row) + 1]  == SW_UP) && (sw == SW_UP) ) switchState[(2 * row) + 1]  = SW_UP;
    else if ( (switchState[(2 * row) + 1] == SW_REL) && (sw == SW_UP) ) switchState[(2 * row) + 1]  = SW_UP;
    else switchState[(2 * row) + 1]  = SW_REL;

    pinMode(SW_PIN_START_C2_WEI_CLK, OUTPUT);
    pinMode(SW_PIN_PWR_STOP_C1_GRL, OUTPUT);
   }

  sw = !digitalRead(SW_PIN_FOOT);
  if (sw == SW_DOWN)  switchState[SW_FOOT] = SW_DOWN;
  else if ( (switchState[SW_FOOT] == SW_UP) && (sw == SW_UP) ) switchState[SW_FOOT] = SW_UP;
  else if ( (switchState[SW_FOOT] == SW_REL) && (sw == SW_UP) ) switchState[SW_FOOT]  = SW_UP;
  else switchState[SW_FOOT]  = SW_REL;
}

// Interrupt is called once a millisecond
unsigned int multi = 0;
SIGNAL(TIMER0_COMPA_vect)
{
  if (multi++ > 10)
  {
    multi = 0;
    elapsedTime++;
  }
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
  if (num > NUMERAL_MAX ) num = 10; // Make it blank
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
    if (encoderNewPosition > encoderOldPosition) tone(BUZZER, TONE_UP, TONE_DURATION);
    else tone(BUZZER, TONE_DOWN, TONE_DURATION);
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
  //  //  UpdateEncoder();
  for (int j = 0; j < 9; j++)
  {
    Serial.print(switchState[j]);
    Serial.print(", ");
    delay(10);
  }
  Serial.println("");

  switch (state)
  {
    case STATE_IDLE:
      if (prevoiusState != state) // First time in so set a few things up
      {
        prevoiusState = state;
        Serial.println("Idle");
      }
      //      if (displayHolder[4]  == COLON) displayHolder[4] = BLANK;
      //      else displayHolder[4] = COLON;
      displayHolder[4] = COLON;
      if (preWeldTime / 10 < 1) displayHolder[0] = BLANK;
      else displayHolder[0] = preWeldTime / 10;
      displayHolder[1] = preWeldTime - ((preWeldTime / 10) * 10);

      if (weldTime / 10 < 1) displayHolder[2] = BLANK;
      else displayHolder[2] = weldTime / 10;
      displayHolder[3] = weldTime - ((weldTime / 10) * 10);

      //      for (int j = 0; j < 5; j++)
      //      {
      //        for (int i = 0; i < 5; i++)
      //        {
      //          if (i == j) displayHolder[i] = 8;
      //          else displayHolder[i] = BLANK;
      //        }
      //        delay(1000);
      //      }

      //Serial.println("");

      if  (switchState[SW_CLK] == SW_REL)
      {
        switchState[SW_CLK] = SW_UP;
        state = STATE_CHANGE_PREWELD;
      }
      else if  (switchState[SW_WEI] == SW_REL)
      {
        switchState[SW_WEI] = SW_UP;
        state = STATE_CHANGE_DWELL;
      }
      else if  (switchState[SW_GRL] == SW_REL)
      {
        switchState[SW_GRL] = SW_UP;
        state = STATE_CHANGE_WELD;
      }
      else if  (switchState[SW_C1] == SW_REL)
      {
        switchState[SW_C1] = SW_UP;
        state = STATE_WELD_COUNT;
      }
      else if  (switchState[SW_PWR] == SW_REL)
      {
        switchState[SW_PWR] = SW_UP;
        state = STATE_ACTIVE;
      }

      break;

    case STATE_CHANGE_PREWELD:
      if (prevoiusState != state) // First time in so set a few things up
      {
        prevoiusState = state;
        Serial.println("Change Preweld");
        displayHolder[0] = P;
        displayHolder[1] = R;
        myEnc.write(preWeldTime * ENCODER_DIV);
      }
      UpdateEncoder();
      preWeldTime = encoderNewPosition / ENCODER_DIV;
      if (preWeldTime / 10 < 1) displayHolder[2] = BLANK;
      else displayHolder[2] = preWeldTime / 10;
      displayHolder[3] = preWeldTime - ((preWeldTime / 10) * 10);

      if  (switchState[SW_C2] == SW_REL)
      {
        switchState[SW_C2] = SW_UP;
        state = STATE_IDLE;
      }
      break;

    case STATE_CHANGE_DWELL:
      if (prevoiusState != state) // First time in so set a few things up
      {
        prevoiusState = state;
        Serial.println("Change Dwell");
        displayHolder[0] = D;
        displayHolder[1] = BLANK;
        myEnc.write(dwellTime * ENCODER_DIV);
      }
      UpdateEncoder();
      dwellTime = encoderNewPosition / ENCODER_DIV;
      if (dwellTime / 10 < 1) displayHolder[2] = BLANK;
      else displayHolder[2] = dwellTime / 10;
      displayHolder[3] = dwellTime - ((dwellTime / 10) * 10);

      if  (switchState[SW_C2] == SW_REL)
      {
        switchState[SW_C2] = SW_UP;
        state = STATE_IDLE;
      }
      break;

    case STATE_CHANGE_WELD:
      if (prevoiusState != state) // First time in so set a few things up
      {
        prevoiusState = state;
        Serial.println("Change Weld");
        displayHolder[0] = W_1;
        displayHolder[1] = W_2;
        myEnc.write(weldTime * ENCODER_DIV);
      }
      UpdateEncoder();
      weldTime = encoderNewPosition / ENCODER_DIV;
      if (weldTime / 10 < 1) displayHolder[2] = BLANK;
      else displayHolder[2] = weldTime / 10;
      displayHolder[3] = weldTime - ((weldTime / 10) * 10);

      if  (switchState[SW_C2] == SW_REL)
      {
        switchState[SW_C2] = SW_UP;
        state = STATE_IDLE;
      }
      break;

    case STATE_WELD_COUNT:
      if (prevoiusState != state) // First time in so set a few things up
      {
        prevoiusState = state;
        Serial.println("Weld Count");
        Serial.println(weldCount);
        displayHolder[0] = C;
        displayHolder[1] = BLANK;
      }
      if (weldCount / 10 < 1) displayHolder[2] = BLANK;
      else displayHolder[2] = weldCount / 10;
      displayHolder[3] = weldCount - ((weldCount / 10) * 10);
      //      if  (switchState[SW_C1]) weldCount = 0;// Need to debounce before we can reset

      if  (switchState[SW_C2] == SW_REL)
      {
        switchState[SW_C2] = SW_UP;
        state = STATE_IDLE;
      }
      break;

    case STATE_ACTIVE:
      if (prevoiusState != state) // First time in so set a few things up
      {
        prevoiusState = state;
        Serial.println("Active");
        displayHolder[4] = 8;
        delay(100);
      }

      if  (switchState[SW_C2] == SW_REL)
      {
        switchState[SW_C2] = SW_UP;
        state = STATE_IDLE;
      }
      if  (switchState[SW_STOP] == SW_REL)
      {
        switchState[SW_STOP] = SW_UP;
        state = STATE_IDLE;
      }
      if  (switchState[SW_START] == SW_REL)
      {
        switchState[SW_START] = SW_UP;
        state = STATE_PREWELD;
      }
      if  (switchState[SW_PIN_FOOT] == SW_REL)
      {
        switchState[SW_PIN_FOOT] = SW_UP;
        state = STATE_PREWELD;
      }
      break;


    case STATE_PREWELD:
      if (prevoiusState != state) // First time in so set a few things up
      {
        Serial.print("Welding.....");
        elapsedTime = 0;
        digitalWrite(SSR_PIN, HIGH);
        tone(BUZZER, TONE_WELD);
        prevoiusState = state;

      }
      if (elapsedTime > preWeldTime)
      {
        digitalWrite(SSR_PIN, LOW);
        noTone(BUZZER);
        state = STATE_DWELL;
      }
      if  (switchState[SW_STOP] == SW_REL)
      {
        switchState[SW_STOP] = SW_UP;
        digitalWrite(SSR_PIN, LOW);
        Serial.println("..... Aborted!");
        state = STATE_IDLE;
        noTone(BUZZER);
      }
      break;

    case STATE_DWELL:
      if (prevoiusState != state) // First time in so set a few things up
      {
        elapsedTime = 0;
        prevoiusState = state;
        //        Serial.println("Dwell");
      }
      if (elapsedTime > dwellTime)state = STATE_WELD;
      if  (switchState[SW_STOP] == SW_REL)
      {
        switchState[SW_STOP] = SW_UP;
        state = STATE_IDLE;
        Serial.println("..... Aborted!");
      }
      break;

    case STATE_WELD:
      if (prevoiusState != state) // First time in so set a few things up
      {
        elapsedTime = 0;
        digitalWrite(SSR_PIN, HIGH);
        tone(BUZZER, TONE_WELD);
        prevoiusState = state;
        //        Serial.println("Weld");
      }
      if (elapsedTime > weldTime)
      {
        digitalWrite(SSR_PIN, LOW);
        Serial.println(".....complete");
        weldCount++;
        noTone(BUZZER);
        state = STATE_ACTIVE;
      }
      if  (switchState[SW_STOP] == SW_REL)
      {
        switchState[SW_STOP] = SW_UP;
        digitalWrite(SSR_PIN, LOW);
        Serial.println("..... Aborted!");
        state = STATE_IDLE;
        noTone(BUZZER);
      }
      break;
  }


}
