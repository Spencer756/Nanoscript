/** \file NanoScript_Litho.h 
*	\brief NanoScript functions for nano lithography
*
*	System must be engaged (or false engaged) to use litho functions.
*
*	Use the #define macrcos LITHO_BEGIN and LITHO_END to bracket litho code.
*	They do important initialization, cleanup, catch exceptions, 
*	and allow programmatic aborting.
*
*	Soft units are typically the signal's software repesentation,
*	such as nanometers.  Hard units are measured by the hardware, typically volts.
*
*	Functions with boolean returns values, return true for success,
*	unless otherwise noted.
*
* Copyright Bruker 1988-2011. All rights reserved.
*/


// Comments in this file follow Doxygen syntax, 
// for external use in documentation. When reading this file directly,
// you can ignore commented words that start with "\", such as "\brief"

#ifndef __NANOSCRIPT_LITHO_H__
#define __NANOSCRIPT_LITHO_H__

#ifdef _MSC_VER
#pragma once
#endif

#ifndef NS_API
#define NS_API __declspec(dllexport)
#endif

#include <exception>
#include <string>
#include <iostream>

using namespace std;

/** \brief List of signals available to certain Lithographie commands
*
* \note The availability of certain signals inputs or outputs depends
* on the selected mode.
* \note Not all signals are available in every mode.
*/
enum LithoSignal
{	
	lsX = 0,			/**<
						* I/O direction: output \n
						* Unit: V or nm (closed loop only) \n
						* Availability:
						* \li all modes as <b>X drive</b>
						*/

	lsY,				/**<
						* I/O direction:output \n
						* Unit: V or nm (closed loop only) \n
						* Availability:
						* \li all modes as <b>Y drive</b>
						*/

	lsZ,				/**<
						* I/O direction: output \n
						* Unit: V or nm \n
						* Availability:
						* \li all modes as <b>relative Z drive</b>
						* Description: Relative Z drive. Positive values indicate
						* a retracted piezo. A value of 0 indicates that the piezo
						* is at the center of the Z limit range. \n
						*/

	lsZlimit,			/**<
						* I/O direction: output \n
						* Unit: V or nm \n
						* Availability:
						* \li all modes as <b>Z MDAC</b>
						*/

	lsBias,				/**<
						* I/O direction: output \n
						* Unit: mV \n
						* Availability:
						* \li AFM mode(s) as <b>sample bias</b>
						* \li STM mode(s) as <b>STM bias</b>
						*/

	lsSetpoint,			/**<
						* I/O direction: output \n
						* Unit: V (AFM Modes) or nA (STM Mode) \n
						* Availability:
						* \li Contact mode as <b>deflection setpoint</b>
						* \li STM mode(s) as <b>current setpoint</b>
						* Notes:
						* \li Not available in Tapping mode
						*/

	lsAna1,				/**<
						* I/O direction: output \n
						* Unit: V \n
						* Availability:
						* \li Contact mode as <b>Analog 1</b>
						* \li Tapping mode as <b>tapping oscillator</b>
						* \li STM mode(s) as <b>application module voltage / DDS1</b>
						* Notes:
						* \li Not available in TUNA (ext)
						*/

	lsAna2,				/**<
						* I/O direction: output \n
						* Unit: V \n
						* Availability:
						* \li All modes as <b>tip bias (DDS2)</b>
						* Notes:
						* \li Not available in TUNA (ext)
						* \li Tip bias has to be enabled in the application for this
						* to be applied correctly. Please consult the Nanoscope documentation
						* for more information on tip bias control.
						*/

	lsAna2HV,			/**<
						* I/O direction: output \n
						* Unit: V \n
						* Availability:
						* \li All modes as <b>tip bias (DDS2)</b>
						* Notes: 
						* \li Not available in TUNA (ext)
						* \li This is the same as lsAna2 but only
						* with a different scaling factor.
						*/

	lsAna3,				/**<
						* I/O direction: output \n
						* Unit: V \n
						* Availability:
						* \li All modes as <b>Analog 3</b> or 
						* <b>application module voltage</b>
						* Notes:
						* \li Not available in TUNA (ext)
						*/

	lsAna4,				/**<
						* I/O direction: output \n
						* Unit: V \n
						* Availability:
						* \li All modes as <b>Analog 4</b>, <b>application module voltage</b>,
						* or <b>PicoForce Angler voltage</b>
						* Notes:
						* \li Not available in TUNA (ext)
						*/

	lsIn0,				/**<
						* I/O direction: input \n
						* Unit: V \n
						* Availability:
						* \li Contact mode as <b>vertical deflection</b>
						* \li Tapping mode as <b>RMS amplitude</b>
						* \li Also as <b>low speed ADC 1 (LSADC1)</b>
						*/

	lsIn1,				/**<
						* I/O direction: input \n
						* Unit: V \n
						* Availability:
						* \li All modes as <b>Input 1</b> unless
						* Fast error, DC2, IN1B <-- What does that mean?
						* Notes:
						* \li BNC user input 1 is routed via LSADC5	<-- Is this correct?
						*/

	lsIn2,				/**<
						* I/O direction: input \n
						* Unit: V \n
						* Availability:
						* \li All modes as <b>Input 2</b> unless
						* Fast Z, DC1 <-- What does that mean?
						* Notes:
						* \li BNC user input 2 <-- Is this correct?
						* \li Routed via LSADC3 <-- Is this correct?
						*/

	lsIn3,				/**<
						* I/O direction: input \n
						* Unit: V \n
						* Availability:
						* \li All modes as <b>Input 3</b> unless
						* DC3, IN1B <-- What does that mean?
						* Notes:
						* \li BNC user lockin 3 or user input 3 <-- Is this correct?
						* \li Routed via LSADC9 <-- Is this correct?
						*/

	lsIn4,				/**<
						* I/O direction: input \n
						* Unit: V \n
						* Availability:
						* \li All modes as <b>Input 4</b> unless SUM <-- What does that mean?
						* Notes:
						* \li Routed via LSADC4 <-- Is this correct?
						*/

	lsAuxA,				/**<
						* I/O direction: input \n
						* Unit: V \n
						* Availability:
						* \li Contact mode as <b>horizontal deflection</b>
						* \li Tapping mode as <b>AuxA</b>
						* Notes:
						* \li BNC user lock-in 2 <-- Is this correct?
						* \li Routed via LSADC2 <-- Is this correct?
						*/

	lsAuxB,				/**<
						* I/O direction: input \n
						* Unit: V \n
						* Availability:
						* \li All modes as <b>X sensor</b>
						* Notes:
						* \li Routed via LSADC6
						*/

	lsAuxC,				/**<
						* I/O direction: input \n
						* Unit: V \n
						* Availability:
						* \li All modes as <b>Y sensor</b>
						* Notes:
						* \li Routed via LSADC7
						*/

	lsAuxD,				/**<
						* I/O direction: input \n
						* Unit: V \n
						* Availability:
						* \li All modes as <b>Z sensor</b>
						* Notes:
						* \li Routed via LSADC8
						*/

	lsZsweep,			/**<
						* I/O direction: internal only \n
						* Unit: nm (force mode) / Zscan signal (ramp mode) \n
						* Availability:
						* \li All modes
						* Notes:
						*/

	lsDriveFreq,		/**<
						* I/O direction: internal only \n
						* Unit: kHz \n
						* Availability:
						* \li Tapping mode as <b>DDS1</b>
						* Notes:
						* \li Not available in Contact mode
						*/

	lsDriveAmpl,		/**<
						* I/O direction: internal only \n
						* Unit: mV \n
						* Availability:
						* \li Tapping mode
						* Notes:
						* \li Same signals as lsDriveFreq (???)
						*/

	lsDrivePhase,		/**<
						* I/O direction: internal only \n
						* Unit: degrees \n
						* Availability:
						* \li Tapping mode
						* Notes:
						* \li Same signals as lsDriveFreq (???)
						*/

	lsIntegralGain,		/**<
						* I/O direction: internal only \n
						* Unit: dimensionless \n
						* Availability:
						* \li All modes
						* Notes:
						* \li Integral Gain feedback
						*/

	lsProportionalGain,	/**<
						* I/O direction: internal only \n
						* Unit: dimensionless \n
						* Availability:
						* \li All modes
						* Notes:
						* \li Proportional Gain feedback
						*/

	lsECBias,			/**<
						* I/O direction: internal use only \n
						* Unit: V <-- Is this correct? \n
						* Availability:
						* \li EC modes
						* Notes:
						* \li Bias for Electro Chemistry mode
						*/

	lsNS5FPInput1,		/**<
						* I/O direction: input
						* Unit: V
						* Availability:
						* \li All modes
						* Notes:
						* \li NS5 Front Panel Input 1
						*/

	lsNS5FPInput2,		/**<
						* I/O direction: input
						* Unit: V
						* Availability:
						* \li All modes
						* Notes:
						* \li NS5 Front Panel Input 2
						*/

	lsNS5FPOutput1,		/**<
						* I/O direction: output
						* Unit: V
						* Availability:
						* \li All modes
						* Notes:
						* \li NS5 Front Panel Output 1
						* \li Output 1 must be configured to 'off'
						* or will get overwritten.
						*/
						
	lsNS5FPOutput2,		/**<
						* I/O direction: output
						* Unit: V
						* Availability:
						* \li All modes
						* Notes:
						* \li NS5 Front Panel Output 2
						* \li Output 2 must be configured to 'off'
						* or will get overwritten.
						*/

	lsCount				// not a real signal. programming trick to mark end of list.
};


/// Trigger lines
enum TriggerLine
{
	tlD0 	= -1,	/*!<	D0		*/
	tlD1 	= -2	/*!<	D1		*/
};


class LithoException : public std::exception
{
public:
	LithoException(const std::string& message) : s(message) {}
	virtual ~LithoException() {}
	virtual const char* what() const throw() { return s.c_str(); }
private:
	std::string s;
};


/// The LITHO_BEGIN and LITHO_END macros should be placed around 
/// the collection of litho calls. They do some important initialization 
/// and provide a try/catch block for catching errors and programmatic aborts.
///
/// example:
///
/// LITHO_BEGIN
///	LithoScan(false);
/// LithoCenterXY();
/// LithoTranslate( size, 0, rate);
/// etc. etc.
/// LITHO_END
/// 

/** \def This macro should be placed at the beginning of a block of lithography commands
*/ 
#define LITHO_BEGIN	\
	try	\
	{ \
		if (LithoBegin()) \
		{

/** \def This macro should be placed at the end of a block of lithography commands		
*/
#define LITHO_END \
		} \
		LithoEnd(); \
	} \
	catch(LithoException& le) \
	{ \
		cout<<("Caught LithoException: %s\n", le.what()); \
		LithoEnd(); \
	}													



/** \brief Abort (Lithographie) Nanoscript
*
* This can bu used inside a block of litho commands
* to jump directly to LITHO_END. Throws a std::exception
* that should be caught in LITHO_END macro pre LithoEnd()
*/
NS_API void LithoAbort();


/** \brief Turn on and off process releases
*
* Used to improve the execution timing for some
* Lithographie commands. This can be helpful for
* time critical applications but locks out all
* user access.
*
* \param allow 'Release' the lithographie process
* (\c TRUE) or 're-capture' it (\c FALSE)
*
* \return \c TRUE if the lithographie process
* is released otherwise \c FALSE.
*/
NS_API bool LithoRelease(bool allow);


/** \brief Determine if the system is scanning
*
* Check if the system is currently scanning in
* SPM XY raster scan mode.
*
* \return \c TRUE if the system is currently
* scanning otherwise \c FALSE.
*/
NS_API bool LithoIsScanning();


/** \brief Turn XY scanning on or off
*
* \param on Activate XY scanning (\c TRUE)
* or deactivate XY scanning (\c FALSE).
*
* \return \c TRUE if the change to the current
* state was successful otherwise \c FALSE.
*/
NS_API bool LithoScan(bool on);
		

/** \brief Move the tip to center of XY scan field
*
* \return \c TRUE if the call was successful
* otherwise \c FALSE.
*/
NS_API bool LithoCenterXY();


/** \brief Turn Z feedback on or off
*
* \param on Turn Z feedback on (\c TRUE)
* or off (\c FALSE)
*
* \return \c TRUE if setting the new Z feedback
* value was successful otherwise \c FALSE.
*/
NS_API bool LithoFeedback(bool on);


/** \brief Determine if LithoFeedback is on or off
*
* \return \c TRUE of LithoFeedback is on. \c FALSE
* if LithoFeedback is off.
*/
NS_API bool LithoIsFeedbackOn();


/** \brief Move the tip in X and/or Y direction
*
* \param dxUm Movement in X direction (delta X) in microns.
* \param dyUm Movement in Y direction (delta Y) in microns.
* \param rateUmPerSec Movement rate for the tip motion
* in microns per second.
*
* \return \c TRUE if the delta movement was successful
* otherwise \c FALSE.
*/
NS_API bool LithoTranslate(
	double dxUm, 				// delta x in microns
	double dyUm, 				// delta y in microns
	double rateUmPerSec);		// rate of tip motion in um/sec


/** \brief Move the tip to specific X and Y position
*
* \param xUm New X position in microns.
* \param yUm New Y position in microns.
* \param rateUmPerSec Movement rate for the tip
* motion in microns per second.
*
* \return \c TRUE if the tip repositioning was
* successful otherwise \c FALSE.
*
* \note Only applies to closed-loop scanners.
*/
NS_API bool LithoTranslateAbsolute(double xUm, double yUm, double rateUmPerSec);


/** \brief Move the tip in Z ('up' or 'down')
* 
* \param dzUm Movement in Z direction (delta Z)
* in microns.
* \param rateUmPerSec Movement rate for the tip
* motion in microns per second.
*
* \return \c TRUE if the delta movement was successful
* otherwise \c FALSE. 
*
* \note Positive values move the tip away from the
* surface negative values move the tip towards the
* surface.
* \note Feedback will automatically be turned off
* during the movement if it is on.
*/
NS_API bool LithoMoveZ(
	double dzUm, 				// delta z in microns
	double rateUmPerSec);		// rate of tip motion in um/sec


/** \brief Pause for the specifed time in seconds
*
* This is a way to improve timing of small wait
* periods ( <= 0.01 s). It performs best when
* used in conjuction with LithoRelease(false).
*
* In most cases the standard Windows' Sleep
* function will perform better and can be used
* over a wider range of wait periods.
*
* However, if your wait period is small and timing
* acuracy is critical, LithoPause is an option.
* You are encouraged to run timing tests specific
* to your experiment to find the optimal solution.
*
* For further information please see the general
* comments about performance issues in the
* Nanoscript documentation.
*
* \param secs The time the script execution shoul
* be paused.
*
* \return \c TRUE when the specified amount of
* time has passed or \c FALSE if an internal
* error occured.
*/
NS_API bool LithoPause(double secs);


/** \brief Set the specified signal in its 'hard' units
*
* \param output Target signal whose value is to be
* changed.
* \param v The new value for the specified signal.
*
* \return \c TRUE if the new value for the specific
* signal was set successfully otherwise \c FALSE.
*
* \note Hard units are typically the singal's
* hardware representation such as volts
*/
NS_API bool LithoSet(LithoSignal output, double v);


/** \brief Set the specified signal in its 'soft' units
*
* \param output Target signal whose value is to be
* changed.
* \param v The new value for the specified signal.
*
* \return \c TRUE if the new value for the specific
* signal was set successfully otherwise \c FALSE.
*
* \note Soft units are typically the signal's
* software repesentation such as nanometers
*/
NS_API bool LithoSetSoft(LithoSignal output, double v);


/** \brief Get value of the specified signal in its 'hard' units
*
* \param input Target signal whose value is to be
* read.
*
* \return The current value of the specified signal
* in its 'hard' units.
*
* \note Hard units are typically the singal's
* hardware representation such as volts
*/
NS_API double LithoGet(LithoSignal input);


/** \brief Get value of the specified signal in its 'soft' units
*
* \param input Target signal whose value is to be
* read.
*
* \return The current value of the specified signal
* in its 'soft' units.
*
* \note Soft units are typically the signal's
* software repesentation such as nanometers
*/
NS_API double LithoGetSoft(LithoSignal input);


/** \brief Ramp the specified output signal
*
* This function gradually increases/decreases
* the value of the specified output signal
* over a specified time.
*
* \param output Target signal whose value is to be
* changed.
* \param startValue The start value for the
* process.
* \param endValue The target value the output
* signal should have when the process finihes.
* \param secs The time the process should take
* to 'ramp' the start value to the end value.
*
* \return \c TRUE if the ramping process was
* executed successfully otherwise \c FALSE.
*
* \note A negative ramp is generated when
* the start value is larger then the end
* value.
* \note The step size for the process is
* calculated internally.
*/
NS_API bool LithoRamp(LithoSignal output, double startValue, double endValue, double secs);


/** \brief Generate timed pulse on the specified output signal
* 
* \param output Target signal whose value is to be
* pulsed.
* \param v The value of the output signal during the
* pulse in volts.
* \param time The 'width' of the pulse in seconds.
*
* \return \c TRUE if the signal pulse was
* generated successfully otherwise \c FALSE.
*
* \note The output returns to its original value
* after the pulse.
* \note Depending on the 'original' value of the
* output signal, both positive and negative pulse
* signals can be generated.
* \note Using this function and a combination of
* loops and wait times various complex signal
* patterns can be generated.
*/
NS_API bool LithoPulse(LithoSignal output, double v, double time);


/** \brief Pause until the specified input signal drops below the specified value
*
* \param input The signal whose value is monitored
* to end the wait period.
* \param v The undershot value for the monitored
* signal. If the value of the signal falls below it
* the wait period ends.
*
* \return \c TRUE once the monitored signal fell
* below the specified value or \c FALSE if a
* failure during the execution occured.
*/
NS_API bool LithoWaitFor(LithoSignal input, double v);


/** \brief Generate a trigger pulse on the specified output
*
* A short - roughly 200ns - trigger pulse is generated
* on the specified line.
*
* \param line The trigger line on which the pulse
* should be generated.
*
* \return \c TRUE if the trigger pulse was generated
* successfully otherwise \c FALSE.
*/
NS_API bool LithoTrigger(TriggerLine line);


/** \brief Get scanner's X position in microns
*
* \return The current X position of the scanner
* in microns.
*/
NS_API double LithoGetXPosUM();	


/** \brief Get scanner's Y position in microns
*
* \return The current Y position of the scanner
* in microns.
*/
NS_API double LithoGetYPosUM();	

///////////////////////////////////////////////////////////////////
// Support functions:

// These functions should only be used by the LITHO_BEGIN and LITHO_END macros.
// Do not call them directly, without thinking through the loss of the try/catch.
// If not using the macros, then these must be called directly by your script.
// They do important initialization and cleanup.

/** \brief Prepare system for lithographie scripts
*
* \return \c TRUE if the initialization process
* was successful otherwise \c FALSE.
*
* \note 
*/
NS_API bool LithoBegin();

/** \brief Cleanup after running lithographie scripts
*/
NS_API void LithoEnd();

#endif // __NANOSCRIPT_LITHO_H__
