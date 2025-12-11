#pragma once
#include <cstddef>

using namespace std;

template <class T>
class TStack
{
protected:
  size_t capacity;
  size_t top;
  T* memory;
public:
  TStack();
  TStack(size_t capacity_);
  TStack(const TStack& other);
  TStack(TStack&& other);
  ~TStack();

  size_t GetCapacity() const;
  size_t GetTop() const;
  T* GetMemory() const;


  void SetCapacity(size_t capacity_);
  void SetTop(size_t top_);
  void SetMemory(T* memory_);

  // Размер стека
  size_t Size() const;

  bool operator==(const TStack<T>& other) const;
  bool operator!=(const TStack<T>& other) const;
  T operator[](size_t index) const;

  void push(const T& element); // Добавление элемента
  T pop(); // Удаление и возврат верхнего элемента
  bool IsEmpty() const;
  bool IsFull() const;

  class TIterator
  {
  protected:
    TStack<T>& p;
    size_t current;
    size_t passed;
  public:
    TIterator(TStack<T> &stack, size_t start_pos, size_t passed_count);
    T& operator*();
    TIterator& operator++();
    TIterator operator++(int);
    bool operator==(const TIterator& other) const;
    bool operator!=(const TIterator& other) const;
  };

  TIterator begin();
  TIterator end();
};

template <class T>
inline TStack<T>::TStack() : capacity(10), top(0), memory(new T[capacity]) {}

template <class T>
inline TStack<T>::TStack(size_t capacity_) : capacity(capacity_), top(0), memory(new T[capacity]) {}

template <class T>
inline TStack<T>::TStack(const TStack& other) : capacity(other.capacity), top(other.top), memory(new T[capacity])
{
  for (size_t i = 0; i < top; ++i)
    memory[i] = other.memory[i];
}

template <class T>
inline TStack<T>::TStack(TStack&& other) : capacity(other.capacity), top(other.top), memory(other.memory)
{
  other.memory = nullptr;
  other.capacity = 0;
  other.top = 0;
}

template <class T>
inline TStack<T>::~TStack()
{
  delete[] memory;
}

// геттеры и сеттеры
template <class T>
inline size_t TStack<T>::GetCapacity() const
{
  return capacity;
}

template <class T>
inline size_t TStack<T>::GetTop() const
{
  return top;
}

template <class T>
inline T* TStack<T>::GetMemory() const
{
  return memory;
}

template <class T>
inline void TStack<T>::SetCapacity(size_t capacity_)
{
  if (capacity_ < top)
    throw "New capacity cannot be smaller";
  T* newMemory = new T[capacity_];
  for (size_t i = 0; i < top; ++i)
    newMemory[i] = memory[i];

  delete[] memory;
  memory = newMemory;
  capacity = capacity_;
}

template <class T>
inline void TStack<T>::SetTop(size_t top_)
{
  if (top_ > capacity)
    throw "Top index exceeds capacity";
  top = top_;
}

template <class T>
inline void TStack<T>::SetMemory(T* memory_)
{
  delete[] memory;
  memory = memory_;
}

template <class T>
inline size_t TStack<T>::Size() const
{
  return top;
}

// операторы

template <class T>
inline bool TStack<T>::operator==(const TStack<T>& other) const
{
  if (top != other.top)
    return false;

  for (size_t i = 0; i < top; ++i)
  {
    if (memory[i] != other.memory[i])
      return false;
  }
  return true;
}

template <class T>
inline bool TStack<T>::operator!=(const TStack<T>& other) const
{
  return !(*this == other);
}

template <class T>
inline T TStack<T>::operator[](size_t index) const
{
  if (index >= top)
    throw "Index out of range";
  return memory[index];
}

// паша поп

template <class T>
inline void TStack<T>::push(const T& element)
{
  if (IsFull()) {
    if (capacity == 0) {
      capacity = 10;
      memory = new T[capacity];
    } else {
      size_t new_capacity = capacity * 2;
      T* new_memory = new T[new_capacity];

      for (size_t i = 0; i < top; ++i) {
        new_memory[i] = memory[i];
      }

      delete[] memory;
      memory = new_memory;
      capacity = new_capacity;
    }
  }
  memory[top] = element;
  top++;
}

template <class T>
inline T TStack<T>::pop()
{
  if (IsEmpty())
    throw "Stack is empty";

  return memory[--top];
}

template <class T>
inline bool TStack<T>::IsEmpty() const
{
  return top == 0;
}

template <class T>
inline bool TStack<T>::IsFull() const
{
  return top == capacity;
}

// итератор
template <class T>
inline TStack<T>::TIterator::TIterator(TStack<T> &stack, size_t start_pos, size_t passed_count)
     : p(stack), current(start_pos), passed(passed_count) {}

template <class T>
inline T& TStack<T>::TIterator::operator*()
{
  return p.memory[current];
}

template <class T>
inline typename TStack<T>::TIterator& TStack<T>::TIterator::operator++()
{
  current++;
  passed++;
  return *this;
}

template <class T>
inline typename TStack<T>::TIterator TStack<T>::TIterator::operator++(int)
{
  TIterator temp = *this;
  ++(*this);
  return temp;
}

template <class T>
inline bool TStack<T>::TIterator::operator==(const TIterator& other) const
{
  return &p == &other.p && passed == other.passed;
}

template <class T>
inline bool TStack<T>::TIterator::operator!=(const TIterator& other) const
{
  return !(*this == other);
}

template <class T>
inline typename TStack<T>::TIterator TStack<T>::begin()
{
  return TIterator(*this, 0, 0);
}

template <class T>
inline typename TStack<T>::TIterator TStack<T>::end()
{
  return TIterator(*this, top, top);
}