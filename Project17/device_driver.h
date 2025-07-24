#pragma once
#include "flash_memory_device.h"
#include "stdexcept"

class ReadFailException : public std::runtime_error {
public:
    // ������: ���� �޽����� ���޹���
    explicit ReadFailException(const std::string& message)
        : std::runtime_error("Read failed: " + message) {}
};

class WriteFailException : public std::runtime_error {
public:
    // ������: ���� �޽����� ���޹���
    explicit WriteFailException(const std::string& message)
        : std::runtime_error("write failed: " + message) {}
};

class DeviceDriver
{
public:
    DeviceDriver(FlashMemoryDevice* hardware);
    int read(long address);
    void write(long address, int data);

protected:
    FlashMemoryDevice* m_hardware;
};