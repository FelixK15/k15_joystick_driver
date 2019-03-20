#include <Arduino.h>
#include <Joystick.h>

//FK: Joypad configuration
#define K15_JOYSTICK_HAS_SERVICE_KEYS        1

#if K15_JOYSTICK_HAS_SERVICE_KEYS
# define K15_JOYSTICK_BUTTON_COUNT 9
#else
# define K15_JOYSTICK_BUTTON_COUNT 6
#endif

#define K15_JOYSTICK_HAT_SWITCH_COUNT  0
#define K15_JOYSTICK_USE_X_AXIS            1
#define K15_JOYSTICK_USE_Y_AXIS            1
#define K15_JOYSTICK_USE_Z_AXIS            0
#define K15_JOYSTICK_USE_RX_AXIS           0
#define K15_JOYSTICK_USE_RY_AXIS           0
#define K15_JOYSTICK_USE_RZ_AXIS           0
#define K15_JOYSTICK_USE_RUDDER            0
#define K15_JOYSTICK_USE_THROTTLE          0
#define K15_JOYSTICK_USE_ACCELERATOR       0
#define K15_JOYSTICK_USE_BREAK             0
#define K15_JOYSTICK_USE_STEERING          0

//FK: No support for Z, Rx, Ry, Rz, Rudder, Throttle, Accelerator, Break and Steering axis for now
#define K15_JOYSTICK_X_AXIS_PIN         21
#define K15_JOYSTICK_Y_AXIS_PIN         20
#define K15_JOYSTICK_BUTTON_1_PIN       4
#define K15_JOYSTICK_BUTTON_2_PIN       5
#define K15_JOYSTICK_BUTTON_3_PIN       6
#define K15_JOYSTICK_BUTTON_4_PIN       7
#define K15_JOYSTICK_BUTTON_5_PIN       8
#define K15_JOYSTICK_BUTTON_6_PIN       9
#define K15_JOYSTICK_BUTTON_P1_PIN      19
#define K15_JOYSTICK_BUTTON_P2_PIN      18
#define K15_JOYSTICK_BUTTON_COIN_PIN    10

typedef struct
{
  uint8_t             bitMask;
  volatile uint8_t*   pRegister;
} k15_button;

typedef struct 
{
  k15_button buttons[K15_JOYSTICK_BUTTON_COUNT];
  Joystick_ *pHID;
} k15_joystick;

k15_joystick  joystick;
uint8_t       initError = 0u;

void setup() 
{
  //FK: Reset SPI pins in case someone messed with them
  pinMode(PIN_SPI_SCK,         OUTPUT);
  pinMode(PIN_SPI_MOSI,        OUTPUT);
  pinMode(PIN_SPI_SS,          OUTPUT);
  pinMode(PIN_SPI_MISO,        INPUT);

  digitalWrite(PIN_SPI_SCK,      HIGH);
  digitalWrite(PIN_SPI_MOSI,     HIGH);
  digitalWrite(PIN_SPI_SS,       HIGH);
  digitalWrite(PIN_SPI_MISO,     HIGH);

  const uint8_t buttonPinArray[] = {
    K15_JOYSTICK_BUTTON_1_PIN, 
    K15_JOYSTICK_BUTTON_2_PIN,
    K15_JOYSTICK_BUTTON_3_PIN,
    K15_JOYSTICK_BUTTON_4_PIN,
    K15_JOYSTICK_BUTTON_5_PIN,
    K15_JOYSTICK_BUTTON_6_PIN,

    #if K15_JOYSTICK_HAS_SERVICE_KEYS
      K15_JOYSTICK_BUTTON_P1_PIN,
      K15_JOYSTICK_BUTTON_P2_PIN,
      K15_JOYSTICK_BUTTON_COIN_PIN
    #endif
  };

  //FK: Get bits and ports of the buttons for faster access later
  for (uint8_t buttonIndex = 0u; buttonIndex < K15_JOYSTICK_BUTTON_COUNT; ++buttonIndex)
  {
    const uint8_t port = digitalPinToPort(buttonPinArray[buttonIndex]);

    pinMode(buttonPinArray[buttonIndex], INPUT);
    joystick.buttons[buttonIndex].pRegister   = portInputRegister(port);
    joystick.buttons[buttonIndex].bitMask     = digitalPinToBitMask(buttonPinArray[buttonIndex]);
  }

#if K15_JOYSTICK_USE_X_AXIS
  pinMode(K15_JOYSTICK_X_AXIS_PIN, INPUT);
#endif

#if K15_JOYSTICK_USE_Y_AXIS
  pinMode(K15_JOYSTICK_Y_AXIS_PIN, INPUT);
#endif

  joystick.pHID = new Joystick_(JOYSTICK_DEFAULT_REPORT_ID, 
    JOYSTICK_TYPE_JOYSTICK, 
    K15_JOYSTICK_BUTTON_COUNT, 
    K15_JOYSTICK_HAT_SWITCH_COUNT,
    K15_JOYSTICK_USE_X_AXIS,
    K15_JOYSTICK_USE_Y_AXIS,
    K15_JOYSTICK_USE_Z_AXIS,
    K15_JOYSTICK_USE_RX_AXIS,
    K15_JOYSTICK_USE_RY_AXIS,
    K15_JOYSTICK_USE_RZ_AXIS,
    K15_JOYSTICK_USE_RUDDER,
    K15_JOYSTICK_USE_THROTTLE,
    K15_JOYSTICK_USE_ACCELERATOR,
    K15_JOYSTICK_USE_BREAK,
    K15_JOYSTICK_USE_STEERING
  );

  if (joystick.pHID == nullptr)
  {
    initError = 1u;
    return;
  }

  // put your setup code here, to run once:
#if K15_JOYSTICK_USE_X_AXIS
  joystick.pHID->setXAxisRange(0, 1024);
#endif

#if K15_JOYSTICK_USE_Y_AXIS
  joystick.pHID->setYAxisRange(0, 1024);
#endif

#if K15_JOYSTICK_USE_Z_AXIS
  joystick.pHID->setZAxisRange(0, 1024);
#endif

#if K15_JOYSTICK_USE_RX_AXIS
  joystick.pHID->setRxAxisRange(0, 1024);
#endif

#if K15_JOYSTICK_USE_RY_AXIS
  joystick.pHID->setRyAxisRange(0, 1024);
#endif

#if K15_JOYSTICK_USE_RZ_AXIS
  joystick.pHID->setRzAxisRange(0, 1024);
#endif

#if K15_JOYSTICK_USE_RUDDER
  joystick.pHID->setRudderRange(0, 1024);
#endif

#if K15_JOYSTICK_USE_THROTTLE
  joystick.pHID->setThrottleRange(0, 1024);
#endif

#if K15_JOYSTICK_USE_ACCELERATOR
  joystick.pHID->setAcceleratorRange(0, 1024);
#endif

#if K15_JOYSTICK_USE_BREAK
  joystick.pHID->setBrakeRange(0, 1024);
#endif

#if K15_JOYSTICK_USE_STEERING
  joystick.pHID->setSteeringRange(0, 1024);
#endif

  joystick.pHID->begin();
}

void loop()
{
  if (initError == 1)
  {
    //FK: TODO: Blink red error LED
  }
  else
  {
  #if K15_JOYSTICK_USE_X_AXIS
    const int16_t xAxisValue = analogRead(K15_JOYSTICK_X_AXIS_PIN);
    joystick.pHID->setXAxis(xAxisValue);
  #endif

  #if K15_JOYSTICK_USE_Y_AXIS
    const int16_t yAxisValue = analogRead(K15_JOYSTICK_Y_AXIS_PIN);
    joystick.pHID->setYAxis(yAxisValue);
  #endif

    for(uint8_t buttonIndex = 0u; buttonIndex < K15_JOYSTICK_BUTTON_COUNT; ++buttonIndex)
    {
        const uint8_t value = (*joystick.buttons[buttonIndex].pRegister & joystick.buttons[buttonIndex].bitMask) > 0u ? 1u : 0u;
        joystick.pHID->setButton(buttonIndex, value);
    }
  }
}
