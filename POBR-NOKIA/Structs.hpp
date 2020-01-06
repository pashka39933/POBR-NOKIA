#ifndef Structs_hpp
#define Structs_hpp
#include "opencv2/opencv.hpp"

struct PixelCoords {
	int i, j;
};

struct Segment {
	int minI, maxI, minJ, maxJ;
	cv::Mat image;
};

#endif
#pragma once
