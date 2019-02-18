//
// Created by wuzaiqiang on 19-2-15.
//

#ifndef DISPLAYIMAGE_TRIANGLEMESH_H
#define DISPLAYIMAGE_TRIANGLEMESH_H


#include <iostream>
#include <fstream>
#include "raytracing.h"
#include "triangle.h"
#include <math.h>
#include <string>
#include "aabb.h"
#include "octree.h"
using namespace std;
template <typename scalar_t>
class TriangleMesh:public surface<scalar_t>
{
public:
    int num_vetices;
    int num_faces;
    AABB<scalar_t> aabb;
    vector3<scalar_t> *vertices;
    vector3<scalar_t> central;
    Octree<scalar_t> octree;
    int *faces;
    Triangle<scalar_t> *triangles;
    TriangleMesh()
    {
        this->mtl=0;
        num_vetices=0;
        num_faces=0;
        vertices=NULL;
        faces=NULL;
        triangles=NULL;
        central=vector3<scalar_t>(0,0,0);

    }
    void Scale(scalar_t scale)
    {
        for(int i=0;i<num_vetices;i++)
        {
            vertices[i]=(vertices[i]-central)*scale+central;
        }
        aabb.x_min=(aabb.x_min-central.x)*scale+central.x;
        aabb.x_max=(aabb.x_max-central.x)*scale+central.x;
        aabb.y_min=(aabb.y_min-central.y)*scale+central.y;
        aabb.y_max=(aabb.y_max-central.y)*scale+central.y;
        aabb.z_min=(aabb.z_min-central.z)*scale+central.z;
        aabb.z_max=(aabb.z_max-central.z)*scale+central.z;
        recompute_octree();
    }
    void Translate(vector3<scalar_t> t)
    {
        for(int i=0;i<num_vetices;i++)
        {
            vertices[i]=vertices[i]+t;
        }
        central=central+t;
        aabb.x_min+=t.x;
        aabb.x_max+=t.x;
        aabb.y_min+=t.y;
        aabb.y_max+=t.y;
        aabb.z_min+=t.z;
        aabb.z_max+=t.z;
        recompute_octree();
    }
    void LoadPly(char *file_name,bool reverse=false)
    {
        ifstream fin ( file_name, ios_base::in );//開檔

        if ( !fin.is_open ( ) )
        {
            cout << "Cannot read the file." << endl;
            cout << "Please check again." << endl;
            exit(0);
        }

        string str;
        int vertex, face;
        char ch;
        int num_property=0;
        bool start_count=false;
        /*讀取header*/

        while ( !fin.eof ( ) )
        {
            fin.get ( ch );
            if( ch != ' ' && ch != '\t' && ch != '\n' )
            {
                str.push_back ( ch );
            }
            else
            {
                //取得vertex個數
                if(str == "vertex")
                {
                    start_count=true;
                    str.clear ( );
                    getline ( fin, str, '\n' );
                    vertex = atoi(str.c_str());
                }
                    //取得face個數
                else if(str == "face")
                {
                    start_count=false;
                    str.clear ( );
                    getline ( fin, str, '\n' );
                    face = atoi(str.c_str());
                }
                else if((str == "float32"||str == "float64"||str=="float")&&start_count)
                {
                    str.clear ( );
                    num_property++;
                }
                else if(str == "end_header")
                {
                    str.clear ( );
                    break;
                }
                else
                    str.clear ( );
            }
        }
        this->num_vetices=vertex;
        this->num_faces=face;

        //動態產生array
        //scalar_t *vertex_arrayX = new scalar_t[vertex];
        //scalar_t *vertex_arrayY = new scalar_t[vertex];
        //scalar_t *vertex_arrayZ = new scalar_t[vertex];
        this->vertices=new vector3<scalar_t>[this->num_vetices];
        this->faces=new int[this->num_faces*3];
        this->triangles=new Triangle<scalar_t>[this->num_faces];

        int pos = 0;
        int counter = 0;
        scalar_t number;

        /*讀取Vertex*/
        while ( !fin.eof ( ) )
        {
            fin.get ( ch );
            if( ch != ' ' && ch != '\t' && ch != '\n' )
                str.push_back ( ch );

            else
            {

                //cout<<"num_property:"<<num_property<<endl;
                if(counter == vertex)	break;
                /*儲存vertex資料*/
                if(str == "")	continue;

                else if(pos%num_property == 0)
                {
                    number = atof(str.c_str());
                    //vertex_arrayX[counter] = number;
                    this->vertices[counter].x=number;
                    str.clear ( );
                }
                else if(pos%num_property == 1)
                {
                    number = atof(str.c_str());
                    //vertex_arrayY[counter] = number;
                    this->vertices[counter].y=number;
                    str.clear ( );
                }
                else if(pos%num_property == 2)
                {
                    number = atof(str.c_str());
                    //vertex_arrayZ[counter] = number;
                    this->vertices[counter].z=number;
                    str.clear ( );
                    //counter++;
                    if(num_property==3)
                    {
                        counter++;
                    }
                }
                else if(pos%num_property == num_property-1)
                {

                    str.clear ( );
                    counter++;
                }
                pos++;
            }
        }

        int point[4];
        int i = 0;
        counter = 0;
        /*畫Polygon*/
        while ( !fin.eof ( ) )
        {
            fin.get ( ch );
            if( ch != ' ' && ch != '\t' && ch != '\n' )
                str.push_back ( ch );
            else
            {
                if(counter == face)		break;
                if(ch == '\n')
                {
                    if(!reverse)
                    {
                        faces[counter*3+0]=point[1];
                        faces[counter*3+1]=point[2];
                        faces[counter*3+2]=point[3];
                    }
                    else
                    {
                        faces[counter*3+1]=point[1];
                        faces[counter*3+0]=point[2];
                        faces[counter*3+2]=point[3];
                    }

                    counter++;
                }
                else if(str == "")	continue;

                else
                {
                    point[i%4] = atoi(str.c_str());
                    i++;
                    str.clear ( );
                }
            }
        }
        fin.close();
        for(int k=0;k<this->num_faces;k++)
        {
            triangles[k].set_vertices(&vertices[faces[k*3+0]],&vertices[faces[k*3+1]],&vertices[faces[k*3+2]]);
        }
        recompute_octree();
    }
    void recompute_aabb()
    {
        for(int k=0;k<this->num_vetices;k++)
        {
            if(vertices[k].x<aabb.x_min)
            {
                aabb.x_min=vertices[k].x;
            }
            if(vertices[k].x>aabb.x_max)
            {
                aabb.x_max=vertices[k].x;
            }
            if(vertices[k].y<aabb.y_min)
            {
                aabb.y_min=vertices[k].y;
            }
            if(vertices[k].y>aabb.y_max)
            {
                aabb.y_max=vertices[k].y;
            }
            if(vertices[k].z<aabb.z_min)
            {
                aabb.z_min=vertices[k].z;
            }
            if(vertices[k].z>aabb.z_max)
            {
                aabb.z_max=vertices[k].z;
            }
        }
    }
    void recompute_octree()
    {
        recompute_aabb();
        octree.generate(triangles,num_faces,aabb);
    }
    bool hit(vector3<scalar_t> e,vector3<scalar_t> d,scalar_t t0,scalar_t t1,IntersectionResult<scalar_t>& rec)
    {
        bool flag=false;
        if(!vertices)
        {
            return false;
        }
        else if(!aabb.ishit(e,d))
        {
            return false;
        }
        else
        {
            /*scalar_t t_min=1000;
            IntersectionResult<scalar_t> result_temp;
            bool is_hit;
            for(int i=0;i<this->num_faces;i++)
            {
                is_hit=triangles[i].ishit(e,d,t0,t1,result_temp);
                if(is_hit)
                {
                    flag=true;
                    if(result_temp.t<t_min)
                    {
                        t_min=result_temp.t;
                        rec=result_temp;
                    }
                }

            }*/
            flag=octree.ishit(e,d,t0,t1,rec);
            if(flag)
            {
                rec.mtl=0;//todo
            }

            return flag;
        }
    }
    ~TriangleMesh()
    {
        if(vertices)
            delete []vertices;
        if(faces)
            delete []faces;
        if(triangles)
            delete []triangles;
    }
};

#endif //DISPLAYIMAGE_TRIANGLEMESH_H
