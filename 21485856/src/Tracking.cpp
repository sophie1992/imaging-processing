#include "Tracking.hpp"
#include "tools.hpp"
//#include "Sequence.hpp"

using namespace CVLab;
using namespace cv;
using namespace std;

Tracking::Tracking(const Calibration &c) : calib(c){
	cerr << "construction called" << endl;	
}

Tracking::Tracking(const Tracking &other) : calib(other.calib){

}

vector<vector<Point2f>> Tracking::operator()(const vector<Mat> &images, const vector<Point2f> &initMarkers) const {
	//throw "Tracking::operator() is not implemented";
	cerr << "operator called" << endl;
	vector<Point2f> prevMarker;
	vector<Point2f> nextMarker;
	vector<uchar> status;
	vector<float> error;
	vector<vector<Point2f>> trackedMarkers;

	int numFrame = images.size();
	trackedMarkers.resize(numFrame, vector<Point2f>(2));
	trackedMarkers[0] = initMarkers;
	prevMarker = initMarkers;
	//goodFeaturesToTrack(images[0], initMarkers, 30, 0.01, 30);
	for (int i = 0; i < numFrame-1; i++)
	{
		calcOpticalFlowPyrLK(images[i], images[i + 1], prevMarker, nextMarker, status, error, Size(11, 11), 5);
		//trackedMarkers[i] = nextMarker;
		prevMarker = nextMarker;
		trackedMarkers[i+1] = nextMarker;
	}
	cerr << "operator runned" << endl;
	return trackedMarkers;
}
