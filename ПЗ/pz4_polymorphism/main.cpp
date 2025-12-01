#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
using namespace std;

class DynamicArray {
protected:
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

    virtual ~DynamicArray() {
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

    virtual void exportToFile() = 0;

protected:
    string getCurrentDateTimeFilename(const string& extension) {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        
        ostringstream oss;
        oss << setfill('0') 
            << setw(4) << (1900 + ltm->tm_year)
            << setw(2) << (1 + ltm->tm_mon)
            << setw(2) << ltm->tm_mday
            << "_"
            << setw(2) << ltm->tm_hour
            << setw(2) << ltm->tm_min
            << setw(2) << ltm->tm_sec
            << "." << extension;
        
        return oss.str();
    }
};

class ArrTxt : public DynamicArray {
public:
    ArrTxt(int arraySize) : DynamicArray(arraySize) {}

    void exportToFile() override {
        string filename = getCurrentDateTimeFilename("txt");
        ofstream file(filename);
        
        if (!file.is_open()) {
            cout << "Ошибка: не удалось открыть файл " << filename << endl;
            return;
        }
        
        for (int i = 0; i < size; i++) {
            file << data[i];
            if (i < size - 1) {
                file << " ";
            }
        }
        file << endl;
        
        file.close();
        cout << "Данные экспортированы в файл: " << filename << endl;
    }
};

class ArrCSV : public DynamicArray {
public:
    ArrCSV(int arraySize) : DynamicArray(arraySize) {}

    void exportToFile() override {
        string filename = getCurrentDateTimeFilename("csv");
        ofstream file(filename);
        
        if (!file.is_open()) {
            cout << "Ошибка: не удалось открыть файл " << filename << endl;
            return;
        }
        
        for (int i = 0; i < size; i++) {
            file << data[i];
            if (i < size - 1) {
                file << ",";
            }
        }
        file << endl;
        
        file.close();
        cout << "Данные экспортированы в файл: " << filename << endl;
    }
};

void exportArray(DynamicArray* arr) {
    arr->exportToFile();
}

int main() {
    ArrTxt* txtArray = new ArrTxt(5);
    ArrCSV* csvArray = new ArrCSV(5);
    
    cout << "Заполняем массив ArrTxt: ";
    txtArray->setValue(0, 10);
    txtArray->setValue(1, -50);
    txtArray->setValue(2, 100);
    txtArray->setValue(3, 0);
    txtArray->setValue(4, -100);
    txtArray->printArray();
    
    cout << "Заполняем массив ArrCSV:" << endl;
    csvArray->setValue(0, 20);
    csvArray->setValue(1, -30);
    csvArray->setValue(2, 75);
    csvArray->setValue(3, -25);
    csvArray->setValue(4, 50);
    csvArray->printArray();
    
    cout << "Экспорт через полиморфизм:" << endl;
    DynamicArray* arrays[2];
    arrays[0] = txtArray;
    arrays[1] = csvArray;
    
    for (int i = 0; i < 2; i++) {
        exportArray(arrays[i]);
    }
    
    // Или напрямую
    cout << "Прямой вызов методов:" << endl;
    txtArray->exportToFile();
    csvArray->exportToFile();
    
    delete txtArray;
    delete csvArray;
    
    return 0;
}

