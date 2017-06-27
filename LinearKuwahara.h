#ifndef LINEARKUWAHARA_HH
#define LINEARKUWAHARA_HH

#include "Algorithm.h"
#include "KernelTab.h"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <limits.h>

using namespace cv;

class LinearKuwahara : public Aipo::Algorithm{
public:
	LinearKuwahara();
	LinearKuwahara(int len, int ang);
	Mat Run(const Mat& image);
	void createKernel();
	int sumTab(Mat* tab);
	void mergeTabs(float*** imageTab, float*** squareTab, float*** sumTab, float*** squareSumTab, Mat* kernelTab);
	void calculateVariance(float*** imageSum, float*** squareSum, float*** valueTab, float*** critTab, int sum);
	void useKuwahara(float*** valueTab, float*** critTab, Mat*kernelTab, float*** resultTab, float*** resultCritTab);
	void setResult(float*** result, float*** resultTab, float*** crit, float*** critTab);
	void filtrImage(Mat* copy, float*** result, double min);

private:
	int variance;
	int length;
	int angle;
	int height;
	int width;
	int channels;
	KernelTab* kernelTab;
	Mat* rotLineMat;
};

#endif