#pragma once

#include <stdio.h>
#include <string>

#include <AR/config.h>
#include <AR/video.h>
#include <AR/param.h>
#include <AR/ar.h>
#include <AR/gsub_lite.h>

using namespace std;

class AkPattern
{
public:
	AkPattern(string patt_name);
	~AkPattern();
	
	string patt_name;

	ARHandle *gARHandle;
	ARPattHandle *gARPattHandle;
	long gCallCountMarkerDetect;

	AR3DHandle *gAR3DHandle;
	ARdouble gPatt_width;
	ARdouble gPatt_trans[3][4];
	int gPatt_found;
	int gPatt_id;

};

