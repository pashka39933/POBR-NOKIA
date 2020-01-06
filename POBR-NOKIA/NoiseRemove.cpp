#include "NoiseRemove.hpp"

#define FILTER_DIMENSION 5

void NoiseRemove(cv::Mat& I, int iterations) {

	int filter[FILTER_DIMENSION][FILTER_DIMENSION] = {
		{ +0, +0, +1, +0, +0 },
		{ +0, +1, +1, +1, +0 },
		{ +1, +1, +1, +1, +1 },
		{ +0, +1, +1, +1, +0 },
		{ +0, +0, +1, +0, +0 }
	};

	cv::Mat_<cv::Vec3b> in = I;
	cv::Mat_<cv::Vec3b> out = in.clone();

	for (int itt = 0; itt < iterations; ++itt) {

		for (int i = 0; i < in.rows; ++i) {
			for (int j = 0; j < in.cols; ++j) {

				int black = 0, blue = 0, white = 0;
				for (int filterX = 0; filterX < FILTER_DIMENSION; ++filterX) {
					for (int filterY = 0; filterY < FILTER_DIMENSION; ++filterY) {
						if (filter[filterX][filterY] == 1) {

							int inI = i + filterX - (FILTER_DIMENSION / 2);
							int inJ = j + filterY - (FILTER_DIMENSION / 2);

							if (inI > 0 && inI < in.rows && inJ > 0 && inJ < in.cols) {
								if ((in(inI, inJ)[0] == 255) &&
									(in(inI, inJ)[1] == 255))
									white++;
								else if ((in(inI, inJ)[0] == 255) &&
									(in(inI, inJ)[1] == 0))
									blue++;
								else if ((in(inI, inJ)[0] == 0) &&
									(in(inI, inJ)[1] == 0))
									black++;
							}

						}
					}
				}
				int max = (black > blue) ? black : blue;
				max = (max > white) ? max : white;
				if (max == black) {
					out(i, j)[0] = 0;
					out(i, j)[1] = 0;
					out(i, j)[2] = 0;
				}
				else if (max == blue) {
					out(i, j)[0] = 255;
					out(i, j)[1] = 0;
					out(i, j)[2] = 0;
				}
				else if(max == white) {
					out(i, j)[0] = 255;
					out(i, j)[1] = 255;
					out(i, j)[2] = 255;
				}

			}
		}
		out.copyTo(in);

	}

}