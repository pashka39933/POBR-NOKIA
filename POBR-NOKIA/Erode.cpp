#include "Erode.hpp"

void Erode(cv::Mat& I, int itt) {

	cv::Mat_<cv::Vec3b> in = I;
	cv::Mat_<cv::Vec3b> out = in.clone();

	for (int iteration = 0; iteration < itt; ++iteration) {
		for (int i = 1; i < in.rows - 1; ++i) {
			for (int j = 1; j < in.cols - 1; ++j) {

				bool border = false;
				for (int x = -1; x < 2; ++x) {
					for (int y = -1; y < 2; ++y) {
						if (in(i + x, j + y)[0] != in(i, j)[0])
							border = true;
						if (in(i + x, j + y)[1] != in(i, j)[1])
							border = true;
						if (in(i + x, j + y)[2] != in(i, j)[2])
							border = true;
					}
				}
				if (border) {
					out(i, j)[0] = 0;
					out(i, j)[1] = 0;
					out(i, j)[2] = 0;
				}

			}
		}
		out.copyTo(in);
	}

}