#include <iostream>
#include <opencv2/opencv.hpp>
#include "raytracing.h"
#include "checkerboard.h"
//#include "color.h"


//using namespace cv;
using namespace std;

int main(int argc, char** argv )
{
    int size=1024;
    Scence<float> scence(size);
    sphere<float> sp0(vector3<float>(0.0f,0.0,-0.3f),0.3,0);
    checkerboard<float> rec0(2,2);
    rec0.translate(0,0,-0.9);
    rec0.rotationX(-1.2f);
    scence.objs.append(&sp0);
    scence.objs.append(&rec0);
    cout<<scence.objs.len<<endl;
    cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
    cv::imshow("Display Image", scence.render().GetOpencvMat());
    cv::waitKey(0);
    return 0;
}
