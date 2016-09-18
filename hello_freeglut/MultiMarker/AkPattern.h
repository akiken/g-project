#pragma once

#include <stdio.h>
#include <string>
#include <vector>

#include <AR/config.h>
#include <AR/video.h>
#include <AR/param.h>
#include <AR/ar.h>
#include <AR/gsub_lite.h>

using namespace std;

namespace AKAR {

class AkMarker {
public:
	AkMarker() {
		gPatt_id = -1;
	}

	string patt_name;
	ARdouble gPatt_width;
	ARdouble gPatt_trans[3][4];
	int gPatt_found;
	int gPatt_id;
};

class AkPattern
{
public:
	AkPattern();
	virtual ~AkPattern();

	void addMarker(string name, ARdouble width);

	bool DetectMarker(ARUint8* gARTImage);
	int getHighestConfidenceId(AkMarker& marker);
	int getHighestConfidenceId();
	
	AkMarker& getHighestConfidenceMarker();

	ARHandle *gARHandle;
	ARPattHandle *gARPattHandle;
	AR3DHandle *gAR3DHandle;
	long gCallCountMarkerDetect;

	vector<AkMarker> markers;

};

}
