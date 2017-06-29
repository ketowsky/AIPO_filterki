#include "PixelQueue.h"

void PixelQueue::putPixel(Point2i pixel){
	pixelQueue.push(pixel);
}

Point2i PixelQueue::popPixel(){
	Point2i temp = pixelQueue.front();
	pixelQueue.pop();
	return temp;
}

//true if empty, false otherwise
bool PixelQueue::isEmpty(){
	return pixelQueue.empty();
}