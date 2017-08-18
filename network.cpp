#include "network.h"

network::network()
{        

}

network::~network()
{   
}

void network::run_cmd(const QJsonObject &o)
{    
    emit network_write(o);
}
