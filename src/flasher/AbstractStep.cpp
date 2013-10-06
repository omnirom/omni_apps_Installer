#include "AbstractStep.h"

#include <QDebug>

//------------------------------------------------------
AbstractStep::AbstractStep(QObject *parent) :
    QObject(parent), mProcess(NULL), mPrepared(false)
{
    mProcess = new QProcess(this);
    connect(mProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(onStdOut()));
    connect(mProcess, SIGNAL(readyReadStandardError()), this, SLOT(onStdErr()));
    connect(mProcess, SIGNAL(finished(int)), this, SLOT(onProcessFinished(int)));
}
//------------------------------------------------------
void AbstractStep::onStdOut()
{
    mProcessStdOut.append(mProcess->readAllStandardOutput());
}
//------------------------------------------------------
void AbstractStep::onStdErr()
{
    mProcessStdOut.append(mProcess->readAllStandardError());
}
//------------------------------------------------------
void AbstractStep::runStep(const QStringList &commands)
{
    mCommands = commands;
}
//------------------------------------------------------
void AbstractStep::onProcessFinished(int exitCode)
{
    qDebug() << "AbstractStep: Process has ended (return code: " << exitCode << ")";

    if (exitCode == EXIT_SUCCESS)
    {
        emit stepEnded(mProcessStdOut);
    }
    else
    {
        qDebug() << mProcessStdErr;
        qDebug() << mProcessStdOut;
        emit stepError(exitCode, mProcessStdOut, mProcessStdErr);
    }
}
//------------------------------------------------------
