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

#define DIGIT0  18 //1 Active High
#define DIGIT1  19 //2
#define DIGIT2  14
#define DIGIT3  6
#define DIGIT4  4
#define NUM_DIGITS 5

#define SEGMENTA    7 // 14 Active Low
#define SEGMENTB    8// 13    
#define SEGMENTC    9// 12  ALSO power switch
#define SEGMENTD    10// 11
#define SEGMENTE    11// 9
#define SEGMENTF    16// 7  ALSO CLOCK SWITCH
#define SEGMENTG    12// 8
#define SEGMENT1    // 10
#define SEGMENT2    // 

#define SW_PWR_CLK          15
#define SW_START_C1         SEGMENTF//16
#define SW_GRL_WEI          SEGMENTG//12
#define SW_STOP_C2          SEGMENTE//11
#define SW_PWR_STOP_C1_GRL  SEGMENTB//8
#define SW_START_C2_WEI_CLK SEGMENTF//7



#define SWITCH_POWER1 9
#define SWITCH_POWER2 15

#define ENCODERA  2 // Use ext interupts
#define ENCODERB  3 // Use ext interupts

#define BUZZER 5

#define FRED 1


#define STATE_IDLE    0
#define STATE_PREWELD 2
#define STATE_WELD    3

#define BLANK 10
#define COLON 1

unsigned int period = 0;
bool updateDisplay = 0;
unsigned int displayHolder[NUM_DIGITS] = {BLANK, BLANK, BLANK, BLANK, BLANK};
//                      zero      one       two         three   four      five      six         seven   eight     nine        blank
const int numeral[] = {B1000000, B1111001, B0100100, B0110000, B0011001, B0010010, B0000010, B1111000, B0000000, B0010000, B1111111};//0b011000000, 0b110110010};
int currentDigit = 1;

unsigned int state = STATE_IDLE;

unsigned int preWeldTime = 0;
unsigned int weldTime = 50;

long oldPosition  = -999;

unsigned int powerSwitchState = 0;

  long newPosition = 0;

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
  // initialize digital pin LED_BUILTIN as an output.
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

  pinMode(SWITCH_POWER2, INPUT);
  
  //  pinMode(SEGMENT1, OUTPUT);
  //  pinMode(SEGMENT2, OUTPUT);

  //  digitalWrite(DIGIT1, HIGH);

  // Timer0 is already used for millis() - we'll just interrupt somewhere
  // in the middle and call the "Compare A" function below
  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);

  pinMode(SWITCH_POWER2, INPUT_PULLUP);
  attachInterrupt(0, SwitchPressed, FALLING);
}

void SwitchPressed()
{
  preWeldTime++;
}

// Interrupt is called once a millisecond, looks for any new GPS data, and stores it
SIGNAL(TIMER0_COMPA_vect)
{
  if (period++ > FRED)
  {
    period = 0;
    enableDigit(currentDigit);
    writeNumeral(displayHolder[currentDigit]);
    if (currentDigit++ >= NUM_DIGITS)
    {
      digitalWrite(SW_PWR_CLK,HIGH);  
      digitalWrite(SW_START_C1,LOW); 
      digitalWrite(SW_GRL_WEI,LOW);  
      digitalWrite(SW_STOP_C2,LOW);  

      pinMode(SW_PWR_STOP_C1_GRL,INPUT_PULLUP);
      pinMode(SW_START_C2_WEI_CLK,OUTPUT);
      digitalWrite(SW_START_C2_WEI_CLK,HIGH);

      powerSwitchState = digitalRead(SW_PWR_STOP_C1_GRL);
      pinMode(SW_PWR_STOP_C1_GRL,OUTPUT);
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
  newPosition = myEnc.read();
  if (newPosition != oldPosition)
  {
    if (newPosition > oldPosition) tone(BUZZER, 1200, 80);
    else tone(BUZZER, 600, 80);
    if (newPosition > 399)
    {
      myEnc.write(399);
      newPosition = 399;
    }
    if (newPosition < 0)
    {
      myEnc.write(0);
      newPosition = 0;
    }
    oldPosition = newPosition;
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
      preWeldTime = newPosition / 4;
      if (preWeldTime / 10 < 1) displayHolder[0] = BLANK;
      else displayHolder[0] = preWeldTime / 10;
      displayHolder[1] = preWeldTime - ((preWeldTime / 10) * 10);
      //if (preWeldTime >22) preWeldTime = 0;

      if (weldTime / 10 < 1) displayHolder[2] = BLANK;
      else displayHolder[2] = weldTime / 10;
      displayHolder[3] = weldTime - ((weldTime / 10) * 10);
      if (weldTime++ > 99) weldTime = 0;

      Serial.println(powerSwitchState);

      //delay(200);
      break;

    case STATE_PREWELD:
      Serial.println("Preweld");
      break;

    case STATE_WELD:
      Serial.println("Weld");
      break;
  }


}
