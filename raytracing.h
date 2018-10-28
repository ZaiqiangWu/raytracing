//
// Created by wuzaiqiang on 18-10-28.
//

#ifndef DISPLAYIMAGE_RAYTRACING_H
#define DISPLAYIMAGE_RAYTRACING_H

#include <math.h>
#include <opencv2/opencv.hpp>
template <typename scalar_t>
class vector3
{
public:
    scalar_t x;
    scalar_t y;
    scalar_t z;
    vector3(scalar_t x=0,scalar_t y=0,scalar_t z=0)
    {
        this->x=x;
        this->y=y;
        this->z=z;
    }
    vector3 operator+(const vector3& v2)
    {
        vector3 res;
        res.x=this->x+v2.x;
        res.y=this->y+v2.y;
        res.z=this->z+v2.z;
        return res;
    }
    vector3 operator-(const vector3& v2)
    {
        vector3 res;
        res.x=this->x-v2.x;
        res.y=this->y-v2.y;
        res.z=this->z-v2.z;
        return res;
    }
    vector3 operator*(scalar_t v) const {
        return vector3(v*x,v*y,v*z);
    }
    vector3 operator/(scalar_t v) const {
        return vector3(x/v,y/v,z/v);
    }
    scalar_t dot(const vector3& v2)
    {
        return this->x*v2.x+this->y*v2.y+this->z*v2.z;
    }
    vector3 cross(const vector3& v2)
    {
        vector3 res;
        res.x=this->y*v2.z-this->z*v2.y;
        res.y=this->z*v2.x-this->x*v2.z;
        res.z=this->x*v2.y-this->y*v2.x;
        return res;
    }
    scalar_t length()
    {
        return sqrt(x*x+y*y+z*z);
    }
    scalar_t squareLength()
    {
        return x*x+y*y+z*z;
    }
    vector3 normalize()
    {
        scalar_t invlen=1/length();
        return *this*invlen;
    }
};

template <typename scalar_t>
class box
{
public:
    vector3<scalar_t> min;
    vector3<scalar_t> max;
    box(vector3<scalar_t> min,vector3<scalar_t> max)
    {
        this->min=min;
        this->max=max;
    }
};

template <typename scalar_t>
class Ray
{
public:
    vector3<scalar_t> origin,direction;
    Ray(vector3<scalar_t> origin,vector3<scalar_t> direction)
    {
        this->origin=origin;
        this->direction=direction;
    }
    vector3<scalar_t> getPoint(scalar_t t)
    {
        return origin+t*direction;
    }
};

template <typename scalar_t>
class IntersectionResult
{
public:
    scalar_t t;
    vector3<scalar_t> normal;
    vector3<scalar_t> position;
    IntersectionResult(scalar_t t,vector3<scalar_t> normal,vector3<scalar_t> position)
    {
        this->t=t;
        this->normal=normal;
        this->position=position;
    }
};

template <typename scalar_t>
class surface
{
public:
    virtual bool hit(vector3<scalar_t> e,vector3<scalar_t> d,scalar_t t0,scalar_t t1,IntersectionResult<scalar_t>& rec)=0;
    virtual box<scalar_t> bounding_box()=0;
};

template <typename scalar_t>
class sphere:public surface<scalar_t>
{
public:
    vector3<scalar_t> center;
    scalar_t radius;
    box<scalar_t> bounding_box()
    {
        vector3<scalar_t> min(center.x-radius,center.y-radius,center.z-radius);
        vector3<scalar_t> max(center.x+radius,center.y+radius,center.z+radius);
        box<scalar_t> bounding(min,max);
        return bounding;
    }

    bool hit(vector3<scalar_t> e,
            vector3<scalar_t> d,
            scalar_t t0,
            scalar_t t1,
            IntersectionResult<scalar_t>& rec)
    {
        vector3<scalar_t> dis=center-e;
        scalar_t projection=dis.dot(d.normalize());
        scalar_t dis2c=sqrt(dis.length()*dis.length()-projection*projection);
        if(dis2c<radius)
        {
            rec.t=-d.dot(e-center)-sqrt(d.dot(e-center).squareLength()-d.dot(d)*((e-center).squareLength()-radius*radius));
            rec.t=rec.t/d.squareLength();
            Ray<scalar_t> ray(e,d);
            rec.position=ray.getPoint(rec.t);
            rec.normal=(rec.position-center).normalize();
            return true;
        }
        else
            return false;
    }
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
    Camera(vector3<scalar_t> postion,vector3<scalar_t> forward,vector3<scalar_t> right,scalar_t FOV,scalar_t f)
    {
        this->position=postion;
        this->forward=forward;
        this->right=right;
        this->FOV=FOV;
        this->f=f;
        this->up=this->right.cross(this->forward)
    }
    Ray<scalar_t> generateRay(int u,int v,const int size)
    {
        vector3<scalar_t> center=position+f*forward.normalize();
        scalar_t pixelSize=f*tan(FOV/2)*2/(scalar_t)size;
        vector3<scalar_t> origin=center-0.5*pixelSize*size*this->right+0.5*pixelSize*size*this->up;
        vector3<scalar_t> location=origin-(0.5+u)*pixelSize*this->up+(0.5+v)*pixelSize;
        Ray<scalar_t> ray(position,(location-position).normalize());
        return ray;
    }
};

template <typename scalar_t>
class Light
{
public:
    vector3<scalar_t> direction;
    Light(vector3<scalar_t> d)
    {
        this->direction=d.normalize();
    }
};

template <typename scalar_t>
class Scence
{
public:
    sphere<scalar_t> sphere0;
    Camera<scalar_t> camera;
    Light<scalar_t> light;
    int size;
    Scence(sphere<scalar_t> sph,Camera<scalar_t> cam,Light<scalar_t> light)
    {
        this->sphere0=sph;
        this->camera=cam;
        this->light=light;
        size=256;
    }
    cv::Mat render()
    {
        cv::Mat img(cv::Size(size,size),CV_32FC1);
        for(int u=0;u<size;u++)
        {
            float* data=img.ptr<float>(u);
            for(int v=0;v<size;v++)
            {
                Ray<scalar_t> ray=camera.generateRay(u,v,size);
                IntersectionResult<scalar_t> result;
                bool ishit=sph.hit(ray.origin,ray.direction,0,100,result);
                if(ishit)
                {
                    result.normal.dot(light.direction.normalize());
                }
            }
        }
        return img;
    }


};

#endif //DISPLAYIMAGE_RAYTRACING_H
