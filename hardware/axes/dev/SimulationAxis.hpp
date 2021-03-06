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

#ifndef _SIM_AXIS_HPP
#define _SIM_AXIS_HPP

#include <ocl/OCL.hpp>
#include <rtt/dev/AxisInterface.hpp>
#include <rtt/dev/DigitalOutput.hpp>
#include <rtt/TimeService.hpp>

namespace OCL
{
    /** @brief Helper class that implements a Position Sensor for a
	SimulationAxis
	@see SimulationAxis
    */
    class SimulationEncoder: public SensorInterface<double>
    {
    public:
	SimulationEncoder(double initial=0, double min=-10, double max=10);
	virtual ~SimulationEncoder() {};

	virtual double readSensor() const;
	virtual int readSensor(double& data) const;
	virtual double maxMeasurement() const {return _max; };
	virtual double minMeasurement() const {return _min; };
	virtual double zeroMeasurement() const {return 0.0;};

	void setDrive(double velocity);

    private:
	double _position, _velocity, _min, _max;
	TimeService::ticks _previous_time;
	TimeService::Seconds _delta_time;
	/** The value of _first_drive is true as long as the setDrive()
	    method has not been called
	*/
	bool _first_drive;

    }; // class



    // Forward declare; see below
    class SimulationVelocitySensor;

    /** @brief Simple implementation of a non-physical axis for
	simulation of a machine/robot
     */
    class SimulationAxis: public AxisInterface
    {
    public:
	SimulationAxis(double initial=0, double min=-10, double max=10);
	virtual ~SimulationAxis();

	virtual bool stop();
	virtual bool lock();
	virtual bool unlock();
	virtual bool drive( double v );
	virtual bool isLocked() const;
	virtual bool isStopped() const;
	virtual bool isDriven() const;
	virtual SensorInterface<double>* getSensor(const std::string& name) const;
	virtual std::vector<std::string> sensorList() const;
	virtual SensorInterface<int>* getCounter(const std::string& name) const { return  0;}
	virtual std::vector<std::string> counterList() const { return std::vector<std::string>();}
	virtual DigitalInput* getSwitch(const std::string& name) const { return 0; }
	virtual std::vector<std::string> switchList() const { return std::vector<std::string>();}

	double getDriveValue() const;
	void   setMaxDriveValue( double v_max ) { _max_drive_value = v_max; }

    DigitalOutput* getBrake();
    DigitalOutput* getEnable();

    private:
	double      _drive_value;
	DigitalOutput  _enable;
	DigitalOutput  _brake;
	double      _velocity;
	double      _max_drive_value;
	SimulationEncoder*         _encoder;
	SimulationVelocitySensor*  _velSensor;
	bool _is_locked, _is_stopped, _is_driven;


    }; // class

    /** @brief Helper class that implements a Velocity Sensor for a
	SimulationAxis
    */
    class SimulationVelocitySensor : public SensorInterface<double>
    {
    public:
	SimulationVelocitySensor(SimulationAxis* axis, double maxvel) : _axis(axis), _maxvel(maxvel)
	{};

	virtual ~SimulationVelocitySensor() {} //std::cout << "SimulationVelocitySensor destructor" << std::endl; }

	virtual int readSensor( double& vel ) const { vel = _axis->getDriveValue(); return 0; }

	virtual double readSensor() const { return _axis->getDriveValue(); }

	virtual double maxMeasurement() const { return _maxvel; }

	virtual double minMeasurement() const { return - _maxvel; }

	virtual double zeroMeasurement() const { return 0; }

    private:
	SimulationAxis* _axis;
	double _maxvel;
    };


} // namespace

#endif //_SIM_AXIS_HPP



