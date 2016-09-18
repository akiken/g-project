#include "AkPattern.h"



AkPattern::AkPattern(string patt_name)
{
	this->patt_name = patt_name;

	gARHandle = NULL;
	gARPattHandle = NULL;
	gCallCountMarkerDetect = 0;

	gAR3DHandle = NULL;
	gPatt_width = 80.0;
	gPatt_found = FALSE;
}


AkPattern::~AkPattern()
{
}
