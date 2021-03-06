/**
 * @file singlebatt.hpp
 * @brief Defines a battery class.
 *
 * A battery is consist of three cells and switches.
 * Battery provides a output voltage and output current when
 * connected to a load. It discharges the cells untill
 * a cutoff voltage is reached.
 *
 * @author Subir Biswas
 * @date 24/04/2016
 * @see singlebatt.cpp
 */
#ifndef  BATTERYSIN_CLASS
#define  BATTERYSIN_CLASS

//forword declaration of Battery class
class cBattery;

#include <mutex>	// std::mutex


/**
 * @brief defines a signle battery
 *
 * get & set different values
 *
 * @see battery
 **/

class cSingleBatt
{
	public:
		cSingleBatt();
		bool setInitialVoltage(double initv);
		bool setSeriesResistance(double sres);
		bool setCapacity(double cap);
		bool lock(cBattery* owner);
		bool unlock(cBattery* owner);
		bool update(cBattery* owner,bool connected, double scurrent, double runtime);
		double getInitialVoltage(void);
		double getSeriesResistance(void);
		double getSourceCurrent(void);
		double getCapacity(void);
		double getRemainingCapacityPercentage(void);
		bool loadDefaults(cBattery* owner);		
		double getCurrentVoltage(void);
	private:
		bool Locked;				///<Denotes the cell is connected to a battery and the parameters are locked
		cBattery* AttachedTo;		///<Denotes which battery it is connected to
		double InitialVoltage;	///<Initial voltage of the cell inn Volts. @see setInitialVoltage @see getInitialVoltage
		double SeriesResistance;	///<Series resistance of the cell in Ohms. @see setSeriesResistance @see getSeriesResistance
		double Capacity;			///<Initial capacity of the cell in AmS. @see setCapacity @see getCapacity
		double Shift;		///<First gradient change in the discharge curve. expressed as percentage of descharged capacity.
		double Drop;				///<Voltage drop at shift1. expressed as percentage
		double Gradient;			///<Current slop of the discharge curve. Measured as Voltage per Capacity
		double ConstantK;			///<Constant factor of discharge curve equation. Measeured as Volt.
		double m1;			///<One of three relative gradients. Constant ratio.
		double DischargedCapacity;  ///<Capacity of the cell already discharged. Unit AmS.
		double RemainigCapacity;	///<Percentage of capacity remaining. (%)
		double SourceCurrent;		///<Current sourced by the cell in Ampere.
		double CurrentVoltage;		///<Current voltage of the cell in Volts.
		void initialise(void);
		std::mutex mtx; 		///<Lock to synchronize access to members from different thread and unlock

};

#endif //BATTERYSIN_CLASS
