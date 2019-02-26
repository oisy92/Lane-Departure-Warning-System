# Lane Departure Warning System

This is an implementation of Hough Lines algorithm to autonmously detect when a car shifts between road lanes. The software utilizes live webcam video capture. The software expects the webcam to be mounted on the car's dashboard with no visual obstacles at the bottom area [Look image below]. 

The current version of the software is not optimum. Although it's capable of detecting road lines, it only does so when road lines are very clear, and no noise present in the frame (cars, buildings, trees). Hough Lines Algorithm will be joined with Neural Network to allow for better line detection performance.

Currently, the software is under development as it requires the following improvements:
1. TensorFlow Machine Learning Framework.
2. Neural Network Implementation.

![alt text](https://github.com/oisy92/Lane-Departure-Warning-System/blob/master/images/LDWS.PNG)

## Prerequisites
1. C++ enviornment (Preferably VS2017)
2. OpenCV 4.0.1

## How to use
1. Create an empty build folder.
2. Navigate the folder and run Cmake to generate a solution files.
3. Compile and run the generated executable.
