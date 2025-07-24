#include "device_driver.h"

DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : m_hardware(hardware)
{}


int DeviceDriver::read(long address)
{
    int ReadValue;
    int PreValue = 0;
    // TODO: implement this method properly
    for (int i = 0; i < 5; i++)
    {
        ReadValue = m_hardware->read(address);
        if (i != 0)
            if (ReadValue != PreValue)
            {
                throw ReadFailException("UECC 발생!");
            }

        PreValue = ReadValue;
    }
    return (int)(ReadValue);
}

void DeviceDriver::write(long address, int data)
{
    int readValue = read(address);
    if (readValue == 0xFF)
        m_hardware->write(address, (unsigned char)data);
    else
        throw WriteFailException("Over Write 시도!");

}