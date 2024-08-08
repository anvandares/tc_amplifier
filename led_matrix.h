#include "Arduino_LED_Matrix.h"
ArduinoLEDMatrix matrix;

const uint32_t mode_Arr[][3] = { //An array with "sel.", "mode" and "M(+)0-9" for printing mode. 
	 {//0
		0x44,
		0xe6ca54a4,
		0x4a44e000

	},
	{ //1 
		0x44,
		0x86c85484,
		0x48448000
	},
	{ //2
		0x44,
		0xe6c254e4,
		0x4844e000

	}, //3
	{
		0x44,
		0xe6c254e4,
		0x4244e000
	
	},  //4
	{
		0x44,
		0xa6ca54e4,
		0x42442000

	},   //5

	{
		0x44,
		0xe6c854e4,
		0x4244e000

	}, //6
	{
		0x44,
		0xe6c854e4,
		0x4a44e000
		
	}, //7
	{
		0x44,
		0xe6c25424,
		0x42442000
	},
	{   //8
		0x44,
		0xe6ca54e4,
		0x4a44e000	
	},
	{   //9
		0x44,
		0xe6ca54e4,
		0x4244e000
	},   //sel
	{
		0x0,
		0x6104d06,
		0xd02906d4
	},   //mode 
	{
		0x0,
		0xa08e0be,
		0xdbadaadb
	}
  };

  void print_frame(int a) //Prints selected frame to built in led 
  { 
    matrix.loadFrame(mode_Arr[a]);

  }