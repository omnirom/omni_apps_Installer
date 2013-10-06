#ifndef NULLSTEP_H
#define NULLSTEP_H

#include "AbstractStep.h"

class NullStep : public AbstractStep
{
    Q_OBJECT

public:
    NullStep(QObject* parent = 0);

    void runStep(const QStringList &commands);

public slots:
    void onStepPrepared();
};

#endif // NULLSTEP_H
