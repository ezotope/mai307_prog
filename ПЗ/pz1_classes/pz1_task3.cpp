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
};

int main() {
    DynamicArray arr(3);
    
    cout << "Исходный массив:" << endl;
    cout << "Размер: " << arr.getSize() << endl;
    arr.printArray();
    
    cout << "\nЗаполняем массив:" << endl;
    arr.setValue(0, 10);
    arr.setValue(1, -50);
    arr.setValue(2, 75);
    arr.printArray();
    
    cout << "\nДобавляем корректные значения:" << endl;
    arr.pushBack(100);
    cout << "После добавления 100:" << endl;
    cout << "Размер: " << arr.getSize() << endl;
    arr.printArray();
    
    arr.pushBack(-100);
    cout << "После добавления -100:" << endl;
    cout << "Размер: " << arr.getSize() << endl;
    arr.printArray();
    
    arr.pushBack(0);
    cout << "После добавления 0:" << endl;
    cout << "Размер: " << arr.getSize() << endl;
    arr.printArray();
    
    cout << "\nТестируем некорректные значения:" << endl;
    arr.pushBack(150);
    arr.pushBack(-150);
    
    cout << "Финальный массив:" << endl;
    cout << "Размер: " << arr.getSize() << endl;
    arr.printArray();
    
    cout << "\nТестируем конструктор копирования:" << endl;
    DynamicArray arrCopy(arr);
    cout << "Оригинальный массив: ";
    arr.printArray();
    cout << "Скопированный массив: ";
    arrCopy.printArray();
    
    arr.pushBack(42);
    cout << "После добавления 42 в оригинал:" << endl;
    cout << "Оригинальный массив: ";
    arr.printArray();
    cout << "Скопированный массив: ";
    arrCopy.printArray();
    
    return 0;
}