//
// Created by wuzaiqiang on 19-1-31.
//
#include "raytracing.h"



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

template <typename scalar_t>
vector3<scalar_t> operator*(const scalar_t &v,const vector3<scalar_t> &vec) const
{
    return v*vec;
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
        vector3<scalar_t> position)
{
    this->t=t;
    this->normal=normal;
    this->position=position;
}

template <typename scalar_t>
surface<scalar_t>::~surface() {;}

template <typename scalar_t>
sphere<scalar_t>::sphere(vector3<scalar_t> central, scalar_t radius) {
    this->center=central;
    this->radius=radius;
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
    this->ambient=0.15;
}

template <typename scalar_t>
surface_node<scalar_t>::surface_node(vector3<scalar_t> central,scalar_t radius)
{
    object=new sphere<scalar_t>(central,radius);
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
void surface_list<scalar_t>::append(vector3<scalar_t> central,scalar_t radius)
{
    len++;
    if(p_head==NULL)
    {
        p_head=new surface_node<scalar_t>(central,radius);
        p_end=p_head;
    }
    else
    {
        p_end->next_surface=new surface_node<scalar_t>(central,radius);
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
            IntersectionResult<scalar_t> result;
            //cout<<"here2"<<endl;
            bool ishit;
            ishit=objs->hit(ray.origin,ray.direction,0,100,result);
            //cout<<"here3"<<endl;
            if(ishit)
            {
                //cout<<"hit"<<endl;
                data[u*size+v]=min(1.0,this->light.ambient+max(0.0,0.9*result.normal.dot(light.direction.normalize())));
            }
            else
            {
                data[u*size+v]=0.0;
            }

        }
    }
    return img;
}

template <typename scalar_t>
Scence<scalar_t>::Scence(int img_size,int batch1)
{
    size=img_size;
    batch=batch1;
    objs=new surface_list<scalar_t>[batch];
}

template <typename scalar_t>
Scence<scalar_t>::~Scence()
{
    delete [] objs;
}