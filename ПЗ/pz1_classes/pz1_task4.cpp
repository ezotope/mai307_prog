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

    DynamicArray(const DynamicArray& other) {
        size = other.size;
        data = new int[size];
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
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

    bool pushBack(int value) {
        if (value < -100 || value > 100) {
            cout << "Ошибка: значение " << value << " не принадлежит промежутку [-100, 100]" << endl;
            return false;
        }

        int newSize = size + 1;
        int* newData = new int[newSize];

        for (int i = 0; i < size; i++) {
            newData[i] = data[i];
        }

        newData[size] = value;

        delete[] data;
        data = newData;
        size = newSize;

        return true;
    }

    void add(const DynamicArray& other) {
        int maxSize = max(size, other.size);
        
        for (int i = 0; i < size; i++) {
            int otherValue = (i < other.size) ? other.data[i] : 0;
            data[i] += otherValue;
        }
    }

    void subtract(const DynamicArray& other) {
        int maxSize = max(size, other.size);
        
        for (int i = 0; i < size; i++) {
            int otherValue = (i < other.size) ? other.data[i] : 0;
            data[i] -= otherValue;
        }
    }
};

int main() {
    DynamicArray arrA(4);
    DynamicArray arrB(3);
    
    cout << "Исходные массивы:" << endl;
    cout << "Массив A (размер " << arrA.getSize() << "): ";
    arrA.printArray();
    cout << "Массив B (размер " << arrB.getSize() << "): ";
    arrB.printArray();
    
    cout << "\nЗаполняем массив A:" << endl;
    arrA.setValue(0, 10);
    arrA.setValue(1, 20);
    arrA.setValue(2, 30);
    arrA.setValue(3, 40);
    cout << "Массив A: ";
    arrA.printArray();
    
    cout << "\nЗаполняем массив B:" << endl;
    arrB.setValue(0, 5);
    arrB.setValue(1, 15);
    arrB.setValue(2, 25);
    cout << "Массив B: ";
    arrB.printArray();
    
    cout << "\nТестируем сложение A + B:" << endl;
    cout << "До сложения - A: ";
    arrA.printArray();
    arrA.add(arrB);
    cout << "После сложения - A: ";
    arrA.printArray();
    
    cout << "\nТестируем вычитание A - B:" << endl;
    cout << "До вычитания - A: ";
    arrA.printArray();
    arrA.subtract(arrB);
    cout << "После вычитания - A: ";
    arrA.printArray();
    
    cout << "\nТестируем с массивами разного размера:" << endl;
    DynamicArray arrC(2);
    DynamicArray arrD(5);
    
    arrC.setValue(0, 100);
    arrC.setValue(1, 200);
    
    arrD.setValue(0, 1);
    arrD.setValue(1, 2);
    arrD.setValue(2, 3);
    arrD.setValue(3, 4);
    arrD.setValue(4, 5);
    
    cout << "Массив C (размер " << arrC.getSize() << "): ";
    arrC.printArray();
    cout << "Массив D (размер " << arrD.getSize() << "): ";
    arrD.printArray();
    
    cout << "\nC + D (C меньше D):" << endl;
    cout << "До сложения - C: ";
    arrC.printArray();
    arrC.add(arrD);
    cout << "После сложения - C: ";
    arrC.printArray();
    
    cout << "\nD + C (D больше C):" << endl;
    cout << "До сложения - D: ";
    arrD.printArray();
    arrD.add(arrC);
    cout << "После сложения - D: ";
    arrD.printArray();
    
    return 0;
}
