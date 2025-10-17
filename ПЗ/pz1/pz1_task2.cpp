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
};

int main() {
    DynamicArray arr(5);
    
    cout << "Размер массива: " << arr.getSize() << endl;
    arr.printArray();
    
    // ТЕСТ с норм значениями
    cout << "\nУстанавливаем значения:" << endl;
    arr.setValue(0, 10);
    arr.setValue(1, -50);
    arr.setValue(2, 100);
    arr.setValue(3, 0);
    arr.setValue(4, -100);
    arr.printArray();
    
    // ТЕСТ с плохими значениями
    cout << "\nТестируем некорректные значения:" << endl;
    arr.setValue(0, 150);  // Превышение диапазона
    arr.setValue(5, 10);   // Выход за границы
    arr.setValue(-1, 10);  // Отрицательный индекс
    
    cout << "\nТестируем геттер:" << endl;
    int value;
    if (arr.getValue(2, value)) {
        cout << "Значение по индексу 2: " << value << endl;
    }
    
    if (arr.getValue(10, value)) {
        cout << "Значение по индексу 10: " << value << endl;
    }
    
    // ТЕСТ конструктора копирования
    cout << "\nТестируем конструктор копирования:" << endl;
    DynamicArray arrCopy(arr);
    cout << "Оригинальный массив: ";
    arr.printArray();
    cout << "Скопированный массив: ";
    arrCopy.printArray();
    
    // Изменяем оригинальный массив
    arr.setValue(0, 99);
    cout << "После изменения оригинала:" << endl;
    cout << "Оригинальный массив: ";
    arr.printArray();
    cout << "Скопированный массив: ";
    arrCopy.printArray();
    
    return 0;
}
