#pragma once
#include <stdexcept>
#include <cassert>
#include <initializer_list>
#include "array_ptr.h"

class ReserveProxyObj {
public:  
    ReserveProxyObj() noexcept = default;
    ReserveProxyObj(size_t capacity) : capacity_(capacity)
    {
    }
    size_t GetCapacity() const noexcept {
        // Напишите тело самостоятельно
        return capacity_;
    }
private:
    size_t capacity_ = 0;
};

template <typename Type>
class SimpleVector {
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept = default;

    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size) : SimpleVector(size, Type{})  
    {
        // Напишите тело конструктора самостоятельно
    }

    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type& value) : items_(size), size_(size), capacity_(size)
    {
        // Напишите тело конструктора самостоятельно
        std::fill(items_.Get(), items_.Get() + size_, value);
    }

    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init) : items_(init.size()), size_(init.size()), capacity_(init.size()) {
        auto it = init.begin();
        for (size_t i = 0; i < init.size(); ++i) {
            items_[i] = *it++;
        }
    }
    
    SimpleVector(const SimpleVector& other) { // подсказал наставник
        SimpleVector<Type> copy(other.GetSize());
        std::copy((other.items_).Get(), ((other.items_).Get() + other.GetSize()), (copy.items_).Get());
        copy.capacity_ = other.capacity_;
        swap(copy);
    }

    SimpleVector(SimpleVector&& other) { // Перемещением
        swap(other); // на удивление всё так просто
    }
    
    SimpleVector& operator=(const SimpleVector& rhs) {
        if ( this != &rhs ) {
            SimpleVector copy(rhs);
            swap(copy);
       }
        return *this;
    }

    SimpleVector& operator=(SimpleVector&& rhs) { // Перемещением
        if ( this != &rhs ) {
            swap(rhs);
       }
        return *this;
    }
    
    SimpleVector(ReserveProxyObj arr)
    {
        Reserve(arr.GetCapacity());
    }

    // Возвращает количество элементов в массиве
    size_t GetSize() const noexcept {
        // Напишите тело самостоятельно
        return size_;
    }

    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept {
        // Напишите тело самостоятельно
        return capacity_;
    }

    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    // Возвращает ссылку на элемент с индексом index
    Type& operator[](size_t index) noexcept {
        // Напишите тело самостоятельно
        return items_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept {
        // Напишите тело самостоятельно
        return items_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index) {
        // Напишите тело самостоятельно
        if (index >= size_) {
            throw std::out_of_range("Out of range");
        }
        return items_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const {
        // Напишите тело самостоятельно
        if (index >= size_) {
            throw std::out_of_range("Out of range");
        }
        return items_[index];
    }

    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept {
        // Напишите тело самостоятельно
        size_ = 0;
    }

    // Изменяет размер массива.
    // При увеличении размера новые элементы получают значение по умолчанию для типа Type
    void Resize(size_t new_size) {//Без copy
        // Напишите тело самостоятельно
        if ( new_size > capacity_ ) {
            ArrayPtr<Type> new_items(new_size);
            for (auto it = new_items.Get(); it != new_items.Get() + new_size; ++it) {
            *it = std::move(Type{});
        }
            std::move(items_.Get(), items_.Get() + capacity_, new_items.Get());
            items_.swap(new_items);
            size_ = std::move(new_size);
            capacity_ = std::move(new_size);
        } else if ( new_size > size_ && new_size < capacity_ ) {
            for (auto it = items_.Get() + size_ ; it != items_.Get() + size_ + new_size; ++it) {
            *it = std::move(Type{});
        }
            size_ = std::move(new_size);
        } else {
            size_ = std::move(new_size);
        }
    }

    // Возвращает итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept {
        // Напишите тело самостоятельно
        return Iterator{items_.Get()};
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept {
        // Напишите тело самостоятельно
        return Iterator{items_.Get() + size_};
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept {
        // Напишите тело самостоятельно
        return Iterator{items_.Get()};
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept {
        // Напишите тело самостоятельно
        return Iterator{items_.Get() + size_};
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept {
        // Напишите тело самостоятельно
        return Iterator{items_.Get()};
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept {
        // Напишите тело самостоятельно
        return Iterator{items_.Get() + size_};
    }

    // Добавляет элемент в конец вектора
    // При нехватке места увеличивает вдвое вместимость вектора
    void PushBack(const Type& item) {
        if ( size_ < capacity_ ) {
            items_[size_ ] = item;
            size_++;
        } else {
            size_t new_capacity = 1;
            if ( capacity_ != 0 ) {
                new_capacity = capacity_ * 2;
            }
            ArrayPtr<Type> new_items(new_capacity);
            std::copy(items_.Get(), items_.Get() + size_, new_items.Get());
            new_items[size_] = item;
            items_.swap(new_items);
            capacity_ = new_capacity;
            size_++;
        }
    }
    
    void PushBack(Type&& item) { //Перемещением 
        if ( size_ < capacity_ ) {
            items_[size_] = std::move(item);
            size_++;
        } else {
            size_t new_capacity = 1;
            if ( capacity_ != 0 ) {
                new_capacity = capacity_ * 2;
            }
            ArrayPtr<Type> new_items(new_capacity);
            std::move(items_.Get(), items_.Get() + size_, new_items.Get());
            new_items[size_] = std::move(item);
            items_.swap(new_items);
            capacity_ = std::move(new_capacity);
            size_++;
        }
    }

    // Вставляет значение value в позицию pos.
    // Возвращает итератор на вставленное значение
    // Если перед вставкой значения вектор был заполнен полностью,
    // вместимость вектора должна увеличиться вдвое, а для вектора вместимостью 0 стать равной 1
    Iterator Insert(ConstIterator pos, const Type& value) {
        assert(pos >= items_.Get() && pos <= items_.Get() + size_);
        size_t insert_position = pos - items_.Get();
        if (capacity_ == 0) {
            ArrayPtr<Type> new_items(1);
            new_items[insert_position] = value;
            items_.swap(new_items);
            ++capacity_;
        } else if (size_ < capacity_) {
            std::copy_backward(items_.Get() + insert_position, items_.Get() + size_, items_.Get() + size_ + 1);
            items_[insert_position] = value;
        } else {
            size_t new_capacity = std::max(size_ + 1, capacity_ * 2);
            ArrayPtr<Type> new_items(capacity_);
            std::copy(items_.Get(), items_.Get() + size_, new_items.Get());
            std::copy_backward(items_.Get() + insert_position, items_.Get() + size_, new_items.Get() + size_ + 1);
            new_items[insert_position] = value;
            items_.swap(new_items);
            capacity_ = new_capacity;
        }
        ++size_;
        return Iterator(items_.Get() + insert_position);
    }
    
    Iterator Insert(Iterator pos, Type&& value) {//Перемещением 
        assert(pos >= items_.Get() && pos <= items_.Get() + size_);
        size_t insert_position = pos - items_.Get();
        if (capacity_ == 0) {
            ArrayPtr<Type> new_items(1);
            new_items[insert_position] = std::move(value);
            items_.swap(new_items);
            ++capacity_;
        } else if (size_ < capacity_) {
            std::move_backward(items_.Get() + insert_position, items_.Get() + size_, items_.Get() + size_ + 1);
            items_[insert_position] = std::move(value);
        } else {
            size_t new_capacity = std::max(size_ + 1, capacity_ * 2);
            ArrayPtr<Type> new_items(capacity_);
            std::move(items_.Get(), items_.Get() + size_, new_items.Get());
            std::move_backward(items_.Get() + insert_position, items_.Get() + size_, new_items.Get() + size_ + 1);
            new_items[insert_position] = std::move(value);
            items_.swap(new_items);
            capacity_ = std::move(new_capacity);
        }
        ++size_;
        return Iterator(items_.Get() + insert_position);
    }

    // "Удаляет" последний элемент вектора. Вектор не должен быть пустым
    void PopBack() noexcept {
        if (size_ != 0) {
            size_ -= 1;
        }
    }

    // Удаляет элемент вектора в указанной позиции
    Iterator Erase(ConstIterator pos) { // Без copy
        assert(pos >= items_.Get() && pos < items_.Get() + size_);
        size_t erase_position = pos - items_.Get(); // засовываем объекты, пропустив 1
        std::move(items_.Get(), items_.Get() + erase_position, items_.Get());
        std::move(items_.Get() + erase_position + 1, items_.Get() + size_, items_.Get() + erase_position);
        --size_;
        return Iterator(items_.Get() + erase_position);
    }

    // Обменивает значение с другим вектором
    void swap(SimpleVector& other) noexcept {
        items_.swap(other.items_);
        std::swap(other.size_, size_);
        std::swap(other.capacity_, capacity_);
    }
    void Reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            ArrayPtr<Type> new_items(new_capacity);
            std::fill(new_items.Get(), new_items.Get() + new_capacity, Type());
            std::copy(items_.Get(), items_.Get() + size_, new_items.Get());
            items_.swap(new_items);
            capacity_ = new_capacity;
        }
    }


private:
    ArrayPtr<Type> items_;
    size_t size_ = 0;
    size_t capacity_ = 0;    
};

ReserveProxyObj Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObj(capacity_to_reserve);
} 

template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return lhs.GetSize() == rhs.GetSize() || std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs > rhs);
}

template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return  rhs < lhs;
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs < rhs);
} 