#ifndef Identify_hpp
#define Identify_hpp
#include "opencv2/opencv.hpp"
#include "Structs.hpp"
#include "BoundingBox.hpp"

void Identify(cv::Mat& image, std::vector<Segment> N, std::vector<Segment> O, std::vector<Segment> K, std::vector<Segment> I, std::vector<Segment> A);

#endif
