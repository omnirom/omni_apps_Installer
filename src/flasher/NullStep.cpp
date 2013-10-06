#include "NullStep.h"

//--------------------------------------
NullStep::NullStep(QObject* parent) : AbstractStep(parent)
{
}
//--------------------------------------
void NullStep::runStep(const QStringList &commands)
{
    Q_UNUSED(commands);
    // Null! :)
}
//--------------------------------------
void NullStep::onStepPrepared()
{

}
//--------------------------------------
