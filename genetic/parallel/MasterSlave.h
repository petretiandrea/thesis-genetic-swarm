//
// Created by andrea on 06/01/21.
//

#ifndef SWARM_GEN_MASTERSLAVE_H
#define SWARM_GEN_MASTERSLAVE_H

#include <vector>
#include <functional>
#include <unistd.h>

template<class SharedMemory> class MasterSlave {

    typedef std::function<void(int)> SlaveBehaviour;

public:
    explicit MasterSlave(SharedMemory memory, int slaves);

    void start(const SlaveBehaviour& behaviour);
    void resumeSlaves();
    void waitSlaves();
    inline SharedMemory sharedMemory() { return this->memory; }

private:
    SharedMemory memory;
    int slaves;
    std::vector<pid_t> slavePids;
};

#endif //SWARM_GEN_MASTERSLAVE_H
