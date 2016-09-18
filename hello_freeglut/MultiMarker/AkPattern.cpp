#include "AkPattern.h"

using namespace AKAR;

AkPattern::AkPattern(){
	gARHandle = NULL;
	gARPattHandle = NULL;
	gCallCountMarkerDetect = 0;

	gAR3DHandle = NULL;
}


AkPattern::~AkPattern(){

}

void AkPattern::addMarker(string name, ARdouble width) {
	AkMarker marker;
	marker.patt_name = name;
	marker.gPatt_width = width;
	marker.gPatt_found = FALSE;
	markers.push_back(marker);
}

bool AkPattern::DetectMarker(ARUint8* gARTImage) {
	if (arDetectMarker(gARHandle, gARTImage) >= 0) {
		return true;
	}
	else {
		return false;
	}
}

int AkPattern::getHighestConfidenceId(AkMarker& marker) {
	int k = -1;
	for (int j = 0; j < gARHandle->marker_num; j++) {
		if (gARHandle->markerInfo[j].id == marker.gPatt_id) {
			if (k == -1) k = j; // First marker detected.
			else if (gARHandle->markerInfo[j].cf > gARHandle->markerInfo[k].cf) k = j; // Higher confidence marker detected.
		}
	}

	return k;
}

int AkPattern::getHighestConfidenceId() {
	int k = -1;
	for (int j = 0; j < gARHandle->marker_num; j++) {
		for (int i = 0; i < markers.size(); i++) {
			if (gARHandle->markerInfo[j].id == markers[i].gPatt_id) {
				if (k == -1) k = j; // First marker detected.
				else if (gARHandle->markerInfo[j].cf > gARHandle->markerInfo[k].cf) k = j; // Higher confidence marker detected.
			}
		}
	}

	return k;
}

AkMarker& AkPattern::getHighestConfidenceMarker() {
	static AkMarker dummy;
	int k = -1;
	for (int j = 0; j < gARHandle->marker_num; j++) {
		if (gARHandle->markerInfo[j].id == markers[0].gPatt_id) {
			if (k == -1) k = j; // First marker detected.
			else if (gARHandle->markerInfo[j].cf > gARHandle->markerInfo[k].cf) k = j; // Higher confidence marker detected.
		}
	}

	if (-1 == k) {
		return dummy;
	}
	else {
		return markers[k];
	}
}

