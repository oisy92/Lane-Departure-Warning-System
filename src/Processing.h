#pragma once
#include <iostream>
#include "opencv2/opencv.hpp"
using namespace cv;
using namespace std;

class Processing
{
public:
	Mat FrameGaus(Mat);
	Mat DrawROI(void);
	Mat FrameCanny(Mat);
	Mat ApplyMask(Mat, Mat);
	Mat DetectLines(Mat, Mat);
};

