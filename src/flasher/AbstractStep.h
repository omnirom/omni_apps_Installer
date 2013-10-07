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
     * method so it must be overriden by children classes to actually run the required command, but
     * this one must be called to save commands in mCommands.
     * A subset of command can be passed (ie. parameters) in the commands stringlist parameter.
     *
     * @param commands A list of subcommands this step should run
     */
    virtual void runStep(const QStringList& commands);

signals:
    /**
     * @brief stepPrepared Signal sent when the step has been prepared (e.g. rebooted to fastboot)
     */
    void stepPrepared();

    /**
     * @brief stepEnded Signal sent when the step has successfully finished
     */
    void stepEnded(QString /*stdout*/);

    /**
     * @brief stepError Signal sent when the step hasn't returned 0
     *
     */
    void stepError(int /*exitCode*/, QString /*stdout*/, QString /*stderr*/);

public slots:
    /**
     * @brief onStdOut Called when the process started outputs something on the standard output
     */
    virtual void onStdOut();

    /**
     * @brief onStdErr Called when the process started outputs something on the error output
     */
    virtual void onStdErr();

    /**
     * @brief onStepPrepared Called when the process has prepared the device, e.g. it rebooted
     * in fastboot mode.
     */
    virtual void onStepPrepared() = 0;

    /**
     * @brief onProcessFinished Called when the process exits
     */
    virtual void onProcessFinished(int exitCode);

protected:
    QProcess* mProcess;
    QString mProcessStdOut;
    QString mProcessStdErr;
    bool mPrepared;
    QStringList mCommands;
    int mCurrentCommand;
};

#endif // ABSTRACTSTEP_H
