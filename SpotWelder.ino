/*

*/

#define DIGIT1  2 //1 Active High
#define DIGIT2  3 //2
#define DIGIT3  4
#define DIGIT4  5
#define DIGIT5  6

#define SEGMENTA    7 // 14 Active Low
#define SEGMENTB    8// 13
#define SEGMENTC    9// 12
#define SEGMENTD    10// 11
#define SEGMENTE    11// 9
#define SEGMENTF    12// 7
#define SEGMENTG    13// 8
#define SEGMENT1    // 10
#define SEGMENT2    // 6

const int numeral[] = {B1000000, B1111001, B0100100, B0110000, B0011001, B0010010, B0000010, B1111000, B0000000, B0010000};//0b011000000, 0b110110010};



// the setup function runs once when you press reset or power the board
void setup()
{
  Serial.begin(9600);
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(DIGIT1, OUTPUT);
  pinMode(DIGIT2, OUTPUT);
  pinMode(DIGIT3, OUTPUT);
  pinMode(DIGIT4, OUTPUT);
  pinMode(DIGIT5, OUTPUT);


  pinMode(SEGMENTA, OUTPUT);
    pinMode(SEGMENTB, OUTPUT);
    pinMode(SEGMENTC, OUTPUT);
    pinMode(SEGMENTD, OUTPUT);
    pinMode(SEGMENTE, OUTPUT);
    pinMode(SEGMENTF, OUTPUT);
    pinMode(SEGMENTG, OUTPUT);
  //  pinMode(SEGMENT1, OUTPUT);
  //  pinMode(SEGMENT2, OUTPUT);

//  digitalWrite(DIGIT1, HIGH);
}
#define fred 500
// the loop function runs over and over again forever
void loop()
{

digitalWrite(DIGIT1, HIGH);
digitalWrite(DIGIT2, LOW);
digitalWrite(DIGIT3, LOW);
digitalWrite(DIGIT4, LOW);
digitalWrite(DIGIT5, LOW);
for (int num = 0; num<=9;num++)
{
  //int num = 0;
  int seg = 0;
  digitalWrite(SEGMENTA, numeral[num] & 1 << seg++);

  digitalWrite(SEGMENTB, numeral[num] & 1 << seg++);
  Serial.println(numeral[num] & 1 << seg);
  digitalWrite(SEGMENTC, numeral[num] & 1 << seg++);
  digitalWrite(SEGMENTD, numeral[num] & 1 << seg++);
  digitalWrite(SEGMENTE, numeral[num] & 1 << seg++);
  digitalWrite(SEGMENTF, numeral[num] & 1 << seg++);
  digitalWrite(SEGMENTG, numeral[num] & 1 << seg++);

  delay(fred);

}
digitalWrite(DIGIT1, LOW);
digitalWrite(DIGIT2, HIGH);
digitalWrite(DIGIT3, LOW);
digitalWrite(DIGIT4, LOW);
digitalWrite(DIGIT5, LOW);
for (int num = 0; num<=9;num++)
{
  //int num = 0;
  int seg = 0;
  digitalWrite(SEGMENTA, numeral[num] & 1 << seg++);

  digitalWrite(SEGMENTB, numeral[num] & 1 << seg++);
  Serial.println(numeral[num] & 1 << seg);
  digitalWrite(SEGMENTC, numeral[num] & 1 << seg++);
  digitalWrite(SEGMENTD, numeral[num] & 1 << seg++);
  digitalWrite(SEGMENTE, numeral[num] & 1 << seg++);
  digitalWrite(SEGMENTF, numeral[num] & 1 << seg++);
  digitalWrite(SEGMENTG, numeral[num] & 1 << seg++);

  delay(fred);

}digitalWrite(DIGIT1, LOW);
digitalWrite(DIGIT2, LOW);
digitalWrite(DIGIT3, HIGH);
digitalWrite(DIGIT4, LOW);
digitalWrite(DIGIT5, LOW);
for (int num = 0; num<=9;num++)
{
  //int num = 0;
  int seg = 0;
  digitalWrite(SEGMENTA, numeral[num] & 1 << seg++);

  digitalWrite(SEGMENTB, numeral[num] & 1 << seg++);
  Serial.println(numeral[num] & 1 << seg);
  digitalWrite(SEGMENTC, numeral[num] & 1 << seg++);
  digitalWrite(SEGMENTD, numeral[num] & 1 << seg++);
  digitalWrite(SEGMENTE, numeral[num] & 1 << seg++);
  digitalWrite(SEGMENTF, numeral[num] & 1 << seg++);
  digitalWrite(SEGMENTG, numeral[num] & 1 << seg++);

  delay(fred);

}digitalWrite(DIGIT1, LOW);
digitalWrite(DIGIT2, LOW);
digitalWrite(DIGIT3, LOW);
digitalWrite(DIGIT4, HIGH);
digitalWrite(DIGIT5, LOW);
for (int num = 0; num<=9;num++)
{
  //int num = 0;
  int seg = 0;
  digitalWrite(SEGMENTA, numeral[num] & 1 << seg++);

  digitalWrite(SEGMENTB, numeral[num] & 1 << seg++);
  Serial.println(numeral[num] & 1 << seg);
  digitalWrite(SEGMENTC, numeral[num] & 1 << seg++);
  digitalWrite(SEGMENTD, numeral[num] & 1 << seg++);
  digitalWrite(SEGMENTE, numeral[num] & 1 << seg++);
  digitalWrite(SEGMENTF, numeral[num] & 1 << seg++);
  digitalWrite(SEGMENTG, numeral[num] & 1 << seg++);

  delay(fred);

}digitalWrite(DIGIT1, LOW);
digitalWrite(DIGIT2, LOW);
digitalWrite(DIGIT3, LOW);
digitalWrite(DIGIT4, LOW);
digitalWrite(DIGIT5, HIGH);
for (int num = 0; num<=9;num++)
{
  //int num = 0;
  int seg = 0;
  digitalWrite(SEGMENTA, numeral[num] & 1 << seg++);

  digitalWrite(SEGMENTB, numeral[num] & 1 << seg++);
  Serial.println(numeral[num] & 1 << seg);
  digitalWrite(SEGMENTC, numeral[num] & 1 << seg++);
  digitalWrite(SEGMENTD, numeral[num] & 1 << seg++);
  digitalWrite(SEGMENTE, numeral[num] & 1 << seg++);
  digitalWrite(SEGMENTF, numeral[num] & 1 << seg++);
  digitalWrite(SEGMENTG, numeral[num] & 1 << seg++);

  delay(fred);

}
  //  digitalWrite(DIGIT1, HIGH);   // turn the LED on (HIGH is the voltage level)
  //  digitalWrite(DIGIT2, LOW);   // turn the LED on (HIGH is the voltage level)
  //  digitalWrite(DIGIT3, LOW);   // turn the LED on (HIGH is the voltage level)
  //  digitalWrite(DIGIT4, LOW);   // turn the LED on (HIGH is the voltage level)
  //     digitalWrite(DIGIT5, LOW);   // turn the LED on (HIGH is the voltage level)
  //  //digitalWrite(SEGMENTA,LOW);
  //  delay(fred);
  //  digitalWrite(DIGIT1, LOW);   // turn the LED on (HIGH is the voltage level)
  //  digitalWrite(DIGIT2, HIGH);   // turn the LED on (HIGH is the voltage level)
  //  digitalWrite(DIGIT3, LOW);   // turn the LED on (HIGH is the voltage level)
  //  digitalWrite(DIGIT4, LOW);   // turn the LED on (HIGH is the voltage level)
  //     digitalWrite(DIGIT5, LOW);   // turn the LED on (HIGH is the voltage level)
  //  //digitalWrite(SEGMENTA,LOW);
  //  delay(fred);
  //  digitalWrite(DIGIT1, LOW);   // turn the LED on (HIGH is the voltage level)
  //  digitalWrite(DIGIT2, LOW);   // turn the LED on (HIGH is the voltage level)
  //  digitalWrite(DIGIT3, HIGH);   // turn the LED on (HIGH is the voltage level)
  //  digitalWrite(DIGIT4, LOW);   // turn the LED on (HIGH is the voltage level)
  //     digitalWrite(DIGIT5, LOW);   // turn the LED on (HIGH is the voltage level)
  //  //digitalWrite(SEGMENTA,LOW);
  //  delay(fred);
  //  digitalWrite(DIGIT1, LOW);   // turn the LED on (HIGH is the voltage level)
  //  digitalWrite(DIGIT2, LOW);   // turn the LED on (HIGH is the voltage level)
  //  digitalWrite(DIGIT3, LOW);   // turn the LED on (HIGH is the voltage level)
  //  digitalWrite(DIGIT4, HIGH);   // turn the LED on (HIGH is the voltage level)
  //    digitalWrite(DIGIT5, LOW);   // turn the LED on (HIGH is the voltage level)
  //  //digitalWrite(SEGMENTA,LOW);
  //  delay(fred);
  //    digitalWrite(DIGIT1, LOW);   // turn the LED on (HIGH is the voltage level)
  //  digitalWrite(DIGIT2, LOW);   // turn the LED on (HIGH is the voltage level)
  //  digitalWrite(DIGIT3, LOW);   // turn the LED on (HIGH is the voltage level)
  //  digitalWrite(DIGIT4, LOW);   // turn the LED on (HIGH is the voltage level)
  //  digitalWrite(DIGIT5, HIGH);   // turn the LED on (HIGH is the voltage level)
  //  //digitalWrite(SEGMENTA,LOW);
  //  delay(fred);


  //  digitalWrite(DIGIT1, LOW);   // turn the LED on (HIGH is the voltage level)
  //  digitalWrite(SEGMENTA,LOW);
  //  delay(fred);





  // wait for a second
  //digitalWrite(DIGIT1, LOW);    // turn the LED off by making the voltage LOW
  //digitalWrite(SEGMENTA,HIGH);
  //delay(fred + fred + fred);                       // wait for a second
}
