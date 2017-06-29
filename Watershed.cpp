#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <vector>

#include "Watershed.h"

using namespace cv;
using namespace std;

Watershed::Watershed() {
};

Watershed::Watershed(cv::Mat source) {
	this->imgSrc = source.clone();
	this->imgRes = source.clone();
};

cv::Mat Watershed::runAnalyse() {

	for (int x = 0; x < this->imgSrc.rows; x++) {
		for (int y = 0; y < this->imgSrc.cols; y++) {
			if (this->imgSrc.at<Vec3b>(x, y) == Vec3b(255, 255, 255)) {
				this->imgSrc.at<Vec3b>(x, y)[0] = 0;
				this->imgSrc.at<Vec3b>(x, y)[1] = 0;
				this->imgSrc.at<Vec3b>(x, y)[2] = 0;
			}
		}
	}


	// Create a kernel that we will use for accuting/sharpening our image
	Mat kernelMatrix = (Mat_<float>(3, 3) <<
		1, 1, 1,
		1, -8, 1,
		1, 1, 1); // an approximation of second derivative, a quite strong kernel
				  // do the laplacian filtering as it is
				  // well, we need to convert everything in something more deeper then CV_8U
				  // because the kernel has some negative values,
				  // and we can expect in general to have a Laplacian image with negative values
				  // BUT a 8bits unsigned int (the one we are working with) can contain values from 0 to 255
				  // so the possible negative number will be truncated
	Mat imgLaplacian;
	this->imgRes = this->imgSrc; // copy source image to another temporary one
	filter2D(this->imgRes, imgLaplacian, CV_32F, kernelMatrix);
	this->imgSrc.convertTo(this->imgRes, CV_32F);
	this->imgRes = this->imgRes - imgLaplacian;
	// convert back to 8bits gray scale
	this->imgRes.convertTo(this->imgRes, CV_8UC3);
	imgLaplacian.convertTo(imgLaplacian, CV_8UC3);
	
	Mat imgGray;
	cvtColor(this->imgRes, imgGray, CV_BGR2GRAY);
	threshold(imgGray, imgGray, 40, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);



	// Perform the distance transform algorithm
	Mat imgDist;
	distanceTransform(imgGray, imgDist, CV_DIST_L2, 3);
	// Normalize the distance image for range = {0.0, 1.0}
	// so we can visualize and threshold it
	normalize(imgDist, imgDist, 0, 1., NORM_MINMAX);


	// Create the CV_8U version of the distance image
	// It is needed for findContours()
	Mat imgDist8;
	imgDist.convertTo(imgDist8, CV_8U);
	// Find total markers
	vector<vector<Point> > contoursVect;
	findContours(imgDist8, contoursVect, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	// Create the marker image for the watershed algorithm
	Mat markers = Mat::zeros(imgDist.size(), CV_32SC1);
	// Draw the foreground markers
	for (size_t i = 0; i < contoursVect.size(); i++)
		drawContours(markers, contoursVect, static_cast<int>(i), Scalar::all(static_cast<int>(i) + 1), -1);
	// Draw the background marker
	circle(markers, Point(5, 5), 3, CV_RGB(255, 255, 255), -1);




	// Perform the watershed algorithm
	watershed(this->imgRes, markers);
	Mat mark = Mat::zeros(markers.size(), CV_8UC1);
	markers.convertTo(mark, CV_8UC1);
	bitwise_not(mark, mark);

	// image looks like at that point
	// Generate random colorVect
	vector<Vec3b> colorVect;
	for (size_t i = 0; i < contoursVect.size(); i++)
	{
		int b = theRNG().uniform(0, 255);
		int g = theRNG().uniform(0, 255);
		int r = theRNG().uniform(0, 255);
		colorVect.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
	}
	// Create the result image
	this->imgRes = Mat::zeros(markers.size(), CV_8UC3);
	// Fill labeled objects with random colorVect
	for (int i = 0; i < markers.rows; i++)
	{
		for (int j = 0; j < markers.cols; j++)
		{
			int index = markers.at<int>(i, j);
			if (index > 0 && index <= static_cast<int>(contoursVect.size()))
				this->imgRes.at<Vec3b>(i, j) = colorVect[index - 1];
			else
				this->imgRes.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
		}
	}
	// Visualize the final image
	//imshow("Final Result", this->imgRes);

	return this->imgRes;
}

bool operator== (Watershed::Watershed_enum &r, Watershed::Watershed_enum &l) {
	return (int)r == (int)l;
}