#include <iostream>
#include <opencv2/opencv.hpp>
#include "raytracing.h"
#include "checkerboard.h"
#include "trianglemesh.h"
//#include "color.h"


//using namespace cv;
using namespace std;

int main(int argc, char** argv )
{
    int size=4;
    Scence<float> scence(size);
    sphere<float> sp0(vector3<float>(0.0f,0.6f,0.0),0.3,0);
    checkerboard<float> rec0(2,2);
    TriangleMesh<float> mesh;
    mesh.LoadPly("../cgmodels/bunny.ply");
    mesh.Scale(8);
    mesh.Translate(vector3<float>(0,-0.4,0));
    rec0.translate(0,0,-0.9);
    rec0.rotationX(-1.2f);
    scence.objs.append(&sp0);
    //scence.objs.append(&rec0);
    cout<<mesh.num_vetices<<" "<<mesh.num_faces<<endl;
    scence.objs.append(&mesh);
    cout<<scence.objs.len<<endl;
    cv::Mat img;
    img=scence.render().GetOpencvMat();
    //cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
    //cv::imshow("Display Image", img);
    //cv::waitKey(0);
    img.convertTo(img,CV_8UC3,255,0);
    cv::imwrite("./render.png",img);
    return 0;
}
