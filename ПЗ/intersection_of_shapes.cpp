#include <iostream>
#include <cmath>
using namespace std;

struct Point{
    double x;
    double y;
};

struct Circle{
    Point center;
    double radius;
};

struct Square{
    Point LeftCorner;
    double Side;
};

Point ReadPoint(){
    Point z;
    cin >> z.x >> z.y;
    return z;
}

Circle ReadCircle(){
    Circle z;
    cin >> z.center.x >> z.center.y >> z.radius;
    return z;
}

Square ReadSquare(){
    Square z;
    cin >> z.LeftCorner.x >> z.LeftCorner.y >> z.Side;
    return z;
}

string InterCircles (Circle a, Circle b){
    double SumRadiuses = a.radius + b.radius;
    double MinRadiuses = abs(a.radius - b.radius);
    double distance = sqrt(pow(a.center.x - b.center.x, 2) + pow(a.center.y - b.center.y, 2));
    if (distance <= SumRadiuses && distance >= MinRadiuses) {
        return "YES";
    } else {
        return "NO";
    }
}

string InterSquares(Square a, Square b) {
    double a_right = a.LeftCorner.x + a.Side;
    double a_top = a.LeftCorner.y + a.Side;
    double b_right = b.LeftCorner.x + b.Side;
    double b_top = b.LeftCorner.y + b.Side;
    
    bool x_overlap = (a.LeftCorner.x < b_right) && (a_right > b.LeftCorner.x);
    
    bool y_overlap = (a.LeftCorner.y < b_top) && (a_top > b.LeftCorner.y);
    
    if (x_overlap && y_overlap){
        return "YES";
    } else {
        return "NO";
    }
}

string InterSqrCirc(Square a, Circle b){

    double left = a.LeftCorner.x;
    double right = a.LeftCorner.x + a.Side;
    double top = a.LeftCorner.y;
    double bottom = a.LeftCorner.y - a.Side;

    double closestX = max(left, min(b.center.x, right));
    double closestY = max(top, min(b.center.y, bottom));

    double dx = b.center.x - closestX;
    double dy = b.center.y - closestY;
    double distance_squared = dx * dx + dy * dy;

    if (distance_squared <= (b.radius * b.radius)){
        return "YES";
    } else {
        return "NO";
    }
}