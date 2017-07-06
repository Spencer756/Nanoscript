// diamondGUI.cpp
// Demo of writing a Nanoscript lithography script with GUI features
// Adds a dialog box to the diamond.cpp example

#include "NanoScript_GUI.h"
#include "NanoScript_Litho.h"

//by Zhiyong
#include "windows.h" // for delay function
#include "iostream" // for file manipulation
#include "fstream"
#include "math.h"
using namespace std;

extern "C" __declspec(dllexport) int macroMain()
{
	
	//===========================================================================================================================
	//												Surface Potential measurement
	//===========================================================================================================================
	//Program Finalized 
	
	//============================================================================================================================
	// Parameters with default values
	double V_step = 100;						// the number of steps in sweep (number)
	double V_start = 8;							 // Start of the Voltage of the pulse (Volts)
	double Pulse_time=1;						// The time that the pulse will run (seconds)	
	double Wait_time = 0;						// The time delay until the voltage sweep runs (seconds)
	double Amp = 0;
	double Phase =0;
	double Amplitude= 0;

	/*ifstream myfile;		   					// Start reading settings from text file
    myfile.open("Untitled_settings.txt");

	myfile >> V_step;
	myfile >> V_start;
	myfile >> Pulse_time;
	myfile >> Wait_time;

    myfile.close();	// End of reading setting from text file
	*/
	double k=V_start;							
	
	LITHO_BEGIN
		LithoScan(false);						// turn off scanning
	
								
	ofstream myfile1;
	myfile1.open("Ferroelectric Char.txt");

	//for (int j=0; j<=4*V_step;j++)							// Top row of data output
	//{
	//	myfile1 << "\t" << 2-(j/V_step) ;
	//}

	for( k; k>=-1*V_start;k--)							 	// Writing the intial pulse
	{
		//myfile1 << k << "\t" ;
		
		LithoScan(false);
		LithoSetSoft(lsNS5FPOutput1,k);						//Units are in Volts
		Sleep(1000*Pulse_time);								//Time that the pulse is being given
		LithoSetSoft(lsNS5FPOutput1,0);
		Sleep(1000*Wait_time);
		

		/*checkzero:
	if (LithoGetSoft(lsNS5FPOutput2)==0)
		{
		Amp=0;
		Sleep(10)
		goto checkzero;									//This Block is for taking a single measurement not a sweep
		}
	else
		{ 
			Amp=1000*LithoGetSoft(lsNS5FPOutput2);
			Phase = LithoGetSoft(lsNS5FPOutput1);
		}
	*/											// This code is for taking a single measurement, not a sweep.										// This code is for taking a single measurement, not a sweep.

	
	
	for (int ii=2*V_step;ii>=-2*V_step;ii--)		//Contorls the number steps
	{
checkzero:
		//LithoPulse(lsBias,1000*Pulse_voltage,Pulse_time);
		LithoSetSoft(lsNS5FPOutput1,(ii / V_step)); //Units are in Volts
										
		Amp=LithoGetSoft(lsNS5FPOutput2);
		Amplitude=1000*Amp;
		//Phase=LithoGetSoft(lsNS5FPOutput1);
		
		myfile1 << k << "\t" << ii << "\t" << Amplitude << "\n";
	}
	                                                   
	
	/*if (LithoGetSoft(lsNS5FPOutput2)==0) {
		Potential=0;
			goto checkzero;}
	else { 
		Potential=Potential+1000*LithoGetSoft(lsNS5FPOutput2);}
	if (LithoGetSoft(lsNS5FPOutput2)==0) {
		Potential=0;
			goto checkzero;}
	else {
		Potential=Potential+1000*LithoGetSoft(lsNS5FPOutput2);}
	*///Potential=(Potential/3);					// Units are in Millivolts

	
					// Outputs fill in the form intial voltage tab 
	
	
	//}
	

/*	for( int p=-1*V_start;p<=V_start;p++)				   	// Writing the intial pulse
	{
		myfile1 << p << "\t" ;
		checkzero1:
		LithoScan(false);
		LithoSetSoft(lsNS5FPOutput1,p);			//Units are in Volts
		Sleep(300);								//Time that the pulse is being given
		LithoSetSoft(lsNS5FPOutput1,0);	
		LithoScan(true);
		
		Sleep(300);		// Writing the new colum for each new voltage
		if (LithoGetSoft(lsNS5FPOutput2)==0) {
		Potential=0;
			goto checkzero1;}
		else { 
			Potential=1000*LithoGetSoft(lsNS5FPOutput2);}
		myfile1 << Potential << "\n";				// Outputs fill in the form intial voltage tab 
		Sleep(10);
	}
	*/

	}
	myfile1.close();
	
	


	LithoSetSoft(lsNS5FPOutput1,0);
	Beep(400,1000);
	//======================================================================================================================================================
	
	LITHO_END

	return 0;	// 0 makes the macro unload. Return 1 to keep the macro loaded.	
}
