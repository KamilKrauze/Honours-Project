#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

using namespace cv;
using namespace std;

int main() {

	try {
		string img_path = "./kamil.png";

		Mat image = imread(img_path, IMREAD_COLOR);
		Mat image2;
		image.copyTo(image2);

		cvtColor(image2, image2, COLOR_RGB2GRAY);
		
		Mat dst;
		equalizeHist(image2, dst);

		imshow("Source Image", image);
		imshow("Grayscaled", image2);

		imshow("Histogram Equalized Image", dst);

		waitKey(0);
		destroyAllWindows();

	}
	catch (cv::Exception& e) {
		cerr << e.msg << endl;
	}

	return 0;
	
}