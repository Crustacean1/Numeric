module;
#include <math.h>

export module geometry:triangle;

export class Point{
    public:
    float x;
    float y;
};

export class Triangle{
    public:
    Point vertices[3];
    float getCircumference();
};

float Triangle::getCircumference(){
    int dist = 0;
    for(int i = 0;i<3;++i){
        dist += sqrt(pow(vertices[i].x - vertices[(i+1)%3].x,2)+
        pow(vertices[i].y - vertices[(i+1)%3].y,2));
    }
    return dist;
}