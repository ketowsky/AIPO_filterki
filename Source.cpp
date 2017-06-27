#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <cstdlib>
#include <ctime>

#include <iostream>
#include <fstream>

#include "MedianFilter.h"
#include "LinearKuwahara.h"

using namespace cv; 


int main(){
	std::cout << "Wybierz metode:\n1) Filtr medianowy\n2) Filtr liniowy Kuwahara" << std::endl;
	int numberM, number;
	std::cin >> numberM;
	std::cout << "Wybierz obraz (wpisz cyfr� od 1-3):" << std::endl;
	std::cin >> number;
	Mat image;
	switch (number){
	case 1:
		image = imread("obr1.jpg",CV_LOAD_IMAGE_GRAYSCALE);
		std::cout << "CHANNEL: " << image.channels() << std::endl;
		break;
	case 2:
		image = imread("obr2.jpg");
		break;
	case 3:
		image = imread("obr3.jpg");
		break;
	default:
		std::cout << "Zla cyfra" << std::endl;
		return 0;
	}

	
	//MedianFilter medianFilter;
	Mat dest;
	if (numberM == 1){
		std::cout << "Wybierz rodzaj tablicy:\n 1)domyslna 3x3\t2)losowa 5x5\t3)wczytaj z pliku(mask.txt)" << std::endl;
		std::cin >> number;
		MedianFilter * medianFilter;
		switch (number){
		case 1:
		{
				  medianFilter = new MedianFilter();
				  dest = medianFilter->operator()(image);
				  break;
		}
		case 2:
		{
				  srand(time(NULL));

				  int ** tab = new int*[5];
				  for (int i = 0; i < 5; i++){
					  tab[i] = new int[5];
					  for (int j = 0; j < 5; j++){
						  tab[i][j] = std::rand() % 6;//przedzial [0-5]
					  }
				  }
				  medianFilter = new MedianFilter(tab, 5);
				  dest = medianFilter->operator()(image);
				  break;
		}
		case 3:
		{
				  int ** tab = NULL;
				  int size;
				  std::ifstream file("mask.txt");
				  if (file.is_open()){
					  file >> size;
					  tab = new int*[size];
					  for (int i = 0; i < size; i++){
						  tab[i] = new int[size];
					  }
					  for (int i = 0; i < size; i++){
						  for (int j = 0; j < size; j++){
							  file >> tab[i][j];
						  }
					  }
				  }
				  
				  medianFilter = new MedianFilter(tab, size);
				  dest = medianFilter->operator()(image);
				  break;
		}
		default:
			std::cout << "Zla cyfra" << std::endl;
			return 0;
		}
	}
	//Linear Kuwahara
	else if (numberM == 2){
		std::cout << "Wybierz wersje:\n1)domyslna\t2)wlasne parametry:" << std::endl;
		std::cin >> number;
		if (number == 1){
			LinearKuwahara linearKuwahara;
			dest = linearKuwahara(image);
		}
		else if (number == 2){
			int len,angle;
			do{
				std::cout << "Podaj dlugosc linii (musi byc nieparzysta):" << std::endl;
				std::cin >> len;
			} while (len%2==0);
			std::cout << "Podaj ilosc katow:" << std::endl;
			std::cin >> angle;
			LinearKuwahara linearKuwahara(len, angle);
			dest = linearKuwahara(image);
		}
		else{
			std::cout << "Zla cyfra" << std::endl;
			return 0;
		}
	}
	
	imshow("Org", image);
	imshow("Dest", dest);
	waitKey(0);

}