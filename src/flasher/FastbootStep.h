#ifndef FASTBOOTSTEP_H
#define FASTBOOTSTEP_H

#include "AbstractStep.h"

class FastbootStep : public AbstractStep
{
    Q_OBJECT

public:
    explicit FastbootStep(QObject *parent = 0);

    void runStep(const QStringList &commands);

signals:



};

#endif // FASTBOOTSTEP_H
