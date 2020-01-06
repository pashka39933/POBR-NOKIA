#include "Identify.hpp"

#define FILTER_DIMENSION 3

void Identify(cv::Mat& image, std::vector<Segment> N, std::vector<Segment> O, std::vector<Segment> K, std::vector<Segment> I, std::vector<Segment> A) {

	cv::Mat_<cv::Vec3b> out = image;
	std::vector<Segment> segments[] = { N, O, K, I, A };

	Segment bestSegmentFound;
	bestSegmentFound.minI = INT_MAX;
	bestSegmentFound.minJ = INT_MAX;
	bestSegmentFound.maxI = INT_MIN;
	bestSegmentFound.maxJ = INT_MIN;

	int segmentColor1[3] = { 0, 255, 0 };
	int segmentColor2[3] = { 0, 255, 255 };
	
	// Identyfikacja po wykrytych N i A - dok³adne po³o¿enie logo
	for (int n = 0; n < N.size(); ++n) {
		for (int a = 0; a < A.size(); ++a) {

			int NsegmentWidth = N[n].image.cols;
			int AsegmentWidth = A[a].image.cols;
			int NsegmentHeight = N[n].image.rows;
			int AsegmentHeight = A[a].image.rows;

			int NsegmentX = N[n].minJ + NsegmentWidth / 2;
			int AsegmentX = A[a].minJ + AsegmentWidth / 2;
			int NsegmentY = N[n].minI + NsegmentHeight / 2;
			int AsegmentY = A[a].minI + AsegmentHeight / 2;

			if (
				std::abs(NsegmentWidth - AsegmentWidth) < NsegmentWidth / 2 &&
				std::abs(NsegmentY - AsegmentY) < NsegmentHeight &&
				std::abs(NsegmentX - AsegmentX) > 2.5 * NsegmentWidth &&
				std::abs(NsegmentX - AsegmentX) < 4 * NsegmentWidth
				) {
				bestSegmentFound.minI = N[n].minI < A[a].minI ? N[n].minI : A[a].minI;
				bestSegmentFound.maxI = N[n].maxI > A[a].maxI ? N[n].maxI : A[a].maxI;
				bestSegmentFound.minJ = N[n].minJ < A[a].minJ ? N[n].minJ : A[a].minJ;
				bestSegmentFound.maxJ = N[n].maxJ > A[a].maxJ ? N[n].maxJ : A[a].maxJ;

				Draw(out, bestSegmentFound, segmentColor1, 8);

				return;
			}
		}
	}
	
	// Identyfikacja po wykrytych dowolnych dwóch literach - przybli¿one po³o¿enie logo
	bool segmentFound = false;
	for (int i = 0; i < 5; ++i) {
		for (int j = i + 1; j < 5; ++j) {

			std::vector<Segment> seg1 = segments[i];
			std::vector<Segment> seg2 = segments[j];

			for (int itt1 = 0; itt1 < seg1.size(); ++itt1) {
				for (int itt2 = 0; itt2 < seg2.size(); ++itt2) {
					int segmentWidth1 = seg1[itt1].image.cols;
					int segmentWidth2 = seg2[itt2].image.cols;
					int segmentHeight1 = seg1[itt1].image.rows;
					int segmentHeight2 = seg2[itt2].image.rows;

					int segmentX1 = seg1[itt1].minJ + segmentWidth1 / 2;
					int segmentX2 = seg2[itt2].minJ + segmentWidth2 / 2;
					int segmentY1 = seg1[itt1].minI + segmentHeight1 / 2;
					int segmentY2 = seg2[itt2].minI + segmentHeight2 / 2;

					if (
						std::abs(segmentWidth1 - segmentWidth2) < segmentWidth1 / 2 &&
						std::abs(segmentY1 - segmentY2) < segmentHeight1 &&
						std::abs(segmentX1 - segmentX2) > 0 &&
						std::abs(segmentX1 - segmentX2) < 4 * segmentWidth1
						) {
						int segmentMinI = seg1[itt1].minI < seg2[itt2].minI ? seg1[itt1].minI : seg2[itt2].minI;
						int segmentMaxI = seg1[itt1].maxI > seg2[itt2].maxI ? seg1[itt1].maxI : seg2[itt2].maxI;
						int segmentMinJ = seg1[itt1].minJ < seg2[itt2].minJ ? seg1[itt1].minJ : seg2[itt2].minJ;
						int segmentMaxJ = seg1[itt1].maxJ > seg2[itt2].maxJ ? seg1[itt1].maxJ : seg2[itt2].maxJ;

						bestSegmentFound.minI = bestSegmentFound.minI < segmentMinI ? bestSegmentFound.minI : segmentMinI;
						bestSegmentFound.maxI = bestSegmentFound.maxI > segmentMaxI ? bestSegmentFound.maxI : segmentMaxI;
						bestSegmentFound.minJ = bestSegmentFound.minJ < segmentMinJ ? bestSegmentFound.minJ : segmentMinJ;
						bestSegmentFound.maxJ = bestSegmentFound.maxJ > segmentMaxJ ? bestSegmentFound.maxJ : segmentMaxJ;

						segmentFound = true;
					}
				}
			}
		}
	}

	if (segmentFound) {
		Draw(out, bestSegmentFound, segmentColor2, 8);
	}

}