#include "Threshold.hpp"

void Threshold(cv::Mat& I) {

	cv::Mat_<cv::Vec3b> in = I;

	for (int i = 0; i < in.rows; ++i) {
		for (int j = 0; j < in.cols; ++j) {

			float R = (float)in(i, j)[2] / 255;
			float G = (float)in(i, j)[1] / 255;
			float B = (float)in(i, j)[0] / 255;

			float max = (R > G) ? R : G;
			max = (B > max) ? B : max;
			float min = (R < G) ? R : G;
			min = (B < min) ? B : min;

			float value = max;
			float saturation = ((value == 0) ? 0 : (value - min) / value);
			float hue = 0;

			if (value == R) {
				hue = 60 * (G - B) / (value - min);
			}
			else if (value == G) {
				hue = 120 + 60 * (B - R) / (value - min);
			}
			else if (value == B) {
				hue = 240 + 60 * (R - G) / (value - min);
			}
			if (hue < 0)
				hue += 360;

			if (value < 0.5) {
				in(i, j)[0] = 0;
				in(i, j)[1] = 0;
				in(i, j)[2] = 0;
			}
			else if (saturation < 0.2) {
				in(i, j)[0] = 255;
				in(i, j)[1] = 255;
				in(i, j)[2] = 255;
			}
			else if (hue > 100 && hue < 300) {
				in(i, j)[0] = 255;
				in(i, j)[1] = 0;
				in(i, j)[2] = 0;
			}
			else {
				in(i, j)[0] = 0;
				in(i, j)[1] = 0;
				in(i, j)[2] = 0;
			}

		}
	}

}