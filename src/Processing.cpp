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


Mat Processing::FrameGaus(Mat frame_gray)
{

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

Mat Processing::DrawROI(void)
{

	/*
	 Function : DrawROI()
	 Discriptopn: Function to load a mask frame. The mask will be used in function ApplyMask to block all
	 unwanted areas within a frame. The mask is already configured in this repo "maskimg.JPG". If you would
	 like to change the mask read HOW TO USE section Below:
	 HOW TO USE:
	 1 - Save the output of function FrameGray() by using imwrite() function.
	 2 - Use 3rd party software (MC Paint) to load the frame.
	 3 - Use absolute black color RGB(0, 0 , 0) to highlight the wanted areas. Do not modify unwanted areas.
	 4 - Save the frame as maskimg.JPG in the same directory.
	 Inputs:
	 1 - Mat: Grayscale frame
	 Outputs:
	 1 - Mat: Mask Template.
	*/

	Mat maskimage, maskmodel;
	maskimage = imread("maskimg.JPG");
	cvtColor(maskimage, maskmodel, COLOR_BGR2GRAY);

	for (int i = 0; i < maskmodel.rows; i++) {
		for (int j = 0; j < maskmodel.cols; j++) {

			if (maskmodel.at<uchar>(i, j) == 0)
			{

				maskmodel.at<uchar>(i, j) = 255;
			}
			else
			{
				maskmodel.at<uchar>(i, j) = 0;
			}
		}
	}
	return maskmodel;
}

Mat Processing::ApplyMask(Mat Canny, Mat Mask)
{

	/*
	Function : ApplyMask()
	Discriptopn: Function to apply preconfigured mask to grayscale frame.
	Inputs:
	1 - Mat: Grayscale frame
	2 - Mat: Grayscale frame with wanted areas paint in absolute black RGB(0, 0, 0)
	Outputs:
	1 - Mat: Grayscale frame with unwanted areas removed.
	*/

	Mat Masked;
	bitwise_and(Canny, Canny, Masked, Mask);
	return Masked;
}

Mat Processing::FrameCanny(Mat blur)
{

	/*
	 Function : FrameCanny()
	 Discriptopn: Function to compare intensity of neighbouring pixels and highlight pixels with big change in intensity. The function uses Canny Filter to perform this.
	 Inputs:
	 1 - Mat: Grayscale frame
	 Outputs:
	 1 - Mat: Grayscale frame with edges detected.

	*/

	Mat edges;
	Canny(blur, edges, 50.0, 100.0);
	return edges;
}

Mat Processing::DetectLines(Mat mask, Mat raw_frame)
{

	/*
	 Function : DetectLines()
	 Discriptopn: Function to detect straight lines within frame. The function mainly uses Hough Lines Algorithm. In addition, it enhances the software capability by enlarging the Region of Interest (ROI) as it autonomously detects the closest line to the centre of the screen from the right and left side. Furthermore, it optimizes the software performance as it doesn't require lines averaging calculations.
	 Inputs:
	 1 - Mat: Raw frame from webcame.
	 2 - Mat: Masked frame.
	 Outputs:
	 1 - Mat: RGB frame with straight lines highlighted.
	*/

	vector<Vec4i> lines;
	vector<float> Rdata, Ldata;
	Mat cdstP;
	int Check_Line;
	float Ycord0 = 480;
	float Ycord1 = 380;
	float Right_Line_Xcord0;
	float Right_Line_Xcord1;
	float Left_Line_Xcord0;
	float Left_Line_Xcord1;
	float Slope_Right_Line;
	float Intercept_Right_Line;
	float Slope_Left_Line;
	float Intercept_Left_Line;
	float slope;
	Vec4i Closest_Right_Line;
	Vec4i Closest_Left_Line;
	



	HoughLinesP(mask, lines, 2, CV_PI / 180, 100, 1, 50);
	cout << lines.size() << endl;
	if (lines.size() > 0)
	{
		for (size_t i = 0; i < lines.size(); i++)
		{
			Vec4i l = lines[i];
			if ((l[2] - l[0]) == 0)
			{
				break;
			}
			slope = (l[3] - l[1]) / (l[2] - l[0]);
			Check_Line = l[2] - 320;
			if ((slope > 0) && (abs(Check_Line) < abs(Closest_Right_Line[0] - 320)) && ((l[2] - l[0] != 0)))
			{
				Closest_Right_Line = l;
				Slope_Right_Line = (Closest_Right_Line[3] - Closest_Right_Line[1]) / (Closest_Right_Line[2] - Closest_Right_Line[0]);
				Intercept_Right_Line = Closest_Right_Line[3] - (Slope_Right_Line*Closest_Right_Line[2]);
				Right_Line_Xcord0 = (Ycord0 - Intercept_Right_Line) / Slope_Right_Line;
				Right_Line_Xcord1 = (Ycord1 - Intercept_Right_Line) / Slope_Right_Line;
				Rdata.push_back(Slope_Right_Line);
			}

			else if ((slope < 0) && (abs(Check_Line) < abs(Closest_Left_Line[0] - 320)) && ((l[2] - l[0] != 0)))
			{
				Closest_Left_Line = l;
				Slope_Left_Line = (Closest_Left_Line[3] - Closest_Left_Line[1]) / (Closest_Left_Line[2] - Closest_Left_Line[0]);
				Intercept_Left_Line = Closest_Left_Line[3] - (Slope_Left_Line*Closest_Left_Line[2]);
				Left_Line_Xcord0 = (Ycord0 - Intercept_Left_Line) / Slope_Left_Line;
				Left_Line_Xcord1 = (Ycord1 - Intercept_Left_Line) / Slope_Left_Line;
				Ldata.push_back(Slope_Left_Line);

			}
		}

	}
	if (Rdata.empty() == 0)
	{
		line(raw_frame, Point(Right_Line_Xcord0, Ycord0), Point(Right_Line_Xcord1, Ycord1), Scalar(255, 0, 0), 5, LINE_AA);
	}
	if (Ldata.empty() == 0)
	{
	line(raw_frame, Point(Left_Line_Xcord0, Ycord0), Point(Left_Line_Xcord1, Ycord1), Scalar(0, 0, 255), 5, LINE_AA);
	}
	return raw_frame;
	}
