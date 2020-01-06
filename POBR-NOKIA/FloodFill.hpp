#ifndef FloodFill_hpp
#define FloodFill_hpp
#include "opencv2/opencv.hpp"
#include "Structs.hpp"

void BoundaryFloodFill(cv::Mat& I);

void RecurrentFloodFill(cv::Mat& I, int startI, int startJ, int replacedColor[3], int replaceColor[3]);

#endif
