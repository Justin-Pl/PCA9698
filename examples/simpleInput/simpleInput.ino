/* Libs */
#include "PCA9698.h"

/* Class Constructor */
PCA9698 gpio;

void setup()
{
    uint8_t modes[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
    gpio.setModePorts(modes, sizeof(modes));
}

void loop()
{
    gpio.updateInput();
    uint8_t input[5];
    gpio.digitalReadPorts(input, sizeof(input));
    for (uint8_t index = 0; index < sizeof(input); index++)
    {
        Serial.print(index, DEC);
        Serial.println(input[index]);
    }
    delay(100);
}