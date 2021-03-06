#ifndef MEDIANFILTER_H
#define MEDIANFILTER_H

#include "algorithm.h"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>


using namespace cv;
class MedianFilter :public Aipo::Algorithm{
public:
	MedianFilter();
	MedianFilter(int **tab, int size);
	~MedianFilter();
	int getMedianGray(const Mat& image, int col, int row);
	int getMedianColor(const Mat& image, int col, int row, int number);
	int countWeights();
	void sortTab(int *tab);
	Mat Run(const Mat& image);
private:
	int size;
	int ** weight_tab;
};

#endif 
