#include <Joystick.h>   // See https://github.com/MHeironimus/ArduinoJoystickLibrary
#include <Rotary.h>     // See https://github.com/brianlow/Rotary

// This sketch uses the same joystick bindings as the CH Yoke
// Z-axis -> throttle
// X-rot -> mixture
// 
// And then uses z-rot for the trim wheel
// Z-rot -> trim wheel

//
// Set your pins here
// Note: The trim wheel uses the CHANGE interrupt handler, so make sure
//       you pick pins that support that on your board.
//
#define MIXTURE_PIN   (A1)
#define THROTTLE_PIN  (A2)
#define WHEEL_PIN_A   (2)
#define WHEEL_PIN_B   (3)

// This multiplier controls how much Z-rot occurs for each step of the rotary encoder.
// For my 30-step encoder, I found values in the range 8-12 worked best.
// Encoders with different precision may need a larger or smaller multiplier.
const int trimWheelRzDeltaPerStep = 10;

//
// Don't change anything below here
//

// Adjust these if you change the range for the Rz axis
#define JOY_RZ_MIN  (0)
#define JOY_RZ_MAX  (1023)

// Manages the encoder
Rotary wheelEncoder(WHEEL_PIN_A, WHEEL_PIN_B);

// Manages the joystick HID output
Joystick_ joystick(
  JOYSTICK_DEFAULT_REPORT_ID,
  JOYSTICK_TYPE_JOYSTICK,
  0,
  0,
  false, false, true,  // X, Y, Z
  true, false, true,     // Rx, Ry, Rz
  false,                // Rudder
  false,                // Throttle
  false,                // Accelerator
  false,                // Brake
  false                 // Steering
);

// Global storage for a few things
int throttlePotRawValue;
int mixturePotRawValue;
int lastRzValueSent = 512;
int newRzValue;

void setup()
{
  Serial.begin(9600);

  // Default range is 0-1023 which is fine for us
  // since it matches the analog range we get from
  // the ADC
  // joystick.setZAxisRange(0,1023);
  // joystick.setRxAxisRange(0,1023);
  // joystick.setRzAxisRange(0,1023);

  joystick.begin();

  attachInterrupt(digitalPinToInterrupt(2), wheelChanged, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), wheelChanged, CHANGE);
  wheelEncoder.begin();
}

void loop()
{
  throttlePotRawValue = analogRead(THROTTLE_PIN);
  mixturePotRawValue = analogRead(MIXTURE_PIN);

  joystick.setZAxis(throttlePotRawValue);
  joystick.setRxAxis(mixturePotRawValue);

  delay(10);
}

unsigned char result;
void wheelChanged()
{
  // Get the new value
  result = wheelEncoder.process();

  if (result == DIR_CW)
  {
    // Nose up
    // Serial.println("DIR_CW");
    newRzValue = lastRzValueSent + trimWheelRzDeltaPerStep;
    if (newRzValue > JOY_RZ_MAX)
      newRzValue = JOY_RZ_MAX;

  }
  else if (result == DIR_CCW)
  {
    // Serial.println("DIR_CCW");
    newRzValue = lastRzValueSent - trimWheelRzDeltaPerStep;
    if (newRzValue < JOY_RZ_MIN)
      newRzValue = JOY_RZ_MIN;
  }
  else
  {
    // NADA
  }
  joystick.setRzAxis(newRzValue);
  lastRzValueSent = newRzValue;
}


