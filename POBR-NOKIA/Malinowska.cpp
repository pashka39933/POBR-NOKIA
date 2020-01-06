#include "Malinowska.hpp"
#include "FloodFill.hpp"

std::vector<double> Malinowska(cv::Mat& I, std::vector<double> referenceValues, double tolerance, bool referenceImage) {

	cv::Mat_<cv::Vec3b> in = I;
	int white[3] = { 255, 255, 255 };
	int blue[3] = { 255, 0, 0 };

	std::vector<double> retVec;

	for (int i = 0; i < in.rows; ++i) {
		for (int j = 0; j < in.cols; ++j) {

			if (in(i, j)[0] == white[0] && in(i, j)[1] == white[1] && in(i, j)[2] == white[2]) {
				retVec.push_back(ProcessMalinowskaCoefficient(I, i, j, white, referenceValues, tolerance));
			}
			if (in(i, j)[0] == blue[0] && in(i, j)[1] == blue[1] && in(i, j)[2] == blue[2] && !referenceImage) {
				retVec.push_back(ProcessMalinowskaCoefficient(I, i, j, blue, referenceValues, tolerance));
			}

		}
	}

	return retVec;

}

double ProcessMalinowskaCoefficient(cv::Mat& I, int startI, int startJ, int color[3], std::vector<double> referenceValues, double tolerance) {

	cv::Mat_<cv::Vec3b> tmp = I.clone();
	std::vector <PixelCoords> buffer;

	buffer.push_back({ startI, startJ });

	double S = 0, L = 0;

	while (buffer.size() > 0) {

		PixelCoords pos = buffer.back();
		buffer.pop_back();
		if (
			tmp(pos.i, pos.j)[0] == color[0] &&
			tmp(pos.i, pos.j)[1] == color[1] &&
			tmp(pos.i, pos.j)[2] == color[2]
			) {

			bool border = false;
			for (int i = -1; i < 2; ++i) {
				for (int j = -1; j < 2; ++j) {
					if (tmp(pos.i + i, pos.j + j)[0] != color[0] && tmp(pos.i + i, pos.j + j)[0] != 1)
						border = true;
					if (tmp(pos.i + i, pos.j + j)[1] != color[1] && tmp(pos.i + i, pos.j + j)[0] != 1)
						border = true;
					if (tmp(pos.i + i, pos.j + j)[2] != color[2] && tmp(pos.i + i, pos.j + j)[0] != 1)
						border = true;
				}
			}
			if (border) {
				L++;
				tmp(pos.i, pos.j)[0] = 1;
			}
			else {
				S++;
				tmp(pos.i, pos.j)[0] = 1;
			}

			if (pos.i > 0)
				buffer.push_back({ pos.i - 1, pos.j });
			if (pos.i < tmp.rows - 1)
				buffer.push_back({ pos.i + 1, pos.j });
			if (pos.j > 0)
				buffer.push_back({ pos.i, pos.j - 1 });
			if (pos.j < tmp.cols - 1)
				buffer.push_back({ pos.i, pos.j + 1 });

		}
	}

	double coeff = (L / (2 * std::sqrt(M_PI * S))) - 1;

	bool found = false;
	for (int i = 0; i < referenceValues.size(); ++i) {
		if (std::abs(referenceValues[i] - coeff) < tolerance)
			found = true;
	}
	int black[3] = { 0, 0, 0 };
	int red[3] = { 0, 0, 255 };
	RecurrentFloodFill(I, startI, startJ, color, found ? red : black);

	return coeff;

}