#pragma once

#include <QString>

/**
 * The BigNumber allow to operate with very big numbers consists from up to MAX_INT digits
 * (restriction from QString).
 */
class BigNumber
{
public:
    /**
     * Default constructor
     */
    BigNumber();

    /**
     * Copy constructor
     */
    BigNumber(const BigNumber& other);

    /**
     * Constructor from string representation of number
     */
    BigNumber(const QString &asciiString);

    /**
     * Destructor
     */
    virtual ~BigNumber();

    /**
     * BigNumber can be invalid if constructed from string which does not represent number
     * or if it is summ of BigNumbers where one of operands was invalid
     * @return true if number is valid
     */
    bool isValid() const;

    /**
     * @return string representation of number
     */
    QString toString() const;

    /**
     * @return number's digit at given @a position (begins from zero and from right side)
     *
     * @note all positions without digits returns 0. For example, all digits in positions
     * above 2 for number 123 will be 0.
     */
    quint8 digit(quint64 position) const;

    /**
     * @return count of digits in number
     */
    quint64 size() const;

private:
    char* _data;
    quint64 _size; // we like very big numbers, but do you realy want to write so long string?
};

BigNumber operator +(const BigNumber& first, const BigNumber& second);
