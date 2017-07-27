#include "bignumber.h"

#include <QString>
#include <QtTest>

class BigNumberTest : public QObject
{
    Q_OBJECT

public:
    BigNumberTest();

private Q_SLOTS:
    void constructInvalid_data();
    void constructInvalid();

    void construtValid_data();
    void construtValid();

    void testDigits_data();
    void testDigits();

    void testToString_data();
    void testToString();

    void testOperatorPlus_data();
    void testOperatorPlus();
};

BigNumberTest::BigNumberTest()
{
}

void BigNumberTest::constructInvalid_data()
{
    QTest::addColumn<QString>("input");

    QTest::newRow("letters") << "123a123";
    QTest::newRow("spaces") << "123 123";
    QTest::newRow("punctuation marks") << "123.123";
    QTest::newRow("negative sign") << "-123123";
    QTest::newRow("empty string") << "";
}

void BigNumberTest::constructInvalid()
{
    QFETCH(QString, input);
    QCOMPARE(BigNumber(input.toLatin1().constData()).isValid(), false);
}

void BigNumberTest::construtValid_data()
{
    QTest::addColumn<QString>("origin");

    QTest::newRow("simple") << "13245678901234567890";
    QTest::newRow("with leading zeroes") << "00013245678901234567890";
}

void BigNumberTest::construtValid()
{
    QVERIFY2(BigNumber("12345678901234567890").isValid(),
             "Construction number from valid string is failed");
}

void BigNumberTest::testDigits_data()
{
    QTest::addColumn<quint64>("position");
    QTest::addColumn<quint8>("digit");

    QTest::newRow("0th from right side") << quint64(0) << quint8(0);
    QTest::newRow("1st from right side") << quint64(1) << quint8(1);
    QTest::newRow("2nd from right side") << quint64(2) << quint8(2);
    QTest::newRow("3rd from right side") << quint64(3) << quint8(3);
    QTest::newRow("4th from right side") << quint64(4) << quint8(4);
    QTest::newRow("5th from right side") << quint64(5) << quint8(5);
    QTest::newRow("6th from right side") << quint64(6) << quint8(6);
    QTest::newRow("7th from right side") << quint64(7) << quint8(7);
    QTest::newRow("8th from right side") << quint64(8) << quint8(8);
    QTest::newRow("9th from right side") << quint64(9) << quint8(9);
    QTest::newRow("10th from right side") << quint64(10) << quint8(0);
}

void BigNumberTest::testDigits()
{
    BigNumber number("9876543210");

    QFETCH(quint8, digit);
    QFETCH(quint64, position);

    quint8 digitFromNumber(number.digit(position));
    QCOMPARE(digitFromNumber, digit);
}

void BigNumberTest::testToString_data()
{
    QTest::addColumn<QString>("origin");
    QTest::addColumn<QString>("fromNumber");

    QTest::newRow("simple") << "123456789" << "123456789";
    QTest::newRow("with leading zero") << "0123456789" << "123456789";
    QTest::newRow("not a number") << "asd" << "NaN";
}

void BigNumberTest::testToString()
{
    QFETCH(QString, origin);
    QFETCH(QString, fromNumber);

    QCOMPARE(BigNumber(origin.toStdString().c_str()).toString().toStdString(), fromNumber.toStdString());
}

void BigNumberTest::testOperatorPlus_data()
{
    QTest::addColumn<QString>("first");
    QTest::addColumn<QString>("second");
    QTest::addColumn<QString>("summ");

    QTest::newRow("simple") << "123" << "456" << "579";
    QTest::newRow("with digit overflow") << "999" << "999" << "1998";
    QTest::newRow("short and long") << "123" << "789888" << "790011";
    QTest::newRow("long and long") << "1234567890123456789013579"
                                   << "1234567890123456789013582"
                                   << "2469135780246913578027161";
    QTest::newRow("number and NaN") << "123" << "asd" << "NaN";
}

void BigNumberTest::testOperatorPlus()
{
    QFETCH(QString, first);
    QFETCH(QString, second);
    QFETCH(QString, summ);

    QCOMPARE((BigNumber(first.toStdString().c_str()) +
              BigNumber(second.toStdString().c_str())).toString().toStdString(), summ.toStdString());
}


QTEST_APPLESS_MAIN(BigNumberTest)

#include "tst_bignumbertest.moc"
