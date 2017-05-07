//TODO:
// 1. segmenty do literek
// 2. moze jakies posprzatanie kodu, nie wiem czy to bedzie czytelne dla Was :D
// 3. zmiana jasnosci i predkosci dziala spoczko, ale zmiana tekstu dopiero za drugim razem,
//    chyba ta funkcja czytaja jest odpalana ze zlego miejsca ale juz nie chce mi sie nad tym kminic dzisiaj
// 4. have fun
// 6. obsluga bledow, moze jakies wyswietlanie ERROR wtedy?
// 7. optymalizacja :3 OSZCZEDZAJ RAM GDZIEKOLWIEK JESTES
// 8. nie no zart na razie
// 9. Wisła Płock
//10. nie ma punktu 5.





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

  Serial.begin(9600);
}
//----------------------------------------------------------------

void loop()
{
  doTheReading();
  myDisplay(BRIGHT, &DISP, TEXT);
}

void doTheReading()
{
  int incomingByte = 0;
  if (Serial.available() > 0)
  {
    incomingByte = Serial.read();
    Serial.print("available chars left: ");
    Serial.println(Serial.available());

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
      // if (Serial.available() == 6)
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
  char c;
  for (int i = 0; i < 6; i++)
  {
    buff[i] = Serial.read();
  }

  for (int i = 0; i < 6; i++)
  {
    c = buff[i];
    if (!((c >= '0' && c <= '9') || c == 'A' || c == 'b' || c == 'C' || c == 'd' || c == 'E' || c == 'F')) {
      return;
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
    doTheReading();
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
//----------------------------------------------------------------

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
//----------------------------------------------------------------

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