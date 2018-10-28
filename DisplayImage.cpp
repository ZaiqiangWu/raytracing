#include <iostream>
#include <opencv2/opencv.hpp>
#include "raytracing.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv )
{
    /*if ( argc != 2 )
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }*/

    Mat image(Size(320,240),CV_32FC1);
    image.at<Vec3d>(0,0)[1]=1;
    int n1=image.rows;
    int nc=image.cols*image.channels();
    for (int j=0;j<n1;j++)
    {
        float* data=image.ptr<float>(j);
        for(int i=0;i<nc;i=i+3)
        {
            data[i+2]=0;
            data[i+1]=0;
            data[i]=1;

        }
    }
    namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow("Display Image", image);

    waitKey(0);

    return 0;
}
