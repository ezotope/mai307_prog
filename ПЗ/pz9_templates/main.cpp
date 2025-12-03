#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <cmath>

template <typename T>
class Array
{
private:
    int size_;
    T* data_;

public:
    explicit Array(int size) : size_(size), data_(nullptr)
    {
        if (size_ < 0)
        {
            throw std::invalid_argument("размер должен быть неотрицательным");
        }

        if (size_ == 0)
        {
            data_ = nullptr;
        }
        else
        {
            data_ = new T[size_];
        }
    }

    ~Array()
    {
        delete[] data_;
    }

    int size() const
    {
        return size_;
    }

    T& operator[](int index)
    {
        return data_[index];
    }

    const T& operator[](int index) const
    {
        return data_[index];
    }

    template <typename U = T>
    typename std::enable_if<std::is_arithmetic<U>::value, void>::type
    set(int index, U value)
    {
        if (index < 0 || index >= size_)
        {
            throw std::out_of_range("индекс вне диапазона");
        }

        data_[index] = value;
    }
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const Array<T>& array)
{
    for (int i = 0; i < array.size(); ++i)
    {
        os << array[i];
        if (i + 1 < array.size())
        {
            os << " ";
        }
    }
    return os;
}

template <typename T>
double distance(const Array<T>& a, const Array<T>& b)
{
    if (a.size() != b.size())
    {
        throw std::invalid_argument("размеры массивов должны совпадать");
    }

    if (!std::is_arithmetic<T>::value)
    {
        throw std::bad_typeid();
    }

    double sum = 0.0;
    for (int i = 0; i < a.size(); ++i)
    {
        double diff = static_cast<double>(a[i]) - static_cast<double>(b[i]);
        sum += diff * diff;
    }

    return std::sqrt(sum);
}

int main()
{
    Array<int> a(3);
    Array<int> b(3);

    a.set(0, 1);
    a.set(1, 2);
    a.set(2, 3);

    b.set(0, 4);
    b.set(1, 5);
    b.set(2, 6);

    std::cout << a << std::endl;
    std::cout << b << std::endl;

    double d = distance(a, b);
    std::cout << d << std::endl;

    return 0;
}