#include <QTest>
#include "tst_sqltest.h"
#include "QSqlTest.h"

void Test1::initTestCase()
{
    QSqlTest::enableLog(false);
}

void Test1::cleanupTestCase()
{
    QSqlTest::enableLog(true);
}

void Test1::success_data()
{
    QTest::addColumn<QString>("filename");

    QTest::newRow("Superheroes") << "fixtures/superheroes.json";
}

void Test1::success()
{
    QSqlTest test;
    QFETCH(QString, filename);

    QString input = QFINDTESTDATA(filename);

    QVERIFY2(!input.isEmpty(), qPrintable(QString("test fixture %1 not found").arg(input)));
    QVERIFY(test.start(input));
}

QTEST_MAIN(Test1)
