#ifndef EGE_CORE_DEVICE_IODEVICE_H
#define EGE_CORE_DEVICE_IODEVICE_H

/*! Interface for different kinds of I/O devices.
 *  As such, I/O devices are capable of reading and writing data.
 */

#include "EGE.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IODevice
{
  public:

    virtual ~IODevice() {}

    /*! Writes data to the device.
     *  @param  data    Data source.
     *  @param  length  Number of bytes to write to the device.
     *  @return Number of bytes actually written to the device. This number can be smaller than requested length.
     *          Negative value is returned in case of an error.
     */
    virtual s64 write(const void* data, s64 length) = 0;

    /*! Reads data from the device.
     *  @param  data    Buffer where read data is to be stored.
     *  @param  length  Number of bytes to be read from the device.
     *  @return Number of bytes actually read from the device. This number can be smaller than requested length.
     *          Negative value is returned in case of an error.
     */
    virtual s64 read(void* data, s64 length) = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_DEVICE_IODEVICE_H