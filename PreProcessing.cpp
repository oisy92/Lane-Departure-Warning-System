#include "pch.h"
#include <iostream>
#include "opencv2/opencv.hpp"
#include "PreProcessing.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <stdint.h>

using namespace cv;
using namespace std;

/**
 *
 *
 *
 * Class for operations prior to detecting road lines
 *
 *
 *
 */



Mat PreProcessing::FrameGray(Mat frame_col)
{

/*
 
 Function : FrameGray()

 Discriptopn: Will change input to grayscale binary frame

 Inputs: 
 1 - Mat: RGB Frame.

 Outputs:
 1 - Mat: GrayScale Frame.
 
 */

	Mat frame_gray;

	cvtColor(frame_col, frame_gray, COLOR_BGR2GRAY);

	return frame_gray;
}

