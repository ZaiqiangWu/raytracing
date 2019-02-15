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
using namespace std;
template <typename scalar_t>
class TriangleMesh:public surface<scalar_t>
{
public:
    int num_vetices;
    int num_faces;
    vector3<scalar_t> *vertices;
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

    }
    void LoadPly(char *file_name)
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
                    str.clear ( );
                    getline ( fin, str, '\n' );
                    vertex = atoi(str.c_str());
                }
                    //取得face個數
                else if(str == "face")
                {
                    str.clear ( );
                    getline ( fin, str, '\n' );
                    face = atoi(str.c_str());
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
                if(counter == vertex)	break;
                /*儲存vertex資料*/
                if(str == "")	continue;

                else if(pos%3 == 0)
                {
                    number = atof(str.c_str());
                    //vertex_arrayX[counter] = number;
                    this->vertices[counter].x=number;
                    str.clear ( );
                }
                else if(pos%3 == 1)
                {
                    number = atof(str.c_str());
                    //vertex_arrayY[counter] = number;
                    this->vertices[counter].y=number;
                    str.clear ( );
                }
                else if(pos%3 == 2)
                {
                    number = atof(str.c_str());
                    //vertex_arrayZ[counter] = number;
                    this->vertices[counter].z=number;
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
                    faces[counter*3+0]=point[1];
                    faces[counter*3+1]=point[2];
                    faces[counter*3+2]=point[3];
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
    }
    bool hit(vector3<scalar_t> e,vector3<scalar_t> d,scalar_t t0,scalar_t t1,IntersectionResult<scalar_t>& rec)
    {
        bool flag=false;
        if(!vertices)
        {
            return false;
        }
        else
        {
            scalar_t t_min=10000;
            IntersectionResult<scalar_t> result_temp;
            for(int i=0;i<this->num_faces;i++)
            {
                if(triangles[i].ishit(e,d,t0,t1,result_temp))
                {
                    flag=true;
                    if(result_temp.t<t_min)
                    {
                        t_min=result_temp.t;
                        rec=result_temp;
                    }
                }
                if(flag)
                {
                    rec.mtl=0;//todo
                }
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
