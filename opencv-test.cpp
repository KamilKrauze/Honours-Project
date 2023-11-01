#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

using namespace cv;
using namespace std;

int main() {
	string img_path = "C:/Users/Kamil Krauze/Pictures/banner.png";
	string out_img = "C:/Users/Kamil Krauze/Pictures/banner-smol.png";

	Mat image = imread(img_path, IMREAD_COLOR);
	cout << image.size().width << ", " << image.size().height << "\n";
	resize(image, image, { image.size().width / 2, image.size().height / 2 }, 0, 0, INTER_LINEAR);

	imshow("Image", image);

	return 0;
	
}