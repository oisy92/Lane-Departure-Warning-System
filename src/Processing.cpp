#include "pch.h"
#include "Processing.h"
#include "pch.h"
#include <iostream>
#include <vector>
#include <numeric>    
#include "opencv2/opencv.hpp"
#include "PreProcessing.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <stdint.h>
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
	GaussianBlur(frame_gray,blur, Size(5, 5), 0);

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
	maskimage = imread("/mask/maskimg.JPG");
	cvtColor(maskimage, maskmodel, COLOR_BGR2GRAY);

	for (int i = 0; i < maskmodel.rows; i++) {
		for (int j = 0; j < maskmodel.cols; j++) {

			if (maskmodel.at<uchar>(i,j) == 0)
			{

				maskmodel.at<uchar>(i,j) = 255;
			}
			else
			{
				maskmodel.at<uchar>(i,j) = 0;
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
	Canny(blur, edges, 50.0, 150.0);
	return edges;
}

Mat Processing::DetectLines(Mat mask, Mat raw_frame)
{

/*

 Function : DetectLines()

 Discriptopn: Function to detect straight lines within frame. The function mainly uses Hough Lines Algorithm. In addition, it optimzes line detection by averaging all detected line, hence, detecting more accurate road lanes. Furthermore, it draws the detected straight 
 on the original Raw_Frame.

 Inputs:
 1 - Mat: Raw frame from webcame.
 2 - Mat: Masked frame.

 Outputs:
 1 - Mat: RGB frame with straight lines highlighted.

*/

	vector<Vec4i> lines;
	Mat cdstP;
	vector<float> Right_Line_Slope;
	vector<float> Right_Line_Intercept;
	vector<float> Left_Line_Slope;
	vector<float> Left_Line_Intercept;



	HoughLinesP(mask, lines, 2, CV_PI / 180, 100, 40, 5);
	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];

		if (lines.size() > 0 && (l[2] - l[0] > 0))
		{
			float slope = (l[3] - l[1]) / (l[2] - l[0]);
			float intercept = l[3] - (slope*l[2]);

			if (slope > 0)
			{
				Right_Line_Slope.push_back(slope);
				Right_Line_Intercept.push_back(intercept);
			}
			else
			{
				Left_Line_Slope.push_back(slope);
				Left_Line_Intercept.push_back(intercept);
			}

		}
	}

	float Right_Line_Slope_Average = accumulate(Right_Line_Slope.begin(), Right_Line_Slope.end(), 0.0) / Right_Line_Slope.size();
	float Right_Line_Intercept_Average = accumulate(Right_Line_Intercept.begin(), Right_Line_Intercept.end(), 0.0) / Right_Line_Intercept.size();
	float Left_Line_Slope_Average = accumulate(Left_Line_Slope.begin(), Left_Line_Slope.end(), 0.0) / Left_Line_Slope.size();
	float Left_Line_Intercept_Average = accumulate(Left_Line_Intercept.begin(), Left_Line_Intercept.end(), 0.0) / Left_Line_Intercept.size();
	
	float Ycord0 = 480;
	float Ycord1 = 355;

	float Right_Line_Xcord0 = (Ycord0 - Right_Line_Intercept_Average) / Right_Line_Slope_Average;
	float Right_Line_Xcord1 = (Ycord1 - Right_Line_Intercept_Average) / Right_Line_Slope_Average;

	float Left_Line_Xcord0 = (Ycord0 - Left_Line_Intercept_Average) / Left_Line_Slope_Average;
	float Left_Line_Xcord1 = (Ycord1 - Left_Line_Intercept_Average) / Left_Line_Slope_Average;

	line(raw_frame, Point(Right_Line_Xcord0, Ycord0), Point(Right_Line_Xcord1, Ycord1), Scalar(255, 0, 0), 5, LINE_AA);
	line(raw_frame, Point(Left_Line_Xcord0, Ycord0), Point(Left_Line_Xcord1, Ycord1), Scalar(255, 0, 45), 5, LINE_AA);

	Right_Line_Slope.clear();
	Right_Line_Intercept.clear();
	Left_Line_Slope.clear();
	Left_Line_Intercept.clear();

	return raw_frame;
}
