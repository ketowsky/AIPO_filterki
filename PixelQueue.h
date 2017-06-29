#ifndef PIXELQUEUE_HH
#define PIXELQUEUE_HH

#include <opencv2/opencv.hpp>

#include <queue>

using namespace cv;
using namespace std;

class PixelQueue{
public:
	void putPixel(Point2i pixel);
	Point2i popPixel();
	bool isEmpty();
private:
	queue<Point2i> pixelQueue;
};
#endif