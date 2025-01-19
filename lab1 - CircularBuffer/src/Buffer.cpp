#include "CircularBuffer.h"
#include <iostream>
#include <cstring>
#include <algorithm>

CircularBuffer::CircularBuffer()
{
    m_buffer = nullptr;
    m_capacity = 0;
    m_firstElement = 0;
    m_lastElement = 0;
    m_currentSize = 0;
}

CircularBuffer::CircularBuffer(int capacity) : CircularBuffer()
{
    m_buffer = new value_type[capacity];
    m_capacity = capacity;
}

CircularBuffer::CircularBuffer(const CircularBuffer& other) : CircularBuffer(other.m_capacity)
{
    std::memcpy(m_buffer, other.m_buffer, m_capacity);
    m_firstElement = other.m_firstElement;
    m_lastElement = other.m_lastElement;
    m_currentSize = other.m_currentSize;
}

CircularBuffer::CircularBuffer(CircularBuffer&& other) : CircularBuffer()
{
    swap(other);
    other.m_buffer = nullptr;
    other.m_capacity = 0;
    other.m_firstElement = 0;
    other.m_lastElement = 0;
    other.m_currentSize = 0;

}

CircularBuffer::CircularBuffer(int otherCapacity, const value_type& elem) : CircularBuffer(otherCapacity)
{
    std::memset(m_buffer, elem, otherCapacity);
    m_currentSize = otherCapacity;
    m_lastElement = otherCapacity - 1;
}

CircularBuffer::~CircularBuffer()
{
    if (m_buffer != nullptr)
    {
        delete[] m_buffer;
    }
}

value_type& CircularBuffer::operator[](int i)
{
    return m_buffer[(m_firstElement + i) % capacity()];
}

const value_type& CircularBuffer::operator[](int i) const
{
    return m_buffer[(m_firstElement + i) % capacity()];
}

value_type& CircularBuffer::at(int i)
{
    if (i < m_currentSize)
    {
        return m_buffer[(m_firstElement + i) % capacity()];
    }
    throw std::out_of_range("Incorrect index");
}

const value_type& CircularBuffer::at(int i) const
{
    if (i <= m_currentSize)
    {
        return m_buffer[(m_firstElement + i) % capacity()];
    }
    throw std::out_of_range("Incorrect index");
}

value_type& CircularBuffer::front()
{
    return m_buffer[m_firstElement];
}

value_type& CircularBuffer::back()
{
    return m_buffer[m_lastElement];
}

const value_type& CircularBuffer::front() const
{
    return m_buffer[m_firstElement];
}

const value_type& CircularBuffer::back() const
{
    return m_buffer[m_lastElement];
}

value_type* CircularBuffer::linearize()
{
    rotate(m_firstElement);
    m_firstElement = 0;
    m_lastElement = size() - 1;
    return m_buffer;
}

bool CircularBuffer::is_linearized() const
{
    return m_firstElement == 0;
}

void CircularBuffer::rotate(int newBegin)
{

    std::rotate(m_buffer, m_buffer + newBegin, m_buffer + size());
}

int CircularBuffer::size() const
{
    return m_currentSize;
}

bool CircularBuffer::empty() const
{
    return size() == 0;
}

bool CircularBuffer::full() const
{
    return size() == capacity();
}

int CircularBuffer::reserve() const
{
    return capacity() - size();
}

int CircularBuffer::capacity() const
{
    return m_capacity;
}

void CircularBuffer::set_capacity(int newCapacity)
{
    value_type* newBuffer = new value_type[newCapacity];
    int newCurrentSize = std::min(size(), newCapacity);
    for (int i = 0; i < newCurrentSize; i++)
    {
        newBuffer[(m_firstElement + i) % newCapacity] = m_buffer[(m_firstElement + i) % capacity()];
    }

    delete[] m_buffer;
    m_buffer = newBuffer;
    m_capacity = newCapacity;
    m_currentSize = newCurrentSize;
    if (newCapacity == 0)
    {
        m_firstElement = 0;
        m_lastElement = 0;
    }
    else
    {
        m_firstElement = m_firstElement % capacity();
        m_lastElement = (m_firstElement + size() - 1) % capacity();
    }
}

void CircularBuffer::resize(int newSize, const value_type& item)
{
    if (newSize <= size())
    {
        m_lastElement = (m_firstElement + newSize - 1) % capacity();
    }
    else
    {
        if (newSize > capacity())
        {
            set_capacity(newSize);
        }

        int sizeDifference = newSize - size();
        while (sizeDifference > 0)
        {
            m_buffer[(m_lastElement + 1) % capacity()] = item;
            m_lastElement = (m_lastElement + 1) % capacity();
            sizeDifference--;
        }
    }

    m_currentSize = newSize;
}

CircularBuffer& CircularBuffer::operator=(CircularBuffer& cb)
{
    value_type* newBuffer = new value_type[cb.m_capacity];
    std::memcpy(newBuffer, cb.m_buffer, cb.m_capacity);
    delete[] m_buffer;
    m_buffer = newBuffer;

    m_capacity = cb.m_capacity;
    m_firstElement = cb.m_firstElement;
    m_lastElement = cb.m_lastElement;
    m_currentSize = cb.m_currentSize;

    return *this;
}

void CircularBuffer::swap(CircularBuffer& other)
{
    std::swap(m_buffer, other.m_buffer);
    std::swap(m_capacity, other.m_capacity);
    std::swap(m_firstElement, other.m_firstElement);
    std::swap(m_lastElement, other.m_lastElement);
    std::swap(m_currentSize, other.m_currentSize);
}

void CircularBuffer::push_back(const value_type& item)
{
    if (capacity() == 0)
    {
        return;
    }

    if (full())
    {
        throw std::out_of_range("Buffer is full");
        //m_buffer[m_firstElement] = item;
        //m_lastElement = m_firstElement;
        //m_firstElement = (m_firstElement + 1) % capacity();
    }
    else
    {
        m_lastElement = (m_lastElement + 1) % capacity();
        m_buffer[m_lastElement] = item;
        m_currentSize++;
        m_firstElement = (m_lastElement + capacity() - size() + 1) % capacity();
    }
}

void CircularBuffer::push_front(const value_type& item)
{
    if (capacity() == 0)
    {
        return;
    }

    if (full())
    {
        throw std::out_of_range("Buffer is full");
        //m_buffer[m_lastElement] = item;
        //m_firstElement = m_lastElement;
        //m_lastElement = (m_lastElement + capacity() - 1) % capacity();
    }
    else
    {
        m_firstElement = (m_firstElement + capacity() - 1) % capacity();
        m_buffer[m_firstElement] = item;
        m_lastElement = (m_firstElement + size()) % capacity();
        m_currentSize++;
    }
}

void CircularBuffer::pop_back()
{
    if (size() == 0)
    {
        return;
    }

    m_currentSize--;
    m_lastElement = (m_lastElement + capacity() - 1) % capacity();
}

void CircularBuffer::pop_front()
{
    if (size() == 0)
    {
        return;
    }

    m_currentSize--;
    m_firstElement = (m_firstElement + 1) % capacity();
}

void CircularBuffer::insert(int pos, const value_type& item)
{
    if (capacity() == 0)
    {
        return;
    }

    value_type* newBuffer = new value_type[capacity()];
    std::memcpy(newBuffer, m_buffer, capacity());
    newBuffer[(m_firstElement + pos) % capacity()] = item;

    for (int i = pos; i < size(); i++)
    {
        newBuffer[(m_firstElement + 1 + i) % capacity()] = m_buffer[(m_firstElement + i) % capacity()];
    }

    int newFirstElement = m_firstElement;
    if (capacity() != size())
    {
        m_currentSize++;
    }
    else
    {
        newFirstElement = (m_firstElement + 1) % capacity();
    }

    delete[] m_buffer;
    m_buffer = newBuffer;
    m_firstElement = newFirstElement;
    m_lastElement = (m_firstElement + size() - 1) % capacity();
}

void CircularBuffer::erase(int first, int last)
{
    value_type* newBuffer = new value_type[capacity()];
    int difference = last - first;
    m_currentSize -= difference;

    for (int i = 0; i < first; i++)
    {
        newBuffer[(m_firstElement + i) % capacity()] = m_buffer[(m_firstElement + i) % capacity()];
    }
    for (int i = first; i < size(); i++)
    {
        newBuffer[(m_firstElement + i) % capacity()] = m_buffer[(m_firstElement + difference + i) % capacity()];
    }

    delete[] m_buffer;
    m_buffer = newBuffer;
    m_lastElement = (m_firstElement + size() - 1) % capacity();
}

void CircularBuffer::clear()
{
    m_currentSize = 0;
    m_firstElement = 0;
    m_lastElement = 0;
}

bool operator==(const CircularBuffer& a, const CircularBuffer& b)
{
    if (a.size() != b.size())
    {
        return false;
    }
    for (int i = 0; i < a.size(); i++)
    {
        if (a[i] != b[i])
        {
            return false;
        }
    }

    return true;
}

bool operator!=(const CircularBuffer& a, const CircularBuffer& b)
{
    return !(a == b);
}

