//TODO:
// 2. moze jakies posprzatanie kodu, nie wiem czy to bedzie czytelne dla Was :D
// 6. obsluga bledow, moze jakies wyswietlanie ERROR wtedy? Obcinanie przy za duzej liczbe znakow? Dunno
// 7. optymalizacja :3 OSZCZEDZAJ RAM GDZIEKOLWIEK JESTES

// PINS CONNECTION
int digit1 = 11; //PWM Display pin 1
int digit2 = 10; //PWM Display pin 2
int digit3 = 9;  //PWM Display pin 6
int digit4 = 6;  //PWM Display pin 8

int segA = 2;
int segB = 3;
int segC = 4;
int segD = 5;
int segE = A0; //pin 6 is used bij display 1 for its pwm function
int segF = 7;
int segG = 8;
//----------------------------------------------------------------

//CROSS-FUNCTION VARIABLES
int BRIGHTNESS[] = {200, 500, 1000, 2000};

char TEXT[7] = "123456";
int BRIGHT[4] = {0, 1, 2, 3};
int DISP = 500;

int buffer[4];
//----------------------------------------------------------------

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
  pinMode(13, OUTPUT);

  Serial.begin(115200);
}
//----------------------------------------------------------------

void loop()
{
  checkSerialInput();
  runDisplay(BRIGHT, &DISP, TEXT);
}

void checkSerialInput()
{

  int incomingByte = 0;
  if (Serial.available() > 0)
  {
    incomingByte = Serial.read();
    switch (incomingByte)
    {
    //B
    case 66:
      if (Serial.available() == 4)
      {
        readBytes();
        changeBrightness();
      }
      else
      {
        Serial.println("ERROR: Pass exactly 4 digits in range 0-3");
      }
      break;
    //D
    case 68:
      if (Serial.available() == 4)
      {
        readBytes();
        changeSpeed();
      }
      else
      {
        Serial.println("ERROR: Pass exactly 4 digits in range 0-9");
      }
      break;
    //T
    case 84:
    {
      readAndChangeText();
    }
    break;
    }
  }
}
//----------------------------------------------------------------

void readAndChangeText()
{
  Serial.println("reading new text...");
  char buff[7];
  for (int i = 0; i < 6; i++)
  {
    buff[i] = Serial.read();
    if (!((buff[i] >= '0' && buff[i] <= '9') || buff[i] == 'A' || buff[i] == 'b' || buff[i] == 'C' || buff[i] == 'd' || buff[i] == 'E' || buff[i] == 'F'))
    {
      Serial.print("Incorrect char: ");
      Serial.println(buff[i]);
      return;
    }
  }

  buff[6] = '\0';
  strcpy(TEXT, buff);
}

void changeSpeed()
{
  int newSpeed = 0;
  int divider = 1000;

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
      Serial.println("ERROR: podaj tylko cyfry!");
      DISP = 500;
      return;
    }
  }
  DISP = newSpeed;
}
//----------------------------------------------------------------

void readBytes()
{
  Serial.print("In buffer: ");
  for (int i = 0; i < 4; i++)
  {
    buffer[i] = Serial.read() - '0';
    Serial.print(buffer[i]);
  }
}
//----------------------------------------------------------------

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
//----------------------------------------------------------------

void runDisplay(int *brightness, int *display_time, char *text)
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
    checkSerialInput();
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
      checkSerialInput();
      displayCharOnDigit(substr, brightness);
    }
  }
}
//----------------------------------------------------------------

void displayCharOnDigit(char *toDisplay, int *bright)
{
#define DISPLAY_BRIGHTNESS 1000

#define DIGIT_ON LOW
#define DIGIT_OFF HIGH

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
    lightSegment(toDisplay[i]);

    delayMicroseconds(BRIGHTNESS[(bright[i])]);
    i++;

    //Display digit for fraction of a second (1us to 5000us, 500 is pretty good)

    //Turn off all segments
    lightSegment('x');

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
//----------------------------------------------------------------

//Given a number, turns on those segments
//If number == x, then turn off number

void lightSegment(char characterToDisplay)
{

#define SEGMENT_ON LOW
#define SEGMENT_OFF HIGH

  switch (characterToDisplay)
  {

  case '0':
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_OFF);
    break;

  case '1':
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    break;

  case '2':
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case '3':
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case '4':
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case '5':
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case '6':
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case '7':
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    break;

  case '8':
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case '9':
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 'A':
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;
  case 'b':
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;
  case 'C':
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_OFF);
    break;
  case 'd':
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_ON);
    break;
  case 'E':
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;
  case 'F':
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 'x':
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