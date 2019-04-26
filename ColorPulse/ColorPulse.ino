/* This example shows how to display a color whose brightness depends on the 
 * heart beat

/* By default, the APA102 library uses pinMode and digitalWrite
 * to write to the LEDs, which works on all Arduino-compatible
 * boards but might be slow.  If you have a board supported by
 * the FastGPIO library and want faster LED updates, then install
 * the FastGPIO library and uncomment the next two lines: */
// #include <FastGPIO.h>
// #define APA102_USE_FAST_GPIO
#include <APA102.h>
#include <Streaming.h>

// Define which pins to use.
const uint8_t dataPin = 11;
const uint8_t clockPin = 12;

// Create an object for writing to the LED strip.
APA102<dataPin, clockPin> ledStrip;

// Set the number of LEDs to control.
const uint16_t ledCount = 64;

// Create a buffer for holding the colors (3 bytes per color).
rgb_color colors[ledCount];

//  Variables

int PulseSensorPurplePin = 0; // Pulse Sensor PURPLE WIRE connected to ANALOG PIN 0
int LED13 = 13;               //  The on-board Arduion LED

int maxBrightness = 15;
int Signal;          // holds the incoming raw data. Signal value can range from 0-1024
int Threshold = 340; // Determine which Signal to "count as a beat", and which to ingore.
int MaxThreshold = 540;
const int Bound = 800;

bool heartArrayFilled[5][64]{
    {0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 1, 1, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 1, 1, 0, 0, 0,
     0, 0, 0, 1, 1, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 1, 0, 0, 1, 0, 0,
     0, 0, 1, 1, 1, 1, 0, 0,
     0, 0, 1, 1, 1, 1, 0, 0,
     0, 0, 0, 1, 1, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 1, 0, 0, 1, 0, 0,
     0, 1, 1, 1, 1, 1, 1, 0,
     0, 1, 1, 1, 1, 1, 1, 0,
     0, 1, 1, 1, 1, 1, 1, 0,
     0, 0, 1, 1, 1, 1, 0, 0,
     0, 0, 0, 1, 1, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 0, 0, 1, 1, 0,
     1, 1, 1, 1, 1, 1, 1, 1,
     1, 1, 1, 1, 1, 1, 1, 1,
     1, 1, 1, 1, 1, 1, 1, 1,
     1, 1, 1, 1, 1, 1, 1, 1,
     0, 1, 1, 1, 1, 1, 1, 0,
     0, 0, 1, 1, 1, 1, 0, 0,
     0, 0, 0, 1, 1, 0, 0, 0}};

bool heartArray[5][64]{
    {0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 1, 1, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 1, 1, 0, 0, 0,
     0, 0, 0, 1, 1, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 1, 0, 0, 1, 0, 0,
     0, 0, 1, 1, 1, 1, 0, 0,
     0, 0, 1, 0, 0, 1, 0, 0,
     0, 0, 0, 1, 1, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 1, 0, 0, 1, 0, 0,
     0, 1, 0, 1, 1, 0, 1, 0,
     0, 1, 0, 0, 0, 0, 1, 0,
     0, 1, 0, 0, 0, 0, 1, 0,
     0, 0, 1, 0, 0, 1, 0, 0,
     0, 0, 0, 1, 1, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 0, 0, 1, 1, 0,
     1, 0, 0, 1, 1, 0, 0, 1,
     1, 0, 0, 0, 0, 0, 0, 1,
     1, 0, 0, 0, 0, 0, 0, 1,
     1, 0, 0, 0, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 0, 1, 0,
     0, 0, 1, 0, 0, 1, 0, 0,
     0, 0, 0, 1, 1, 0, 0, 0}};

int dataCalibrateInterval = 5;

// Set the brightness to use (the maximum is 31).
uint8_t brightness;

void setup()
{
  pinMode(LED13, OUTPUT); // pin that will blink to your heartbeat!
  Serial.begin(9600);     // Set's up Serial Communication at certain speed.
}

/* Converts a color from HSV to RGB.
 * h is hue, as a number between 0 and 360.
 * s is the saturation, as a number between 0 and 255.
 * v is the value, as a number between 0 and 255. */
rgb_color hsvToRgb(uint16_t h, uint8_t s, uint8_t v)
{
  uint8_t f = (h % 60) * 255 / 60;
  uint8_t p = (255 - s) * (uint16_t)v / 255;
  uint8_t q = (255 - f * (uint16_t)s / 255) * (uint16_t)v / 255;
  uint8_t t = (255 - (255 - f) * (uint16_t)s / 255) * (uint16_t)v / 255;
  uint8_t r = 0, g = 0, b = 0;
  switch ((h / 60) % 6)
  {
  case 0:
    r = v;
    g = t;
    b = p;
    break;
  case 1:
    r = q;
    g = v;
    b = p;
    break;
  case 2:
    r = p;
    g = v;
    b = t;
    break;
  case 3:
    r = p;
    g = q;
    b = v;
    break;
  case 4:
    r = t;
    g = p;
    b = v;
    break;
  case 5:
    r = v;
    g = p;
    b = q;
    break;
  }
  return rgb_color(r, g, b);
}

void readPulseData()
{
  Signal = analogRead(PulseSensorPurplePin); // Read the PulseSensor's value.
                                             // Assign this value to the "Signal" variable.
}

void displaySignalOnLed()
{
  //  Serial.println(Signal); // Send the Signal value to Serial Plotter.
  if (Signal > Threshold)
  { // If the signal is above "550", then "turn-on" Arduino's on-Board LED.
    digitalWrite(LED13, HIGH);
  }
  else
  {
    digitalWrite(LED13, LOW); //  Else, the sigal must be below "550", so "turn-off" this LED.
  }
}

void sendColor(uint16_t power, uint8_t red, uint8_t green, uint8_t blue)
{
  // Choose the lowest possible 5-bit brightness that will work.
  uint8_t brightness5Bit = 1;
  while (brightness5Bit * 255 < power && brightness5Bit < 31)
  {
    brightness5Bit++;
  }

  // Set brightness8Bit to be power divided by brightness5Bit,
  // rounded to the nearest whole number.
  uint8_t brightness8Bit = (power + (brightness5Bit / 2)) / brightness5Bit;

  // Send the white color to the LED strip.  At this point,
  // brightness8Bit multiplied by brightness5Bit should be
  // approximately equal to power.
  ledStrip.sendColor((float)red * power / 31, (float)green * power / 31, (float)blue * power / 31, brightness5Bit);
}

void displayBackgroundMatrix()
{
  ledStrip.startFrame();

  for (uint16_t i = 0; i < ledCount; i++)
  {
    sendColor(10, 255, 255, 255);
  }
  ledStrip.endFrame(ledCount);
}

void displayMatrix()
{
  uint8_t time = millis() >> 4;

  float percent = constrain((float)(Signal - Threshold) / (MaxThreshold - Threshold), 0, 1);
  int power = constrain((float)(Signal - Threshold) / (MaxThreshold - Threshold), 0, 1) * maxBrightness;
  Serial << "Signal: " << Signal << ", threshold: " << Threshold << ", percent:" << percent << ", power: " << power << endl;

  if (power == 1)
  {
    ledStrip.startFrame();
    for (uint16_t i = 0; i < ledCount; i++)
    {
      sendColor(31, 255, 0, 0);
    }
    ledStrip.endFrame(ledCount);

    delay(250);
  }
  else if (power == 0)
  {
    ledStrip.startFrame();
    for (uint16_t i = 0; i < ledCount; i++)
    {
      sendColor(0, 255, 0, 0);
    }
    ledStrip.endFrame(ledCount);
  }
  else
  {
    ledStrip.startFrame();
    for (uint16_t i = 0; i < ledCount; i++)
    {
      if (heartArrayFilled[(int)(percent * 4)][i])
      {
        sendColor(31, 255, 0, 0);
      }
      else
      {
        sendColor(0, 0, 0, 0);
      }
    }
    ledStrip.endFrame(ledCount);
  }
}

void loop()
{
  readPulseData();
  displaySignalOnLed();
  displayMatrix();
  //  displayBackgroundMatrix();
  delay(5);
}
