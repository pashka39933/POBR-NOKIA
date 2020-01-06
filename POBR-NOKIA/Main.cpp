#include "Sharpen.hpp"
#include "Threshold.hpp"
#include "Smooth.hpp"
#include "NoiseRemove.hpp"
#include "FloodFill.hpp"
#include "Erode.hpp"
#include "Malinowska.hpp"
#include "ExtractSegments.hpp"
#include "ImageMoments.hpp"
#include "Identify.hpp"

// Ustawienia
#define N_PICS						16
#define NOISE_REMOVE_ITERATIONS		8
#define MALINOWSKA_TOLERANCE		0.125
#define MOMENT_1					1
#define MOMENT_1_TOLERANCE			0.03
#define MOMENT_2					7
#define MOMENT_2_TOLERANCE			0.005

// Kolory
int blue[3] = { 255, 0, 0 };
int black[3] = { 0, 0, 0 };
int red[3] = { 0, 0, 255 };
int white[3] = { 255, 255, 255 };

// Funkcja pomocnicza do zapisu obrazka
void saveImage(cv::Mat image, int id, std::string postfix) {
		std::string path = "images/image" + std::to_string(id) + "/image_" + postfix + ".jpg";
		cv::imwrite(path, image);
}

int main(int, char *[]) {

	// Za³adowanie zdjêæ
	cv::Mat images[N_PICS] = { 
		cv::imread("images/image0/image.jpg"),
		cv::imread("images/image1/image.jpg"),
		cv::imread("images/image2/image.jpg"),
		cv::imread("images/image3/image.jpg"),
		cv::imread("images/image4/image.jpg"),
		cv::imread("images/image5/image.jpg"),
		cv::imread("images/image6/image.jpg"),
		cv::imread("images/image7/image.jpg"),
		cv::imread("images/image8/image.jpg"),
		cv::imread("images/image9/image.jpg"),
		cv::imread("images/image10/image.jpg"),
		cv::imread("images/image11/image.jpg"),
		cv::imread("images/image12/image.jpg"),
		cv::imread("images/image13/image.jpg"),
		cv::imread("images/image14/image.jpg"),
		cv::imread("images/image15/image.jpg")
	};

	// Wyostrzanie
	for (int i = 0; i < N_PICS; ++i) {
		std::cout << "Sharpening " << i << "\n";
		Sharpen(images[i]);
		saveImage(images[i], i, "1");
	}

	std::cout << "\n" << "###" << "\n\n";

	// Progowanie
	for (int i = 0; i < N_PICS; ++i) {
		std::cout << "Thresholding " << i << "\n";
		Threshold(images[i]);
		saveImage(images[i], i, "2");
	}

	std::cout << "\n" << "###" << "\n\n";

	// Redukcja szumu
	for (int i = 0; i < N_PICS; ++i) {
		std::cout << "NoiseRemoving " << i << "\n";
		NoiseRemove(images[i], NOISE_REMOVE_ITERATIONS);
		saveImage(images[i], i, "3");
	}

	std::cout << "\n" << "###" << "\n\n";

	// Wype³nianie t³a
	for (int i = 0; i < N_PICS; ++i) {
		std::cout << "BoundaryFloodFill " << i << "\n";
		BoundaryFloodFill(images[i]);
		saveImage(images[i], i, "4");
	}

	std::cout << "\n" << "###" << "\n\n";

	// Odrzucenie wg wspó³czynnika Malinowskiej
	std::vector<double> fakeReferenceValues;
	fakeReferenceValues.push_back(0);
	cv::Mat tmp = images[0].clone();
	std::vector<double> referenceValues = Malinowska(images[0], fakeReferenceValues, INT_MAX, true);

	std::cout << "Malinowska (reference values):\n";
		std::cout
		<< "N" << "\t" << referenceValues[0] << "\n"
		<< "O" << "\t" << referenceValues[1] << "\n"
		<< "K" << "\t" << referenceValues[2] << "\n"
		<< "I" << "\t" << referenceValues[3] << "\n"
		<< "A" << "\t" << referenceValues[4] << "\n"
		<< "\n";

	std::cout << "\n" << "###" << "\n\n";

	images[0] = tmp;
	for (int i = 0; i < N_PICS; ++i) {
		std::cout << "Malinowska " << i << "\n";
		Malinowska(images[i], referenceValues, MALINOWSKA_TOLERANCE, false);
		saveImage(images[i], i, "5");
	}

	std::cout << "\n" << "###" << "\n\n";
	

	// Ekstrakcja segmentów
	std::vector<std::vector <Segment>> segments;
	for (int i = 0; i < N_PICS; ++i) {
		std::cout << "Extracting segments " << i << "\n";
		std::string command = ("rmdir /S /Q images\\image" + std::to_string(i) + "\\image_6_segments 2> nul");
		system(command.c_str());
		command = ("mkdir images\\image" + std::to_string(i) + "\\image_6_segments 2> nul");
		system(command.c_str());
		segments.push_back(ExtractSegments(images[i], red, black, i));
	}

	std::cout << "\n" << "###" << "\n\n";

	// Niezmienniki momentowe - wartoœci referencyjne
	std::vector<double> N_moments = ImageMoments(segments[0][0].image);
	std::vector<double> O_moments = ImageMoments(segments[0][1].image);
	std::vector<double> K_moments = ImageMoments(segments[0][2].image);
	std::vector<double> I_moments = ImageMoments(segments[0][3].image);
	std::vector<double> A_moments = ImageMoments(segments[0][4].image);

	std::cout << "Image moments (reference values):\n";

	for (int i = 1; i < N_moments.size(); i++)
		std::cout
		<< "\t" << N_moments[i]
		<< "\t" << O_moments[i]
		<< "\t" << K_moments[i]
		<< "\t" << I_moments[i]
		<< "\t" << A_moments[i]
		<< "\n";

	std::cout << "\n" << "###" << "\n\n";
	
	// Niezmienniki momentowe - szukanie pasuj¹cych
	std::vector<Segment> foundN[N_PICS], foundO[N_PICS], foundK[N_PICS], foundI[N_PICS], foundA[N_PICS];
	for (int i = 0; i < N_PICS; ++i) {

		std::string command = ("rmdir /S /Q images\\image" + std::to_string(i) + "\\image_7_moments_match 2> nul");
		system(command.c_str());
		command = ("mkdir images\\image" + std::to_string(i) + "\\image_7_moments_match 2> nul");
		system(command.c_str());

		for (int img = 0; img < segments[i].size(); ++img) {

			std::vector<double> moments = ImageMoments(segments[i][img].image);
			if (std::abs(moments[MOMENT_1] - N_moments[MOMENT_1]) < MOMENT_1_TOLERANCE &&
				std::abs(moments[MOMENT_2] - N_moments[MOMENT_2]) < MOMENT_2_TOLERANCE) {
				foundN[i].push_back(segments[i][img]);
				cv::imwrite("images/image" + std::to_string(i) + "/image_7_moments_match/N_" + std::to_string(img) + ".jpg", segments[i][img].image);
			}
			if (std::abs(moments[MOMENT_1] - O_moments[MOMENT_1]) < MOMENT_1_TOLERANCE &&
				std::abs(moments[MOMENT_2] - O_moments[MOMENT_2]) < MOMENT_2_TOLERANCE) {
				foundO[i].push_back(segments[i][img]);
				cv::imwrite("images/image" + std::to_string(i) + "/image_7_moments_match/O_" + std::to_string(img) + ".jpg", segments[i][img].image);
			}
			if (std::abs(moments[MOMENT_1] - K_moments[MOMENT_1]) < MOMENT_1_TOLERANCE &&
				std::abs(moments[MOMENT_2] - K_moments[MOMENT_2]) < MOMENT_2_TOLERANCE) {
				foundK[i].push_back(segments[i][img]);
				cv::imwrite("images/image" + std::to_string(i) + "/image_7_moments_match/K_" + std::to_string(img) + ".jpg", segments[i][img].image);
			}
			if (std::abs(moments[MOMENT_1] - I_moments[MOMENT_1]) < MOMENT_1_TOLERANCE &&
				std::abs(moments[MOMENT_2] - I_moments[MOMENT_2]) < MOMENT_2_TOLERANCE) {
				foundI[i].push_back(segments[i][img]);
				cv::imwrite("images/image" + std::to_string(i) + "/image_7_moments_match/I_" + std::to_string(img) + ".jpg", segments[i][img].image);
			}
			if (std::abs(moments[MOMENT_1] - A_moments[MOMENT_1]) < MOMENT_1_TOLERANCE &&
				std::abs(moments[MOMENT_2] - A_moments[MOMENT_2]) < MOMENT_2_TOLERANCE) {
				foundA[i].push_back(segments[i][img]);
				cv::imwrite("images/image" + std::to_string(i) + "/image_7_moments_match/A_" + std::to_string(img) + ".jpg", segments[i][img].image);
			}

		}
	}

	// Za³adowanie zdjêæ koñcowych
	cv::Mat finalImages[N_PICS] = {
		cv::imread("images/image0/image.jpg"),
		cv::imread("images/image1/image.jpg"),
		cv::imread("images/image2/image.jpg"),
		cv::imread("images/image3/image.jpg"),
		cv::imread("images/image4/image.jpg"),
		cv::imread("images/image5/image.jpg"),
		cv::imread("images/image6/image.jpg"),
		cv::imread("images/image7/image.jpg"),
		cv::imread("images/image8/image.jpg"),
		cv::imread("images/image9/image.jpg"),
		cv::imread("images/image10/image.jpg"),
		cv::imread("images/image11/image.jpg"),
		cv::imread("images/image12/image.jpg"),
		cv::imread("images/image13/image.jpg"),
		cv::imread("images/image14/image.jpg"),
		cv::imread("images/image15/image.jpg")
	};

	// Identyfikacja
	for (int i = 0; i < N_PICS; ++i) {
		std::cout << "Identyfying " << i << "\n";
		Identify(finalImages[i], foundN[i], foundO[i], foundK[i], foundI[i], foundA[i]);
		saveImage(finalImages[i], i, "FINAL");
	}

	std::cout << "\n" << "### END ###" << "\n\n";

	std::getchar();
	return 0;

}
