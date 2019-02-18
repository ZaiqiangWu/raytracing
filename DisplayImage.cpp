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
    int size=400;
    Scence<float> scence(size);
    sphere<float> sp0(vector3<float>(0.0f,0.6f,0.0),0.3,0);
    checkerboard<float> rec0(2,2);
    TriangleMesh<float> mesh;
    mesh.LoadPly("../cgmodels/dragon.ply");
    mesh.Scale(5);
    mesh.Translate(vector3<float>(0.15,-0.8,0.6));
    rec0.translate(0.0,0,-0.9);
    rec0.rotationX(-1.2f);
    //scence.objs.append(&sp0);
    //scence.objs.append(&rec0);
    cout<<mesh.num_vetices<<" "<<mesh.num_faces<<endl;
    cout<<mesh.aabb.x_min<<" "<<mesh.aabb.x_max<<endl;
    cout<<mesh.aabb.y_min<<" "<<mesh.aabb.y_max<<endl;
    cout<<mesh.aabb.z_min<<" "<<mesh.aabb.z_max<<endl;
    cout<<mesh.vertices[1].x<<" "<<mesh.vertices[1].y<<endl;
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
