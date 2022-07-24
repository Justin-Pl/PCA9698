/*  
    Arduino Library for the PCA9698 40-Bit GPIO Module.
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    Created by Justin Plobst, July 21rd, 2022.
    PCA9698 - Version 1.0
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
    uint8_t _adress;
    uint32_t _speed;
    uint8_t _mode[5];
    uint8_t _outputPort[5];
    uint8_t _inputPort[5];
    void writeI2C(uint8_t command, uint8_t data);
    void writeI2C(uint8_t command, uint8_t *data, uint8_t length);
    void readI2C(uint8_t command, uint8_t *data, uint8_t length);
public:
    PCA9698(uint8_t addr = PCA9698_SLAVE_ADDRESS, uint32_t I2Cspeed = I2C_STANDARD_MODE);
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