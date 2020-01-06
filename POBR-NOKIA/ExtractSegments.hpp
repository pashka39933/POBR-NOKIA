#ifndef ExtractSegments_hpp
#define ExtractSegments_hpp
#include "opencv2/opencv.hpp"
#include "Structs.hpp"

std::vector<Segment> ExtractSegments(cv::Mat& I, int segmentsColor[3], int backgroundColor[3], int imageID);

#endif
#pragma once
