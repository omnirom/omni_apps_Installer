#include "AbstractStep.h"

//------------------------------------------------------
AbstractStep::AbstractStep(QObject *parent) :
    QObject(parent), mProcess(NULL)
{
    mProcess = new QProcess(this);
    connect(mProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(onStdOut()));
    connect(mProcess, SIGNAL(readyReadStandardError()), this, SLOT(onStdErr()));
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
