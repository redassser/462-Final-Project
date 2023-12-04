#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>

using namespace std;
using namespace cv;
int main()
{
    Mat og = imread("462input.jpg", CV_32FC3);
    Mat bright = imread("462input.jpg", CV_32FC3);

    Mat o50 = Mat(og.rows, og.cols, CV_32FC3, Scalar(80,80,80));

    Mat filtered, mask, thresh, invmask, out = Mat(og.rows, og.cols, CV_32FC3, Scalar(180,180,180));
    Mat blurred = filtered = mask = thresh = invmask = out;

    bright += o50;

    GaussianBlur(bright, blurred, Size(7, 7), 0, 0, BORDER_DEFAULT);

    Laplacian(blurred, filtered, CV_32F, 5);

    threshold(filtered, thresh, 140, 255, THRESH_BINARY);

    thresh.convertTo(mask, CV_8U);

    bitwise_not(mask, invmask);

    og.copyTo(out, mask);
    imwrite("output.jpg", out+invmask);
}
