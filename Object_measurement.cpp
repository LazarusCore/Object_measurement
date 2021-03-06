// Object_measurement.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

Mat image, image_gray, contour_image;
vector<Vec4i> hierarchy;
string filename = ("Reference Image.png");
float rect_width = 100;

int main(int argc, char** argv)
{
	vector<vector<Point>> contour_points;
	vector<Vec4i> hierarchy;
	vector<Vec3f> circles;
	int radius;
	int _levels = 0;
	image = imread(filename.c_str(), IMREAD_COLOR);

	if (image.empty())
	{
		cout << "No image to display" << endl;
		return -1;
	}

	namedWindow("Reference Image", WINDOW_KEEPRATIO);

	//Prepare the image for Hough Circle Transform
	cvtColor(image, image_gray, CV_BGR2GRAY);
	blur(image_gray, image_gray, Size (3,3));
	//Carry out Hough Transform: This finds the centre and radius of the circle
	HoughCircles(image_gray, circles, CV_HOUGH_GRADIENT, 1, image_gray.rows/8, 300, 100);
	
	for (size_t i = 0; i < circles.size(); i++)
	{
		//Round the points to the nearest int and draw them on the image for reference
		Point centre(cvRound(circles[i][0]), cvRound(circles[i][1]));
		radius = cvRound(circles[i][2]);
		circle(image, centre, 3, Scalar(128, 255, 0), -1, 8, 0);
		circle(image, centre, radius, Scalar(0, 0, 255), 3, 8, 0);
	}

	//As there is only 1 circle detected we can assume a few things
	float diameter_in_pixels = 2 * circles[0][2];
	float image_width = float(image.cols);
	//convert the diameter and the image width to a float before doing divisions. 
	//If they are int's they will round the answer to 0
	float circle_diameter = ((diameter_in_pixels / image_width) * rect_width);

	cout << "Diameter of Circle: "; cout << circle_diameter; cout << " mm" << endl;

	imshow("Reference Image", image );
	waitKey(0);
    return 0;
}
