#pragma once
#include <cstddef>
#include <StackClass.h>
using namespace std;

template <class T>
class TMultiStack
{
protected:
  T *memory;
  size_t stacksCount;
  size_t totalCapacity;
  size_t *tops;
  size_t *capacities;
public:
  TMultiStack();
  TMultiStack(size_t stacksCount_, size_t totalCapacity_);
  TMultiStack(const TMultiStack& other);
  TMultiStack(TMultiStack&& other);
  ~TMultiStack();

  size_t GetStacksCount() const;
  size_t GetTotalCapacity() const;
  T* GetMemory() const;
  size_t* GetTops() const;
  size_t* GetCapacities() const;

  void SetStacksCount(size_t stacksCount_);
  void SetTotalCapacity(size_t totalCapacity_);
  void SetMemory(T* memory_);
  void SetTops(size_t* tops_);
  void SetCapacities(size_t* capacities_);

  void push(size_t stackIndex, const T& element);
  T pop(size_t stackIndex);
  bool IsEmpty(size_t stackIndex) const;
  bool IsFull(size_t stackIndex) const;
  size_t Size(size_t stackIndex) const;

  void Repack(); // Если переполнен
  bool operator==(const TMultiStack<T>& other) const;
  bool operator!=(const TMultiStack<T>& other) const;


  class TIterator
  {
  protected:
    TMultiStack<T>& p;
    size_t stack;
    size_t cur;
    size_t prev;
  public:
    TIterator(TMultiStack<T>& multistack, size_t stackIndex, size_t start, size_t cnt);
    T& operator*();
    TIterator& operator++();
    TIterator operator++(int);

    bool operator==(const TIterator& other) const;
    bool operator!=(const TIterator& other) const;
  };

  TIterator begin(size_t stackIndex);
  TIterator end(size_t stackIndex);
};



template <class T>
TMultiStack<T>::TMultiStack() : stacksCount(0), totalCapacity(0),
memory(nullptr), tops(nullptr), capacities(nullptr) {}

template <class T>
TMultiStack<T>::TMultiStack(size_t stacksCount_, size_t totalCapacity_) : stacksCount(stacksCount_), totalCapacity(totalCapacity_)
{
  memory = new T[totalCapacity];
  tops = new size_t[stacksCount];
  capacities = new size_t[stacksCount];
  size_t baseCapacity = totalCapacity / stacksCount;

  for (size_t i = 0; i < stacksCount; ++i)
  {
    tops[i] = 0;
    capacities[i] = baseCapacity;
  }

  capacities[stacksCount - 1] = totalCapacity - baseCapacity * (stacksCount - 1);
}

template <class T>
TMultiStack<T>::TMultiStack(const TMultiStack& other) : stacksCount(other.stacksCount), totalCapacity(other.totalCapacity)
{
  memory = new T[totalCapacity];
  tops = new size_t[stacksCount];
  capacities = new size_t[stacksCount];

  for (size_t i = 0; i < totalCapacity; ++i)
    memory[i] = other.memory[i];

  for (size_t i = 0; i < stacksCount; ++i)
  {
    tops[i] = other.tops[i];
    capacities[i] = other.capacities[i];
  }
}

template <class T>
TMultiStack<T>::TMultiStack(TMultiStack&& other) : stacksCount(other.stacksCount), totalCapacity(other.totalCapacity),
                                                   memory(other.memory), tops(other.tops), capacities(other.capacities)
{
  other.memory = nullptr;
  other.tops = nullptr;
  other.capacities = nullptr;
  other.stacksCount = 0;
  other.totalCapacity = 0;
}

template <class T>
TMultiStack<T>::~TMultiStack()
{
  delete[] memory;
  delete[] tops;
  delete[] capacities;
}

template <class T>
size_t TMultiStack<T>::GetStacksCount() const
{
  return stacksCount;
}

template <class T>
size_t TMultiStack<T>::GetTotalCapacity() const
{
  return totalCapacity;
}

template <class T>
T* TMultiStack<T>::GetMemory() const
{
  return memory;
}

template <class T>
size_t* TMultiStack<T>::GetTops() const
{
  return tops;
}

template <class T>
size_t* TMultiStack<T>::GetCapacities() const
{
  return capacities;
}

template <class T>
void TMultiStack<T>::SetStacksCount(size_t stacksCount_)
{
  size_t* newTops = new size_t[stacksCount_];
  size_t* newCaps = new size_t[stacksCount_];

  size_t copyLimit = (stacksCount < stacksCount_) ? stacksCount : stacksCount_;
  for (size_t i = 0; i < copyLimit; ++i)
  {
    newTops[i] = tops[i];
    newCaps[i] = capacities[i];
  }
  if (stacksCount_ > stacksCount)
  {
    for (size_t i = stacksCount; i < stacksCount_; ++i)
    {
      newTops[i] = 0;
      newCaps[i] = 0;
    }
  }
  delete[] tops;
  delete[] capacities;
  tops = newTops;
  capacities = newCaps;
  stacksCount = stacksCount_;
}

template <class T>
void TMultiStack<T>::SetTotalCapacity(size_t totalCapacity_)
{
  T* newMem = new T[totalCapacity_];

  if (totalCapacity_ < totalCapacity)
  {
    for (size_t i = 0; i < totalCapacity_; ++i)
      newMem[i] = memory[i];
  }
  else
  {
    for (size_t i = 0; i < totalCapacity; ++i)
      newMem[i] = memory[i];
  }

  delete[] memory;
  memory = newMem;
  totalCapacity = totalCapacity_;
}

template <class T>
void TMultiStack<T>::SetMemory(T* memory_)
{
  delete[] memory;
  memory = memory_;
}

template <class T>
void TMultiStack<T>::SetTops(size_t* tops_)
{
  delete[] tops;
  tops = tops_;
}

template <class T>
void TMultiStack<T>::SetCapacities(size_t* capacities_)
{
  delete[] capacities;
  capacities = capacities_;
}

template <class T>
void TMultiStack<T>::push(size_t stackIndex, const T& element)
{
  if (stackIndex >= stacksCount)
    throw("Invalid stack index");

  if (IsFull(stackIndex)) Repack();

  size_t start = 0;
  for (size_t i = 0; i < stackIndex; ++i)
    start += capacities[i];

  memory[start + tops[stackIndex]] = element;
  tops[stackIndex]++;
}

template <class T>
T TMultiStack<T>::pop(size_t stackIndex)
{
  if (stackIndex >= stacksCount)
    throw("Invalid stack index");
  if (IsEmpty(stackIndex))
    throw("Empty stack");

  size_t start = 0;
  for (size_t i = 0; i < stackIndex; ++i)
    start += capacities[i];

  tops[stackIndex]--;
  return memory[start + tops[stackIndex]];
}

template <class T>
inline bool TMultiStack<T>::IsEmpty(size_t stackIndex) const
{
  if (stackIndex >= stacksCount)
    throw("Invalid stack index");
  return tops[stackIndex] == 0;
}
template <class T>
inline bool TMultiStack<T>::IsFull(size_t stackIndex) const
{
  if (stackIndex >= stacksCount)
    throw("Invalid stack index");
  return tops[stackIndex] >= capacities[stackIndex];
}

template <class T>
size_t TMultiStack<T>::Size(size_t stackIndex) const
{
  return tops[stackIndex];
}

template <class T>
void TMultiStack<T>::Repack()
{
  size_t newTotalCapacity = totalCapacity * 2;
  T* newMemory = new T[newTotalCapacity];
  size_t newBaseCapacity = newTotalCapacity / stacksCount;

  size_t currentPos = 0;
  for (size_t i = 0; i < stacksCount; ++i)
  {
    size_t newCap;
    if (i == stacksCount - 1) newCap = newTotalCapacity - currentPos;
    else newCap = newBaseCapacity;
    for (size_t j = 0; j < tops[i]; ++j)
      newMemory[currentPos + j] = memory[capacities[i] * i + j];
    capacities[i] = newCap;
    currentPos += newCap;
  }

  delete[] memory;
  memory = newMemory;
  totalCapacity = newTotalCapacity;
}
template <class T>
bool TMultiStack<T>::operator==(const TMultiStack<T>& other) const
{
  if (stacksCount != other.stacksCount)
    return false;

  for (size_t i = 0; i < stacksCount; ++i)
  {
    if (tops[i] != other.tops[i] || capacities[i] != other.capacities[i])
      return false;
  }

  for (size_t i = 0; i < stacksCount; ++i)
  {
    size_t start = 0;
    size_t otherStart = 0;
    for (size_t j = 0; j < i; ++j)
    {
      start += capacities[j];
      otherStart += other.capacities[j];
    }
    for (size_t j = 0; j < tops[i]; ++j)
    {
      if (memory[start + j] != other.memory[otherStart + j])
        return false;
    }
  }
  return true;
}

template <class T>
bool TMultiStack<T>::operator!=(const TMultiStack<T>& other) const
{
  return !(*this == other);
}

template <class T>
TMultiStack<T>::TIterator::TIterator(TMultiStack<T>& multistack, size_t stackIndex, size_t start, size_t cnt)
     : p(multistack), stack(stackIndex), cur(start), prev(cnt) {}

template <class T>
T& TMultiStack<T>::TIterator::operator*()
{
  size_t start = 0;
  for (size_t i = 0; i < stack; ++i)
    start += p.capacities[i];
  return p.memory[start + cur];
}

template <class T>
typename TMultiStack<T>::TIterator& TMultiStack<T>::TIterator::operator++()
{
  cur++;
  prev++;
  return *this;
}

template <class T>
typename TMultiStack<T>::TIterator TMultiStack<T>::TIterator::operator++(int)
{
  TIterator temp = *this;
  ++(*this);
  return temp;
}

template <class T>
bool TMultiStack<T>::TIterator::operator==(const TIterator& other) const
{
  return &p == &other.p && stack == other.stack && prev == other.prev;
}

template <class T>
bool TMultiStack<T>::TIterator::operator!=(const TIterator& other) const
{
  return !(*this == other);
}

template <class T>
typename TMultiStack<T>::TIterator TMultiStack<T>::begin(size_t stackIndex)
{
  return TIterator(*this, stackIndex, 0, 0);
}

template <class T>
typename TMultiStack<T>::TIterator TMultiStack<T>::end(size_t stackIndex)
{
  return TIterator(*this, stackIndex, tops[stackIndex], tops[stackIndex]);
}

