#pragma once

typedef char value_type;

class CircularBuffer
{
public:
    CircularBuffer();
    ~CircularBuffer();
    CircularBuffer(const CircularBuffer &other);
    CircularBuffer(CircularBuffer&& other);
    explicit CircularBuffer(int capacity);
    CircularBuffer(int capacity, const value_type &elem);

    value_type &operator[](int i);
    const value_type &operator[](int i) const;

    value_type &at(int i);
    const value_type &at(int i) const;

    value_type &front();
    value_type &back();
    const value_type &front() const;
    const value_type &back() const;

    value_type *linearize();
    bool is_linearized() const;
    void rotate(int new_begin);
    int size() const;
    bool empty() const;
    bool full() const;
    int reserve() const;
    int capacity() const;

    void set_capacity(int newCapacity);
    void resize(int newSize, const value_type &item = value_type());
    CircularBuffer &operator=(CircularBuffer &cb);
    void swap(CircularBuffer &cb);

    void push_back(const value_type &item = value_type());
    void push_front(const value_type &item = value_type());
    void pop_back();
    void pop_front();

    void insert(int pos, const value_type &item = value_type());
    void erase(int first, int last);
    void clear();

private:
    value_type *m_buffer;
    int m_capacity;
    int m_firstElement;
    int m_lastElement;
    int m_currentSize;
};

bool operator==(const CircularBuffer &a, const CircularBuffer &b);
bool operator!=(const CircularBuffer &a, const CircularBuffer &b);