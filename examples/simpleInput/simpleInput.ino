/* Libs */
#include "PCA9698.h"

/* Class Constructor */
PCA9698 driver;

/* Variables */
uint8_t ledState;

void setup()
{
    uint8_t modes[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
    driver.setModePorts(modes, sizeof(modes));

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{
    driver.updateInput();
    uint8_t input[5];
    driver.digitalReadPorts(input, sizeof(input));
    for (uint8_t index = 0; index < sizeof(input); index++)
    {
        if (input[index])
        {
            digitalWrite(LED_BUILTIN, !ledState);
            ledState = !ledState;
        }
    }
}