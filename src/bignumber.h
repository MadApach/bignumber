#pragma once

//#include <QtGlobal>

#include <QString>

class BigNumber
{
public:
    BigNumber();
    BigNumber(const BigNumber& other);
    BigNumber(const QString &asciiString);
    virtual ~BigNumber();
    bool isValid() const;
    QString toString() const;
    quint8 digit(quint64 index) const;
    quint64 size() const;

private:
    char* _data;
    quint64 _size; // we like very big numbers, but do you realy want to write so long string?
};

BigNumber operator +(const BigNumber& first, const BigNumber& second);
