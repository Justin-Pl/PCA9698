/* Libs */
#include <PCA9698.h>

/* Class Constructor */
PCA9698 driver;

void setup()
{
    uint8_t modes[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    driver.setModePorts(modes, sizeof(modes));
}

void loop()
{
    uint8_t toggle[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    driver.togglePorts(toggle, sizeof(toggle));
    delay(1000);
    driver.updateOutput();
}