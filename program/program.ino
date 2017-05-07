/*This code is public domain but you buy me a beer if you use this and we meet
  someday (Beerware license).
  4 digit 7 segment display:
  http://www.sparkfun.com/products/9483
  Datasheet:
  http://www.sparkfun.com/datasheets/Components/LED/7-Segment/YSD-439AR6B-35.pdf
  This is an example of how to drive a 7 segment LED display from an ATmega
  without the use of current limiting resistors. This technique is very common
  but requires some knowledge of electronics - you do run the risk of dumping
  too much current through the segments and burning out parts of the display.
  If you use the stock code you should be ok, but be careful editing the
  brightness values.
  This code should work with all colors (red, blue, yellow, green) but the
  brightness will vary from one color to the next because the forward voltage
  drop of each color is different. This code was written and calibrated for the
  red color.
  This code will work with most Arduinos but you may want to re-route some of
  the pins.
  7 segments
  4 digits
  1 colon
  =
  12 pins required for full control
*/

int digit1 = 11; //PWM Display pin 1
int digit2 = 10; //PWM Display pin 2
int digit3 = 9;  //PWM Display pin 6
int digit4 = 6;  //PWM Display pin 8

//Pin mapping from Arduino to the ATmega DIP28 if you need it
//http://www.arduino.cc/en/Hacking/PinMapping
int segA = 2;
int segB = 3;
int segC = 4;
int segD = 5;
int segE = A0; //pin 6 is used bij display 1 for its pwm function
int segF = 7;
int segG = 8;

void setup()
{
  pinMode(segA, OUTPUT);
  pinMode(segB, OUTPUT);
  pinMode(segC, OUTPUT);
  pinMode(segD, OUTPUT);
  pinMode(segE, OUTPUT);
  pinMode(segF, OUTPUT);
  pinMode(segG, OUTPUT);

  pinMode(digit1, OUTPUT);
  pinMode(digit2, OUTPUT);
  pinMode(digit3, OUTPUT);
  pinMode(digit4, OUTPUT);
  Serial.begin(9600);

  pinMode(13, OUTPUT);
}
int incomingByte = 0; // for incoming serial data

int BRIGHTNESS[] = {200, 500, 1000, 2000};

int BRIGHT[4] = {0, 1, 2, 3};
int DISP = 500;
char TEXT[7] = "123456";
int buffer[4];
void loop()
{

  doTheReading();

  myDisplay(BRIGHT, &DISP, TEXT);
}

void doTheReading()
{
  if (Serial.available() > 0)
  {
    // read the incoming byte:
    incomingByte = Serial.read();

    switch (incomingByte)
    {
    //B
    case 66:
      if (Serial.available() == 4)
        readBytes();
      changeBrightness();
      break;
    //D
    case 68:
      if (Serial.available() == 4)
        readBytes();
      changeSpeed();
      break;
    //T
    case 84:
      if (Serial.available() == 6)
      {
        readAndChangeText();
      }
      break;
    }
  }
}

void readAndChangeText()
{
  char buff[7];
  char c;
  for (int i = 0; i < 6; i++)
  {
    c = Serial.read();
    if ((c >= '0'  && c <= '9') || c == 'A' || c == 'b' || c == 'C' || c == 'd' || c == 'E' || c == 'F')
    {
      buff[i] = c;
    }
  }
  buff[6] = '\0';
  Serial.println("Data in text buffer:");
  Serial.println(buff);
  strcpy(TEXT, buff);
}
void changeSpeed()
{
  int newSpeed = 0;
  int divider = 1000;
  Serial.print("Speed changed to: ");

  for (int i = 0; i < 4; i++)
  {
    if (buffer[i] >= 0 && buffer[i] <= 9)
    {
      newSpeed += (buffer[i]) * divider;
      divider /= 10;
    }
    else
    {
      //...error, zle parametry, zrob cos, achtuuung!
      DISP = 500;
      break;
    }
  }
  DISP = newSpeed;
  Serial.println(DISP);
}
void readBytes()
{
  Serial.print("In buffer: ");
  for (int i = 0; i < 4; i++)
  {
    buffer[i] = Serial.read() - '0';
    Serial.print(buffer[i]);
  }
}

void changeBrightness()
{
  Serial.print("Bright changed to: ");
  for (int i = 0; i < 4; i++)
  {
    if (buffer[i] >= 0 && buffer[i] <= 3)
    {
      BRIGHT[i] = buffer[i];
      Serial.print(BRIGHT[i]);
      Serial.print(" ");
    }
    else
    {
      //...error, zle parametry, zrob cos, achtuuung!
      BRIGHT[i] = 3;
    }
  }
  Serial.println("");
}

void myDisplay(int *brightness, int *display_time, char *text)
{
  long startTime;
  char *txt = text;
  int length = strlen(txt);

  char substr[5] = "    ";
  char spaces[4] = "   ";
  int j = 0;
  int EMPTY = 3;
  int nchars = 1;
  for (int i = length + 3; i >= 0; i--)
  {
    if (EMPTY > 0)
    {
      strncpy(substr, spaces, EMPTY);
      strncpy(substr + EMPTY--, txt, nchars++);
    }
    else
    {
      strncpy(substr, txt + j, 4);
      j++;
    }

    startTime = millis();
    while ((millis() - startTime) / 1 <= *display_time)
    {
      doTheReading();
      displayCharOnDigit(substr, brightness);
    }
  }
}

void displayCharOnDigit(char *toDisplay, int *bright)
{
#define DISPLAY_BRIGHTNESS 1000

#define DIGIT_ON LOW
#define DIGIT_OFF HIGH

  // int brightness[] = {200, 500, 1000, 2000};
  long beginTime = millis();
  int i = 0;
  for (int digit = 1; digit <= 4; digit++)
  {

    //Turn on a digit for a short amount of time
    switch (digit)
    {
    case 1:
      digitalWrite(digit1, DIGIT_ON);
      break;
    case 2:
      digitalWrite(digit2, DIGIT_ON);
      break;
    case 3:
      digitalWrite(digit3, DIGIT_ON);
      break;
    case 4:
      digitalWrite(digit4, DIGIT_ON);
      break;
    }

    //Turn on the right segments for this digit
    // if (digit == onDigit)
    lightNumber(toDisplay[i] - '0');
    // toDisplay /= 10;

    // delayMicroseconds(DISPLAY_BRIGHTNESS);
    delayMicroseconds(BRIGHTNESS[(bright[i])]);
    i++;

    //Display digit for fraction of a second (1us to 5000us, 500 is pretty good)

    //Turn off all segments
    lightNumber(10);

    //Turn off all digits
    digitalWrite(digit1, DIGIT_OFF);
    digitalWrite(digit2, DIGIT_OFF);
    digitalWrite(digit3, DIGIT_OFF);
    digitalWrite(digit4, DIGIT_OFF);
  }

  while ((millis() - beginTime) < 10)
    ;
  //Wait for 20ms to pass before we paint the display again
}

//Given a number, we display 10:22
//After running through the 4 numbers, the display is left turned off

//Display brightness
//Each digit is on for a certain amount of microseconds
//Then it is off until we have reached a total of 20ms for the function call
//Let's assume each digit is on for 1000us
//Each digit is on for 1ms, there are 4 digits, so the display is off for 16ms.
//That's a ratio of 1ms to 16ms or 6.25% on time (PWM).
//Let's define a variable called brightness that varies from:
//5000 blindingly bright (15.7mA current draw per digit)
//2000 shockingly bright (11.4mA current draw per digit)
//1000 pretty bright (5.9mA)
//500 normal (3mA)
//200 dim but readable (1.4mA)
//50 dim but readable (0.56mA)
//5 dim but readable (0.31mA)
//1 dim but readable in dark (0.28mA)

void displayNumber(int toDisplay)
{
#define DISPLAY_BRIGHTNESS 1000

#define DIGIT_ON LOW
#define DIGIT_OFF HIGH

  long beginTime = millis();

  for (int digit = 4; digit > 0; digit--)
  {

    //Turn on a digit for a short amount of time
    switch (digit)
    {
    case 1:
      digitalWrite(digit1, DIGIT_ON);
      break;
    case 2:
      digitalWrite(digit2, DIGIT_ON);
      break;
    case 3:
      digitalWrite(digit3, DIGIT_ON);
      break;
    case 4:
      digitalWrite(digit4, DIGIT_ON);
      break;
    }

    //Turn on the right segments for this digit
    lightNumber(toDisplay % 10);
    toDisplay /= 10;

    delayMicroseconds(DISPLAY_BRIGHTNESS);
    //Display digit for fraction of a second (1us to 5000us, 500 is pretty good)

    //Turn off all segments
    lightNumber(10);

    //Turn off all digits
    digitalWrite(digit1, DIGIT_OFF);
    digitalWrite(digit2, DIGIT_OFF);
    digitalWrite(digit3, DIGIT_OFF);
    digitalWrite(digit4, DIGIT_OFF);
  }

  while ((millis() - beginTime) < 10)
    ;
  //Wait for 20ms to pass before we paint the display again
}

//Given a number, turns on those segments
//If number == 10, then turn off number
void lightNumber(int numberToDisplay)
{

#define SEGMENT_ON LOW
#define SEGMENT_OFF HIGH

  switch (numberToDisplay)
  {

  case 0:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_OFF);
    break;

  case 1:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    break;

  case 2:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 3:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 4:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 5:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 6:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 7:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    break;

  case 8:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 9:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 10:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    break;
  }
}