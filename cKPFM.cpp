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
	double V_now;
	//read
while(1)
{
	ifstream myfile1;
    myfile1.open("Trig.txt");
	myfile1 >> Trig;
	//myfile1 >> V_max;
	//myfile1 >> V_step;
	//myfile1 >> pulse_dura;
	//myfile1 >> post_pulse_time;
    myfile1.close();
	if (Trig==0)
		goto AA;
	else
	{
		double Vs_max=2;
		double Vs_step=0.5;
		double Vs_now=-2;
		V_max=8;
		V_step=1;
		V_now=-8;
		pulse_dura=0.1;
		post_pulse_time=0.1;
		myfile.open("A_zhiyong.txt");
		while(V_now<=V_max)//sweep up
		{			
			LithoPulse(lsBias,1000*V_now,pulse_dura);
			Sleep(post_pulse_time*1000);
			myfile <<"\n" << V_now << "\n";
			Vs_now=-2;
			while(Vs_now<=Vs_max)
			{
				LithoSet(lsBias,1000*Vs_now);
				double amp=1000*LithoGetSoft(lsNS5FPOutput1);
				myfile << Vs_now << "\t"<< amp << "\n";
				Vs_now += Vs_step;
			}
			LithoSet(lsBias,0);
			V_now=V_now+V_step;
			Beep(300,100);
		}
		Beep(300,2500);
		while(V_now>=-V_max)//sweep dn
		{		
			LithoPulse(lsBias,1000*V_now,pulse_dura);
			Sleep(post_pulse_time*1000);
			myfile <<"\n" << V_now << "\n";
			Vs_now=-2;
			while(Vs_now<=Vs_max)
			{
				LithoSet(lsBias,1000*Vs_now);
				double amp=1000*LithoGetSoft(lsNS5FPOutput1);
				myfile << Vs_now << "\t"<< amp << "\n";
				Vs_now += Vs_step;
			}
			LithoSet(lsBias,0);
			V_now=V_now-V_step;
			Beep(300,100);
		}
		myfile.close();
		Beep(300,1500);
	}
}
AA:
	//myfile.close(); //close the file.
	Beep(300,1000);
	//======================================================================================================================================================	
	LITHO_END
	return 0;	// 0 makes the macro unload. Return 1 to keep the macro loaded.	
}
