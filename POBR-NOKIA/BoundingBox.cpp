#include "BoundingBox.hpp"

void Draw(cv::Mat& I, Segment segment, int color[3], int thickness) {

	cv::Mat_<cv::Vec3b> out = I;

	for (int i = segment.minI; i < segment.maxI; ++i) {
		for (int p = -thickness / 2; p < thickness / 2 + 1; ++p) {
			out(i, segment.minJ + p)[0] = color[0];
			out(i, segment.minJ + p)[1] = color[1];
			out(i, segment.minJ + p)[2] = color[2];
			out(i, segment.maxJ + p)[0] = color[0];
			out(i, segment.maxJ + p)[1] = color[1];
			out(i, segment.maxJ + p)[2] = color[2];
		}
	}
	for (int j = segment.minJ; j < segment.maxJ; ++j) {
		for (int p = -thickness / 2; p < thickness / 2 + 1; ++p) {
			out(segment.minI + p, j)[0] = color[0];
			out(segment.minI + p, j)[1] = color[1];
			out(segment.minI + p, j)[2] = color[2];
			out(segment.maxI + p, j)[0] = color[0];
			out(segment.maxI + p, j)[1] = color[1];
			out(segment.maxI + p, j)[2] = color[2];
		}
	}

}