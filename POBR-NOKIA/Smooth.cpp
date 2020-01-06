#include "Smooth.hpp"

#define FILTER_DIMENSION 3

void Smooth(cv::Mat& I) {

	int filter[FILTER_DIMENSION][FILTER_DIMENSION] = {
		{ 1, 1, 1 },
		{ 1, 32, 1 },
		{ 1, 1, 1 },
	};

	cv::Mat_<cv::Vec3b> in = I;
	cv::Mat_<cv::Vec3b> out = in.clone();

	for (int i = (FILTER_DIMENSION / 2); i < in.rows - (FILTER_DIMENSION / 2); ++i) {
		for (int j = (FILTER_DIMENSION / 2); j < in.cols - (FILTER_DIMENSION / 2); ++j) {

			for (int color = 0; color < 3; ++color) {
				int value = 0;
				for (int filterX = 0; filterX < FILTER_DIMENSION; ++filterX) {
					for (int filterY = 0; filterY < FILTER_DIMENSION; ++filterY) {
						value += (filter[filterX][filterY] * in(i + filterX - (FILTER_DIMENSION / 2), j + filterY - (FILTER_DIMENSION / 2))[color]);
					}
				}
				value = value > 255 ? 255 : value;
				value = value < 0 ? 0 : value;
				out(i, j)[color] = value;
			}

		}
	}
	out.copyTo(in);

}