#include <iostream>
#include <cstring>

namespace
{
    class String
    {
    private:
        char* m_storage;
        std::size_t m_size;
    public:
        String(const char* str = "") 
        {
            m_size = size(str);
            m_storage = new char[m_size + 1];
            
            std::memcpy(m_storage, str, m_size);
            m_storage[m_size] = '\0';
            
            std::cout << "The constructor called for this this: " << this << std::endl;
        }
        
        String(const String& other)
        {
            m_size = other.m_size;
            m_storage = new char[m_size + 1];
            
            std::memcpy(m_storage, other.m_storage, m_size);
            m_storage[m_size] = '\0';

            std::cout << "The copy constructor called for this this: " << this << std::endl;
        }

        ~String()
        {
            delete[] m_storage;
            
            std::cout << "The destructor called for this this: " << this << std::endl;
        }

        static std::size_t size(const char* str) 
        {
            std::size_t strLen = 0;
            for (std::size_t i = 0; str[i] != '\0'; i++) {
                strLen++;
            }

            return strLen;
        }

        const String& operator += (const String& right)
        {
            std::size_t newSize = m_size + right.m_size;
            char* newStorage = new char[newSize + 1];
            
            std::memcpy(newStorage, m_storage, m_size);
            for (std::size_t i = 0; i < right.m_size; i++) {
                newStorage[i + m_size] = right.m_storage[i];
            }

            newStorage[newSize] = '\0';
            delete[] m_storage;
            m_size = newSize;
            m_storage = newStorage;

            return *this;
        }

        const char* c_str() const
        {
            return m_storage;
        }
    };
    
} // namespace

int main() 
{
    String str{"Hello Wor"};
    str += "ld! 23209";
    std::cout << str.c_str() << std::endl;
    
    String strCopy{str};
    std::cout << strCopy.c_str() << std::endl;
    
    return 0;
}
