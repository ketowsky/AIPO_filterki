#include "medianfilter.h"


MedianFilter::MedianFilter(){
	size = 3;
	weight_tab = new int*[size];
	for (int i = 0; i<size; i++){
		weight_tab[i] = new int[size];
	}
	for (int i = 0; i<size; i++){
		for (int j = 0; j<size; j++){
			weight_tab[i][j] = 1;
		}
	}
}

MedianFilter::MedianFilter(int** tab, int size){
	this->size = size;
	weight_tab = new int*[size];
	for (int i = 0; i<size; i++){
		weight_tab[i] = new int[size];
	}

	for (int i = 0; i<size; i++){
		for (int j = 0; j<size; j++){
			weight_tab[i][j] = tab[i][j];
		}
	}
}

MedianFilter::~MedianFilter(){
	for (int i = 0; i<size; i++){
		delete weight_tab[i];
	}
	delete weight_tab;
}

Mat MedianFilter::Run(const Mat &image){
	int cols = image.cols;//450
	int rows = image.rows;//333
	int channels = image.channels();
	Mat hsv_image;
	if (channels == 3){
		cvtColor(image, hsv_image, CV_RGB2HSV);
	}
	else if (channels == 1){
		hsv_image = image.clone();
	}
	Mat dest = Mat(rows-2*size, cols-2*size, hsv_image.type());
	for (int i = size; i<cols - size-1; i++){
		for (int j = size; j<rows - size-1; j++){
			if (channels == 1){
				int val = getMedianGray(hsv_image, i, j);
				dest.at<uchar>(j - size, i - size) = val;
			}
			else if (channels == 3){
				int val1 = getMedianColor(hsv_image, i, j, 0);
				int val2 = getMedianColor(hsv_image, i, j, 1);
				int val3 = getMedianColor(hsv_image, i, j, 2);
				dest.at<Vec3b>(j-size, i-size) = Vec3b(val1,val2,val3);
			}
		}
	}
	if (channels == 3){
		cvtColor(dest, dest, CV_HSV2RGB);
	}
	return dest;
}

int MedianFilter::getMedianColor(const Mat& image, int col, int row, int number){
	int step = size / 2;
	int ** temp_tab;
	temp_tab = new int*[size];
	for (int i = 0; i<size; i++){
		temp_tab[i] = new int[size];
	}

	for (int i = -step; i <= step; i++){
		for (int j = -step; j <= step; j++){
				temp_tab[i + step][j + step] = image.at<Vec3b>(row + i, col + j)[number];
			
		}
	}


	int sum = countWeights();

	int* medianTab = new int[sum];
	for (int i = 0; i<size; i++){
		for (int j = 0; j<size; j++){
			for (int k = 0; k<weight_tab[i][j]; k++){
				medianTab[size*i + j + k] = temp_tab[i][j];
			}
		}
	}


	sortTab(medianTab);

	uchar median = medianTab[(size*size) / 2 + 1];


	for (int i = 0; i<size; i++){
		delete temp_tab[i];
	}
	delete temp_tab;
	return median;
}

int MedianFilter::getMedianGray(const Mat& image, int col, int row){
	int step = size / 2;
	int ** temp_tab;
	temp_tab = new int*[size];
	for (int i = 0; i<size; i++){
		temp_tab[i] = new int[size];
	}

	for (int i = -step; i <= step; i++){
		for (int j = -step; j <= step; j++){
			temp_tab[i + step][j + step] = image.at<uchar>(row + i, col + j);
		}
	}


	int sum = countWeights();

	int* medianTab = new int[sum];
	for (int i = 0; i<size; i++){
	for (int j = 0; j<size; j++){
	for (int k = 0; k<weight_tab[i][j]; k++){
	medianTab[size*i + j + k] = temp_tab[i][j];
	}
	}
	}


	sortTab(medianTab);

	uchar median = medianTab[(size*size) / 2 + 1];


	for (int i = 0; i<size; i++){
	delete temp_tab[i];
	}
	delete temp_tab;
	return median;
}

int MedianFilter::countWeights(){
	int sum = 0;
	for (int i = 0; i<size; i++){
		for (int j = 0; j<size; j++){
			sum += weight_tab[i][j];
		}
	}
	return sum;
}

void MedianFilter::sortTab(int* tab){
	for (int i = 0; i<size*size; i++){
		for (int j = 0; j<size*size - 1; j++){
			if (tab[j] > tab[j + 1]){
				int temp = tab[j];
				tab[j] = tab[j + 1];
				tab[j + 1] = temp;
			}
		}
	}
}
