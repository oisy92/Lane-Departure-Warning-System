# Lane Departure Warning System

This is an implementation of Hough Lines algorithm to autonmously detect when a car shifts between road lanes. The software utilizes live webcam video capture. The software expects the webcam to be mounted on the car's dashboard with no visual obstacles at the bottom area [Look image below].
Currently, the software is under development as it requires the following improvements:
1. Shadows detection and elimination.
2. Curved lines detection.
3. Line crossing detection and alarming.
4. Practicle testing on road. (Software has only been tested on YouTube videos by projecting webcam to a second screen showing YouTube videos of car dashcams. You can find such videos by visiting "4K Relaxation Channel" on YouTube)

![alt text](https://raw.githubusercontent.com/oisy92/Lane-Departure-Warning-System/master/images/ProcessedFrame.PNG)

## Prerequisites
1. C++ enviornment (Preferably VS2017)
2. OpenCV 4.0.1

## How to use
### Linux
Run Makefile
### VS2017
Run Main.cpp 