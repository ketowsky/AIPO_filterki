#include "FillHoles.h"

Mat FillHoles::Run(const Mat& image){
	cols = image.cols;
	rows = image.rows;

	Mat output = Mat::zeros(rows,cols,CV_8UC1);
	Mat copy;
	if (image.channels() > 1){
		cvtColor(image, copy, CV_RGB2GRAY);
	}
	else{
		copy = image.clone();
	}
	threshold(copy, copy, 0, 255, CV_THRESH_OTSU);
	imshow("Threshold", copy);

	fillHoles(copy,output,255,0);

	bitwise_not(output,output);

	return output;
}

void FillHoles::fillHoles(const Mat& copy, Mat& output, int objColor, int backColor){
	bool** checkedPixels = new bool*[cols];
	for (int i = 0; i<cols; i++){
		checkedPixels[i] = new bool[rows];
	}

	for (int i = 0; i<cols; i++){
		for (int j = 0; j<rows; j++){
			checkedPixels[i][j] = false;
		}
	}

	for (int i = 0; i < cols; i++){
		for (int j = 0; j < rows; j++){
			if (copy.at<uchar>(j, i) == objColor){
				//search first background pixel
				for (int k = 0; k < cols; k++){
					if (!(i - k < 0 || i + k >= copy.cols || j - k <0 || j + k>= copy.rows)){
						
						if (copy.at<uchar>(i - k, j) == backColor){
							myFloodFill(i - k, j, copy, output, checkedPixels, backColor, objColor);
							break;
						}
						if (copy.at<uchar>(i - k, j - k) == backColor){
							myFloodFill(i - k, j - k, copy, output, checkedPixels, backColor, objColor);
							break;
						}
						if (copy.at<uchar>(i - k, j + k) == backColor){
							myFloodFill(i - k, j + k, copy, output, checkedPixels, backColor, objColor);
							break;
						}
						if (copy.at<uchar>(i + k, j) == backColor){
							myFloodFill(i + k, j, copy, output, checkedPixels, backColor, objColor);
							break;
						}
						if (copy.at<uchar>(i + k, j - k) == backColor){
							myFloodFill(i + k, j - k, copy, output, checkedPixels, backColor, objColor);
							break;
						}
						if (copy.at<uchar>(i + k, j + k) == backColor){
							myFloodFill(i + k, j + k, copy, output, checkedPixels, backColor, objColor);
							break;
						}
						if (copy.at<uchar>(i, j - k) == backColor){
							myFloodFill(i, j - k, copy, output, checkedPixels, backColor, objColor);
							break;
						}
						else if (copy.at<uchar>(i, j + k) == backColor){
							myFloodFill(i, j + k, copy, output, checkedPixels, backColor, objColor);
							break;
						}//end if
					}
				}//end for k
			}//end if ==255
		}//end for j
	}//end for i

	//deallocate
	for (int i = 0; i < cols; i++){
		delete checkedPixels[i];
	}
	delete checkedPixels;
}

void FillHoles::myFloodFill(int x, int y, const Mat& copy, Mat& output, bool** checkedPixels, int backColor, int objColor){
	PixelQueue pixelQueue;
	pixelQueue.putPixel(Point2i(x,y));
	while (!pixelQueue.isEmpty()){
		Point2i pos = pixelQueue.popPixel();
		if (checkedPixels[pos.x][pos.y] == false){
			if (copy.at<uchar>(pos) == backColor){
				output.at<uchar>(pos) = objColor;
				if (pos.x - 1 >= 0){
					pixelQueue.putPixel(Point2i(pos.x - 1, pos.y));
				}
				if (pos.x + 1 < copy.cols){
					pixelQueue.putPixel(Point2i(pos.x + 1, pos.y));
				}
				if (pos.y - 1 >= 0){
					pixelQueue.putPixel(Point2i(pos.x, pos.y - 1));
				}
				if (pos.y + 1 < copy.rows){
					pixelQueue.putPixel(Point2i(pos.x, pos.y + 1));
				}
			}
			checkedPixels[pos.x][pos.y] = true;
		}
	}
}