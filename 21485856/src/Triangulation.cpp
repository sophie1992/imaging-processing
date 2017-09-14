#include "Triangulation.hpp"

using namespace CVLab;
using namespace cv;
using namespace std;

Triangulation::Triangulation(const Calibration &c) : calib(c) {
}

Triangulation::Triangulation(const Triangulation &other) : calib(other.calib) {
}

vector<Point3f> Triangulation::operator()(const vector<Point2f> &markers1, const vector<Point2f> &markers2) const {
	//triangulate the positions for a single frame
	
	//throw "Triangulation::operator() is not implemented";
	vector<Point3f> resultofFrame;
	//resultofFrame.resize(2);
	Mat pnts3D;
	Mat extrinCamera1 = (Mat_<float>(3, 4) << 1,0,0,0,0,1,0,0,0,0,1,0);
	Mat projMatCamera1 = Mat_<float>(3, 4);
	Mat projMatCamera2 = Mat_<float>(3, 4);
	
	//calculate projection matrix of camere
	projMatCamera1 = calib.getCamera1()*extrinCamera1;
	projMatCamera2 = calib.getCamera2()*calib.getTransCamera1Camera2();

	Mat fundMat = calib.getFundamentalMat();
	//projecting 3D points into the image plane using a perspective transformation
	//http://docs.opencv.org/2.4/modules/calib3d/doc/camera_calibration_and_3d_reconstruction.html
	correctMatches(fundMat, markers1, markers2,markers1, markers2);
	//Reconstructs points by triangulation
	triangulatePoints(projMatCamera1, projMatCamera2, markers1, markers2, pnts3D);
	
	//prepare Input vector of N-dimensional points for function convertPointsFromHomogeneous
	Mat expandTrans;
	calib.getTransCamera1World().copyTo(expandTrans);
	Mat elmemt4expand = (Mat_<float>(1, 4) << 0, 0, 0, 1);
	expandTrans.push_back(elmemt4expand);
	//cerr << expandTrans << endl;

	Mat result = expandTrans * pnts3D;
	//Converts points from homogeneous to Euclidean space
	convertPointsFromHomogeneous(result.t(), resultofFrame);

	//resultofFrame.insert(); //result.col(0);

	return resultofFrame;
}

vector<vector<Point3f>> Triangulation::operator()(const vector<vector<Point2f>> &markers1, const vector<vector<Point2f>> &markers2) const {
	//triangulate the positions for a whole sequence
	
	// do nothing if there is no data
	if (markers1.empty()) {
		return vector<vector<Point3f>>();
	}

	// check for same number of frames
	if (markers1.size() != markers2.size()) {
		throw "different number of frames";
	}

	// create result vector
	vector<vector<Point3f>> result(markers1.size());

	// trinagulate each frame for itself and store result
	for (unsigned int i = 0; i < markers1.size(); ++i) {
		result[i] = (*this)(markers1[i], markers2[i]);
	}
	
	// and return result
	return result;
}


vector<vector<Point3f>> Triangulation::calculateMotion(const vector<vector<Point3f>> &data) {
	//get the triangulated marker data and will calculate the motion from it. 
	//The motion is nothing but the marker position relative to the first marker in the first frame.
	//throw "Triangulation::calculateMotion is not implemented";

	vector<vector<Point3f>> output;
	output.resize(data.size(), vector<Point3f>(2));

	for (int i = 0; i < data.size(); i++){
		subtract(data[i], data[0], output[i]);
	}

	return  output;
}
