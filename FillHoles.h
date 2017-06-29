#ifndef FILLHOLES_HH
#define FILLHOLES_HH

#include "Algorithm.h"
#include "PixelQueue.h"

#include <vector>
#include <cmath>

using namespace cv;
using namespace std;

class FillHoles : public Aipo::Algorithm{
public:
	Mat Run(const Mat& image); 
	void myFloodFill(int x, int y, const Mat& copy, Mat& output, bool** checkedPixels, int oldColor, int newColor);
	void fillHoles(const Mat& copy, Mat& output, int color, int newColor);
private:
	int cols;
	int rows;
};

#endif