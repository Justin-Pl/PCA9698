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

/* Registers */
#define INPUT_PORT_BANK0            0x00
#define INPUT_PORT_BANK1            0x01
#define INPUT_PORT_BANK2            0x02
#define INPUT_PORT_BANK3            0x03
#define INPUT_PORT_BANK4            0x04
#define OUTPUT_PORT_BANK0           0x08
#define OUTPUT_PORT_BANK1           0x09
#define OUTPUT_PORT_BANK2           0x0A
#define OUTPUT_PORT_BANK3           0x0B
#define OUTPUT_PORT_BANK4           0x0C
#define POLARITY_INVERSION_BANK0    0x10  
#define POLARITY_INVERSION_BANK1    0x11
#define POLARITY_INVERSION_BANK2    0x12
#define POLARITY_INVERSION_BANK3    0x13
#define POLARITY_INVERSION_BANK4    0x14
#define IO_CONFIG_BANK0             0x18
#define IO_CONFIG_BANK1             0x19
#define IO_CONFIG_BANK2             0x1A
#define IO_CONFIG_BANK3             0x1B
#define IO_CONFIG_BANK4             0x1C
#define MASK_INTERRUPT_BANK0        0x20
#define MASK_INTERRUPT_BANK1        0x21
#define MASK_INTERRUPT_BANK2        0x22
#define MASK_INTERRUPT_BANK3        0x23
#define MASK_INTERRUPT_BANK4        0x24
#define OUTPUT_CONFIG               0x28
#define ALL_BANK                    0x29
#define MODE                        0x2A
#define AUTO_INCREMENT              0x80
#define MODE_SMBA                   0x10
#define MODE_IOAC                   0x08
#define MODE_OCH                    0x02
#define MODE_OEPOL                  0x01

/* Defines */
#define PCA9698_SLAVE_ADDRESS   0x20  
#define DEFAULT_MODE            0x02
#define OE_POL_LOW              (0 << 0)
#define OE_POL_HIGH             (1 << 0)
#define OUTPUT_CHANGE_STOP      (0 << 1)
#define OUTPUT_CHANGE_ACK       (1 << 1)
#define GPIO_ALL_CALL_ON        (0 << 3)
#define GPIO_ALL_CALL_OFF       (1 << 3)
#define SMBA_OFF                (0 << 4)
#define SMBA_ON                 (1 << 4)
#define INT_ON                  0x01
#define INT_OFF                 0x02
#define I2C_STANDARD_MODE       100000UL
#define I2C_FAST_MODE           400000UL
#define I2C_FAST_MODE_PLUS      1000000UL

/* Class */
class PCA9698
{
private:
    TwoWire *_I2CPort;
    uint8_t _adress;
    uint8_t _mode[5];
    uint8_t _outputPort[5];
    uint8_t _inputPort[5];
    void writeI2C(uint8_t command, uint8_t data);
    void writeI2C(uint8_t command, uint8_t *data, uint8_t length);
    uint8_t readI2C(uint8_t command);
    void readI2C(uint8_t command, uint8_t *data, uint8_t length);
public:
    PCA9698(uint8_t addr = PCA9698_SLAVE_ADDRESS, TwoWire &wirePort = Wire);
    void begin(uint8_t modeConfig = DEFAULT_MODE, uint32_t speed = I2C_STANDARD_MODE);
    void config(uint8_t mode);
    void setMode(uint8_t pin, uint8_t mode);
    void setModePort(uint8_t portNum, uint8_t port);
    void setModePorts(uint8_t *ports, uint8_t length);
    void setINT(uint8_t pin, uint8_t mode);
    void setINTPort(uint8_t portNum, uint8_t port);
    void setINTPorts(uint8_t *ports, uint8_t length);
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
