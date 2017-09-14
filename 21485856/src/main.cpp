#include <opencv2/opencv.hpp>

#include "tools.hpp"
#include "Constants.hpp"
#include "Calibration.hpp"
#include "Sequence.hpp"
#include "Tracking.hpp"
#include "Triangulation.hpp"
#include <string>
#include <iostream>

using namespace CVLab;
using namespace cv;
using namespace std;

int main(int argc, char **argv) {
	try {
		// get calibration folder, sequence folder and output file from command line
		string calibFolder, sequenceFolder, outputFile;
		if (argc == 4) {
			calibFolder = string(argv[1]) + "/";
			sequenceFolder = string(argv[2]) + "/";
			outputFile = string(argv[3]);
		} else {
			cerr << "Please specify folder with calibration data, folder with sequence and output file" << endl;
			return EXIT_FAILURE;
		}

		// load calibration data
		logMessage("load calibration data from " + calibFolder);
		Calibration calib(calibFolder);
		logMessage("loaded calibration data");

		// load sequence
		logMessage("load sequence from " + sequenceFolder);
		Sequence sequence(sequenceFolder, calib);
		logMessage("finished loading sequence with " + to_string(sequence.getNumberOfFrames()) + " frames");

		// track the markers in the sequence
		logMessage("start tracking of markers");
		// TODO execute tracking
		Tracking track(calib);
		//Tracking track2(sequence,track );
		
		vector<vector<Point2f>> trackingMarkers[2];

		for (int i = 0; i < 2 ; i++){
				cerr << "enterring loop" << endl;
				trackingMarkers[i] = track(sequence[i], sequence.getMarkers(i));
			}
		//showSequenceMarkers(sequence[0], trackingMarkers[0], "", false);
		//showSequenceMarkers(sequence[1], trackingMarkers[1], "", false);

		logMessage("finished tracking of markers");

		// triangulate the marker positions
		logMessage("start triangulation");
		// TODO execute triangulation
		Triangulation triang(calib);
		vector<vector<Point3f>> triangResult = triang(trackingMarkers[0], trackingMarkers[1]);
		showTriangulation(triangResult,"",true);
		logMessage("finished triangulation");

		// calculate the motion of the markers
		logMessage("calculate motion of markers");
		// TODO calculate motion of markers
		showTriangulation(triang.calculateMotion(triangResult), "", true);

		logMessage("finished calculation of motion of markers");

		// write the result to the output file
		logMessage("write results to " + outputFile);
		// TODO write result
		writeResult(outputFile, triang.calculateMotion(triang(trackingMarkers[0], trackingMarkers[1])));
		//writeResult(outputFile, triangResult);

		logMessage("finished writing results");

		// and exit program with code for success
		return EXIT_SUCCESS;
	} catch (const string &err) {
		// print error message and exit program with code for failure
		cerr << err << endl;
		return EXIT_FAILURE;
	}
}
