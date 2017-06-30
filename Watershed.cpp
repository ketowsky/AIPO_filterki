#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <string>
#include <queue>
#include <algorithm>
#include "Watershed.h"



WPixel::WPixel(int x, int y, char h) {
	this->x = x;
	this->y = y;
	this->h = h;
	lab = init;
	dist = 0;
	neighVect.reserve(8);
}
WPixel::WPixel() {
	lab = fictitious;
}

void WPixel::addNeighPixel(WPixel* neigh) {
	neighVect.push_back(neigh);
}
std::vector<WPixel*>& WPixel::getNeighPixels() {
	return neighVect;
}

char WPixel::getHeight() const {
	return h;
}
int WPixel::getIntHeight() const {
	return (int)h & 0xff;
}
int WPixel::getX() const {
	return x;
}
int WPixel::getY() const {
	return y;
}

void WPixel::setLabel(int lab) {
	this->lab = lab;
}
void WPixel::setInitLabel() {
	lab = init;
}
void WPixel::setMaskLabel() {
	lab = mask;
}
void WPixel::setWatershedLabel() {
	lab = wshed;
}
int WPixel::getLabel() {
	return lab;
}

bool WPixel::isInit() {
	return lab == init;
}
bool WPixel::isMask() {
	return lab == mask;
}
bool WPixel::isWatershed() {
	return lab == wshed;
}

void WPixel::setDistance(int dist) {
	dist = dist;
}
int WPixel::getDistance() {
	return dist;
}

bool WPixel::isFictitious() {
	return lab == fictitious;
}

bool WPixel::areAllNeighWatershed() {
	for (unsigned i = 0; i < neighVect.size(); i++) {
		WPixel* r = neighVect.at(i);
		if (!r->isWatershed())
			return false;
	}
	return true;
}



WStruct::WStruct(char* pixels, int w, int h) {

	structure.reserve(w * h);
	for (int y = 0; y < h; ++y)
	for (int x = 0; x < w; ++x)
		structure.push_back(new WPixel(x, y, pixels[x + y*w]));


	for (int y = 0; y < h; ++y) {
		int offset = y * w;
		int topOffset = offset + w;
		int bottomOffset = offset - w;

		for (int x = 0; x < w; ++x) {
			int currentindex = x + y*w;
			WPixel* currentPixel = structure.at(currentindex);

			if (x - 1 >= 0) {
				currentPixel->addNeighPixel(structure.at(currentindex - 1));
				if (y - 1 >= 0)
					currentPixel->addNeighPixel(structure.at(currentindex - 1 - w));
				if (y + 1 < h)
					currentPixel->addNeighPixel(structure.at(currentindex - 1 + w));
			}

			if (x + 1 < w) {
				currentPixel->addNeighPixel(structure.at(currentindex + 1));
				if (y - 1 >= 0)
					currentPixel->addNeighPixel(structure.at(currentindex + 1 - w));
				if (y + 1 < h)
					currentPixel->addNeighPixel(structure.at(currentindex + 1 + w));
			}

			if (y - 1 >= 0)
				currentPixel->addNeighPixel(structure.at(currentindex - w));

			if (y + 1 < h)
				currentPixel->addNeighPixel(structure.at(currentindex + w));
		}
	}

	std::sort(structure.begin(), structure.end(),
		[](WPixel * pl, WPixel * pr) { return pl->getIntHeight() < pr->getIntHeight(); });
}

WStruct::~WStruct() {
	while (!structure.empty()) {
		WPixel* p = structure.back();
		delete p; p = NULL;
		structure.pop_back();
	}
}

int WStruct::size() {
	return structure.size();
}

WPixel* WStruct::at(int i) {
	return structure.at(i);
}



Mat Watershed::Run(IplImage* imgSrc, const std::string& imgName) {
	std::string imgNameTmp;
	IplImage* pGray = cvCreateImage(cvGetSize(imgSrc), IPL_DEPTH_8U, 1);
	if (imgSrc->nChannels == 3) {
		cvCvtColor(imgSrc, pGray, CV_BGR2GRAY);
	}
	else if (imgSrc->nChannels == 1)
		pGray = imgSrc;
	IplImage* imgGray = cvCreateImage(cvGetSize(pGray), IPL_DEPTH_8U, 1);
	cvAdaptiveThreshold(pGray, imgGray, 255, 0, 0, 31);

	char* pixels = imgGray->imageData;
	int w = imgGray->width;
	int h = imgGray->height;

	WStruct  wStructure(pixels, w, h);

	std::queue<WPixel*> pQue;
	int curLabel = 0;
	int hIndex1 = 0;
	int hIndex2 = 0;
	for (int h = hMin; h < hMax; ++h) {
		for (int pIndex = hIndex1; pIndex < wStructure.size(); ++pIndex) {
			WPixel* p = wStructure.at(pIndex);


			if (p->getIntHeight() != h) { hIndex1 = pIndex; break; }

			p->setMaskLabel();

			std::vector<WPixel*> neigh = p->getNeighPixels();
			for (unsigned i = 0; i < neigh.size(); ++i) {
				WPixel* q = neigh.at(i);


				if (q->getLabel() >= 0) { p->setDistance(1); pQue.push(p); break; }
			}
		}

		int curDist = 1;
		pQue.push(new WPixel());




		while (true) {

			WPixel* p = pQue.front();
			pQue.pop();
			if (p->isFictitious())
			if (pQue.empty()) {
				delete p;
				p = NULL;
				break;
			}
			else {
				pQue.push(new WPixel());
				curDist++;
				delete p;
				p = pQue.front();
				pQue.pop();
			}

			std::vector<WPixel*> neigh = p->getNeighPixels();
			for (unsigned i = 0; i < neigh.size(); ++i) {
				WPixel* q = neigh.at(i);


				if ((q->getDistance() <= curDist) && (q->getLabel() >= 0)) {

					if (q->getLabel() > 0) {
						if (p->isMask())
							p->setLabel(q->getLabel());
						else if (p->getLabel() != q->getLabel())
							p->setWatershedLabel();
					}
					else if (p->isMask())
						p->setWatershedLabel();
				}
				else if (q->isMask() && (q->getDistance() == 0)) {
					q->setDistance(curDist + 1);
					pQue.push(q);
				}
				else {
					break;
				}
			}

		}

		for (int pIndex = hIndex2; pIndex < wStructure.size(); pIndex++) {
			WPixel* p = wStructure.at(pIndex);

			if (p->getIntHeight() != h) { hIndex2 = pIndex; break; }

			p->setDistance(0);

			if (p->isMask()) {
				curLabel++;
				p->setLabel(curLabel);
				pQue.push(p);

				while (!pQue.empty()) {
					WPixel* q = pQue.front();
					pQue.pop();

					std::vector<WPixel*> neighbours = q->getNeighPixels();

					for (unsigned i = 0; i < neighbours.size(); i++) {
						WPixel* r = neighbours.at(i);

						if (r->isMask()) { r->setLabel(curLabel); pQue.push(r); }
					}
				}
			}
		}
	}
	IplImage* pWS = cvCreateImage(cvGetSize(imgGray), IPL_DEPTH_8U, 1);
	cvZero(imgGray);
	char* wsPixels = pWS->imageData;
	char* grayPixels = pGray->imageData;
	for (int pixelIndex = 0; pixelIndex < wStructure.size(); pixelIndex++) {
		WPixel* p = wStructure.at(pixelIndex);

		if (p->isWatershed() && !p->areAllNeighWatershed()) {
			wsPixels[p->getX() + p->getY()*w] = (char)255;
			grayPixels[p->getX() + p->getY()*w] = (char)255;
		}
	}
	
	Mat output = cvarrToMat(pWS);

	return output;
}


