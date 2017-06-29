#ifndef watershed_h
#define watershed_h

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <string>
#include <queue>
#include <algorithm>


class WPixel {
	static const int init = -1;
	static const int mask = -2;
	static const int wshed = 0;
	static const int fictitious = -3;

	int x;
	int y;
	char h;
	int lab;
	int dist;

	std::vector<WPixel*> neighVect;

public:
	WPixel(int x, int y, char h);
	WPixel();

	void addNeighPixel(WPixel* neigh);
	std::vector<WPixel*>& getNeighPixels();
	char getHeight() const;
	int getIntHeight() const;
	int getX() const;
	int getY() const;
	int getLabel();
	int getDistance();
	void setLabel(int label);
	void setInitLabel();
	void setMaskLabel();
	void setWatershedLabel();
	void setDistance(int dist);
	bool isInit();
	bool isMask();
	bool isWatershed();
	bool isFictitious();
	bool areAllNeighWatershed();
};


class WStruct {
	std::vector<WPixel*> structure;

public:
	WStruct(char* pixels, int w, int h);

	~WStruct();

	int size();

	WPixel* at(int i);
};

class Watershed {
	static const int hMin = 0;
	static const int hMax = 256;

public:
	void Run(IplImage* imgSrc, const std::string& imgName);
};

#endif

