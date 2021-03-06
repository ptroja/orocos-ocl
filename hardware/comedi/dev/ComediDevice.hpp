// Copyright (C) 2002 Peter Soetens
// Copyright (C) 2003,2007 Klaas Gadeyne
/***************************************************************************
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the Free Software   *
 *   Foundation, Inc., 59 Temple Place,                                    *
 *   Suite 330, Boston, MA  02111-1307  USA                                *
 *                                                                         *
 ***************************************************************************/

/* Klaas Gadeyne, August 14, 2003
   Major rewrite of original file by Peter Soetens
*/

#ifndef OCL_COMEDIDEVICE_HPP
#define OCL_COMEDIDEVICE_HPP

#include <boost/shared_ptr.hpp>

namespace OCL
{
  /**
   * Minimal C++ API to a comedi device.
   * You require the ComediSubDevice family classes in order to
   * access subdevices.
   * @see http://www.comedi.org
   */
  class ComediDevice
  {
  protected:
      class DeviceInfo;
      boost::shared_ptr<DeviceInfo> d;
  public:
    typedef unsigned int Data;

    ComediDevice( unsigned int minor );
    virtual ~ComediDevice( );

    /**
       Returns the maximum valid data value for channel chan of
       subdevice subdevice belonging to this device. This function
       returns 0 on error.
       @return Maximum data value on that subdevice (always unsigned int
       in comedi)
       @param subdevice : the number of the subdevice to interrogate
     */
    Data getMaxData(unsigned int subd);

    /**
       Read a single sample on the channel specified by the subdevice
       subdevice, and the channel channel.  For the A/D conversion (if
       appropriate), the device is configured to use range
       specification range and (if appropriate) analog reference type
       aref.  Analog reference types that are not supported by the
       device are silently ignored.  The function reads one data value
       from the specified channel and places the data value in the
       location pointed to by data.

       Data values returned by this function are unsigned integers
       less than or equal to the maximum sample value of the channel,
       which can be determined using the function getMaxData().

       @return 0 on succes (not with all drivers, unfortunately, -1 on error
       @param subd : the number of the subdevice to read on
       @param chanNr : the number of the channel of subdevice subdevice to read on
       @param value : the data
       @param range : number between 0 and get_num_ranges(), @see get_num_ranges()
       @param aref : possible options (see comedi doc) AREF_GROUND,
       AREF_COMMON, AREF_DIFF, AREF_OTHER
    */
    int read( unsigned int subd, unsigned int chanNr,
	      unsigned int range, unsigned int aref, Data& value );

    /*
      Writes a single sample on the channel that is specified by the
      subdevice subdevice, and the channel channel.  If appropriate,
      the device is configured to use range specification range and
      analog reference type aref.  Analog reference types that are not
      supported by the device are silently ignored.  The function
      writes the data value specified by the parameter data to the
      specified channel.

      @return 0 on succes (not with all drivers, unfortunately, -1 on error
      @param subd : the subdevice to write on
      @param chanNr : the channel of subdevice subdevice to write on
      @param value : the data sample to write
      @param range : number between 0 and get_num_ranges(), @see get_num_ranges()
      @param aref : possible options (see comedi doc) AREF_GROUND,
      AREF_COMMON, AREF_DIFF, AREF_OTHER
    */
    int write( unsigned int subd, unsigned int chanNr,
	       unsigned int range, unsigned int aref,
	       const ComediDevice::Data& value);

    /**
       Returns an integer describing the type of subdevice that belongs to this comedi device
       and has the index subdevice.


       @return an integer describing the type of subdevice, -1 if there is an error.
       @param subd : the number of the subdevice of which the type has to be retrieved
    */
    int getSubDeviceType(unsigned int subd);

    /**
       Return a pointer to a comedi device info.
       This may be null if the device could not be found.
    */
    DeviceInfo* getDevice();

  };


};

#endif
