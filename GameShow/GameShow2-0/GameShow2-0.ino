#include <Wire.h>

#include <SparkFunSX1509.h>

// SX1509 I2C address (set by ADDR1 and ADDR0 (00 by default):
const byte SX1509_ADDRESS = 0x3E;  // SX1509 I2C address
SX1509 io; // Create an SX1509 object to be used throughout

// SX1509 pin definitions:
const byte SX1509_LED_PIN = 15; // LED connected to pin 15

// Define the active LED pins (these are on the SX1509)
const byte BEAVER_ACTIVE_LED_PIN = 0;
const byte BOBWHITE_ACTIVE_LED_PIN = 1;
const byte EAGLE_ACTIVE_LED_PIN = 2;
const byte FOX_ACTIVE_LED_PIN = 3;
const byte OWL_ACTIVE_LED_PIN = 4;
const byte BEAR_ACTIVE_LED_PIN = 5;
const byte BUFFALO_ACTIVE_LED_PIN = 6;
const byte ANTELOPE_ACTIVE_LED_PIN = 7;

// Define the Wrong LED pins (these are on the SX1509)
const byte BEAVER_WRONG_LED_PIN = 15;
const byte BOBWHITE_WRONG_LED_PIN = 14;
const byte EAGLE_WRONG_LED_PIN = 13;
const byte FOX_WRONG_LED_PIN = 12;
const byte OWL_WRONG_LED_PIN = 11;
const byte BEAR_WRONG_LED_PIN = 10;
const byte BUFFALO_WRONG_LED_PIN = 9;
const byte ANTELOPE_WRONG_LED_PIN = 8;

// Define the pins for the buttons (these are on the arduino)
const byte BEAVER_BUTTON_PIN = 3;
const byte BOBWHITE_BUTTON_PIN = 4;
const byte EAGLE_BUTTON_PIN = 5;
const byte FOX_BUTTON_PIN = 6;
const byte OWL_BUTTON_PIN = 7;
const byte BEAR_BUTTON_PIN = 8;
const byte BUFFALO_BUTTON_PIN = 9;
const byte ANTELOPE_BUTTON_PIN = 10;

const byte RESET_BUTTON_PIN = A0;
const byte WRONG_BUTTON_PIN = A1;
//const byte WRONG_BUTTON_PIN = 7;

const byte BEAVER_MASK = 0x01;
const byte BOBWHITE_MASK = 0x02;
const byte EAGLE_MASK = 0x04;
const byte FOX_MASK = 0x08;
const byte OWL_MASK = 0x10;
const byte BEAR_MASK = 0x20;
const byte BUFFALO_MASK = 0x40;
const byte ANTELOPE_MASK = 0x80;

const byte ACTIVE_LED_ARRAY[] =   {BEAVER_ACTIVE_LED_PIN, 
                                  BOBWHITE_ACTIVE_LED_PIN, 
                                  EAGLE_ACTIVE_LED_PIN, 
                                  FOX_ACTIVE_LED_PIN,
                                  OWL_ACTIVE_LED_PIN, 
                                  BEAR_ACTIVE_LED_PIN, 
                                  BUFFALO_ACTIVE_LED_PIN, 
                                  ANTELOPE_ACTIVE_LED_PIN}; 
const byte WRONG_LED_ARRAY[] =  {BEAVER_WRONG_LED_PIN, 
                                BOBWHITE_WRONG_LED_PIN, 
                                EAGLE_WRONG_LED_PIN, 
                                FOX_WRONG_LED_PIN, 
                                OWL_WRONG_LED_PIN, 
                                BEAR_WRONG_LED_PIN, 
                                BUFFALO_WRONG_LED_PIN, 
                                ANTELOPE_WRONG_LED_PIN}; 

const byte BEAVER_INDEX = 0;
const byte BOBWHITE_INDEX = 1;
const byte EAGLE_INDEX = 2;
const byte FOX_INDEX = 3;
const byte OWL_INDEX = 4;
const byte BEAR_INDEX = 5;
const byte BUFFALO_INDEX = 6;
const byte ANTELOPE_INDEX = 7;

int speakerPin = 11;

byte current_active, current_wrong;
byte wrong_state = LOW;  // The Wrong button is stated, so we need to remember the state of the wrong switch so we can detect each press

void set_active_led(const byte index, byte level) {
  Serial.print("Write pin ");
  Serial.print(ACTIVE_LED_ARRAY[index], DEC);
  io.digitalWrite(ACTIVE_LED_ARRAY[index], level);
  if (level == LOW) {
    digitalWrite(speakerPin, HIGH);
    delay(900);
    digitalWrite(speakerPin, LOW);
  }
}

void set_wrong_led(const byte index, byte level) {
  Serial.print("Write pin ");
  Serial.print(WRONG_LED_ARRAY[index], DEC);
  io.digitalWrite(WRONG_LED_ARRAY[index], level);
}

byte get_active_index() {
  byte temp = current_active, i;
  for (i = 0; i < 8; i++) {
    if (temp & 0x01) {
      return(i);
    }
    else {
      temp = temp >> 1;
    }
  }
  return(0xff);
}



void play( char note, int beats)
{
  int numNotes = 14;  // number of notes in our note and frequency array (there are 15 values, but arrays start at 0)
  
  //Note: these notes are C major (there are no sharps or flats)

  //this array is used to look up the notes
  char notes[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C', 'D', 'E', 'F', 'G', 'A', 'B', ' '};
  //this array matches frequencies with each letter (e.g. the 4th note is 'f', the 4th frequency is 175)
  int frequencies[] = {660, 147, 165, 175, 196, 220, 247, 262, 294, 330, 349, 392, 440, 494, 0};
  
  int currentFrequency = 0;    //the frequency that we find when we look up a frequency in the arrays
  int beatLength = 50;   //the length of one beat (changing this will speed up or slow down the tempo of the song)

  //look up the frequency that corresponds to the note
  for (int i = 0; i < numNotes; i++)  // check each value in notes from 0 to 14
  {
    if (notes[i] == note)             // does the letter passed to the play function match the letter in the array?
    {
      currentFrequency = frequencies[i];   // Yes! Set the current frequency to match that note
    }
  }

  //play the frequency that matched our letter for the number of beats passed to the play function
  tone(speakerPin, currentFrequency, beats * beatLength);   
  delay(beats* beatLength);   //wait for the length of the tone so that it has time to play
  delay(50);                  //a little delay between the notes makes the song sound more natural

}

/* CHART OF FREQUENCIES FOR NOTES IN C MAJOR
Note      Frequency (Hz)
c        131
d        147
e        165
f        175
g        196
a        220
b        247
C        262
D        294
E        330
F        349
G        392
A        440
B        494
*/

void setup() {

  byte i;
  Serial.begin(9600);
  // Call io.begin(<address>) to initialize the SX1509. If it
  // successfully communicates, it'll return 1.
  if (!io.begin(SX1509_ADDRESS))
  {
    while (1) ; // If we fail to communicate, loop forever.
  }
  
  // Call io.pinMode(<pin>, <mode>) to set an SX1509 pin as
  // an output:
  for (i = 0; i < 16; i++) {
      io.pinMode(i, OUTPUT);
  }

  // Set up button read pins
  pinMode(BEAVER_BUTTON_PIN, INPUT);
  pinMode(BOBWHITE_BUTTON_PIN, INPUT);
  pinMode(EAGLE_BUTTON_PIN, INPUT);
  pinMode(FOX_BUTTON_PIN, INPUT);
  pinMode(OWL_BUTTON_PIN, INPUT);
  pinMode(BEAR_BUTTON_PIN, INPUT);
  pinMode(BUFFALO_BUTTON_PIN, INPUT);
  pinMode(ANTELOPE_BUTTON_PIN, INPUT);

  pinMode(WRONG_BUTTON_PIN, INPUT);
  pinMode(WRONG_BUTTON_PIN, INPUT_PULLUP);
  pinMode(RESET_BUTTON_PIN, INPUT);
  pinMode(RESET_BUTTON_PIN, INPUT_PULLUP);

  pinMode(speakerPin, OUTPUT);

  wrong_state = digitalRead(WRONG_BUTTON_PIN);
  
  current_wrong = 0;
}

void loop() {
  byte i, wrong_button;
  // put your main code here, to run repeatedly:
  if (current_active == 0) {
    if ((digitalRead(BEAVER_BUTTON_PIN) == HIGH) && !(current_wrong & BEAVER_MASK)) {
      current_active = BEAVER_MASK;
      set_active_led(BEAVER_INDEX, LOW);
    }
    else if ((digitalRead(BOBWHITE_BUTTON_PIN) == HIGH) && !(current_wrong & BOBWHITE_MASK)) {
      current_active = BOBWHITE_MASK;
      set_active_led(BOBWHITE_INDEX, LOW);
    }
    else if ((digitalRead(EAGLE_BUTTON_PIN) == HIGH) && !(current_wrong & EAGLE_MASK)) {
      current_active = EAGLE_MASK;
      set_active_led(EAGLE_INDEX, LOW);
    }
    else if ((digitalRead(FOX_BUTTON_PIN) == HIGH) && !(current_wrong & FOX_MASK)) {
      current_active = FOX_MASK;
      set_active_led(FOX_INDEX, LOW);
    }
    else if ((digitalRead(OWL_BUTTON_PIN) == HIGH) && !(current_wrong & OWL_MASK)) {
      current_active = OWL_MASK;
      set_active_led(OWL_INDEX, LOW);
    }
    else if ((digitalRead(BEAR_BUTTON_PIN) == HIGH) && !(current_wrong & BEAR_MASK)) {
      current_active = BEAR_MASK;
      set_active_led(BEAR_INDEX, LOW);
    }
    else if ((digitalRead(BUFFALO_BUTTON_PIN) == HIGH) && !(current_wrong & BUFFALO_MASK)) {
      current_active = BUFFALO_MASK;
      set_active_led(BUFFALO_INDEX, LOW);
    }
    else if ((digitalRead(ANTELOPE_BUTTON_PIN) == HIGH) && !(current_wrong & ANTELOPE_MASK)) {
      current_active = ANTELOPE_MASK;
      set_active_led(ANTELOPE_INDEX, LOW);
    }
  }
  wrong_button = digitalRead(WRONG_BUTTON_PIN);
  if (digitalRead(RESET_BUTTON_PIN) == LOW) {
    current_active = 0;
    current_wrong = 0;
    for (i=0; i < 8; i++) {
      Serial.print("Active Reset ");
      Serial.print(i, DEC);
      Serial.print(" ");
      set_active_led(i, HIGH);
      Serial.print("Wrong Reset ");
      Serial.print(i, DEC);
      set_wrong_led(i, HIGH);
      Serial.print("\n");
   }
  }
  else if (wrong_button != wrong_state) {
    wrong_state = wrong_button;
    byte index = get_active_index();
    Serial.print(index, DEC);
    if (index != 0xff) {
      set_wrong_led(index, LOW);
      set_active_led(index, HIGH);
      current_wrong = current_wrong | current_active;
      current_active = 0;
    }
    else
    {
      set_wrong_led(BEAVER_INDEX, LOW);
      set_wrong_led(BOBWHITE_INDEX, LOW);
      set_wrong_led(EAGLE_INDEX, LOW);
      set_wrong_led(FOX_INDEX, LOW);
      set_wrong_led(OWL_INDEX, LOW);
      set_wrong_led(BEAR_INDEX, LOW);
      set_wrong_led(BUFFALO_INDEX, LOW);
      set_wrong_led(ANTELOPE_INDEX, LOW);
      current_wrong = 0xFF;
    }
  }
}


