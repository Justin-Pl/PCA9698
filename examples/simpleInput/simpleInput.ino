/* Libs */
#include "PCA9698.h"

/* Class Constructor */
PCA9698 gpio;

void setup()
{
    gpio.begin();   //Begin I2C
    
    //Set all pins to input
    uint8_t modes[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
    gpio.setModePorts(modes, sizeof(modes));
}

void loop()
{
    gpio.updateInput(); //Reads input over I2C
    uint8_t input[5];   //Array for input data
    gpio.digitalReadPorts(input, sizeof(input));    //Reads all inputs 
    for (uint8_t index = 0; index < sizeof(input); index++) 
    {
        Serial.print(index, DEC);       //Print port number
        Serial.println(input[index]);   //Print port as HEX
    }
    delay(100);
}
