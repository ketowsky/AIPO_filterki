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
    int cols = image.cols;
    int rows = image.rows;
    Mat hsv_image;
    cvtColor(image, hsv_image, CV_RGB2HSV);
   // Mat dest = Mat(cols-2*size, rows-2*size, hsv_image.type());
    Mat dest = Mat(rows, cols, hsv_image.type());
    int step = size / 2;
    for (int i = step; i<rows - step; i++){
        for (int j = step; j<cols - step; j++){
            int val1 = getMedian(hsv_image, i, j, 0);
            int val2 = getMedian(hsv_image, i, j, 1);
            int val3 = getMedian(hsv_image, i, j, 2);
            //dest.at<Vec3b>(i-size, j-size) = Vec3b(val1,val2,val3);
            dest.at<Vec3b>(i,j) = Vec3b(val1,val2,val3);
        }
    }
    std::cout<<"ORG: "<<image.cols<<" x "<<image.rows<<std::endl;
    std::cout<<"DEST: "<<dest.cols<<" x "<<dest.rows<<std::endl;
    cvtColor(dest,dest,CV_HSV2RGB);
    return dest;
}

int MedianFilter::getMedian(const Mat& image, int col, int row, int number){
    int step = size / 2;
    int ** temp_tab;
    temp_tab = new int*[size];
    for (int i = 0; i<size; i++){
        temp_tab[i] = new int[size];
    }
    for (int i = -step; i <= step; i++){
        for (int j = -step; j <= step; j++){
            temp_tab[i + step][j + step] = image.at<Vec3b>(col + i, row + j)[number];
        }
    }


    int sum = countWeights();

    int* medianTab = new int[sum];
    int counter = 0;
    for (int i = 0; i<size; i++){
        for (int j = 0; j<size; j++){
            for (int k = 0; k<weight_tab[i][j]; k++){
                medianTab[counter++] = temp_tab[i][j];
            }
        }
    }


    sortTab(medianTab);

    int median = medianTab[(size*size) / 2 ];


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
