#ifndef ABSTRACTSTEP_H
#define ABSTRACTSTEP_H

#include <QObject>
#include <QProcess>

class AbstractStep : public QObject
{
    Q_OBJECT
public:
    explicit AbstractStep(QObject *parent = 0);

    /**
     * @brief runStep runs the step that is provided by this class. In AbstractStep, it is a virtual
     * method so it must be overriden by children classes to actually run the required command.
     * A subset of command can be passed (ie. parameters) in the commands stringlist parameter.
     *
     * @param commands A list of subcommands this step should run
     */
    virtual void runStep(const QStringList& commands) = 0;

signals:
    /**
     * @brief stepStarted Signal sent when the step has been started (ie. the method is running)
     */
    void stepStarted();

    /**
     * @brief stepEnded Signal sent when the step has successfully finished
     */
    void stepEnded(QString stdout);

    /**
     * @brief stepError Signal sent when the step hasn't returned 0
     */
    void stepError(QString stdout, QString stderr);

public slots:
    /**
     * @brief onStdOut Called when the process started outputs something on the standard output
     */
    virtual void onStdOut();

    /**
     * @brief onStdErr Called when the process started outputs something on the error output
     */
    virtual void onStdErr();

protected:
    QProcess* mProcess;
    QString mProcessStdOut;
    QString mProcessStdErr;
};

#endif // ABSTRACTSTEP_H
