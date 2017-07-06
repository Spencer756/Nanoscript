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
	//Program Finalized 5-25-17
	
	//============================================================================================================================
	// Parameters with default values
	LITHO_BEGIN	
	LithoScan(true);
	/*double V_step = 100;							 // the number of steps
	double Potential = 0;						 // Dont touch
	double V_start = 2;							 // Voltage to sweep
	double potential_1=0;
	float z=0;
	ifstream myfile;		   					// Start reading settings from text file
    myfile.open("KPFM_settings.txt");
	myfile >> z;
	myfile >> V_start;
	myfile >> V_step;
    myfile.close();								// End of reading setting from text file
	double k=V_start;							//Dont touch
*/
	
	/*// turn off scanning
	ofstream myfile1;
	myfile1.open("KPFM.txt");

	for (int j=0; j<=4*V_step;j++)				// Top row of data output
	{
		myfile1 << "\t" << 2-(j/V_step) ;
	}

	for( k;k>=-1*V_start;k--)					// Writing the intial pulse
	{
		LithoSetSoft(lsNS5FPOutput1,k);			 //Units are in Volts
		Sleep(1000);
		myfile1 << "\n" << k;					// Writing the new colum for each new voltage

	
	
	for (int ii=2*V_step;ii>=-2*V_step;ii--)		//Contorls the number steps
	{
		LithoSetSoft(lsNS5FPOutput1,(ii / V_step)); //Units are in Volts
			checkzero:	//This whole set checks for zeros and then corrects for them. It also averages the values by taking data 3 times for every voltage between 2 and -2.
		potential_1=LithoGetSoft(lsNS5FPOutput2);
	if (potential_1!=0)
		{
	Potential=potential_1;
	Potential=1000*Potential;}
	else {
		Sleep(10);
		goto checkzero;}// Units are in Millivolts
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


	/*myfile1 <<"\t"<< Potential ;				// Outputs fill in the form intial voltage tab 
	Sleep(10);
	
	}
	}

	myfile1.close();*/

	for (int ii=0;ii<=2;ii++)
	{
	LithoSetSoft(lsBias,1000);
	Sleep(20000);
	LithoSetSoft(lsBias,0);
	Sleep(500);
	}

	Beep(400,1000);
	//======================================================================================================================================================
	
	LITHO_END

	return 0;	// 0 makes the macro unload. Return 1 to keep the macro loaded.	
}
