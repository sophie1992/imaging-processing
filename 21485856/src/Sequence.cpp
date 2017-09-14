#include "Sequence.hpp"

#include "tools.hpp"
#include "Constants.hpp"

using namespace CVLab;
using namespace cv;
using namespace std;

Sequence::Sequence(const string &folder, const Calibration &c) : calib(c) {
	// read both videos
	readVideo(folder + Constants::sequence1File, images[0], calib.getCamera1(), calib.getDistortion1());
	readVideo(folder + Constants::sequence2File, images[1], calib.getCamera2(), calib.getDistortion2());

	// check if both videos have the same amount of frames
	if (images[0].size() != images[1].size()) {
		throw "both videos have different number of frames";
	}

	// load marker positions for both videos
	readMarkers(folder + Constants::markers1File, markers[0], images[0][0]);
	readMarkers(folder + Constants::markers2File, markers[1], images[1][0]);

	// check if both videos have the same amount of markers
	if (markers[0].size() != markers[1].size()) {
		throw "both videos have different number of markers";
	}

	// sort the markers so that they have the same ordering for both videos
	sortMarkers();
}

Sequence::Sequence(const Sequence &other) : calib(other.calib) {
	// loop over all cameras
	for (unsigned int camera = 0; camera < 2; ++camera) {
		// copy images
		images[camera].resize(other.images[camera].size());
		for (unsigned int frame = 0; frame < images[camera].size(); ++frame) {
			images[camera][frame] = other.images[camera][frame].clone();
		}

		// copy marker positions
		markers[camera] = other.markers[camera];
	}
}

unsigned int Sequence::getNumberOfFrames() const {
	return images[0].size();
}

const vector<Mat> & Sequence::operator[](unsigned int camera) const {
	// check camera index
	if (camera > 1) {
		throw "there are only two cameras";
	}

	// return sequence of images
	return images[camera];
}

vector<Point2f> Sequence::getMarkers(unsigned int camera) const {
	// check camera index
	if (camera > 1) {
		throw "there are only two cameras";
	}

	// return marker positions
	return markers[camera];
}

void Sequence::readVideo(const string &file, vector<Mat> &data, const Mat &K, const Mat &distortion) {
	// open video file
	VideoCapture vid(file);
	if (!vid.isOpened()) {
		throw "could not open video file " + file;
	}

	// get number of frames from the video file
	const unsigned int numberOfFrames = static_cast<unsigned int>(vid.get(CAP_PROP_FRAME_COUNT));
	
	// resize vector to number of frames
	data.clear();
	data.resize(numberOfFrames);

	// load images from video
	for (unsigned int i = 0; i < numberOfFrames; ++i) {
		Mat img, gray, undistorted;

		// load next frame
		vid >> img;

		// convert frame to grayscale
		cvtColor(img, gray, COLOR_BGR2GRAY);

		// undistort the image
		undistort(gray, undistorted, K, distortion);

		// save the undistorted image in the vector
		undistorted.copyTo(data[i]);
	}
}

void Sequence::readMarkers(const string &file, vector<Point2f> &data, const Mat &firstImage) {
	// read raw data from file
	Mat markerData = readMatrix(file);

	// check matrix dimension for validity
	checkMatrixDimensions(markerData, -1, 2, "marker positions");

	// resize vector to take marker positions
	data.clear();
	data.resize(markerData.rows);

	// save marker positions in the vector
	for (int i = 0; i < markerData.rows; ++i) {
		data[i].x = markerData.at<float>(i, 0);
		data[i].y = markerData.at<float>(i, 1);
	}

	// and refine the marker positions
	cornerSubPix(firstImage, data, Constants::markerRefinementWindowSize, Constants::markerRefinementZeroZone, Constants::markerRefinementCriteria);
}

void Sequence::sortMarkers() {
	cerr << "Sequence::sortMarkers is not implemented" << endl;
	float sourceMarker_X = markers[0][0].x;
	float sourceMarker_Y = markers[0][0].y;
	float targetMarker_X = markers[1][0].x;
	float targetMarker_Y = markers[1][0].y;
	float sectargetMarker_X = markers[1][1].x;
	float sectargetMarker_Y = markers[1][1].y;
	
	Mat sourceMarker = (Mat_<float>(3, 1) << sourceMarker_X, sourceMarker_Y, 1);
	Mat targetMarker = (Mat_<float>(3, 1) << targetMarker_X, targetMarker_Y, 1);
	Mat sectargetMarker = (Mat_<float>(3, 1) << sectargetMarker_X, sectargetMarker_Y, 1);

	//std::vector<cv::Point2f> newmarkers[2];
	//Mat newsourceMarker = (Mat_<cv::Point_<float>>(1, 2) << newmarkers[1][0], newmarkers[1][1]);
	//Mat newtargetMarker = (Mat_<cv::Point_<float>>(1, 2) << newmarkers[1][0], newmarkers[1][1]);

	Mat fundMat = calib.getFundamentalMat();
	//Mat firstEpipolar = (sourceMarker*fundMat)*targetMarker;
	//Mat secondEpipolar = (sourceMarker*fundMat)*sectargetMarker;

	Mat firstEpipolar = (targetMarker.t()*fundMat)*sourceMarker;
	Mat secondEpipolar = (sectargetMarker.t()*fundMat)*sourceMarker;
	//float* firEpiVal = (float*)firstEpipolar.data;
	bool isFirpointclose2zero = abs(*(float*)firstEpipolar.data) < abs(*(float*)secondEpipolar.data);
	//bool isFirpointclose2zero = abs(firstEpipolar) < abs(*(float*)secondEpipolar.data);

	if (!isFirpointclose2zero){
		std::vector<cv::Point2f> newmarkers[1];
		newmarkers[0] = markers[1];
		markers[1][0] = markers[1][1];
		markers[1][1] = newmarkers[0][0];
	}

	cerr << firstEpipolar << endl;
	cerr << secondEpipolar << endl;


	showImageMarkers(images[0][0], markers[0],"",false);
	showImageMarkers(images[1][0], markers[1], "", false);
}
