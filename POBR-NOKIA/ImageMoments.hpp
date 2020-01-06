#ifndef ImageMoments_hpp
#define ImageMoments_hpp
#include "opencv2/opencv.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

std::vector<double> ImageMoments(cv::Mat& I);

#endif
