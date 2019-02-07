//
// Created by wuzaiqiang on 18-10-28.
//


#ifndef DISPLAYIMAGE_RAYTRACING_H
#define DISPLAYIMAGE_RAYTRACING_H

#include <opencv2/opencv.hpp>
#include <cmath>
#define min(a,b)  (((a)<(b))?(a):(b))
#define max(a,b)  (((a)>(b))?(a):(b))



template <typename scalar_t>
class vector3
{
public:
    scalar_t x;
    scalar_t y;
    scalar_t z;
    vector3(scalar_t x=0,scalar_t y=0,scalar_t z=0);
    vector3 operator+(const vector3& v2);

    vector3 operator-(const vector3& v2);

    vector3 operator*(scalar_t v) const;
    template <typename scalar_t1>
    friend vector3<scalar_t1> operator*(const scalar_t1 &v,const vector3<scalar_t1> &vec);
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
    int mtl;
    IntersectionResult(scalar_t t=0,vector3<scalar_t> normal=vector3<scalar_t>(0,0,0),vector3<scalar_t> position=vector3<scalar_t>(0,0,0),int mtl=0);
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
    int mtl;

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
    sphere(vector3<scalar_t> central,scalar_t radius,int mtl=0);

    bool hit(vector3<scalar_t> e,
             vector3<scalar_t> d,
             scalar_t t0,
             scalar_t t1,
             IntersectionResult<scalar_t>& rec);
};

template <typename scalar_t>
class rectangle:public surface<scalar_t>
{
public:
    vector3<scalar_t> central;
    scalar_t width;
    scalar_t height;
    vector3<scalar_t> normal;
    vector3<scalar_t> up;
    vector3<scalar_t> right;
    vector3<scalar_t> p0;
    vector3<scalar_t> p1;
    vector3<scalar_t> p2;
    vector3<scalar_t> p3;
    explicit plane(scalar_t h=1.0, scalar_t w=1.0)
    {
        central=vector3<scalar_t>(0,0,0);
        height=h;
        width=w;
        normal=vector3<scalar_t>(0,0,1);
        up=vector3<scalar_t>(0,1,0);
        right=vector3<scalar_t>(1,0,0);
        p0=central+0.5*height*up-0.5*width*right;
        p1=central-0.5*height*up-0.5*width*right;
        p2=central-0.5*height*up+0.5*width*right;
        p3=central+0.5*height*up+0.5*width*right;
    }
    void translate(scalar_t x,scalar_t y,scalar_t z)
    {
        central+=vector3<scalar_t>(x,y,z);
        p0+=vector3<scalar_t>(x,y,z);
        p1+=vector3<scalar_t>(x,y,z);
        p2+=vector3<scalar_t>(x,y,z);
        p3+=vector3<scalar_t>(x,y,z);
    }
    void rotationX(scalar_t theta)
    {
        p0=vectorRotation('x',theta,p0-central)+central;
        p1=vectorRotation('x',theta,p1-central)+central;
        p2=vectorRotation('x',theta,p2-central)+central;
        p3=vectorRotation('x',theta,p3-central)+central;
        normal=vectorRotation('x',theta,normal);
        up=vectorRotation('x',theta,up);
        right=vectorRotation('x',theta,right);
    }
    void rotationY(scalar_t theta)
    {
        p0=vectorRotation('y',theta,p0-central)+central;
        p1=vectorRotation('y',theta,p1-central)+central;
        p2=vectorRotation('y',theta,p2-central)+central;
        p3=vectorRotation('y',theta,p3-central)+central;
        normal=vectorRotation('y',theta,normal);
        up=vectorRotation('y',theta,up);
        right=vectorRotation('y',theta,right);
    }
    void rotationZ(scalar_t theta)
    {
        p0=vectorRotation('z',theta,p0-central)+central;
        p1=vectorRotation('z',theta,p1-central)+central;
        p2=vectorRotation('z',theta,p2-central)+central;
        p3=vectorRotation('z',theta,p3-central)+central;
        normal=vectorRotation('z',theta,normal);
        up=vectorRotation('z',theta,up);
        right=vectorRotation('z',theta,right);
    }
    vector3<scalar_t> vectorRotation(unsigned char mode,scalar_t theta,vector3<scalar_t> v)
    {
        vector3<scalar_t> v_temp;
        if('x'==mode)
        {
            v_temp.x=v.x;
            v_temp.y=v.y*cos(theta)+v.z*sin(theta);
            v_temp.z=v.z*cos(theta)-v.y*sin(theta);
        }
        else if('y'==mode)
        {
            v_temp.y=v.y;
            v_temp.x=v.x*cos(theta)+v.z*sin(theta);
            v_temp.z=v.z*cos(theta)-v.x*sin(theta);
        }
        else if('z'==mode)
        {
            v_temp.z=v.z;
            v_temp.x=v.x*cos(theta)-v.y*sin(theta);
            v_temp.y=v.y*cos(theta)+v.x*sin(theta);
        }
        else
            v_temp=v;
        return v_temp;
    }
    bool hit(vector3<scalar_t> e,
             vector3<scalar_t> d,
             scalar_t t0,
             scalar_t t1,
             IntersectionResult<scalar_t>& rec)
    {
        Ray<scalar_t> ray(e,d);
        scalar_t D;
        D=-normal.x*central.x-normal.y*central.y-normal.z*central.z;
        scalar_t t_intersect;
        t_intersect=-(normal.x*e.x+normal.y*e.y+normal.z*e.z+D)/(normal.x*d.x+normal.y*d.y+normal.z*d.z);
        if(t_intersect<t0||t_intersect>t1)
        {
            return false;
        }
        vector3<scalar_t> point;
        point=ray.getPoint(t_intersect);
        vector3<scalar_t> v0,v1,v2,v3;
        v0=p0-point;
        v1=p1-point;
        v2=p2-point;
        v3=p3-point;
        bool flag=true;
        if(v3.cross(v0).dot(normal)<0)
            flag=false;
        if(v0.cross(v1).dot(normal)<0)
            flag=false;
        if(v1.cross(v2).dot(normal)<0)
            flag=false;
        if(v2.cross(v3).dot(normal)<0)
            flag=false;
        if(!flag)
            return flag;
        rec.normal=normal.normalize();
        rec.position=point;
        rec.mtl=0;
        rec.t=t_intersect;
        return flag;

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
    //scalar_t ambient;
    explicit Light(vector3<scalar_t> d=vector3<scalar_t>(1,1,1));//against the direction light from
};

template <typename scalar_t>
class PointLight
{
    vector3<scalar_t> position;
    explicit PointLight(vector3<scalar_t> pos=vector3<scalar_t>(0,0,0))
    {
        position=pos;
    }
};

//the data structure of object stored as linked list
template <typename scalar_t>
class surface_node
{
public:
    surface<scalar_t> *object;
    surface_node<scalar_t> *next_surface;
    surface_node(vector3<scalar_t> central,scalar_t radius,int mtl=0);
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

    void append(vector3<scalar_t> central,scalar_t radius,int mtl=0);

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
    scalar_t* ptr();
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
    int max_depth;
    scalar_t ambient;
    Scence(int img_size,int batch1);
    ~Scence();
    scalar_t IntersectColor(vector3<scalar_t> origin,vector3<scalar_t> direction,int current_depth);
    Image<scalar_t> render();

};

template <typename scalar_t>
vector3<scalar_t>::vector3(scalar_t x, scalar_t y, scalar_t z) {
    this->x=x;
    this->y=y;
    this->z=z;
}

template <typename scalar_t>
vector3<scalar_t> vector3<scalar_t>::operator+(const vector3<scalar_t>& v2)
{
    vector3 res;
    res.x=this->x+v2.x;
    res.y=this->y+v2.y;
    res.z=this->z+v2.z;
    return res;
}

template <typename scalar_t>
vector3<scalar_t> vector3<scalar_t>::operator-(const vector3<scalar_t>& v2)
{
    vector3 res;
    res.x=this->x-v2.x;
    res.y=this->y-v2.y;
    res.z=this->z-v2.z;
    return res;
}

template <typename scalar_t>
vector3<scalar_t> vector3<scalar_t>::operator*(scalar_t v) const //表示这个函数只有读的操作，没有写操作，不允许函数改变成员变量的值
{
    return vector3(v*x,v*y,v*z);
}

template <typename scalar_t1>
vector3<scalar_t1> operator*(const scalar_t1 &v,const vector3<scalar_t1> &vec)
{
    return vec*v;
}

template <typename scalar_t>
vector3<scalar_t> vector3<scalar_t>:: operator/(scalar_t v) const
{
    return vector3(x/v,y/v,z/v);
}

template <typename scalar_t>
scalar_t vector3<scalar_t>::dot(const vector3& v2)
{
    return this->x*v2.x+this->y*v2.y+this->z*v2.z;
}

template <typename scalar_t>
vector3<scalar_t> vector3<scalar_t>::cross(const vector3<scalar_t> &v2)
{
    vector3 res;
    res.x=this->y*v2.z-this->z*v2.y;
    res.y=this->z*v2.x-this->x*v2.z;
    res.z=this->x*v2.y-this->y*v2.x;
    return res;
}

template <typename scalar_t>
scalar_t vector3<scalar_t>::squareLength()
{
    return x*x+y*y+z*z;
}

template <typename scalar_t>
scalar_t vector3<scalar_t>::length()
{
    return sqrt(x*x+y*y+z*z);
}

template <typename scalar_t>
vector3<scalar_t> vector3<scalar_t>::normalize()
{
    scalar_t invlen=1/length();
    return *this*invlen;
}

template <typename scalar_t>
Box<scalar_t>::Box(vector3<scalar_t> min,vector3<scalar_t> max)
{
    this->min_xyz=min;
    this->max_xyz=max;
}

template <typename scalar_t>
Ray<scalar_t>::Ray(vector3<scalar_t> origin,vector3<scalar_t> direction)
{
    this->origin=origin;
    this->direction=direction;
}

template <typename scalar_t>
vector3<scalar_t> Ray<scalar_t>::getPoint(scalar_t t)
{
    return origin+direction*t;
}

template <typename scalar_t>
IntersectionResult<scalar_t>::IntersectionResult(scalar_t t,
                                                 vector3<scalar_t> normal,
                                                 vector3<scalar_t> position,
                                                 int mtl)
{
    this->t=t;
    this->normal=normal;
    this->position=position;
    this->mtl=mtl;
}

template <typename scalar_t>
surface<scalar_t>::~surface() {;}

template <typename scalar_t>
sphere<scalar_t>::sphere(vector3<scalar_t> central, scalar_t radius,int mtl) {
    this->center=central;
    this->radius=radius;
    this->mtl=mtl;
}

template <typename scalar_t>
bool sphere<scalar_t>::hit(vector3<scalar_t> e,
                           vector3<scalar_t> d,
                           scalar_t t0,
                           scalar_t t1,
                           IntersectionResult<scalar_t>& rec)
{
    vector3<scalar_t> dis=this->center-e;
    scalar_t projection=dis.dot(d.normalize());
    scalar_t dis2c=sqrt(dis.length()*dis.length()-projection*projection);
    if(dis2c<this->radius)
    {
        rec.t=-d.dot(e-center)-sqrt((d.dot(e-center))*(d.dot(e-center))-d.dot(d)*((e-center).squareLength()-radius*radius));
        if(rec.t<t0||rec.t>t1)
            return false;
        rec.t=rec.t/d.squareLength();
        Ray<scalar_t> ray(e,d);
        rec.position=ray.getPoint(rec.t);
        rec.normal=(rec.position-center).normalize();
        rec.mtl=this->mtl;
        return true;
    }
    else
        return false;
}

template <typename scalar_t>
Camera<scalar_t>::Camera(vector3<scalar_t> postion,
                         vector3<scalar_t> forward,
                         vector3<scalar_t> right,
                         scalar_t FOV,
                         scalar_t f)
{
    this->position=postion;
    this->forward=forward;
    this->right=right;
    this->FOV=FOV;
    this->f=f;
    this->up=this->right.cross(this->forward);
}

template <typename scalar_t>
Ray<scalar_t> Camera<scalar_t>::generateRay(int u,int v,const int size)
{
    vector3<scalar_t> center=position+forward.normalize()*f;
    scalar_t pixelSize=f*tan(FOV/2)*2/(scalar_t)size;
    vector3<scalar_t> origin=center-this->right*0.5*pixelSize*size+this->up*0.5*pixelSize*size;
    vector3<scalar_t> location=origin-this->up*(0.5+u)*pixelSize+(0.5+v)*pixelSize;
    Ray<scalar_t> ray(position,(location-position).normalize());
    return ray;
}

template <typename scalar_t>
Light<scalar_t>::Light(vector3<scalar_t> d)//against the direction light from
{
    this->direction=d.normalize();
    //this->ambient=0.15;
}

template <typename scalar_t>
surface_node<scalar_t>::surface_node(vector3<scalar_t> central,scalar_t radius,int mtl)
{
    object=new sphere<scalar_t>(central,radius,mtl);
    next_surface=NULL;
}

template <typename scalar_t>
surface_node<scalar_t>::~surface_node()
{
    delete object;
}

template <typename scalar_t>
surface_list<scalar_t>::surface_list()
{
    len=0;
    p_head=NULL;
    p_end=NULL;
}

template <typename scalar_t>
surface_list<scalar_t>::~surface_list()
{
    if(p_head)
    {
        free_memory(p_head);
    }
}

template <typename scalar_t>
void surface_list<scalar_t>::free_memory(surface_node<scalar_t> *p)
{
    if(p)
    {
        if(p->next_surface)
        {
            free_memory(p->next_surface);
        }
    }

    delete p;
}

template <typename scalar_t>
void surface_list<scalar_t>::append(vector3<scalar_t> central,scalar_t radius,int mtl)
{
    len++;
    if(p_head==NULL)
    {
        p_head=new surface_node<scalar_t>(central,radius,mtl);
        p_end=p_head;
    }
    else
    {
        p_end->next_surface=new surface_node<scalar_t>(central,radius,mtl);
        p_end=p_end->next_surface;
    }
}

template <typename scalar_t>
bool surface_list<scalar_t>::hit(vector3<scalar_t> e,
                                 vector3<scalar_t> d,
                                 scalar_t t0,
                                 scalar_t t1,
                                 IntersectionResult<scalar_t>& rec)
{
    if(!p_head)
        return false;
    else
    {
        surface_node<scalar_t> *p=p_head;
        IntersectionResult<scalar_t> record;
        scalar_t smallest_t=10000.0;
        bool ishit1=false;
        for(int i=0;i<this->len;i++)
        {
            if(p->object->hit(e,d,t0,t1,record))
            {
                ishit1=true;
                if(record.t<smallest_t)
                {
                    smallest_t=record.t;
                    rec=record;
                }
            }
            p=p->next_surface;
        }
        return ishit1;
    }

}

template <typename scalar_t>
Image<scalar_t>::Image(const int h, const int w, const int c) {
    this->height = h;
    this->width = w;
    this->channels = c;
    if(channels>1) channels=3;
    this->img_p = new scalar_t[h * w * c];
}

template<typename scalar_t>
Image<scalar_t>::Image(const Image<scalar_t> &img_b) {
    this->height = img_b.height;
    this->width = img_b.width;
    this->channels = img_b.channels;
    this->img_p = new scalar_t[height * width * channels];
    for (int i = 0; i < height * width * channels; i++) {
        (this->img_p)[i] = (img_b.img_p)[i];
    }
}

template <typename scalar_t>
cv::Mat Image<scalar_t>::GetOpencvMat() {
    cv::Mat cvMat;
    if(1==channels) cvMat.create(height,width,CV_32FC1);
    else cvMat.create(height,width,CV_32FC3);
    int c = cvMat.channels();
    int rows = cvMat.rows; //通道数 * 每个通道包含的列数
    int cols = c * cvMat.cols;

    //判断是否是连续存储
    if (cvMat.isContinuous())
    {
        cols *= rows;
        rows = 1;
    }

    float* p;
    for(int i = 0; i < rows; i++)
    {
        p = cvMat.ptr<float>(i);
        for (int j = 0; j < cols; j++)
        {
            //rgb bgr
            if(3==c)
            {
                p[j-j%3+2]=(float)img_p[i*cols+j-j%3];
                p[j-j%3+1]=(float)img_p[i*cols+j-j%3+1];
                p[j-j%3]=(float)img_p[i*cols+j-j%3+2];
            }
            else
                p[j]=(float)img_p[i*cols+j];
        }
    }
    return cvMat;
}

template<typename scalar_t>
Image<scalar_t>::~Image() {
    delete [] img_p;
}

template <typename scalar_t>
scalar_t Scence<scalar_t>::IntersectColor(vector3<scalar_t> origin, vector3<scalar_t> direction,int current_depth)
{
    IntersectionResult<scalar_t> result;
    IntersectionResult<scalar_t> result1;
    scalar_t color=0.0;
    //cout<<"here2"<<endl;
    bool ishit;
    bool isinshadow;
    ishit=objs->hit(origin,direction,0,100,result);
    if(ishit)
    {
        color+=ambient;
        //cout<<"hit"<<endl;
        //is in shadow?
        isinshadow=objs->hit(result.position,light.direction,0,100,result1);
        if(isinshadow)
        {
            ;
        }
        else
            color+=max(0.0,0.5*result.normal.dot(light.direction.normalize()));
        if(1==result.mtl&&current_depth<max_depth)
        {
            color+=0.1*IntersectColor(result.position,direction-2*direction.dot(result.normal)*result.normal,current_depth+1);//reflection direction
        }
    }
    else
    {
        color=0.0;
    }

    return min(1.0,color);

}

template <typename scalar_t>
Image<scalar_t> Scence<scalar_t>::render()
{
    //cv::Mat img(cv::Size(size,size),CV_32FC1);
    Image<scalar_t> img(size,size,1);
    scalar_t *data=img.ptr();
    for(int u=0;u<size;u++)
    {
        for(int v=0;v<size;v++)
        {
            //cout<<"here0"<<endl;
            Ray<scalar_t> ray=camera.generateRay(u,v,size);
            //cout<<"here1"<<endl;

            data[u*size+v]=IntersectColor(ray.origin,ray.direction,0);


        }
    }
    return img;
}

template <typename scalar_t>
scalar_t* Image<scalar_t>::ptr()
{
    return img_p;
}

template <typename scalar_t>
Scence<scalar_t>::Scence(int img_size,int batch1)
{
    size=img_size;
    batch=batch1;
    objs=new surface_list<scalar_t>[batch];
    max_depth=5;
    ambient=0.1;
}

template <typename scalar_t>
Scence<scalar_t>::~Scence()
{
    delete [] objs;
}
#endif //DISPLAYIMAGE_RAYTRACING_H

