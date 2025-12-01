#include <iostream>
#include <stdexcept>
#include <string>
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

    void setValue(int index, int value) {
        if (index < 0 || index >= size) {
            throw out_of_range("Индекс " + to_string(index) + " выходит за границы массива (0-" + to_string(size-1) + ")"); // Исключение: индекс выходит за границы массива
        }
        
        if (value < -100 || value > 100) {
            throw invalid_argument("Значение " + to_string(value) + " не принадлежит промежутку [-100, 100]"); // Исключение: значение не принадлежит промежутку
        }
        
        data[index] = value;
    }

    int getValue(int index) {
        if (index < 0 || index >= size) {
            throw out_of_range("Индекс " + to_string(index) + " выходит за границы массива (0-" + to_string(size-1) + ")"); // Исключение: индекс выходит за границы массива
        }
        
        return data[index];
    }

    int getSize() {
        return size;
    }

    void pushBack(int value) {
        if (value < -100 || value > 100) {
            throw invalid_argument("Значение " + to_string(value) + " не принадлежит промежутку [-100, 100]"); // Исключение: значение не принадлежит промежутку
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
    }

    void add(const DynamicArray& other) {
        for (int i = 0; i < size; i++) {
            int otherValue = (i < other.size) ? other.data[i] : 0;
            data[i] += otherValue;
        }
    }

    void subtract(const DynamicArray& other) {
        for (int i = 0; i < size; i++) {
            int otherValue = (i < other.size) ? other.data[i] : 0;
            data[i] -= otherValue;
        }
    }
};

int main() {
    DynamicArray arr(5);
    
    cout << "Исходный массив:" << endl;
    cout << "Размер: " << arr.getSize() << endl;
    arr.printArray();
    
    cout << "Корректные операции:" << endl;
    try {
        arr.setValue(0, 10);
        arr.setValue(1, -50);
        arr.setValue(2, 100);
        arr.setValue(3, 0);
        arr.setValue(4, -100);
        arr.printArray();
        
        int value = arr.getValue(2);   // Значение по индексу 2
        cout << "Значение по индексу 2: " << value << endl;
    }
    catch (const exception& e) {
        cout << "Ошибка: " << e.what() << endl;
    }
    
    cout << "\n=== Тест: out_of_range при setValue ===" << endl;
    try {
        arr.setValue(10, 50);  // Несуществующий индекс
    }
    catch (const out_of_range& e) {
        cout << "Поймано исключение out_of_range: " << e.what() << endl;
    }
    catch (const exception& e) {
        cout << "Поймано исключение: " << e.what() << endl;
    }
    
    cout << "\n=== Тест: out_of_range при getValue ===" << endl;
    try {
        int value = arr.getValue(-1);  // Несуществующий индекс
        cout << "Значение: " << value << endl;
    }
    catch (const out_of_range& e) {
        cout << "Поймано исключение out_of_range: " << e.what() << endl;
    }
    catch (const exception& e) {
        cout << "Поймано исключение: " << e.what() << endl;
    }
    
    cout << "\n=== Тест: invalid_argument при setValue (значение > 100) ===" << endl;
    try {
        arr.setValue(0, 150);  // Значение > 100
    }
    catch (const invalid_argument& e) {
        cout << "Поймано исключение invalid_argument: " << e.what() << endl;
    }
    catch (const exception& e) {
        cout << "Поймано исключение: " << e.what() << endl;
    }
    

    cout << "\n=== Тест: invalid_argument при setValue (значение < -100) ===" << endl;
    try {
        arr.setValue(0, -150);
    }
    catch (const invalid_argument& e) {
        cout << "Поймано исключение invalid_argument: " << e.what() << endl;
    }
    catch (const exception& e) {
        cout << "Поймано исключение: " << e.what() << endl;
    }
    
    cout << "\n=== Тест: invalid_argument при pushBack ===" << endl;
    try {
        arr.pushBack(200);
    }
    catch (const invalid_argument& e) {
        cout << "Поймано исключение invalid_argument: " << e.what() << endl;
    }
    catch (const exception& e) {
        cout << "Поймано исключение: " << e.what() << endl;
    }
    
    cout << "\n=== Тест: корректный pushBack ===" << endl;
    try {
        arr.pushBack(42);
        cout << "После добавления элемента:" << endl;
        arr.printArray();
    }
    catch (const exception& e) {
        cout << "Ошибка: " << e.what() << endl;
    }
    
    cout << "\n=== Тест: комбинация исключений ===" << endl;
    try {
        arr.setValue(100, 50);  // Сначала out_of_range
    }
    catch (const out_of_range& e) {
        cout << "Поймано out_of_range: " << e.what() << endl;
    }
    catch (const invalid_argument& e) {
        cout << "Поймано invalid_argument: " << e.what() << endl;
    }
    catch (const exception& e) {
        cout << "Поймано исключение: " << e.what() << endl;
    }
    
    return 0;
}

