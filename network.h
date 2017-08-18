#ifndef NETWORK_H
#define NETWORK_H

#include "network_controller.h"
#include "network_utils.h"

class network : public network_controller
{
    Q_OBJECT

public:
     explicit network();
     ~network();

    void run_cmd(const QJsonObject & o);
};

#endif // NETWORK_H
