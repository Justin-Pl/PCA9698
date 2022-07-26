/*  
    Arduino Library for the PCA9698 40-Bit GPIO Module.
    
    MIT License

    Copyright (c) 2022 Justin-Pl

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#ifndef PCA9698_H
#define PCA9698_H

/* Libs */
#include <Arduino.h>
#include <Wire.h>

/* Defines */
#define PCA9698_SLAVE_ADDRESS   0x20  
#define I2C_STANDARD_MODE       100000UL
#define I2C_FAST_MODE           400000UL
#define I2C_FAST_MODE_PLUS      1000000UL

/* Class */
class PCA9698
{
private:
	TwoWire *_I2CPort;
    uint8_t _adress;
    uint32_t _speed;
    uint8_t _mode[5];
    uint8_t _outputPort[5];
    uint8_t _inputPort[5];
    void writeI2C(uint8_t command, uint8_t data);
    void writeI2C(uint8_t command, uint8_t *data, uint8_t length);
    void readI2C(uint8_t command, uint8_t *data, uint8_t length);
public:
    PCA9698(uint8_t addr = PCA9698_SLAVE_ADDRESS, uint32_t I2Cspeed = I2C_STANDARD_MODE, TwoWire &wirePort = Wire);
    void begin();
    void setMode(uint8_t pin, uint8_t mode);
    void setModePort(uint8_t portNum, uint8_t port);
    void setModePorts(uint8_t *ports, uint8_t length);
    void digitalWrite(uint8_t pin, uint8_t state);
    void digitalWritePort(uint8_t portNum, uint8_t port);
    void digitalWritePorts(uint8_t *ports, uint8_t length);
    uint8_t getOutputPin(uint8_t pin);
    uint8_t getOutputPort(uint8_t portNum);
    void getOutputPorts(uint8_t *ports, uint8_t length);
    void togglePin(uint8_t pin);
    void togglePort(uint8_t portNum, uint8_t port);
    void togglePorts(uint8_t *ports, uint8_t length);
    uint8_t digitalRead(uint8_t pin);
    uint8_t digitalReadPort(uint8_t portNum);
    void digitalReadPorts(uint8_t *ports, uint8_t length);
    void updateOutput();
    void updateInput();
    void updateAll();
};

#endif PCA9698_H