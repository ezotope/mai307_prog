#include <iostream>
using namespace std;

struct Point{
    double x;
    double y;
};

struct Square{
    Point LeftCorner;
    double Side;
};

Square readSquare(){
    Square s;
    cin >> s.LeftCorner.x >> s.LeftCorner.y >> s.Side;
}

void Perimeter(Square s){
    cout << 4 * s.Side;
}

void Area(Square s){
    cout << s.Side * s.Side;
}

void printSquare(Square s){
    cout << s.LeftCorner.x << " " << s.LeftCorner.y << " " << s.Side;
}

int main(){
    Square s = readSquare();
    printSquare(s);
    Perimeter(s);
    Area(s);
    return 0;
}