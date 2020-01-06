#include "ExtractSegments.hpp"
#include "FloodFill.hpp"

std::vector<Segment> ExtractSegments(cv::Mat& I, int segmentsColor[3], int backgroundColor[3], int imageID) {

	cv::Mat_<cv::Vec3b> in = I.clone();
	std::vector <PixelCoords> buffer;

	std::vector <Segment> segments;

	for (int j = 0; j < in.cols; ++j) {
		for (int i = 0; i < in.rows; ++i) {

			if (in(i, j)[0] == segmentsColor[0] && in(i, j)[1] == segmentsColor[1] && in(i, j)[2] == segmentsColor[2]) {

				buffer.push_back({ i, j });
				PixelCoords minI = { in.rows, 0 }, maxI = { 0, 0 }, minJ = { 0, in.cols }, maxJ = { 0, 0 };

				while (buffer.size() > 0) {

					PixelCoords pos = buffer.back();
					buffer.pop_back();
					if (in(pos.i, pos.j)[0] == segmentsColor[0] && in(pos.i, pos.j)[1] == segmentsColor[1] && in(pos.i, pos.j)[2] == segmentsColor[2]) {

						bool border = false;
						for (int x = -1; x < 2; ++x) {
							for (int y = -1; y < 2; ++y) {
								if (in(pos.i + x, pos.j + y)[0] == backgroundColor[0])
									border = true;
								if (in(pos.i + x, pos.j + y)[1] == backgroundColor[1])
									border = true;
								if (in(pos.i + x, pos.j + y)[2] == backgroundColor[2])
									border = true;
							}
						}
						if (border) {
							if (pos.i < minI.i) {
								minI.i = pos.i;
								minI.j = pos.j;
							}
							if (pos.i > maxI.i) {
								maxI.i = pos.i;
								maxI.j = pos.j;
							}
							if (pos.j < minJ.j) {
								minJ.i = pos.i;
								minJ.j = pos.j;
							}
							if (pos.j > maxJ.j) {
								maxJ.i = pos.i;
								maxJ.j = pos.j;
							}
						}

						in(pos.i, pos.j)[0] = 0;
						in(pos.i, pos.j)[1] = 255;
						in(pos.i, pos.j)[2] = 255;

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

				cv::Mat_<cv::Vec3b> segmentImage = I.clone();
				cv::Mat_<cv::Vec3b> tmp = I.clone();
				resize(segmentImage, segmentImage, cv::Size(maxJ.j - minJ.j, maxI.i - minI.i));
				for (int itt1 = 0; itt1 < segmentImage.rows; ++itt1) {
					for (int itt2 = 0; itt2 < segmentImage.cols; ++itt2) {
						segmentImage(itt1, itt2) = tmp(minI.i + itt1, minJ.j + itt2);
					}
				}
				int white[3] = { 255, 255, 255 };
				RecurrentFloodFill(segmentImage, 0, minI.j - minJ.j, segmentsColor, white);
				for (int itt1 = 0; itt1 < segmentImage.rows; ++itt1) {
					for (int itt2 = 0; itt2 < segmentImage.cols; ++itt2) {
						if (segmentImage(itt1, itt2)[0] == segmentsColor[0] && segmentImage(itt1, itt2)[1] == segmentsColor[1] && segmentImage(itt1, itt2)[2] == segmentsColor[2]) {
							segmentImage(itt1, itt2)[0] = backgroundColor[0];
							segmentImage(itt1, itt2)[1] = backgroundColor[1];
							segmentImage(itt1, itt2)[2] = backgroundColor[2];
						}
					}
				}
				
				std::string path = "images/image" + std::to_string(imageID) + "/image_6_segments/" + std::to_string(minJ.j) + "_" + std::to_string(maxJ.j) + "_" + std::to_string(minI.i) + "_" + std::to_string(maxI.i) + ".jpg";
				cv::imwrite(path, segmentImage);

				Segment segment;
				segment.image = segmentImage;
				segment.minI = minI.i;
				segment.minJ = minJ.j;
				segment.maxI = maxI.i;
				segment.maxJ = maxJ.j;
				segments.push_back(segment);

			}

		}
	}

	return segments;

}