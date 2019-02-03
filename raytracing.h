//
// Created by wuzaiqiang on 18-10-28.
//


#ifndef DISPLAYIMAGE_RAYTRACING_H
#define DISPLAYIMAGE_RAYTRACING_H

#include <opencv2/opencv.hpp>
#include <math.h>
#include <iostream>
#define min(a,b)  (((a)<(b))?(a):(b))
#define max(a,b)  (((a)>(b))?(a):(b))

template <typename scalar_t>
class vector3
{
public:
    scalar_t x;
    scalar_t y;
    scalar_t z;
    vector3(scalar_t x,scalar_t y,scalar_t z);
    vector3 operator+(const vector3& v2);

    vector3 operator-(const vector3& v2);

    vector3 operator*(scalar_t v) const;
    friend vector3<scalar_t> operator*(const scalar_t &v,const vector3<scalar_t> &vec) const;
    vector3 operator/(scalar_t v) const;
    scalar_t dot(const vector3& v2);
    vector3 cross(const vector3& v2);
    scalar_t length();
    scalar_t squareLength();
    vector3 normalize();
};

template <typename scalar_t>
class Box
{
public:
    vector3<scalar_t> min_xyz;
    vector3<scalar_t> max_xyz;
    Box(vector3<scalar_t> min,vector3<scalar_t> max);
};

template <typename scalar_t>
class Ray
{
public:
    vector3<scalar_t> origin,direction;
    Ray(vector3<scalar_t> origin,vector3<scalar_t> direction);
    vector3<scalar_t> getPoint(scalar_t t);
};

template <typename scalar_t>
class IntersectionResult
{
public:
    scalar_t t;
    vector3<scalar_t> normal;
    vector3<scalar_t> position;
    IntersectionResult(scalar_t t=0,vector3<scalar_t> normal=vector3<scalar_t>(0,0,0),vector3<scalar_t> position=vector3<scalar_t>(0,0,0));
};

template <typename scalar_t>
class surface
{
public:
    //vector3<scalar_t> center;
    //scalar_t radius;
    virtual bool hit(vector3<scalar_t> e,vector3<scalar_t> d,scalar_t t0,scalar_t t1,IntersectionResult<scalar_t>& rec)=0;
    //virtual box<scalar_t> bounding_box()=0;
    virtual ~surface();

};

template <typename scalar_t>
class sphere:public surface<scalar_t>
{
public:
    /*box<scalar_t> bounding_box()
    {
        vector3<scalar_t> min(center.x-radius,center.y-radius,center.z-radius);
        vector3<scalar_t> max(center.x+radius,center.y+radius,center.z+radius);
        box<scalar_t> bounding(min,max);
        return bounding;
    }*/
    vector3<scalar_t> center;
    scalar_t radius;
    sphere(vector3<scalar_t> central,scalar_t radius);

    bool hit(vector3<scalar_t> e,
             vector3<scalar_t> d,
             scalar_t t0,
             scalar_t t1,
             IntersectionResult<scalar_t>& rec);
};

template <typename scalar_t>
class Camera
{
public:
    vector3<scalar_t> position;
    vector3<scalar_t> forward;
    vector3<scalar_t> right;
    vector3<scalar_t> up;
    scalar_t FOV;
    scalar_t f;
    Camera(vector3<scalar_t> postion=vector3<scalar_t>(0,0,2),
            vector3<scalar_t> forward=vector3<scalar_t>(0,0,-1),
            vector3<scalar_t> right=vector3<scalar_t>(1,0,0),
            scalar_t FOV=1.2,
            scalar_t f=1);
    Ray<scalar_t> generateRay(int u,int v,const int size);
};

template <typename scalar_t>
class Light
{
public:
    vector3<scalar_t> direction;
    scalar_t ambient;
    explicit Light(vector3<scalar_t> d=vector3<scalar_t>(1,1,1));//against the direction light from
};


//the data structure of object stored as linked list
template <typename scalar_t>
class surface_node
{
public:
    surface<scalar_t> *object;
    surface_node<scalar_t> *next_surface;
    surface_node(vector3<scalar_t> central,scalar_t radius);
    ~surface_node();
};

template <typename scalar_t>
class surface_list
{
public:
    int len;
    surface_node<scalar_t> *p_head;
    surface_node<scalar_t> *p_end;
    surface_list();
    ~surface_list();
    void free_memory(surface_node<scalar_t> *p);

    void append(vector3<scalar_t> central,scalar_t radius);

    bool hit(vector3<scalar_t> e,
             vector3<scalar_t> d,
             scalar_t t0,
             scalar_t t1,
             IntersectionResult<scalar_t>& rec);
};

template <typename scalar_t>
class Image
{
public:
    int height;
    int width;
    int channels;
    Image(const int h,const int w,const int c);
    Image(const Image<scalar_t> &img_b);
    ~Image();
    cv::Mat GetOpencvMat();
    scalar_t* ptr()
    {
        return img_p;
    }
private:
    scalar_t *img_p;
};

template <typename scalar_t>
class Scence
{
public:
    Light<scalar_t> light;
    int batch;
    surface_list<scalar_t> *objs;
    Camera<scalar_t> camera;
    int size;
    Scence(int img_size,int batch1);
    ~Scence();
    Image<scalar_t> render();

};

#endif //DISPLAYIMAGE_RAYTRACING_H

