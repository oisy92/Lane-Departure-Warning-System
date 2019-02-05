#include "pch.h"
#include "Processing.h"
#include "pch.h"
#include <iostream>
#include <vector>
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
	maskimage = imread("C:/Users/simsi/source/repos/ConsoleApplication2/ConsoleApplication2/maskimg.JPG");
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

 Discriptopn: Function to detect straight lines within frame. The function mainly uses Hough Lines Algorithm. Furthermore, it draws the detected straight 
 on the original Raw_Frame.

 Inputs:
 1 - Mat: Raw frame from webcame.
 2 - Mat: Masked frame.

 Outputs:
 1 - Mat: RGB frame with straight lines highlighted.

*/

	vector<Vec4i> lines;
	Mat cdstP;
	HoughLinesP(mask, lines, 2, CV_PI / 180, 100, 40, 5);

	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];
		line(raw_frame, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255, 0, 0), 5, LINE_AA);
	}
	return raw_frame;
}
