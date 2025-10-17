#include <iostream>
using namespace std;
#include <cmath>

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

Circle ReadCircle(){
    Circle c;
    cin >> c.center.x >> c.center.y >> c.radius;
    return c;
}

Square ReadSquare(){
    Square s;
    cin >> s.LeftCorner.x >> s.LeftCorner.y >> s.Side;
    return s;
}

Point ReadPoint(){
    Point p;
    cin >> p.x >> p.y;  
    return p;
}

string PointInEdgeCircle(Point p, Circle c){
    double distance = sqrt(pow(p.x - c.center.x, 2) + pow(p.y - c.center.y, 2));
    if (distance == c.radius){
        return "YES";
    }
    else{
        return "NO";
    }
}

string PointInEdgeSquare(Point p, Square s){
    if (p.x == s.LeftCorner.x || p.x == s.LeftCorner.x + s.Side || p.y == s.LeftCorner.y || p.y == s.LeftCorner.y + s.Side){
        return "YES";
    }
    else{
        return "NO";
    }
}
    

int main(){
    Point p = ReadPoint();
    Circle c = ReadCircle();
    Square s = ReadSquare();
    cout << PointInEdgeCircle(p, c) << endl;
    cout << PointInEdgeSquare(p, s) << endl;
    return 0;
}