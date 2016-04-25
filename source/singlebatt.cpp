/**
 * @file cell.cpp
 * @brief Implementation of the cell class
 *
 * Cell is the primary element of a battery.
 * A battery contains multiple cells connected in parallel.
 * Characteristics of a cell like discharge curve, capacity
 * etc decides the characteristics of the battery.
 * A cell should be added to a battarey for operation.
 * @author Dipta Pandit
 * @date 3/12/2014
 * @see cell.hpp
 */
#include "../header/singlebatt.hpp"

/**
 * @brief Constructor of a cell object
 *
 * Creates a cell object and initialize it with some default value
 * @param void
 * @return void
 */
SingleBatt::SingleBatt()
{
	Capacity = 800*3600;		//800 mAH
	SeriesResistance = 0.001; 	//10 mOhm
	InitialVoltage = 12;		//12 V
	Shift = 10;
	Drop = 5;
	AttachedTo = (Battery*)0;
	Locked = false;
}
/**
 * @brief Sets the initialvoltage of the cell
 *
 * Initial voltage is the voltage that the cell voltage
 * starts with when it is updated.
 * Cellvoltage will be set to this voltage when the cell is resetted.
 *
 * @param double initial voltage to set in Volts
 * @return bool true if successfully set the initial voltage
 * false if the cell is locked.
 */
bool SingleBatt::setInitialVoltage(double initv)
{
	if(Locked)
		return false;
	InitialVoltage = initv;
	return true;
}

/**
 * @brief Sets the internal series resistance of the cell
 *
 * series resistance is the fixed resistance of the cell
 * that can be assumed is connected in series with the cell
 * when it is operatied
 *
 * @param double series resistance to be set in Ohms
 * @return bool true if successfully set the series resistance
 * false if the cell is locked.
 */
bool SingleBatt::setSeriesResistance(double sres)
{
	if(Locked)
		return false;
	SeriesResistance = sres;
	return true;
}

/**
 * @brief Sets the capacity of the cell
 *
 * capacity is the ability of the cell torun for a
 * particular time when delivering a particulat current.
 * It is the product of amount of current it can deliver and for the time
 *
 * @param double cap capacity to set in mAH
 * @return bool true if successfully set the capacity
 * false if the cell is locked.
 */
bool SingleBatt::setCapacity(double cap)
{
	if(Locked)
		return false;
	Capacity = cap * 3600;
	return true;
}

/**
 * @brief locks the cell to a battery
 *
 * @param Battery Pointer to the battery who wants the lock
 * @return bool true if successfully locked.
 * false if it is already locked
 */
bool SingleBatt::lock(Battery* owner)
{
	if(Locked)
		return false;
	AttachedTo = owner;
	Locked = true;
	initialise();
	return true;
}
/**
 * @brief unlocks the cell from its battery
 *
 * @param Battery Owner of the cell
 * @return bool true if successfully unlocked.
 * false if owner does not own the battery or it is not locked
 */
bool SingleBatt::unlock(Battery* owner)
{
	if(!Locked)
		return false;
	if(AttachedTo != owner)
		return false;
	AttachedTo = (Battery*)0;
	Locked = false;
	return true;
}

/**
 * @brief Returns the initial voltage of the cell
 *
 * @param void
 * @return double voltage in Volts
 */
double SingleBatt::getInitialVoltage(void)
{
	double result;
	mtx.lock();
	result = InitialVoltage;
	mtx.unlock();
	return result;
}

/**
 * @brief Returns the series resistance of the cell
 *
 * The series resistance is a constant resistance known
 * as internal series resistance or equvalant series resistance
 * @param void
 * @return double resistance in Ohms
 */
double SingleBatt::getSeriesResistance(void)
{
	double result;
	mtx.lock();
	result = SeriesResistance;
	mtx.unlock();
	return result;
}

/**
 * @brief Returns the current being sourced by the cell
 *
 * The current current being sourced by the cell is returned in Ampere
 * @param void
 * @return double current in A
 */
double SingleBatt::getSourceCurrent(void)
{
	double result;
	mtx.lock();
	result = SourceCurrent;
	mtx.unlock();
	return result;
}

/**
 * @brief Returns the capacity of the cell
 *
 * @param void
 * @return double capacity of the cell in mAH
 */
double SingleBatt::getCapacity(void)
{
	double result;
	mtx.lock();
	result = Capacity/3600;
	mtx.unlock();
	return result;
}

/**
 * @brief Returns the remaining capacity of the cell as percentage
 *
 * @param void
 * @return double remaining capacity as percentage.
 */
double SingleBatt::getRemainingCapacityPercentage(void)
{
	return RemainigCapacity;
}

/**
 * @brief Initializes the battery characteristics parameters
 *
 * Sets the discharge curve characteristics and gradients
 * @param void
 * @return void
 */
void SingleBatt::initialise(void)
{
	mtx.lock();
	m1 = (InitialVoltage * Drop) / (Capacity * Shift);

	CurrentVoltage = InitialVoltage;
	DischargedCapacity = 0;
	RemainigCapacity = 100;
	Gradient = m1;
	ConstantK = 0;
	mtx.unlock();
	return;
}

/**
 * @brief Updates the cell parameters
 *
 * Updates the cell parameters as if it is connected to a output
 * voltage for specific time. This actually implements the three
 * stage liniar approximation of the discharge curve of the cell
 *
 * @param  owner Owner of te cell
 * @param connected Weather or not the cell is connected
 * @param vout 	 The output voltage it is connected to in Volt
 * @param runtime For how long it was connected in milisec
 * @return true if Successfully update the cell.false if
 * The cell is not owned by the battery or runtime is 0
 */
bool SingleBatt::update(Battery* owner,bool connected, double scurrent, double runtime)
{
	if(AttachedTo != owner)
		return false;
	if(0 == runtime)
		return false;
	if(!connected)
	{
		SourceCurrent = 0;
		return true;
	}
	mtx.lock();
	SourceCurrent = scurrent;
	DischargedCapacity += (SourceCurrent * runtime);
	RemainigCapacity = ((Capacity - DischargedCapacity) / Capacity) * 100;
	CurrentVoltage = CurrentVoltage - Gradient*SourceCurrent*runtime + ConstantK;

	if((Gradient == m1) && (DischargedCapacity >= ((Shift*Capacity)/100)))
	{
		//Gradient = m2;
		//ConstantK += (m2 - m1)*DischargedCapacity;
		ConstantK += (m1)*DischargedCapacity;
	}

	mtx.unlock();
	return true;
}

/**
 * @brief Loads the default values of the cell
 *
 * Resets the cell voltage to initial voltage
 * and, capacity to its full.
 *
 * @param owner Pointer to the Battery pack that the caell is attached to
 * @return true if successfuly loaded the default values.false if The battrey does not own the cell.
 */
bool SingleBatt::loadDefaults(Battery* owner)
{
	if(owner != AttachedTo)
		return false;
	mtx.lock();
	CurrentVoltage = InitialVoltage;
	RemainigCapacity = 100;
	DischargedCapacity = 0;
	Gradient = m1;
	ConstantK = 0;
	mtx.unlock();
	return true;
}

/**
 * @brief Returns the current voltage level of the cell
 *
 * Curent voltage denotes the voltagelevel the cell is
 * having at any particular time. It si set to initial
 * voltage when the cell is resetted
 *
 * @param void
 * @return double current voltage in Volts
 */
double SingleBatt::getCurrentVoltage(void)
{
	double result;
	mtx.lock();
	result = CurrentVoltage;
	mtx.unlock();
	return result;
}

