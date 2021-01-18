//
// Created by andrea on 06/01/21.
//

#include "MasterSlave.h"
#include <iostream>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <csignal>

using namespace std;


template<class SharedMemory>
MasterSlave<SharedMemory>::MasterSlave(SharedMemory memory, int slaves) : memory(memory), slaves(slaves) { }

template<class SharedMemory>
void MasterSlave<SharedMemory>::start(const SlaveBehaviour& behaviour) {
    for(int i = 0; i < slaves; i++) {
        slavePids.push_back(::fork());
        if(slavePids.back() == 0) {
            behaviour(i);
        } else if (slavePids.back() < 0) {
            cerr << "Error creating slave " << endl;
        }
    }
    ::sleep(3);
}

template<class SharedMemory>
void MasterSlave<SharedMemory>::resumeSlaves() {
    for(auto pid : slavePids) {
        // resume child process
        ::kill(pid, SIGCONT);
    }
}

template<class SharedMemory>
void MasterSlave<SharedMemory>::waitSlaves() {
    int slaveInfo;
    pid_t slavePid;
    for(int waitSize = slavePids.size(); waitSize > 0; waitSize--) {
        slavePid = ::waitpid(-1, &slaveInfo, WUNTRACED);
        if (!WIFSTOPPED(slaveInfo)) {
            cerr << "[FATAL] Slave process with PID " << slavePid
                 << " exited, can't continue. Check file ARGoS_LOGERR_" << slavePid << " for more information."
                 << std::endl;
            ::exit(1);
        }
    }
}
