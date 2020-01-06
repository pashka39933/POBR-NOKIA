#include "FloodFill.hpp"

void BoundaryFloodFill(cv::Mat& I) {

	int white[3] = { 255, 255, 255 };
	int blue[3] = { 255, 0, 0 };
	int black[3] = { 0, 0, 0 };

	for (int i = 0; i < I.rows; ++i) {
		RecurrentFloodFill(I, i, 0, white, black);
		RecurrentFloodFill(I, i, I.cols - 1, white, black);
		RecurrentFloodFill(I, i, 0, blue, black);
		RecurrentFloodFill(I, i, I.cols - 1, blue, black);
	}
	for (int j = 0; j < I.cols; ++j) {
		RecurrentFloodFill(I, 0, j, white, black);
		RecurrentFloodFill(I, I.rows - 1, j, white, black);
		RecurrentFloodFill(I, 0, j, blue, black);
		RecurrentFloodFill(I, I.rows - 1, j, blue, black);
	}

}

void RecurrentFloodFill(cv::Mat& I, int startI, int startJ, int replacedColor[3], int replaceColor[3]) {

	cv::Mat_<cv::Vec3b> in = I;
	std::vector <PixelCoords> buffer;

	buffer.push_back({ startI, startJ });

	while (buffer.size() > 0) {

		PixelCoords pos = buffer.back();
		buffer.pop_back();
		if (
			in(pos.i, pos.j)[0] == replacedColor[0] &&
			in(pos.i, pos.j)[1] == replacedColor[1] &&
			in(pos.i, pos.j)[2] == replacedColor[2]
			) {

			in(pos.i, pos.j)[0] = replaceColor[0];
			in(pos.i, pos.j)[1] = replaceColor[1];
			in(pos.i, pos.j)[2] = replaceColor[2];

			if (pos.i > 0)
				buffer.push_back({ pos.i - 1, pos.j });
			if (pos.i < in.rows - 1)
				buffer.push_back({ pos.i + 1, pos.j });
			if (pos.j > 0)
				buffer.push_back({ pos.i, pos.j - 1 });
			if (pos.j < in.cols - 1)
				buffer.push_back({ pos.i, pos.j + 1 });

		}
	}

}