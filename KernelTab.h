#ifndef KERNELTAB_HH
#define KERNELTAB_HH

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>

using namespace cv;

class KernelTab{
public:
	KernelTab(int sizex, int sizey);
	~KernelTab();
	//Mat* getKernel(int x, int y);
	Mat* getKernel(int x);
	void putKernel(Mat* tab);
	int getSize();
private:
	//Mat*** tab;
	Mat** tab;
	int sizex;
	int sizey;
	int allSizex;
	int allSizey;
};

#endif