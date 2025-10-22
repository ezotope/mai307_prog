#include <iostream>
using namespace std;

struct Point{
    double x;
    double y;
};

struct Circle{
    Point center;
    double radius;
};

Circle readCircle(){
    Circle c;
    cin >> c.center.x >> c.center.y >> c radius;
    return c;
}

void printCircle(Circle c){
    cout << c.center.x << " " << c.center.y << " " << c.radius;
}

double circleLength(Circle c){
    cout << 2 * 3.14159 * c.radius;
}

double circleArea(Circle c){
    cout << 3.14159 * c.radius * c.radius;
}

int main(){
    circle c = readCircle();
    printCircle(c);
    circleLength(c);
    circleArea(c);
    return 0;
}