/* Libs */
#include <PCA9698.h>

/* Class Constructor */
PCA9698 gpio;

void setup()
{
    gpio.begin();   //Init I2C
    
    uint8_t modes[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    gpio.setModePorts(modes, sizeof(modes));  //Set all pins as output
}

void loop()
{
    uint8_t toggle[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    gpio.togglePorts(toggle, sizeof(toggle)); //Toggle all pins
    gpio.updateOutput();  //Send outputs to PCA9698
    delay(1000);
}
