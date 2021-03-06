/***************************************************************************
  tag: Peter Soetens  Thu Oct 10 16:22:43 CEST 2002  AnalogDrive.hpp

                        AnalogDrive.hpp -  description
                           -------------------
    begin                : Thu October 10 2002
    copyright            : (C) 2002 Peter Soetens
    email                : peter.soetens@mech.kuleuven.ac.be

 ***************************************************************************
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
/* Klaas Gadeyne, Mon August 11 2003
   - Adapted to new AnalogOutInterface API
*/

#ifndef DEVICE_DRIVERS_ANALOGDRIVE_HPP
#define DEVICE_DRIVERS_ANALOGDRIVE_HPP

#include <ocl/OCL.hpp>
#include "rtt/dev/AnalogOutInterface.hpp"
#include "rtt/dev/DigitalOutInterface.hpp"
#include "rtt/dev/DriveInterface.hpp"
#include "rtt/dev/AnalogOutput.hpp"
#include "rtt/dev/DigitalOutput.hpp"
#include <rtt/Event.hpp>

#include <math.h>

extern "C" int isnan(double);

namespace OCL
{
    /**
     * @brief A AnalogDrive Object converts a physical unit (position, torque,...)
     * to an analog output. It can also be enabled.
     *
     * Uses an analog output which is connected to
     * a hardware drive with position/velocity/acceleration/torque control
     * and the enabling of the drive through a digital output.
     */
    class AnalogDrive
        : public DriveInterface
    {
        typedef double InputStruct;

    public:
        /**
         * Construct an Analog Drive object and aggregate
         * \a an_out and \a dig_out. The voltage sent to the AnalogOutput
         * is calculated as :
         * @verbatim
         * volt = (setpoint + offset)/scale
         * @endverbatim
         * Example for a velocity drive :
         * @verbatim
         * scale = 10 mm/s/Volt
         * offset = 5
         * velocity = X
         * output   = Y
         *  Y Volt = ( - X (mm/s) + 5 (mm/s) )/ ( 10 ( mm/s/ Volt ) )
         * @endverbatim
         *
         * @param _scale The scale of unit per volt such that unit = volt * _scale
         * @param _offset The offset to be added to the unit such that new_unit = vel + offset
         */
        AnalogDrive( AnalogOutput* an_out,
               DigitalOutput* dig_out, double _scale=1.0, double _offset=0.0 )
            : analogDevice( an_out ),
              enableDevice(dig_out), mySpeed(0.0),
              scale(_scale), offset( _offset ),
              lowvel( an_out->lowest()*scale),
              highvel( an_out->highest()*scale)
        {
            driveSet(0);
            disableDrive();
        }

        ~AnalogDrive()
        {
            driveSet(0);
            disableDrive();

            delete analogDevice;
            delete enableDevice;
        }

        /**
         * Enable the drive.
         */
        bool enableDrive()
        {
            enableDevice->switchOn();
            return enableDevice->isOn();
        }

        /**
         * Disable the drive.
         */
        bool disableDrive()
        {
            enableDevice->switchOff();
            return !enableDevice->isOn();
        }

        DigitalOutput* getEnable()
        {
            return enableDevice;
        }

        bool isEnabled() const { return enableDevice->isOn(); }

        /**
         * add an offset to the current offset
         */
        void addOffset(double addoffset)
        {
            offset += addoffset;
        }


        /**
         * get the current offset
         */
        double getOffset()
        {
            return offset;
        }


        /**
         * Limit the velocity of the drive.
         */
        void limit(double lower, double higher, const Event<void(std::string)>& ev)
        {
            lowvel = lower;
            highvel = higher;
            event = ev;
        }

        /**
         * Sets a new velocity.
         */
        bool driveSet( double v )
        {
            if ( enableDevice->isOn() == false )
                return false;

            // limit the speed send to robot;
            if ( v+offset > lowvel && v+offset < highvel && !isnan(v) && !isnan(offset) && !isnan(scale)){
                mySpeed = v;
                analogDevice->value( (mySpeed+offset)/scale );
            }
            else{
                log(Error)<<"Drive value: "<<v<<endlog();
                event("Axis drive value exceeds boundaries");
            }
            return true;
        }

        /**
         * Returns the current drive velocity.
         */
        double driveGet() const
        {
            return mySpeed;
        }

        /**
         * Returns the maximum drive velocity without limitations.
         */
        double maxDriveGet() const
        {
            return analogDevice->highest()*scale-offset;
        }

        /**
         * Returns the minimum drive velocity without limitations.
         */
        double minDriveGet() const
        {
            return analogDevice->lowest()*scale-offset;
        }

    protected:

        AnalogOutput* analogDevice;
        DigitalOutput* enableDevice;

        double mySpeed;
        double scale, offset;
        double lowvel, highvel;
        Event<void(std::string)> event;
    };

}

#endif
