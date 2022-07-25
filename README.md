# PCA9698
Arduino library for the GPIO Driver PCA9698 from NXP.

## Installation 
1. Navigate to the Releases page.
2. Download the latest release.
3. Rename to PCA9698.zip
4. In the Arduino IDE, navigate to Sketch > Include Library > Add .zip library

## Functions
setMode()\
setModePort()\
setModePorts()\
digitalWrite()\
digitalWritePort()\
digitalWritePorts()\
getOutputPin()\
getOutputPort()\
getOutputPorts()\
togglePin()\
togglePort()\
togglePorts()\
digitalRead()\
digitalReadPort()\
digitalReadPorts()\
updateOutput()\
updateInput()\
updateAll()

## Guide
First include the library and create a class object. When no arguments used the class will be use 0x20 as basic I2C adress and 100kHz as I2C speed.
```c++
#include <PCA9698.h>

PCA9698 gpio;

void setup()
{

}

void loop()
{

}
```
Or use some custom adress and other speeds. The speeds are predefined as **I2C_Standard_Mode** , **I2C_FAST_MODE** , **I2C_FAST_MODE_PLUS**.
```c++
#include <PCA9698.h>

#define addr  0x21  //I2C Adress of the PCA9698

PCA9698 gpio = (addr, I2C_FAST_MODE);  //Config the PCA9698 with adress 0x21 and 400000KHz bus speed

void setup()
{

}

void loop()
{

}
```
<br/>

To configure an IO to input or output there are three functions. After the function call the data will transmitted to the slave.
```c++
#include <PCA9698.h>

PCA9698 gpio;

void setup()
{
    //First param is the pin from 0 - 39
    //Second param is the mode of the IO
    gpio.setMode(0, OUTPUT);  
    gpio.setMode(1, INPUT);
    
    //First param is the IO bank. There are 5 banks of 8bit.
    //Second param is the bitmask for the bank. 1 = output, 0 = input.
    gpio.setModePort(1, 0b11110000);
    
    //This function use a bitmask array to config the IOs. 
    //All IOs can be configured with only two lines of code.
    //First param is the bitmask array. Every index is one 8bit bank.
    //Second param is the size of the array. Max size is 5.
    uint8_t modes[5] = {0xFF, 0x00, 0xFF, 0xFF, 0xFF};
    gpio.setModePorts(modes, sizeof(modes));
}

void loop()
{

}
```
<br/>

For changing outputs there are multiple functions. After the call of **updateOutput()** or **updateAll()** the changes will transmitted to the slave.
```c++
#include <PCA9698.h>

PCA9698 gpio;

void setup()
{
    uint8_t modes[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};  //Config all IOs as output
    gpio.setModePorts(modes, sizeof(modes));
}

void loop()
{
    uint8_t states[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    gpio.digitalWrite(0, 1);  //Set IO0 to HIGH
    gpio.digitalWritePort(1, 0b11110000);  //Set IO8 - 11 to LOW, IO12 - 15 to HIGH
    gpio.digitalWritePorts(states, sizeof(states)); //Set all IOs to HIGH
    
    gpio.togglePin(1);  //Toggle IO1
    gpio.togglePort(2, 0b11110000); //Toggle IO20 - 23
    gpio.togglePorts(states, sizeof(states)); //Toggle all IOs
    
    //Call this function at the end of the loop
    //This function will transmit the IO states from the RAM
    gpio.updateOutput();  
    
    delay(1000);
}
```
<br/>

For reading the state of the input pins call the **updateInput()** or **updateAll()** function first and then the reading functions. 
```c++
#include <PCA9698.h>

PCA9698 gpio;

void setup()
{
    uint8_t modes[5] = {0x00, 0x00, 0x00, 0x00, 0x00};  //Config all IOs as input
    gpio.setModePorts(modes, sizeof(modes));
}

void loop()
{
    //Call this function at the beginning of the loop. 
    //The input states will be read from the slave.
    gpio.updateInput();
    
    Serial.println(gpio.digitalRead(0));  //Print input state of IO0
    Serial.println(gpio.digitalReadPort(1), BIN); //Print input states of bank1
    
    //Print input states of all banks
    uint8_t states[5];
    gpio.digitalReadPorts(states, sizeof(states));
    for (uint8_t index = 0; index < sizeof(states); index++)
    {
        Serial.println(states[index], HEX);
    }
    
    delay(100);
}
```
