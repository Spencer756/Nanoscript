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
	//                                  Ramp with pulse mode to extract the PE loop of ferroelectric material
	//=============================================================by Zhiyong====================================================
	//============================================================================================================================
	// Parameters with default values
	float V_max = 8.0f; 		// the maximum voltage
	float V_step = 0.05f;	    // the voltage step
	float pulse_dura=0.1f;          //the time of the pulse
	float post_pulse_time=0.3f;    //the time wait until capture

	LITHO_BEGIN	

	LithoScan(false);			// turn off scanning
	LithoCenterXY();			// move tip to center of field
	
	//The start of the ramp
	int V_N=int(2*V_max/V_step);    //the total steps
	int ii;
	int Trig;
	ofstream myfile;
	double flag=1;
	double Volt_now=0;
	//read
while(1)
{
	ifstream myfile1;
    myfile1.open("Trig.txt");
	myfile1 >> Trig;
	myfile1 >> V_max;
	myfile1 >> V_step;
	myfile1 >> pulse_dura;
	myfile1 >> post_pulse_time;
    myfile1.close();
	if (Trig==0)
		goto AA;
	else
	{
		if (abs(Volt_now)>V_max)
		{	flag=-flag;
			Volt_now+=(V_step*flag);
		}	
		else
		{	
			Volt_now+=(V_step*flag);
			LithoPulse(lsBias,1000*Volt_now,pulse_dura);
			Sleep(post_pulse_time*1000);
			double amp=1000*LithoGetSoft(lsNS5FPOutput1);
			amp=amp+1000*LithoGetSoft(lsNS5FPOutput1);
			amp=amp+1000*LithoGetSoft(lsNS5FPOutput1);
			amp=amp/3;
			double pha=180/10*LithoGetSoft(lsNS5FPOutput2);
			pha=pha+180/10*LithoGetSoft(lsNS5FPOutput2);
			pha=pha+180/10*LithoGetSoft(lsNS5FPOutput2);
			pha=pha/3;
			myfile.open("A_zhiyong.txt");
			myfile << Volt_now << "\t" << pha<<"\t"<<amp<< "\n";
			myfile.close();
		}
	}
}
AA:
	myfile.close(); //close the file.
	Beep(300,1000);
	//======================================================================================================================================================
	
	LITHO_END

	return 0;	// 0 makes the macro unload. Return 1 to keep the macro loaded.	
}
