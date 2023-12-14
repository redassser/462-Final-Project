#include <iostream>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace std;
using namespace cv;

struct settings {
    string filein;
    string fileout = "out.jpg";
    int inc = 80;
    int rad = 7;
    int val = 140;
};

settings makeSetting(int argc, char* argv[]) {
    settings setting;

    if (argv[1] == NULL) { cout << "Please type the filename ending with .jpg : ";cin >> setting.filein; return setting; }
    else setting.filein = argv[1];

    if (!argv[2]) return setting;
    setting.fileout = argv[2];

    if (!argv[3]) return setting;
    setting.inc = stoi(argv[3]);

    if (!argv[4]) return setting;
    setting.rad = stoi(argv[4]);

    if (!argv[5]) return setting;
    setting.val = stoi(argv[5]);

    return setting;
}

int main(int argc, char* argv[])
{
    settings setting = makeSetting(argc, argv);
    
    setting.inc = setting.inc > 255 ? 255 : setting.inc;
    setting.inc = setting.inc < 0 ? 0 : setting.inc;
    setting.rad = setting.rad > 32 ? 32 : setting.rad;
    setting.rad = setting.rad < 1 ? 1 : setting.rad;
    setting.rad = setting.rad % 2 == 0 ? setting.rad - 1 : setting.rad;
    setting.val = setting.val > 255 ? 255 : setting.val;
    setting.val = setting.val < 0 ? 0 : setting.val;

    cout << "Scanning with brightness increase " << setting.inc;
    cout << ", blur radius " << setting.rad;
    cout << ", and threshold " << setting.val << endl;

    Mat og = imread(setting.filein, CV_32FC3);

    if (og.empty()) throw runtime_error("File could not be read");

    Mat bright = imread(setting.filein, CV_32FC3);

    Mat o50 = Mat(og.rows, og.cols, CV_32FC3, Scalar(setting.inc, setting.inc, setting.inc));

    Mat filtered, mask, thresh, invmask, out = Mat(og.rows, og.cols, CV_32FC3);
    Mat blurred = filtered = mask = thresh = invmask = out;

    bright += o50;

    GaussianBlur(bright, blurred, Size(setting.rad, setting.rad), 0, 0, BORDER_DEFAULT);

    Laplacian(blurred, filtered, CV_32F, 5);

    threshold(filtered, thresh, setting.val, 255, THRESH_BINARY);

    thresh.convertTo(mask, CV_8U);

    bitwise_not(mask, invmask);

    og.copyTo(out, mask);
    imwrite(setting.fileout, out + invmask);

    cout << "Output to " << setting.fileout;
}
