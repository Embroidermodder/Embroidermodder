/*
 * EmbroiderBot
 * Copyright 2021-2022 The Embroidermodder Team
 * Distributed under the terms of the zlib license (see LICENSE).
 */

#include <SPI.h>
//#include <SD.h>
#include <LiquidCrystal.h>

#include "embroidery.h"

//TODO: This is a work in progress sketch using libembroidery
//TODO: NOTE: you may need to modify the SD_CS and/or supply your own sample file
#define SD_CS                 5 // Card select

#define WELCOME               0
#define QUESTION              1
#define ANSWER                2

#define UP                    0
#define DOWN                  1
#define LEFT                  2
#define RIGHT                 3
#define SELECT                4
#define NONE                  5

#define MAX_STRING_LENGTH  1024

extern int error_code;

//SPCR States (SPI Control Register)
uint8_t SPI_SDCARD = 0; // Set the SPCR to this before reading

uint32_t stitchCount = 0;

char key = NONE;
const int pin_RS = 8;
const int pin_EN = 9; 
const int pin_d4 = 4; 
const int pin_d5 = 5; 
const int pin_d6 = 6; 
const int pin_d7 = 7;
const int pin_BL = 10;
int score = 0;
int questions_asked = 0;
int option = 0;
int noptions = 4;
int correct = 2;
int select = 0;
int mode = 0;
char question[16];
char options[4][16];
LiquidCrystal lcd( pin_RS,  pin_EN,  pin_d4,  pin_d5,  pin_d6,  pin_d7);

void setup() {
 lcd.begin(16, 2);
}

void gen_question()
{
  /*
  int i;
  int a[4];
  correct = random() % 4;
  for (i=0; i<4; i++) {
    a[i] = 16*(2*(random()%words)+1);
    strncpy(options[i], translation_table+a[i], 16);
  }
  strncpy(question, translation_table+a[correct]-16, 16);
  questions_asked++;
  */
}

const char *message[] = {
   "EmbroiderBot    ",
   "Version 0.1     "
};

void welcome() {
 int i = 0;
 lcd.setCursor(0,0);
 /*
  * scrolling output
  * lcd.print(message+i);
  * lcd.print(message);
  */
 lcd.print(message[i]);
 lcd.setCursor(0,1);
 lcd.print(message[i+1]);
 lcd.print("                ");
 if (key==SELECT) {
   mode = QUESTION;
   gen_question();
 }
}

void question_f() {
  lcd.setCursor(0,0);
  lcd.print(question);
  lcd.setCursor(0,1);
  lcd.print("<");
  lcd.print(options[option]);
  lcd.setCursor(15,1);
  lcd.print(">");
  switch (key) {
  case LEFT:
  case UP:
    option = (option+1)%noptions;
    break;
  case RIGHT:
  case DOWN:
    option = (option+noptions-1)%noptions;
    break;
  case SELECT:
    mode = ANSWER;
    if (option == correct) {
      score++;
    }
    break;
  default:
    break;
  }
}

void answer_f() {
  switch (key) {
  case LEFT:
  case UP:
    option = (option+1)%noptions;
    break;
  case RIGHT:
  case DOWN:
    option = (option+noptions-1)%noptions;
    break;
  case SELECT:
    gen_question();
    mode = QUESTION;
    break;
  default:
    break;
  }
}

void key_handler() {
  int x;
  x = analogRead(0);
  key = NONE;
  if (x < 60) {
    key = RIGHT;
    delay(300);
  }
  else if (x < 200) {
    key = UP;
    delay(300);
  }
  else if (x < 400){
    key = DOWN;
    delay(300);
  }
  else if (x < 600){
    key = LEFT;
    delay(300);
  }
  else if (x < 800){
    key = SELECT;
    delay(300);
  } 
}

void loop() {
  key_handler();
  switch (mode) {
   case WELCOME: welcome(); break;
   case QUESTION: question_f(); break;
   case ANSWER: answer_f(); break;
   default: break;
 }
}

#if 0

void setup(void)
{
    Serial.begin(9600);
    Serial.println(F("libembroidery barebones example"));

    Serial.print(F("Initializing SD card..."));
    if (!SD.begin(SD_CS))
    {
        Serial.println(F("failed!"));
        return;
    }
    Serial.println(F("OK!"));
    SPI_SDCARD = SPCR; //Save the state for later use
}

void loop(void)
{
    loadEmb("sample.exp");
}

void loadEmb(char* fileName)
{
    SPCR = SPI_SDCARD;

    if (!SD.exists(fileName))
    {
        Serial.println("file DOES NOT exist!.");
        return;
    }

    //==============================
    // Reading
    //==============================
    EmbPattern* p = 0;
    int successful = 0;

    p = embPattern_create();
    if (!p) { Serial.println(F("ERROR: Could not allocate memory for embroidery pattern.")); return; }

    successful = embPattern_read(p, fileName);
    if (!successful)
    {
        Serial.print("ERROR: Reading file was unsuccessful: ");
        Serial.println(fileName);
        embPattern_free(p);
        return;
    }

    //==============================
    // Report Design Info
    //==============================
    Serial.print(F("Total Stitches: "));
    Serial.println(stitchCount, DEC); //TODO: For some reason we are missing the last 2 stitches. Investigate embPattern_addStitchAbs() causing a small memory leak.
    stitchCount = 0;

    //==============================
    // Cleanup
    //==============================
    embPattern_free(p);
}

void eventHandler_addStitchAbs(EmbPattern* p, double x, double y, int flags, int isAutoColorIndex)
{
    stitchCount++;
    Serial.print(F("Adding Stitch: "));
    Serial.println(stitchCount, DEC);
}

/**************************************************/
/* EventHandlers - These need to be in the sketch */
/**************************************************/
extern void eventHandler_addStitchAbs(InoPattern*, double, double, int, int);

/**************************************************/
/* EventSenders                                   */
/**************************************************/
void inoEvent_addStitchAbs(InoPattern* p, double x, double y, int flags, int isAutoColorIndex)
{
    eventHandler_addStitchAbs(p, x, y, flags, isAutoColorIndex);
}

struct InoFile_
{
    File file;
};

InoFile* inoFile_open(const char* fileName, const char* mode)
{
    Serial.println(F("inoFile_open"));

    InoFile* iFile;

    iFile = (InoFile*)malloc(sizeof(InoFile));
    if (!iFile)
        return 0;

    iFile->file = SD.open(fileName, FILE_READ); /* ARDUINO TODO: add FILE_WRITE capability */
    if (!iFile->file)
        Serial.println(F("ERROR: SD.open() failed"));

    return iFile;
}

int inoFile_close(InoFile* stream)
{
    Serial.println(F("inoFile_close"));

    stream->file.close();
    free(stream);
    stream = 0;
    return 0;
}

int inoFile_eof(InoFile* stream)
{
    //Serial.println(F("inoFile_eof"));

    int avail = stream->file.available();
    //Serial.print(F("avail: "));
    //Serial.println(avail);
    if (avail)
    {
        return 0;
    }
    Serial.println(F("EOF"));
    return 1;
}

int inoFile_getc(InoFile* stream)
{
    //Serial.println(F("inoFile_getc"));

    return stream->file.read();
}

int inoFile_seek(InoFile* stream, long offset, int origin)
{
    Serial.println(F("TODO: inoFile_seek"));

    return 0;//TODO: fseek(stream->file, offset, origin);
}

long inoFile_tell(InoFile* stream)
{
    Serial.println(F("TODO: inoFile_tell"));

    return 0;//TODO: ftell(stream->file);
}

InoFile* inoFile_tmpfile(void)
{
    Serial.println(F("inoFile_tmpfile"));

    return 0; //TODO: finish inoFile_tmpfile()
}

int inoFile_putc(int ch, InoFile* stream)
{
    Serial.println(F("inoFile_putc"));

    return 0; //TODO: finish inoFile_putc()
}

int inoFile_printf(InoFile* stream, const char* msg)
{
    return stream->file.print(msg);
}

void inoLog_serial(const char* msg)
{
    Serial.println(msg);
}
#endif

const char *welcome_message[] = {
    "EMBROIDER EMBEDDED VERSION 0.1\n\n",
    "    Copyright 2021 The Embroidermodder Team\n",
    "    Licensed under the zlib license.\n",
    "    Please refer to the manual, included in your download,\n",
    "    for help on usage. If you do not have a copy visit:\n",
    "        https://embroidermodder.org\n\n"
};

const float emb_factorial[] = {
  1.0,
  1.0,
  2.0,
  6.0,
  24.0,
  120.0,
  720.0,
  5040.0,
  40320.0,
  362880.0,
  3628800.0
};

int error_code = 0;


/****************************************************************
 * Calculates the intersection points of two overlapping circles.
 * Returns true if the circles intersect.
 * Returns false if the circles do not intersect.
 *
 * (EmbCircle c0, EmbCircle c1, EmbVector* p3, EmbVector* p4)
 * returns int 
 ****************************************************************/
int getCircleCircleIntersections2(EmbCircle c0, EmbCircle c1, EmbVector *p3, EmbVector *p4)
{
  EmbVector diff, p2, m;
  float a, h, d;
  embVector_subtract(c1.center, c0.center, &diff);
  d = embVector_length(diff); /* Distance between centers */

/* Circles share centers. This results in division by zero,
 * infinite solutions or one circle being contained within the other.
 */
  if (d == 0.0) {
    return 0;
  }
  /* Circles do not touch each other */
  else if (d > (c0.radius + c1.radius)) {
    return 0;
  }
  /* One circle is contained within the other */
  else if (d < (c0.radius - c1.radius)) {
    return 0;
  }

  /*
   * Considering the two right triangles p0p2p3 and p1p2p3 we can write:
   * a^2 + h^2 = r0^2 and b^2 + h^2 = r1^2
   *
   * BEGIN PROOF
   *
   * Remove h^2 from the equation by setting them equal to themselves:
   *     r0^2 - a^2 = r1^2 - b^2
   * Substitute b with (d - a) since it is proven that d = a + b:
   *     r0^2 - a^2 = r1^2 - (d - a)^2
   * Complete the square:
   *     r0^2 - a^2 = r1^2 - (d^2 -2da + a^2)
   * Subtract r1^2 from both sides:
   *     r0^2 - r1^2 - a^2 = -(d^2 -2da + a^2)
   * Invert the signs:
   *     r0^2 - r1^2 - a^2 = -d^2 + 2da - a^2
   * Adding a^2 to each side cancels them out:
   *     r0^2 - r1^2 = -d^2 + 2da
   * Add d^2 to both sides to shift it to the other side:
   *     r0^2 - r1^2 + d^2 = 2da
   * Divide by 2d to finally solve for a:
   *     a = (r0^2 - r1^2 + d^2)/ (2d)
   *
   * END PROOF
   */

  a = ((c0.radius * c0.radius) - (c1.radius * c1.radius) + (d * d)) / (2.0 * d);
  /* Solve for h by substituting a into a^2 + h^2 = r0^2 */
  h = sqrt((c0.radius * c0.radius) - (a * a));

  /*Find point p2 by adding the a offset in relation to line d to point p0 */

  embVector_multiply(diff, a / d, &p2);
  embVector_add(c0.center, p2, &p2);

  /* Tangent circles have only one intersection
   *
   * TODO: using == in floating point arithmetic
   * doesn't account for the machine accuracy, having
   * a stated (float) tolerance value would help.
   */
  if (d == (c0.radius + c1.radius)) {
    *p3 = *p4 = p2;
    return 1;
  }

  /* Get the perpendicular slope by multiplying by the negative reciprocal
   * Then multiply by the h offset in relation to d to get the actual offsets 
   */
  m.x = -(diff.y * h / d);
  m.y = (diff.x * h / d);

  /* Add the offsets to point p2 to obtain the intersection points */
  embVector_add(p2, m, p3);
  embVector_subtract(p2, m, p4);

  return 1;
}
