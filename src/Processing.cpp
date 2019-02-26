#include <windows.h>
#include <stdint.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <vector>
#include <numeric>    
#include <algorithm>   
#include "pch.h"
#include "Processing.h"
#include "opencv2/opencv.hpp"
#include "PreProcessing.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

/**
 *
 *
 *
 * Class for operations to detect road lines.
 *
 *
 *
 */

Mat Processing::FrameGaus(Mat frame_gray) {

	/*
  Function : FrameGaus()
  Discriptopn: Function to smoothen the edges of the frame input
  Inputs:
  1 - Mat: Grayscale frame
  Outputs:
  1 - Mat: Blurred grayscale frame
  */

	Mat blur;
	GaussianBlur(frame_gray, blur, Size(5, 5), 0);

	return blur;
}

Mat Processing::DrawROI(void) {

	/*
	 Function : DrawROI()
	 Discriptopn: Function to load a mask frame. The mask will be used in function
	 ApplyMask to block all unwanted areas within a frame. The mask is already
	 configured in this repo "maskimg.JPG". If you would like to change the mask
	 read HOW TO USE section Below: HOW TO USE: 1 - Save the output of function
	 FrameGray() by using imwrite() function. 2 - Use 3rd party software (MC
	 Paint) to load the frame. 3 - Use absolute black color RGB(0, 0 , 0) to
	 highlight the wanted areas. Do not modify unwanted areas. 4 - Save the frame
	 as maskimg.JPG in the same directory. Inputs: 1 - Mat: Grayscale frame
	 Outputs:
	 1 - Mat: Mask Template.
	*/

	Mat maskimage, maskmodel;
	maskimage = imread("maskimg.JPG");
	cvtColor(maskimage, maskmodel, COLOR_BGR2GRAY);

	for (int i = 0; i < maskmodel.rows; i++) {
		for (int j = 0; j < maskmodel.cols; j++) {

			if (maskmodel.at<uchar>(i, j) == 0) {

				maskmodel.at<uchar>(i, j) = 255;
			}
			else {
				maskmodel.at<uchar>(i, j) = 0;
			}
		}
	}
	return maskmodel;
}

Mat Processing::ApplyMask(Mat Canny, Mat Mask) {

	/*
	Function : ApplyMask()
	Discriptopn: Function to apply preconfigured mask to grayscale frame.
	Inputs:
	1 - Mat: Grayscale frame
	2 - Mat: Grayscale frame with wanted areas paint in absolute black RGB(0, 0,
	0) Outputs: 1 - Mat: Grayscale frame with unwanted areas removed.
	*/

	Mat Masked;
	bitwise_and(Canny, Canny, Masked, Mask);
	return Masked;
}

Mat Processing::FrameCanny(Mat blur) {

	/*
	 Function : FrameCanny()
	 Discriptopn: Function to compare intensity of neighbouring pixels and
	 highlight pixels with big change in intensity. The function uses Canny Filter
	 to perform this. Inputs: 1 - Mat: Grayscale frame Outputs: 1 - Mat: Grayscale
	 frame with edges detected.

	*/
	int LowThreshhold = 50; // Lower this value to increase edge detection
							// intensity i.e detect more edges.
	int HighThreshhold =
		LowThreshhold * 3; // OpenCV documnetation recommends 3:1
						   // ratio between high and low threshold
	Mat edges;
	Canny(blur, edges, LowThreshhold, HighThreshhold);
	return edges;
}

Mat Processing::DetectLines(Mat mask, Mat raw_frame) {

	/*
	 Function : DetectLines()
	 Discriptopn: Function to detect straight lines within frame. The function
	 mainly uses Hough Lines Algorithm. In addition, it enhances the software
	 capability by enlarging the Region of Interest (ROI) as it autonomously
	 detects the closest line to the centre of the screen from the right and left
	 side. Furthermore, it optimizes the software performance as it doesn't
	 require lines averaging calculations. Inputs: 1 - Mat: Raw frame from
	 webcame. 2 - Mat: Masked frame. Outputs: 1 - Mat: RGB frame with straight
	 lines highlighted.
	*/

	vector<Vec4i> lines;
	vector<float> Rdata, Ldata;
	Mat cdstP;
	double MinLineLength = 30.0;
	double MaxLineGap = 600.0;
	float Ycord0 = 480;
	float Ycord1 = 380;
	float Right_Line_Xcord0;
	float Right_Line_Xcord1;
	float Left_Line_Xcord0;
	float Left_Line_Xcord1;
	float Slope_Right_Line;
	float Intercept_Right_Line;
	float Intercept_Left_Line;
	float slope;
	Vec4i Closest_Right_Line;
	Vec4i Closest_Left_Line;

	HoughLinesP(mask, lines, 2, CV_PI / 180, 50, MinLineLength, MaxLineGap);
	if (lines.size() > 0) {
		for (size_t i = 0; i < lines.size(); i++) {
			Vec4i l = lines[i];
			if ((l[2] - l[0] == 0))
			{
				continue;
			}
			slope = float((float(l[3]) - float(l[1])) / (float(l[2]) - float(l[0])));
			if (slope < 0) {
				Intercept_Right_Line = 
					l[3] - (slope * l[2]);
				Right_Line_Xcord0 = (Ycord0 - Intercept_Right_Line) / slope;
				Right_Line_Xcord1 = (Ycord1 - Intercept_Right_Line) / slope;
				Rdata.push_back(slope);
			}

			else if (slope > 0) {
				Intercept_Left_Line =
					l[3] - (slope * l[2]);
				Left_Line_Xcord0 = (Ycord0 - Intercept_Left_Line) / slope;
				Left_Line_Xcord1 = (Ycord1 - Intercept_Left_Line) / slope;
				Ldata.push_back(slope);
			}
		}
	}
	if (Rdata.empty() == 0) {
		line(raw_frame, Point(Right_Line_Xcord0, Ycord0),
			Point(Right_Line_Xcord1, Ycord1), Scalar(255, 0, 0), 5, LINE_AA);
		Rdata.clear();
	}
	if (Ldata.empty() == 0) {
		line(raw_frame, Point(Left_Line_Xcord0, Ycord0),
			Point(Left_Line_Xcord1, Ycord1), Scalar(0, 0, 255), 5, LINE_AA);
		Ldata.clear();

	}

	line(raw_frame, Point(320, 400), Point(320, 480), Scalar(0, 255, 0), 5,
		LINE_AA);
	return raw_frame;
}
