#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

/**
 * Loads The Images From The DataSet Folder
 */
bool loadImages(Mat &mat1, Mat &mat2, Mat &mat3, Mat &mat4, Mat &matLogo) {
	mat1 = imread("Assignment1DataSet/L1.jpg");
	mat2 = imread("Assignment1DataSet/L2.jpg");
	mat3 = imread("Assignment1DataSet/L3.jpg");
	mat4 = imread("Assignment1DataSet/L4.jpg");
	matLogo = imread("Assignment1DataSet/logo.jpg");
	return !(!mat1.data || !mat2.data || !mat3.data || !mat4.data
			|| !matLogo.data);
}

void answerQ1() {
	//Gray Scale Matrix
	Mat mat1 = imread("Assignment1DataSet/L1.jpg", 0);

	//Binary image to hold the result
	Mat result(mat1.size(), mat1.type());

	//The threshold value
	int th = 100;

	for (int y = 0; y < mat1.rows; y++) {
		for (int x = 0; x < mat1.cols; x++) {
			if (mat1.at<uchar>(y, x) >= th)
				result.at<uchar>(y, x) = 255;
			else
				result.at<uchar>(y, x) = 0;
		}
	}

	//Show the result
	imshow("Question1 Answer", result);
	waitKey(0);

}

void answerQ2() {
	Mat mat1 = imread("Assignment1DataSet/L1.jpg", 0);
	Mat matLogo = imread("Assignment1DataSet/logo.jpg", 0);
	Mat result(mat1.size(), mat1.type());
	Mat logoResized;

	//Resize logo to be the same size as mat1
	resize(matLogo, logoResized, mat1.size());

	double alpha = 4.0 / 5.0;
	double beta = (1.0 - alpha);
	//addWeighted(mat1, alpha, logoResized, beta, 0.0, result);

	for (int i = 0; i < mat1.rows; i++)
		for (int j = 0; j < mat1.cols; j++) {
			if (i < mat1.rows && j < mat1.cols)
				result.at<uchar>(i, j) = alpha * mat1.at<uchar>(i, j)
						+ beta * logoResized.at<uchar>(i, j);
		}

	//Show the result
	imshow("Question2 Answer", result);
	waitKey(0);
}

void answerQ3(Mat &mat2) {
	Mat result = Mat::zeros(mat2.size(), mat2.type());

	//Brightness
	double beta = 50;

	for (int y = 0; y < mat2.rows; y++) {
		for (int x = 0; x < mat2.cols; x++) {
			for (int c = 0; c < 3; c++) {
				result.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(
						(mat2.at<Vec3b>(y, x)[c]) + beta);
			}
		}
	}

	//Show the result
	imshow("Question3 Answer", result);
	waitKey(0);
}
float clamp(float x, float a, float b) {
	return x < a ? a : (x > b ? b : x);
}
void answerQ4(Mat &mat3) {

	Point2f p1_dst(7, 6);
	Point2f p2_dst(991, 147);
	Point2f p3_dst(910, 691);

	Point2f p1_src(0, 0);
	Point2f p2_src(mat3.cols - 1, 0);
	Point2f p3(mat3.cols - 1, mat3.rows - 1);

	Matx33f m(p1_src.x, p1_src.y, 1, p2_src.x, p2_src.y, 1, p3.x, p3.y, 1);
	Matx33f mInv(m.inv().val);
	Matx31f a1(mInv * Matx31f(p1_dst.x, p2_dst.x, p3_dst.x));
	Matx31f a2(mInv * Matx31f(p1_dst.y, p2_dst.y, p3_dst.y));
	Matx31f a3(mInv * Matx31f(1, 1, 1));
	Matx33f AffMat(a1.val[0], a1.val[1], a1.val[2], a2.val[0], a2.val[1],
			a2.val[2], a3.val[0], 0, a3.val[2]);
	Mat res(mat3.size(), mat3.type());
	for (int y = 0; y < mat3.rows; y++)
		for (int x = 0; x < mat3.cols; x++) {
			Matx31f dest(AffMat * Matx31f(x, y, 1));
			int xpos = (int) clamp(dest.val[0], 0, mat3.cols);
			int ypos = (int) clamp(dest.val[1], 0, mat3.rows);
			res.at<Vec3b>(y, x) = mat3.at<Vec3b>(ypos, xpos);
		}
	//Show the result
	imshow("Question4 Answer", res);
	waitKey(0);

}

void answerQ5(Mat &mat4) {

// Destination image. The aspect ratio of the book is 3/4
	Size size(200, 200);
	Mat result = Mat::zeros(size, CV_8UC3);

// Create a vector of destination points.
	vector<Point2f> distPoints;

	distPoints.push_back(Point2f(0, 0));
	distPoints.push_back(Point2f(size.width - 1, 0));
	distPoints.push_back(Point2f(size.width - 1, size.height - 1));
	distPoints.push_back(Point2f(0, size.height - 1));

	vector<Point2f> srcPoints;
//Add the corners of the plate starting with top-left and ending with bottom left
	srcPoints.push_back(Point2f(59, 60));
	srcPoints.push_back(Point2f(186, 31));
	srcPoints.push_back(Point2f(181, 144));
	srcPoints.push_back(Point2f(62, 181));

// Set data for mouse event
	Mat im_temp = mat4.clone();

// Calculate the homography Matrix
	Mat h = findHomography(srcPoints, distPoints);

// Warp source image to destination
	warpPerspective(mat4, result, h, size);
// Show image
	imshow("Question5 Answer", result);
	waitKey(0);

}

int main(int argc, char** argv) {
	Mat mat1, mat2, mat3, mat4, matLogo;
	if (!loadImages(mat1, mat2, mat3, mat4, matLogo)) {
		printf("No image data \n");
		return -1;
	}
	int q;
	while (1) {
		cout << "Enter Question Number (1-5) or 0 to exit: " << endl;
		cin >> q;
		if (q == 0)
			break;
		switch (q) {
		case 1:
			answerQ1();
			break;
		case 2:
			answerQ2();
			break;
		case 3:
			answerQ3(mat2);
			break;
		case 4:
			answerQ4(mat3);
			break;
		case 5:
			answerQ5(mat4);
			break;
		}
	}

	return 0;
}
