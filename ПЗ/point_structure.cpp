#include <iostream>
using namespace std;

struct Point {
    double x;
    double y;
};

Point readPoint() {
    Point p;
    cin >> p.x >> p.y;
    return p;
}

void printPoint(Point p) {
    cout << p.x << " " << p.y;
}

int main() {
    Point a = readPoint();
    printPoint(a);
    
    system("pause");
    
    return 0;
}