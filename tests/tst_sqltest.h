#ifndef TEST1_H
#define TEST1_H

#include <QObject>

class Test1 : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void cleanupTestCase();

    void success_data();
    void success();
};

#endif
