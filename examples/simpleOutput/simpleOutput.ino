/* Libs */
#include <PCA9698.h>

/* Class Constructor */
PCA9698 driver;

void setup()
{
    gpio.begin();   //Init I2C
    
    uint8_t modes[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    driver.setModePorts(modes, sizeof(modes));  //Set all pins as output
}

void loop()
{
    uint8_t toggle[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    driver.togglePorts(toggle, sizeof(toggle)); //Toggle all pins
    driver.updateOutput();  //Send outputs to PCA9698
    delay(1000);
}
