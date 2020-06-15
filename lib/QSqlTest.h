/**
 * @file QSqlTest.h
 * @brief Load and execute a QSqlTest project.
 */

#ifndef QSqlTest_H
#define QSqlTest_H

#include <QObject>
#include <QString>

class QSqlTestPrivate;

class QSqlTestCase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(bool finished READ finished WRITE setFinished NOTIFY finishedChanged)
    Q_PROPERTY(bool passed READ passed WRITE setPassed NOTIFY finishedChanged)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)
public:
    QSqlTestCase(const QString &testName, bool done = false, bool result = false,
                 QObject *parent = nullptr)
        : QObject()
        , m_name(testName)
        , m_finished(done)
        , m_passed(result)
        , m_status("Unfinished")
    {
    }

    QString name() const
    {
        return m_name;
    }

    bool finished() const
    {
        return m_finished;
    }

    bool passed() const
    {
        return m_passed;
    }

    QString status() const
    {
        return m_status;
    }

    void setFinished(bool finished)
    {
        if (m_finished != finished) {
            m_finished = finished;
            emit finishedChanged();
        }
    }

    void setPassed(bool passed)
    {
        if (m_passed != passed) {
            m_passed = passed;
            emit passedChanged();
        }
    }

    void setStatus(QString status)
    {
        if (m_status != status) {
            m_status = status;
            emit statusChanged();
        }
    }

signals:
    void nameChanged();
    void finishedChanged();
    void passedChanged();
    void statusChanged();

private:
    QString m_name;
    bool m_finished;
    bool m_passed;
    QString m_status;
};

// typedef QSqlTestCase::Status TestStatus;
// Q_DECLARE_METATYPE(TestStatus);
// qRegisterMetaType<TestStatus>("TestStatus")

/**
 * @class QSqlTest
 * @brief SQL test suite.
 */
class QSqlTest : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool loaded READ isLoaded CONSTANT)
    Q_PROPERTY(bool connected READ isConnected CONSTANT)
    Q_PROPERTY(int passed READ passed NOTIFY update)
    Q_PROPERTY(int failed READ failed NOTIFY update)
    Q_PROPERTY(int skipped READ skipped NOTIFY update)
    Q_PROPERTY(QString errorString READ errorString)
    Q_PROPERTY(QList<QSqlTestCase *> tests READ tests NOTIFY testsChanged)

public:
    explicit QSqlTest(QObject *parent = nullptr);

    enum SqlTestError
    {
        NoError, ///< Success.
        ProjectFileError, ///< Error opening Project JSON file.
        ParsingError, ///< Error parsing project JSON file.
        ConnectingError, ///< Cannot connect to database.
        CsvFileError, ///< Invalid CSV file.
        TestFailedError, ///< A test case has failed.
    };
    Q_ENUM(SqlTestError)

    enum SqlTestUpdate
    {
        /** Tests are now running. */
        Starting,
        /** All testing has completed. */
        Finished,
        /** Project JSON file has been loaded. */
        Loaded,
        /** A test case has passed. */
        TestPassed,
        /** A test case has failed. */
        TestFailed,
        TestSkipped,
        Connected,
        Disconnected,
    };
    Q_ENUM(SqlTestUpdate)

    /** Project file loaded. */
    bool isLoaded() const;
    /** Connected to a database. */
    bool isConnected() const;
    /** Number of test cases that have passed. */
    int passed() const;
    /** Number of test cases that have failed. */
    int failed() const;
    int skipped() const;
    /** Most recent error. */
    QString errorString() const;
    /** Test cases. */
    QList<QSqlTestCase *> tests() const;

    /** Enable/disable log messages. */
    static void enableLog(bool enable);

signals:
    void error(const QString &err);
    void update(SqlTestUpdate status);

    void testsChanged();

public slots:
    /**
     * Load and execute tests.
     * @param[in] fileName JSON project file to load.
     * @returns Test results. `true` if the project was loaded and all tests executed successfully.
     * `false` otherwise.
     */
    bool start(const QString &fileName);

private:
    bool load(const QString &jsonFilename);
    bool connect();
    bool createTables();
    bool startTest(const QString &testName);

    QSqlTestPrivate *d;
};

#endif
