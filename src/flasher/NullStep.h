#ifndef NULLSTEP_H
#define NULLSTEP_H

#include "AbstractStep.h"

class NullStep : public AbstractStep
{
    Q_OBJECT

public:
    NullStep();

    void runStep(const QStringList &commands);
};

#endif // NULLSTEP_H
