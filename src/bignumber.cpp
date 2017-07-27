#include "bignumber.h"

#include <QString>

#include <math.h>
#include <cstring>

BigNumber::BigNumber()
    : _data(nullptr)
    , _size(0)
{
}

BigNumber::BigNumber(const BigNumber &other)
    : _data(nullptr)
    , _size(other._size)
{
    int dataSize(other._size / 2 + other._size % 2);
    _data = (char*) calloc(dataSize, sizeof(char));
    memcpy(_data, other._data, dataSize);
}

namespace
{
bool isAsciiCodedDigit(char symbol)
{
    return ((symbol >= char('0')) && (symbol <= char('9')));
}

bool isAsciiCodedNumber(const char* string)
{
    if(*string == 0)
    {
        return false;
    }

    const char* symbol = string;
    while(*symbol != 0)
    {
        if(!isAsciiCodedDigit(*symbol))
        {
            return false;
        }
        ++symbol;
    }
    return true;
}

quint64 countOfLeadingZeroes(const char* string)
{
    int count = 0;
    const char* symbol = string;
    while(*symbol == '0')
    {
        count++;
        symbol++;
    }
    return count;
}

const char ASCII_SHIFT = 0x30; // digits are stored in ascii-table from 0x30 position one by one

} // anonymous

BigNumber::BigNumber(const QString& asciiString)
    : _data(nullptr)
    , _size(0)
{
    const char* number(asciiString.toLatin1().constData()); // this way should be faster than work with QString
    // and we can se signed char 'cause of first part of asci-table is sign-insensitive
    if(isAsciiCodedNumber(number)) // it will be faster than matching by QRegExp
    {
        quint64 zeroesCount = countOfLeadingZeroes(number);
        const char* num = number + zeroesCount;

        _size = asciiString.size() - zeroesCount;

        int halfByteFactor(size() % 2); // we will use half byte to store one digit

        _data = (char*) calloc((size() + halfByteFactor) / 2, sizeof(char));

        for(int i = size() - 1, j = 0; i >= halfByteFactor; i -= 2, ++j)
        {
            _data[j] = ((num[i] - ASCII_SHIFT) << 4) + (num[i - 1] - ASCII_SHIFT);
        }
        if(halfByteFactor != 0)
        {
            _data[size() / 2] = ((num[0] - ASCII_SHIFT) << 4);
        }
    }
}

BigNumber::~BigNumber()
{
    if(isValid())
    {
        free(_data);
        _size = 0;
        _data = nullptr;
    }
}

bool BigNumber::isValid() const
{
    return (_data != nullptr);
}

QString BigNumber::toString() const
{
    if(!isValid())
    {
        return "NaN";
    }

    QString number;
    for(quint64 i = size() - 1; i != quint64(-1); --i) // we can't write "i >= 0" 'cause of
                                                      // "i" never drops below zero
    {
        number.push_back(char(digit(i)) + ASCII_SHIFT);
    }

    return number;
}

quint8 BigNumber::digit(quint64 position) const
{
    if(position >= size())
    {
        return 0;
    }
    int shift = (position % 2) == 0 ? 4 : 0;
    static const unsigned char mask(0x0F);
    return quint8((_data[position / 2] >> shift) & mask);
}

quint64 BigNumber::size() const
{
    return _size;
}

BigNumber operator +(const BigNumber &first, const BigNumber &second)
{
    if(!first.isValid() || !second.isValid())
    {
        return BigNumber();
    }
    quint64 count(std::max(first.size(), second.size()));
    char* resString = (char*) calloc(count + 2, sizeof(char)); // it's faster than append to QString
    quint8 sum(0);
    for(quint64 i = 0; i < count; ++i)
    {
        sum = first.digit(i) + second.digit(i) + sum;
        resString[count - i] = char((sum % 10)) + '0';
        sum = sum / 10;
    }
    resString[0] = char(sum) + '0';
    BigNumber result(resString);
    free(resString);
    resString = nullptr;
    return result;
}
