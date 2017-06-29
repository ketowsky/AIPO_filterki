#ifndef watershed_h
#define watershed_h

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"


class Watershed {
public:

	enum Watershed_enum {
		CIRCLE = 1,
		LINES = 2
	};

	Watershed();
	Watershed(cv::Mat source);
	cv::Mat runAnalyse();
	friend bool operator== (Watershed::Watershed_enum &r, Watershed::Watershed_enum &l);
private:
	cv::Mat imgSrc;
	cv::Mat imgRes;
};

#endif //watershed_h