#include "device_driver.h"

class Application
{
public:
	Application(DeviceDriver* dd) : dd(dd) {
	}
	void readAndPrint(long startAddr, long endAddr)
	{
		for (long i = startAddr; i < endAddr; i++)
		{
			dd->read(i);
		}
	}

	void writeAll(unsigned char value)
	{
		for (long addr = 0; addr < MAX_ADDRESS; addr++)
		{
			dd->write(addr, value);
		}
	}

	static const long MAX_ADDRESS = 0x1000;

private:
	DeviceDriver* dd;
};