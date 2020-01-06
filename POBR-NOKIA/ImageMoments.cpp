#include "ImageMoments.hpp"

std::vector<double> ImageMoments(cv::Mat& I) {

	cv::Mat_<cv::Vec3b> in = I;

	std::vector<double> moments;

	double m00 = 0, m01 = 0, m02 = 0, m03 = 0, m10 = 0, m11 = 0, m12 = 0, m13 = 0, m20 = 0, m21 = 0, m22 = 0, m23 = 0, m30 = 0, m31 = 0, m32 = 0, m33 = 0;

	using namespace std;

	for (int i = 0; i < in.rows; ++i) {
		for (int j = 0; j < in.cols; ++j) {

			if ((in(i, j)[0] != 0 && in(i, j)[1] != 0 && in(i, j)[2] != 0)) {

				m00 += (pow(i, 0) * pow(j, 0));
				m01 += (pow(i, 0) * pow(j, 1));
				m02 += (pow(i, 0) * pow(j, 2));
				m03 += (pow(i, 0) * pow(j, 3));

				m10 += (pow(i, 1) * pow(j, 0));
				m11 += (pow(i, 1) * pow(j, 1));
				m12 += (pow(i, 1) * pow(j, 2));
				m13 += (pow(i, 1) * pow(j, 3));

				m20 += (pow(i, 2) * pow(j, 0));
				m21 += (pow(i, 2) * pow(j, 1));
				m22 += (pow(i, 2) * pow(j, 2));
				m23 += (pow(i, 2) * pow(j, 3));

				m30 += (pow(i, 3) * pow(j, 0));
				m31 += (pow(i, 3) * pow(j, 1));
				m32 += (pow(i, 3) * pow(j, 2));
				m33 += (pow(i, 3) * pow(j, 3));

			}

		}
	}

	double M00 = m00;
	double M01 = m01 - (m01 / m00) * m00;
	double M10 = m10 - (m10 / m00) * m00;
	double M11 = m11 - m10 * m01 / m00;
	double M20 = m20 - pow(m10, 2) / m00;
	double M02 = m02 - pow(m01, 2) / m00;
	double i = m10 / m00;
	double j = m01 / m00;
	double M21 = m21 - 2 * m11 * i - m20 * j + 2 * m01 * pow(i, 2);
	double M12 = m12 - 2 * m11 * j - m02 * i + 2 * m10 * pow(j, 2);
	double M30 = m30 - 3 * m20 * i + 2 * m10 * pow(i, 2);
	double M03 = m03 - 3 * m02 * j + 2 * m01 * pow(j, 2);

	moments.push_back(-1);
	moments.push_back((M20 + M02) / pow(m00, 2));
	moments.push_back(pow(M20 - M02, 2) + 4 * pow(M11, 2) / pow(m00, 4));
	moments.push_back((pow(M30 - 3 * M12, 2) + pow(3 * M21 - M03, 2)) / pow(m00, 5));
	moments.push_back((pow(M30 + M12, 2) + pow(M21 + M03, 2)) / pow(m00, 5));
	moments.push_back(((M30 - 3 * M12) * (M30 + M12) * (pow(M30 + M12, 2) - 3 * pow(M21 + M03, 2)) + (3 * M21 - M03) * (M21 + M03) * (3 * pow(M30 + M12, 2) - pow(M21 + M03, 2))) / pow(m00, 10));
	moments.push_back(((M20 - M02) * (pow(M30 + M12, 2) - pow(M21 + M03, 2)) + 4 * M11 * (M30 + M12) * (M21 + M03)) / pow(m00, 7));
	moments.push_back((M20 * M02 - pow(M11, 2)) / pow(m00, 4));
	moments.push_back((M30 * M12 + M21 * M03 - pow(M12, 2) - pow(M21, 2)) / pow(m00, 5));
	moments.push_back((M20 * (M21 * M03 - pow(M12, 2)) + M02 * (M03 * M12 - pow(M21, 2)) - M11 * (M30 * M03 - M21 * M12)) / pow(m00, 7));
	moments.push_back((pow(M30 * M03 - M12 * M21, 2) - 4 * (M30 * M12 - pow(M21, 2)) * (M03 * M21 - M12)) / pow(m00, 10));

	return moments;

}