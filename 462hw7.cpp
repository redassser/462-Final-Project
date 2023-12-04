#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>

using namespace std;
using namespace cv;
int main()
{
    Mat input = imread("hw7input.jpg", IMREAD_GRAYSCALE);
    Mat r1 = (Mat_<char>(2, 2) << -1, 0,
                                   0, 1 );
    Mat r2 = (Mat_<char>(2, 2) <<  0,-1,
                                   1, 0 );
    Mat p1 = (Mat_<char>(3, 3) << -1,-1,-1,
                                   0, 0, 0,
                                   1, 1, 1 );
    Mat p2 = (Mat_<char>(3, 3) << -1, 0, 1,
                                  -1, 0, 1,
                                  -1, 0, 1);

    Mat robx, roby, robxy = Mat(512, 512, CV_32F), robth = Mat(512, 512, CV_64F);
    filter2D(input, robx, CV_32F, r1);
    filter2D(input, roby, CV_32F, r2);
    robx = abs(robx);
    roby = abs(roby);
    robxy = robx + roby;

    Mat pewx, pewy, pewxy = Mat(512, 512, CV_32F), pewth = Mat(512, 512, CV_32F);
    filter2D(input, pewx, CV_32F, p1);
    filter2D(input, pewy, CV_32F, p2);
    pewx = abs(pewx);
    pewy = abs(pewy);
    pewxy = pewx + pewy;

    //Begin auto thresh calculation
    int prevT=0, T=100, current=0, belowTnum=0, aboveTnum=0, belowTtotal=0, aboveTtotal = 0;
    while (abs(T - prevT) > 10) {
        aboveTnum = belowTnum = aboveTtotal = belowTtotal = 0;
        for (int i=0;i < 512;i++) {
            for (int j=0;j < 512;j++) {
                current = (int)robxy.at<float>(i, j);
                if (current < T) {
                    belowTnum++; belowTtotal += current;
                }
                else {
                    aboveTnum++; aboveTtotal += current;
                }
            }
        }
        prevT = T;
        T = (belowTtotal / belowTnum + aboveTtotal / aboveTnum) / 2;
    }
    cout << T;

    threshold(robxy, robth, T, 255, THRESH_BINARY);
    threshold(pewxy, pewth, 100, 255, THRESH_BINARY);

    imwrite("robx.jpg", robx);
    imwrite("roby.jpg", roby);
    imwrite("robxy.jpg", robxy);
    imwrite("robth.jpg", robth);

    imwrite("pewx.jpg", pewx);
    imwrite("pewy.jpg", pewy);
    imwrite("pewxy.jpg", pewxy);
    imwrite("pewth.jpg", pewth);
}
