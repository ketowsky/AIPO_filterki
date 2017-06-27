#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <opencv2/opencv.hpp>

namespace Aipo {
	class Algorithm {
	public:
		virtual cv::Mat Run(const cv::Mat& image) = 0;
		cv::Mat operator()(const cv::Mat &image) { return Run(image); };
	};

}

#endif // ALGORITHM_H
