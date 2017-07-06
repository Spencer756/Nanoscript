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
#include <time.h>
using namespace std;



extern "C" __declspec(dllexport) int macroMain()
{
	
	//===========================================================================================================================
	//												Surface Potential measurement
	//===========================================================================================================================
	//Program Finalized 
	
	//============================================================================================================================
	// Parameters with default values
	//double V_step = 100;		

   

					 // Dont touch
	double Pulse_voltage = 7;	// Voltage to sweep
	int Restart_flag = 0;
	int Stop_flag=0;
    double Pulse_time = 1;                             // Pulse duration
    double wait=0;                                 // Wait time after pulse
	double Amp=0;
	//double current_time=11.2154;
	LITHO_BEGIN
       
	LithoScan(false);
	ifstream myfile;
	ofstream myfile1;
	ofstream myfile2; 
	clock_t begin_time = clock();
    clock_t now_time=0;
RESTART:
	// Start reading settings from text file
    myfile.open("Relaxor_settings.txt");
	myfile >> Stop_flag;
	myfile >> Restart_flag;
    myfile >> Pulse_time;
    myfile >> wait;
    myfile >> Pulse_voltage;	
	myfile.close();
    							// End of reading setting from text file
	//double k=V_start;
	
	//Dont touch                       
							// turn off scanning
	
	
	
	if (Restart_flag==1) 
	{	
		myfile1.open("Relaxor.txt");
		
		myfile2.open("Relaxor_settings.txt");
		myfile2 << Stop_flag <<"\t" << 0 << "\t"<< Pulse_time <<"\t" << wait << "\t" << Pulse_voltage;
		myfile2.close();
		LithoPulse(lsBias,1000*Pulse_voltage,Pulse_time);
		//LithoSetSoft(lsNS5FPOutput1,Pulse_voltage);
		//Sleep(1000*Pulse_time);
		//LithoSetSoft(lsNS5FPOutput1,0);
		Sleep(1000*wait);
		begin_time = clock();
record:
		Amp=LithoGetSoft(lsNS5FPOutput2);
		double now_time=((double)clock()-(double)begin_time)/1000;
		myfile1 << now_time <<"\t"<< Amp << "\n" ;
		myfile.open("Relaxor_settings.txt");
		myfile >> Stop_flag;
		myfile >> Restart_flag;
		myfile.close(); 
		if(Restart_flag==1)
		{
			myfile1.close();
			goto RESTART;

		}
		if(Stop_flag==0) 
		{
			goto record;
		}
		else 
		{
			myfile1.close();
		}
	}
	else
	{
		goto RESTART;

	}
	
	LithoSetSoft(lsNS5FPOutput1,0);
	
	Beep(400,1000);
	//======================================================================================================================================================
	
	LITHO_END

	return 0;	// 0 makes the macro unload. Return 1 to keep the macro loaded.	
}
