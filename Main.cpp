#include"pch.h"
#include <iostream>
#include "opencv2/opencv.hpp"
#include "PreProcessing.h"
#include "Processing.h"

using namespace cv;
using namespace std;


int main(int argv, char** argc)
{
	//Frames Decleration
	Mat Raw_Frame;
	Mat Gray_Frame;
	Mat Blurred_Frame;
	Mat EdgesDetected_Frame;
	Mat Mask_Template;
	Mat Masked_Frame;
	Mat LinesDetected_Frame;

	//WebCam Feed
	VideoCapture vid(0);

	//Objects Delecration
	PreProcessing PreProcessedFrame; 
	Processing UnderProcessingFrame;

	Mask_Template = UnderProcessingFrame.DrawROI();

	while (vid.read(Raw_Frame)) //Frames feeding
	{
		
		Gray_Frame = PreProcessedFrame.FrameGray(Raw_Frame); //Convert RGB frame to grayscale.
		Blurred_Frame = UnderProcessingFrame.FrameGaus(Gray_Frame); //Gaussian Blur: Blur the edges in the grayscale frame.
		EdgesDetected_Frame = UnderProcessingFrame.FrameCanny(Blurred_Frame); //Canny Filter: Detect pixels with high change in gradient.E,g. Detect edges within blurred frame.
		Masked_Frame = UnderProcessingFrame.ApplyMask(EdgesDetected_Frame, Mask_Template); //Apply Mask_Template to block all unwanted area within frame.
		LinesDetected_Frame = UnderProcessingFrame.DetectLines(Masked_Frame, Raw_Frame); //Hough Lines: Detect straight lines within the masked frame, and draw them on Raw_Frame.

		imshow("Processed Frame", LinesDetected_Frame); //Show processed frame.



		if (waitKey(1000) >= 0)
			break;
	}
}
