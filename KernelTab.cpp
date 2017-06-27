#include "KernelTab.h"

KernelTab::KernelTab(int sizex, int sizey){
	this->sizex = sizex;
	this->sizey = sizey;
	/*tab = new Mat**[sizex];
	for (int i = 0; i < sizex; i++){
		tab[i] = new Mat*[sizey];
	}*/
	tab = new Mat*[sizex*sizey];
	allSizex = 0;
	allSizey = 0;
}

KernelTab::~KernelTab(){
	for (int i = 0; i < sizex; i++){
		delete tab[i];
	}
	delete tab;
}

//Mat* KernelTab::getKernel(int x, int y){
Mat* KernelTab::getKernel(int x){
	//return tab[x][y];
	return tab[x];
}

void KernelTab::putKernel(Mat* matTab){
	/*tab[allSizex][allSizey] = matTab;
	allSizey++;
	if (allSizey == sizey){
		allSizey = 0;
		allSizex++;
	}*/
	tab[allSizex] = matTab;
	allSizex++;
}

int KernelTab::getSize(){
	return allSizex;
}
