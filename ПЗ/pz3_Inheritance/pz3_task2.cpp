#include <iostream>
using namespace std;

class DynamicArray {
private:
    int* data;
    int size;

public:
    DynamicArray(int arraySize) {
        size = arraySize;
        data = new int[size];
        for (int i = 0; i < size; i++) {
            data[i] = 0;
        }
    }

    ~DynamicArray() {
        delete[] data;
    }

    void printArray() {
        cout << "Arr: ";
        for (int i = 0; i < size; i++) {
            cout << data[i] << " ";
        }
        cout << endl;
    }

    bool setValue(int index, int value) {
        if (index < 0 || index >= size) {
            cout << "Ошибка: индекс " << index << " выходит за границы массива (0-" << (size-1) << ")" << endl;
            return false;
        }
        
        if (value < -100 || value > 100) {
            cout << "Ошибка: значение " << value << " не принадлежит промежутку [-100, 100]" << endl;
            return false;
        }
        
        data[index] = value;
        return true;
    }

    bool getValue(int index, int& value) {
        if (index < 0 || index >= size) {
            cout << "Ошибка: индекс " << index << " выходит за границы массива (0-" << (size-1) << ")" << endl;
            return false;
        }
        
        value = data[index];
        return true;
    }

    int getSize() {
        return size;
    }
};

class ExtendedArray : public DynamicArray {
public:
    ExtendedArray(int arraySize) : DynamicArray(arraySize) {}
    
    double getAverage() {
        int sum = 0;
        for (int i = 0; i < getSize(); i++) {
            int value;
            getValue(i, value);
            sum += value;
        }
        return (double)sum / getSize();
    }
    
    double getMedian() {
        int* temp = new int[getSize()];
        for (int i = 0; i < getSize(); i++) {
            getValue(i, temp[i]);
        }
        
        for (int i = 0; i < getSize() - 1; i++) {
            for (int j = 0; j < getSize() - i - 1; j++) {
                if (temp[j] > temp[j + 1]) {
                    int swap = temp[j];
                    temp[j] = temp[j + 1];
                    temp[j + 1] = swap;
                }
            }
        }
        
        double median;
        if (getSize() % 2 == 1) {
            median = temp[getSize() / 2];
        } else {
            median = (temp[getSize() / 2 - 1] + temp[getSize() / 2]) / 2.0;
        }
        
        delete[] temp;
        return median;
    }
};

int main() {
    ExtendedArray arr(5);
    
    cout << "Размер массива: " << arr.getSize() << endl;
    arr.printArray();
    
    arr.setValue(0, 10);
    arr.setValue(1, 20);
    arr.setValue(2, 30);
    arr.setValue(3, 40);
    arr.setValue(4, 50);
    
    arr.printArray();
    
    cout << "Ср. знач: " << arr.getAverage() << endl;
    cout << "Медиана: " << arr.getMedian() << endl;
    
    return 0;
}