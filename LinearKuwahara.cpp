#include "LinearKuwahara.h"

LinearKuwahara::LinearKuwahara(){
	this->length = 5;
	this->angle = 15;
}

LinearKuwahara::LinearKuwahara(int len, int ang){
	this->length = len;
	this->angle = ang;
}

Mat LinearKuwahara::Run(const Mat& image){
	Mat copy = Mat::zeros(image.size(),image.type());
	createKernel();
	height = image.cols;
	width = image.rows;
	channels = image.channels();

	float*** imageTab = new float**[height];
	for (int i = 0; i < height; i++){
		imageTab[i] = new float*[width];
		for (int j = 0; j < width; j++){
			imageTab[i][j] = new float[channels];
		}
	}
	float*** square = new float**[height];
	for (int i = 0; i < height; i++){
		square[i] = new float*[width];
		for (int j = 0; j < width; j++){
			square[i][j] = new float[channels];
		}
	}
	float*** imageSum = new float**[height];
	for (int i = 0; i < height; i++){
		imageSum[i] = new float*[width];
		for (int j = 0; j < width; j++){
			imageSum[i][j] = new float[channels];
		}
	}
	float*** squareSum = new float**[height];
	for (int i = 0; i < height; i++){
		squareSum[i] = new float*[width];
		for (int j = 0; j < width; j++){
			squareSum[i][j] = new float[channels];
		}
	}
	float*** valueTab = new float**[height];
	for (int i = 0; i < height; i++){
		valueTab[i] = new float*[width];
		for (int j = 0; j < width; j++){
			valueTab[i][j] = new float[channels];
		}
	}
	float*** crit = new float**[height];
	for (int i = 0; i < height; i++){
		crit[i] = new float*[width];
		for (int j = 0; j < width; j++){
			crit[i][j] = new float[channels];
		}
	}
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			for (int k = 0; k < channels;k++)
				crit[i][j][k] = std::numeric_limits<float>::max();
		}
	}
	float*** critTab = new float**[height];
	for (int i = 0; i < height; i++){
		critTab[i] = new float*[width];
		for (int j = 0; j < width; j++){
			critTab[i][j] = new float[channels];
		}
	}
	float*** result = new float**[height];
	for (int i = 0; i < height; i++){
		result[i] = new float*[width];
		for (int j = 0; j < width; j++){
			result[i][j] = new float[channels];
		}
	}
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			for (int k = 0; k < channels; k++)
				result[i][j][k] = 0;
		}
	}
	float*** resultTab = new float**[height];
	for (int i = 0; i < height; i++){
		resultTab[i] = new float*[width];
		for (int j = 0; j < width; j++){
			resultTab[i][j] = new float[channels];
		}
	}
	float*** resultCritTab = new float**[height];
	for (int i = 0; i < height; i++){
		resultCritTab[i] = new float*[width];
		for (int j = 0; j < width; j++){
			resultCritTab[i][j] = new float[channels];
		}
	}

	double min;
	minMaxLoc(image, &min);

	//prepare arrays of image value minus min and square image
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			if (channels == 1){
				imageTab[i][j][0] = image.at<uchar>(i, j) - min;
				square[i][j][0] = imageTab[i][j][0] * imageTab[i][j][0];
			}
			else if (channels == 3){
				for (int k = 0; k < channels; k++){
					imageTab[i][j][k] = image.at<Vec3b>(i, j)[k] - min;
					square[i][j][k] = imageTab[i][j][k] * imageTab[i][j][k];
				}
			}
		}
	}

	for (int i = 0; i < kernelTab->getSize(); i++){
			Mat* tempMat = kernelTab->getKernel(i);
			mergeTabs(imageTab, square, imageSum, squareSum,tempMat);
			int sum = sumTab(tempMat);
			calculateVariance(imageSum, squareSum, valueTab, critTab, sum);
			useKuwahara(valueTab,critTab,tempMat,resultTab,resultCritTab);
			setResult(result, resultTab, crit, resultCritTab);
			filtrImage(&copy, result, min);
	}
	

	//deallocate
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			delete valueTab[i][j];
			delete crit[i][j];
			delete critTab[i][j];
			delete result[i][j];
			delete resultTab[i][j];
			delete resultCritTab[i][j];
			delete imageTab[i][j];
			delete square[i][j];
			delete imageSum[i][j];
			delete squareSum[i][j];
		}
		delete valueTab[i];
		delete crit[i];
		delete critTab[i];
		delete result[i];
		delete resultTab[i];
		delete resultCritTab[i];
		delete imageTab[i];
		delete square[i];
		delete imageSum[i];
		delete squareSum[i];
	}
	delete valueTab;
	delete crit;
	delete critTab;
	delete result;
	delete resultTab;
	delete resultCritTab;
	delete imageTab;
	delete square;
	delete imageSum;
	delete squareSum;

	return copy;
}

int LinearKuwahara::sumTab(Mat* tab){
	int sum = 0;
	for (int i = 0; i < tab->cols; i++){
		for (int j = 0; j < tab->rows; j++){
			sum += tab->at<uchar>(j,i);
		}
	}
	return sum;
}

void LinearKuwahara::createKernel(){
	kernelTab = new KernelTab(length, length);
	int pom = 3;
	int sizeTemp = length + 2 * pom;
	Mat ipLine=Mat::zeros(sizeTemp,sizeTemp,CV_8UC1);
	

	int x1 = (sizeTemp - 1) / 2;
	for (int y1 = 0; y1 < sizeTemp; y1++){
		ipLine.at<uchar>(x1,y1) = 1.0;
	}

	double rotAngle = 180.0 / (double)angle;

	Mat rotatedLine(sizeTemp,sizeTemp, CV_8UC1);
	
	
	for (int i = 0; i < angle; i++){
		rotatedLine = ipLine.clone();
		rotLineMat = new Mat(angle,length*length,CV_8UC1);
		//rotate
		Point2f center(rotatedLine.cols/2.0F, rotatedLine.rows/2.0F);
		Mat rot_mat = getRotationMatrix2D(center,(double)i*rotAngle,1.0);
		Mat dst;
		warpAffine(rotatedLine,dst, rot_mat,rotatedLine.size());

		//write to matrix
		int j = 0;
		for (int a = pom; a < sizeTemp - pom; a++){
			for (int b = pom; b < sizeTemp - pom; b++){
				rotLineMat->at<uchar>(i,j) = dst.at<uchar>(b,a);
				j++;
			}
		}
		//add to KernelTab
		kernelTab->putKernel(rotLineMat);
	}
}

void LinearKuwahara::mergeTabs(float*** imageTab, float*** squareTab, float*** sumTab, float*** squareSumTab, Mat* kernelTab){
	int xmin = (length - 1) / 2;
	int xmax = height - xmin - 1;
	int ymin = (length - 1) / 2;
	int ymax = width - ymin - 1;

	//change to 1D
	int* kernelArray = new int[kernelTab->cols*kernelTab->rows];
	int count = 0;
	for (int i = 0; i < kernelTab->cols; i++){
		for (int j = 0; j < kernelTab->rows; j++){
			kernelArray[count++] = kernelTab->at<uchar>(j,i);
		}
	}

	for (int i = xmin; i <= xmax; i++){
		for (int j = ymin; j <= ymax; j++){
			for (int c = 0; c < channels; c++){
				int x2min = i - xmin;
				int x2max = i + xmin;
				int y2min = j - ymin;
				int y2max = j + ymin;
				float sum = 0, sum2 = 0;
				int pom = 0;
				for (int k = x2min; k <= x2max; k++){
					for (int l = y2min; l <= y2max; l++){
						//for (int z = 0; z < channels; z++){
							sum += imageTab[l][k][c] * (float)kernelArray[pom];
							sum2 += squareTab[l][k][c] * (float)kernelArray[pom];
						//}
						pom++;
					}
				}
				sumTab[i][j][c] = (float)sum;
				squareSumTab[i][j][c] = (float)sum2;
			}
		}
	}
	delete kernelArray;
}

void LinearKuwahara::calculateVariance(float*** imageSum, float*** squareSum, float*** valueTab, float*** critTab, int sum){
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			for (int k = 0; k < channels; k++){
				valueTab[i][j][k] = imageSum[i][j][k]/ (float)sum;
				critTab[i][j][k] = (squareSum[i][j][k] / (float)sum - valueTab[i][j][k] * valueTab[i][j][k]);
			}
		}
	}
}

void LinearKuwahara::useKuwahara(float*** valueTab, float*** critTab, Mat* kernelTab, float*** resultTab, float*** resultCritTab){
	int xmin = (length - 1) / 2;
	int xmax = height - xmin - 1;
	int ymin = (length - 1) / 2;
	int ymax = width - ymin - 1;

	//change to 1D
	int* kernelArray = new int[kernelTab->cols*kernelTab->rows];
	int count = 0;
	for (int i = 0; i < kernelTab->cols; i++){
		for (int j = 0; j < kernelTab->rows; j++){
			kernelArray[count++] = kernelTab->at<uchar>(j, i);
		}
	}


	for (int i = xmin; i <= xmax; i++){
		for (int j = ymin; j <= ymax; j++){
			for (int c = 0; c < channels; c++){
				int x2min = i - xmin;
				int x2max = i + xmin;
				int y2min = j - ymin;
				int y2max = j + ymin;

				float min = std::numeric_limits<float>::max();
				int xminPos = i;
				int yminPos = j;
				int pom = 0;
				for (int k = x2min; k <= x2max; k++){
					for (int l = y2min; l <= y2max; l++){
						if (kernelArray[pom++] > 0){
							//for (int z = 0; z<channels; z++){
								if (critTab[k][l][c] < min){
									min = critTab[k][l][c];
									xminPos = k;
									yminPos = l;
								}
							//}
						}
					}
				}
				resultTab[i][j][c] = valueTab[xminPos][yminPos][c];
				resultCritTab[i][j][c] = min;
			}
		}
	}

	delete kernelArray;
}

void LinearKuwahara::setResult(float*** result,float*** resultTab,float*** crit,float*** critTab){
	for (int i = length; i < height; i++){
		for (int j = length; j < width; j++){
			for (int k = 0; k < channels; k++){
				if (critTab[i][j][k] < crit[i][j][k]){
					crit[i][j][k] = critTab[i][j][k];
					result[i][j][k] = resultTab[i][j][k];
				}
			}
		}
	}
}

void LinearKuwahara::filtrImage(Mat* copy, float*** result, double min){
	int xTemp, yTemp;
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			xTemp = i;
			yTemp = j;
			if (i < length){
				xTemp = length;
			}
			if (i >= width - length){
				xTemp = width - length - 1;
			}
			if (j < length){
				yTemp = length;
			}
			if (j >= height - length){
				yTemp = height - length - 1;
			}
			if (channels == 1){
				copy->at<uchar>(j, i) += (uchar)(result[xTemp][yTemp][0] + 0.5 + min);
			}
			else if (channels == 3){
				for (int k = 0; k < channels; k++){
					copy->at<Vec3b>(j, i)[k] += (result[xTemp][yTemp][k] + min);
				}
			}
		}
	}
}