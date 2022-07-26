/* Header */
#include "PCA9698.h"

/* Class Functions */
/* private */
void PCA9698::writeI2C(uint8_t command, uint8_t data)
{
    _I2CPort->beginTransmission(_adress);
    _I2CPort->write(command);
    _I2CPort->write(data);
    _I2CPort->endTransmission();
}

void PCA9698::writeI2C(uint8_t command, uint8_t *data, uint8_t length)
{
    _I2CPort->beginTransmission(_adress);
    _I2CPort->write(command);
    _I2CPort->write(data, length);
    _I2CPort->endTransmission();
}

uint8_t PCA9698::readI2C(uint8_t command)
{
    _I2CPort->beginTransmission(_adress);
    _I2CPort->write(command);
    _I2CPort->endTransmission();
    _I2CPort->requestFrom(_adress, 1);
    return _I2CPort->read();
}

void PCA9698::readI2C(uint8_t command, uint8_t *data, uint8_t length)
{
    _I2CPort->beginTransmission(_adress);
    _I2CPort->write(command);
    _I2CPort->endTransmission();
    _I2CPort->requestFrom(_adress, length);
    _I2CPort->readBytes(data, length);
}

/* public */
/*!
    @brief  Constructor for PCA9698 IO-Expander.
    @param  addr    
            I2C Adress for PCA9698. If empty 0x20.
    @param  I2Cspeed
            Speed of I2C Bus. If empty 100kHz.
    @return PCA9698 obejct.
*/
PCA9698::PCA9698(uint8_t addr, TwoWire &wirePort)
{
    _I2CPort = &wirePort;
    _adress = addr;
    for (uint8_t _byteNum = 0; _byteNum < sizeof(_mode); _byteNum++)
    {
        _mode[_byteNum] = 0x00;
        _outputPort[_byteNum] = 0x00;
        _inputPort[_byteNum] = 0x00;
    }
}

/*!
    @brief  Init I2C bus.
    @param  modeConfig
            Can control the OE polarity, output change on ack or stop, the ability of the device to respond to a 'GPIO All Call' and the respond to SMBAlerts. Default is 0x02.
    @param  speed
            Sets the speed of the bus. 100KHz default.
    @return None (void). 
*/
void PCA9698::begin(uint8_t modeConfig, uint32_t speed)
{
    uint8_t intMask[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
    _I2CPort->begin();
    _I2CPort->setClock(speed);
    writeI2C(MODE, modeConfig & (MODE_OEPOL | MODE_OCH | MODE_IOAC | MODE_SMBA));
    writeI2C(IO_CONFIG_BANK0 | AUTO_INCREMENT, _mode, sizeof(_mode));
    writeI2C(MASK_INTERRUPT_BANK0 | AUTO_INCREMENT, intMask, sizeof(intMask));
    updateAll();
}

/*!
    @brief  Change the PCA9698 mode configuration.
    @param  modeConfig
            Can control the OE polarity, output change on ack or stop, the ability of the device to respond to a 'GPIO All Call' and the respond to SMBAlerts. Default is 0x02.
    @return None (void). 
*/
void PCA9698::config(uint8_t mode)
{
    mode &= (MODE_OEPOL | MODE_OCH | MODE_IOAC | MODE_SMBA);
    writeI2C(MODE, mode);
}

/*!
    @brief  Set single pin to output or input.
    @param  pin
            Pin number. 0 - 39.
    @param  mode
            Mode of the pin. Output = 1. Input = 0.
    @return None (void). 
*/
void PCA9698::setMode(uint8_t pin, uint8_t mode)
{
    uint8_t portNum = pin / 8;
    if (portNum >= sizeof(_mode)) return;
    if (mode)
    {
        _mode[portNum] |= 1 << (pin - (8 * portNum));  
    }
    else 
    {
        _mode[portNum] &= ~(1 << (pin - (8 * portNum))); 
    }
    writeI2C((IO_CONFIG_BANK0 + portNum), _mode[portNum]); //0x18 first IO Register plus register num
}

/*!
    @brief  Set input or output of one whole port.
    @param  portNum
            Number of the port. 0 - 4.
    @param  port
            Bitmask for setting mode. Output = 1. Input = 0.
    @return None (void). 
*/
void PCA9698::setModePort(uint8_t portNum, uint8_t port)
{
    if (portNum >= sizeof(_mode)) return;
    _mode[portNum] = port;
    writeI2C((IO_CONFIG_BANK0 + portNum), _mode[portNum]);
}

/*!
    @brief  Set mode of multiple ports.
    @param  ports
            Array of bitmasks for setting modes. Size of array max. 5.
    @param  length
            Size of ports.
    @return None (void). 
*/
void PCA9698::setModePorts(uint8_t *ports, uint8_t length)
{
    if (length > sizeof(_mode)) return;
    memcpy(_mode, ports, length);
    writeI2C(IO_CONFIG_BANK0 | AUTO_INCREMENT, _mode, sizeof(_mode));
}

/*!
    @brief  Activate or deactivate single pin interrupt.
    @param  pin
            Pin number. 0 - 39.
    @param  mode
            Interrupt state. INT on = 1. INT off = 0.
    @return None (void). 
*/
void PCA9698::setINT(uint8_t pin, uint8_t mode)
{
    uint8_t portNum = pin / 8;
    if (portNum >= sizeof(_mode)) return;
    uint8_t port = readI2C(MASK_INTERRUPT_BANK0 + portNum);
    port &= ~(1 << (pin - (8 * portNum)));
    if (mode) port |= (1 << (pin - (8 * portNum)));
    writeI2C((MASK_INTERRUPT_BANK0 + portNum), port); 
}

/*!
    @brief  Set INT mask of one whole port.
    @param  portNum
            Number of the port. 0 - 4.
    @param  port
            Bitmask for setting INT. INT on = 1. INT off = 0.
    @return None (void). 
*/
void PCA9698::setINTPort(uint8_t portNum, uint8_t port)
{
    if (portNum >= 5) return;
    writeI2C((MASK_INTERRUPT_BANK0 + portNum), port);
}

/*!
    @brief  Set INT mask of multiple ports.
    @param  ports
            Array of bitmasks for setting INT. Size of array max. 5.
    @param  length
            Size of ports.
    @return None (void). 
*/
void PCA9698::setINTPorts(uint8_t *ports, uint8_t length)
{
    if (length > 5) return;
    writeI2C(MASK_INTERRUPT_BANK0 | AUTO_INCREMENT, ports, length);
}

/*!
    @brief  Set single output pin to 1 or 0.
    @param  pin
            Pin number. 0 - 39.
    @param  state
            Output = 1. Input = 0.
    @return None (void). 
*/
void PCA9698::digitalWrite(uint8_t pin, uint8_t state)
{
    uint8_t portNum = pin / 8;
    if (portNum >= sizeof(_outputPort)) return;
    if (state)
    {
        _outputPort[portNum] |= 1 << (pin - (portNum * 8));
    }
    else 
    {
        _outputPort[portNum] &= ~(1 << (pin - (portNum * 8)));
    }
}

/*!
    @brief  Set the state of one whole port.
    @param  portNum
            Number of the port. 0 - 4.
    @param  port
            Bitmask of the port. Output = 1. Input = 0.
    @return None (void). 
*/
void PCA9698::digitalWritePort(uint8_t portNum, uint8_t port)
{
    if (portNum >= sizeof(_outputPort)) return;
    _outputPort[portNum] = port;
}

/*!
    @brief  Set the state of multiple ports.
    @param  ports
            Array of bitmasks for setting states. Size of array max. 5.
    @param  length
            Size of ports.
    @return None (void). 
*/
void PCA9698::digitalWritePorts(uint8_t *ports, uint8_t length)
{
    if (length > sizeof(_outputPort)) return;
    memcpy(_outputPort, ports, length);
}

/*!
    @brief  Get the state of a single output pin.
    @param  pin
            Pin number. 0 - 39.
    @return State of the output pin. 1 or 0. 
*/
uint8_t PCA9698::getOutputPin(uint8_t pin)
{
    uint8_t portNum = pin / 8;
    if (portNum >= sizeof(_outputPort)) return 0;
    if (_outputPort[portNum] & (1 << (pin - (8 * portNum)))) return 1;
    return 0;
}

/*!
    @brief  Get the state of a single port.
    @param  portNum
            Number of the port. 0 - 4.
    @return State of a single port as byte. 
*/
uint8_t PCA9698::getOutputPort(uint8_t portNum)
{
    if (portNum >= sizeof(_outputPort)) return 0;
    return _outputPort[portNum];
}

/*!
    @brief  Get the state of multiple ports. Ports will be copied in array.
    @param  ports
            Destination array. Data will be copied here. Size of array max. 5.
    @param  length
            Size of ports.
    @return None (void). 
*/
void PCA9698::getOutputPorts(uint8_t *ports, uint8_t length)
{
    if (length > sizeof(_outputPort)) return;
    memcpy(ports, _outputPort, length);
}

/*!
    @brief  Toggle one single output pin.
    @param  pin
            Pin number. 0 - 39.
    @return None (void). 
*/
void PCA9698::togglePin(uint8_t pin)
{
    uint8_t portNum = pin / 8;
    if (portNum >= sizeof(_outputPort)) return;
    _outputPort[portNum] ^= 1 << (pin - (portNum * 8));
}

/*!
    @brief  Toggle one single output port.
    @param  portNum
            Number of port. 0 - 4.
    @param  port
            Bitmask for toggling pins. 1 = toggle.
    @return None (void). 
*/
void PCA9698::togglePort(uint8_t portNum, uint8_t port)
{
    if (portNum >= sizeof(_outputPort)) return;
    _outputPort[portNum] ^= port;
}

/*!
    @brief  Toggling of multiple ports.
    @param  ports
            Array of bitmasks for toggling pins. Size of array max. 5.
    @param  length
            Size of ports.
    @return None (void). 
*/
void PCA9698::togglePorts(uint8_t *ports, uint8_t length)
{
    if (length > sizeof(_outputPort)) return;
    uint8_t port;
    for (uint8_t portNum = 0; portNum < length; portNum++)
    {
        port = ports[portNum];
        togglePort(portNum, port);
    }
}

/*!
    @brief  Read one single pin.
    @param  pin
            Pin number. 0 - 39.
    @return 1 = High. 0 = Low. 
*/
uint8_t PCA9698::digitalRead(uint8_t pin)
{
    uint8_t portNum = pin / 8;
    if (portNum >= sizeof(_inputPort)) return 0;
    if (_inputPort[portNum] & (1 << (pin - (8 * portNum)))) return 1;
    return 0;
}

/*!
    @brief  Read one single port.
    @param  portNum
            Port number. 0 - 4.
    @return Input bitmask as byte. 
*/
uint8_t PCA9698::digitalReadPort(uint8_t portNum)
{
    if (portNum >= sizeof(_inputPort)) return 0;
    return _inputPort[portNum];
}

/*!
    @brief  Read input of multiple ports. 
    @param  ports
            Destination array. Data will be copied here. Size of array max. 5.
    @param  length
            Size of ports.
    @return None (void). 
*/
void PCA9698::digitalReadPorts(uint8_t *ports, uint8_t length)
{
    if (length > sizeof(_inputPort)) return;
    memcpy(ports, _inputPort, length);
}

/*!
    @brief  Updates all output pins.  
    @return None (void). 
*/
void PCA9698::updateOutput()
{
    writeI2C(0x88, _outputPort, sizeof(_outputPort));
}

/*!
    @brief  Reads all input pins and saves them to RAM.
    @return None (void). 
*/
void PCA9698::updateInput()
{
    readI2C(AUTO_INCREMENT, _inputPort, sizeof(_inputPort));
}

/*!
    @brief  Updates all output pins and reads all input pins.  
    @return None (void). 
*/
void PCA9698::updateAll()
{
    writeI2C(OUTPUT_PORT_BANK0 | AUTO_INCREMENT, _outputPort, sizeof(_outputPort));
    readI2C(AUTO_INCREMENT, _inputPort, sizeof(_inputPort));
}
