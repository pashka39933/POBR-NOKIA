#ifndef Malinowska_hpp
#define Malinowska_hpp
#include "opencv2/opencv.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Structs.hpp"

std::vector<double> Malinowska(cv::Mat& I, std::vector<double> referenceValues, double tolerance, bool referenceImage);

double ProcessMalinowskaCoefficient(cv::Mat& I, int startI, int startJ, int color[3], std::vector<double> referenceValues, double tolerance);

#endif
