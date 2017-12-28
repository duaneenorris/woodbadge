/*
SparkFun Inventor's Kit
Example sketch 05

PUSH BUTTONS

  Use pushbuttons for digital input

  Previously we've used the analog pins for input, now we'll use
  the digital pins for input as well. Because digital pins only
  know about HIGH and LOW signals, they're perfect for interfacing
  to pushbuttons and switches that also only have "on" and "off"
  states.
  
  We'll connect one side of the pushbutton to GND, and the other
  side to a digital pin. When we press down on the pushbutton,
  the pin will be connected to GND, and therefore will be read
  as "LOW" by the Arduino.
  
  But wait - what happens when you're not pushing the button?
  In this state, the pin is disconnected from everything, which 
  we call "floating". What will the pin read as then, HIGH or LOW?
  It's hard to say, because there's no solid connection to either
  5 Volts or GND. The pin could read as either one.
  
  To deal with this issue, we'll connect a small (10K, or 10,000 Ohm)
  resistance between the pin and 5 Volts. This "pullup" resistor
  will ensure that when you're NOT pushing the button, the pin will
  still have a weak connection to 5 Volts, and therefore read as
  HIGH.
  
  (Advanced: when you get used to pullup resistors and know when
  they're required, you can activate internal pullup resistors
  on the ATmega processor in the Arduino. See
  http://arduino.cc/en/Tutorial/DigitalPins for information.)

Hardware connections:

  Pushbuttons:
  
    Pushbuttons have two contacts that are connected if you're
    pushing the button, and disconnected if you're not.
    
    The pushbuttons we're using have four pins, but two pairs
    of these are connected together. The easiest way to hook up
    the pushbutton is to connect two wires to any opposite corners.

    Connect any pin on pushbutton 1 to ground (GND).
    Connect the opposite diagonal pin of the pushbutton to
    digital pin 2.

    Connect any pin on pushbutton 2 to ground (GND).
    Connect the opposite diagonal pin of the pushbutton to
    digital pin 3.

    Also connect 10K resistors (brown/black/red) between
    digital pins 2 and 3 and GND. These are called "pullup"
    resistors. They ensure that the input pin will be either
    5V (unpushed) or GND (pushed), and not somewhere in between.
    (Remember that unlike analog inputs, digital inputs are only
    HIGH or LOW.)

  LED:
  
    Most Arduinos, including the Uno, already have an LED
    and resistor connected to pin 13, so you don't need any
    additional circuitry.

    But if you'd like to connect a second LED to pin 13,

    Connect the positive side of your LED to Arduino digital pin 13
    Connect the negative side of your LED to a 330 Ohm resistor
    Connect the other side of the resistor to ground

This sketch was written by SparkFun Electronics,
with lots of help from the Arduino community.
This code is completely free for any use.
Visit http://learn.sparkfun.com/products/2 for SIK information.
Visit http://www.arduino.cc to learn about the Arduino.

Version 2.0 6/2012 MDG
*/


// First we'll set up constants for the pin numbers.
// This will make it easier to follow the code below.
int datapin = 2; 
int clockpin = 3;
int latchpin = 4;

byte data;

const int beaverButtonPin = 8;  // pushbutton 1 pin
const int bobwhiteButtonPin = 9;  // pushbutton 1 pin
const int resetButtonPin = 6;  // pushbutton 2 pin
const int incorrectButtonPin = 5; // pushbutton 3 pin
const int beaverLedPin =  0;    // LED pin
const int beaverIncorrectLedPin = 4;
const int bobwhiteLedPin =  1;    // LED pin
const int bobwhiteIncorrectLedPin = 5;

int beaverState = HIGH;
int beaverIncorrectState = HIGH;
int bobwhiteState = HIGH;
int bobwhiteIncorrectState = HIGH;
int buttonPressed = 0;


void setup()
{
  pinMode(datapin, OUTPUT);
  pinMode(clockpin, OUTPUT);
  pinMode(latchpin, OUTPUT);
  
  // Set up the pushbutton pins to be an input:
  pinMode(beaverButtonPin, INPUT);
  pinMode(bobwhiteButtonPin, INPUT);
  pinMode(resetButtonPin, INPUT);
  pinMode(incorrectButtonPin, INPUT);

}

void shiftWrite(int desiredPin, boolean desiredState)

// This function lets you make the shift register outputs
// HIGH or LOW in exactly the same way that you use digitalWrite().

// Like digitalWrite(), this function takes two parameters:

//    "desiredPin" is the shift register output pin
//    you want to affect (0-7)

//    "desiredState" is whether you want that output
//    to be HIGH or LOW

// Inside the Arduino, numbers are stored as arrays of "bits",
// each of which is a single 1 or 0 value. Because a "byte" type
// is also eight bits, we'll use a byte (which we named "data"
// at the top of this sketch) to send data to the shift register.
// If a bit in the byte is "1", the output will be HIGH. If the bit
// is "0", the output will be LOW.

// To turn the individual bits in "data" on and off, we'll use
// a new Arduino commands called bitWrite(), which can make
// individual bits in a number 1 or 0.
{
  // First we'll alter the global variable "data", changing the
  // desired bit to 1 or 0:

  bitWrite(data,desiredPin,desiredState);
  
  // Now we'll actually send that data to the shift register.
  // The shiftOut() function does all the hard work of
  // manipulating the data and clock pins to move the data
  // into the shift register:

  shiftOut(datapin, clockpin, MSBFIRST, data);

  // Once the data is in the shift register, we still need to
  // make it appear at the outputs. We'll toggle the state of
  // the latchPin, which will signal the shift register to "latch"
  // the data to the outputs. (Latch activates on the high-to
  // -low transition).

  digitalWrite(latchpin, HIGH);
  digitalWrite(latchpin, LOW);
}

void loop()
{
  int beaverButtonState, bobwhiteButtonState, resetButtonState, incorrectButtonState;  // variables to hold the pushbutton states

  // Since a pushbutton has only two states (pushed or not pushed),
  // we've run them into digital inputs. To read an input, we'll
  // use the digitalRead() function. This function takes one
  // parameter, the pin number, and returns either HIGH (5V)
  // or LOW (GND).

  // Here we'll read the current pushbutton states into
  // two variables:

  beaverButtonState = digitalRead(beaverButtonPin);
  bobwhiteButtonState = digitalRead(bobwhiteButtonPin);
  resetButtonState = digitalRead(resetButtonPin);
  incorrectButtonState = digitalRead(incorrectButtonPin);

  // Remember that if the button is being pressed, it will be
  // connected to GND. If the button is not being pressed,
  // the pullup resistor will connect it to 5 Volts.

  // So the state will be LOW when it is being pressed,
  // and HIGH when it is not being pressed.
  
  // Now we'll use those states to control the LED.
  // Here's what we want to do:
  
  // "If either button is being pressed, light up the LED"
  // "But, if BOTH buttons are being pressed, DON'T light up the LED"
  
  // Let's translate that into computer code. The Arduino gives you
  // special logic functions to deal with true/false logic:
  
  // A == B means "EQUIVALENT". This is true if both sides are the same.
  // A && B means "AND". This is true if both sides are true.
  // A || B means "OR". This is true if either side is true.
  // !A means "NOT". This makes anything after it the opposite (true or false).
  
  // We can use these operators to translate the above sentences
  // into logic statements (Remember that LOW means the button is
  // being pressed)
  
  // "If either button is being pressed, light up the LED"
  // becomes:
  // if ((button1State == LOW) || (button2State == LOW)) // light the LED
  
  // "If BOTH buttons are being pressed, DON'T light up the LED"
  // becomes:
  // if ((button1State == LOW) && (button2State == LOW)) // don't light the LED

  // Now let's use the above functions to combine them into one statement:

  if (incorrectButtonState == LOW)
  {
    buttonPressed = 0;
    if (beaverState == LOW)
    {
      beaverIncorrectState = LOW;
      beaverState = HIGH;
    }
    if (bobwhiteState == LOW)
    {
      bobwhiteIncorrectState = LOW;
      bobwhiteState = HIGH;
    }
  }

  if (buttonPressed = 0)
  {
    if ((beaverButtonState == LOW ) && (beaverIncorrectState == HIGH))
    {
      beaverState = LOW;
      buttonPressed = 1;
    }
    else if ((bobwhiteButtonState == LOW ) && (bobwhiteIncorrectState == HIGH))
    {
      bobwhiteState = LOW;
      buttonPressed = 1;
    }
  } 

  if (resetButtonState == LOW)
  {
    buttonPressed = 0;
    beaverState = HIGH;
    beaverIncorrectState = HIGH;
    bobwhiteState = HIGH;
    bobwhiteIncorrectState = HIGH;
  }

  // Set the Beaver LED
  if (beaverState == LOW) 
  {
    shiftWrite(beaverLedPin, HIGH);  // turn the LED on
  }
  else
  {
    shiftWrite(beaverLedPin, LOW);  // turn the LED off
  }

  // Set the Bobwhite LED
  if (bobwhiteState == LOW) 
  {
    shiftWrite(bobwhiteLedPin, HIGH);  // turn the LED on
  }
  else
  {
    shiftWrite(bobwhiteLedPin, LOW);  // turn the LED off
  }

  // Set the beaver incorrect LED
  if (beaverIncorrectState == LOW)
  {
    shiftWrite(beaverIncorrectLedPin, HIGH);
  }
  else 
  {
    shiftWrite(beaverIncorrectLedPin, LOW);
  }

  // Set the bobwhite incorrect LED
  if (bobwhiteIncorrectState == LOW)
  {
    shiftWrite(bobwhiteIncorrectLedPin, HIGH);
  }
  else 
  {
    shiftWrite(bobwhiteIncorrectLedPin, LOW);
  }
     	
  // As you can see, logic operators can be combined to make
  // complex decisions!

  // Don't forget that we use = when we're assigning a value,
  // and use == when we're testing a value for equivalence.
}


