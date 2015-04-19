#ifndef EGE_CORE_DATA_TESTS_UNITTESTS_IODEVICE_MOCK_H
#define EGE_CORE_DATA_TESTS_UNITTESTS_IODEVICE_MOCK_H

#include <EGEIODevice.h>
#include <gmock/gmock.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IODeviceMock : public IODevice
{
  public:

    IODeviceMock() {}
    virtual ~IODeviceMock() {}

    MOCK_METHOD2(write, s64 (const void* data, s64 length));
    MOCK_METHOD2(read, s64 (void* data, s64 length));
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_DATA_TESTS_UNITTESTS_IODEVICE_MOCK_H