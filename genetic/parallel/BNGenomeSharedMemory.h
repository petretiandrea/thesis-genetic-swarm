//
// Created by andrea on 05/01/21.
//

#ifndef SWARM_GEN_BNGENOMESHAREDMEMORY_H
#define SWARM_GEN_BNGENOMESHAREDMEMORY_H

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/string.hpp>

#include <iostream>
#include <algorithm>
#include <iterator>

namespace inter = boost::interprocess;

using shortallocator = inter::allocator<short, inter::managed_shared_memory::segment_manager>;
struct SharedBinaryGenome {
    //using string = inter::basic_string<char, std::char_traits<char>, typename Alloc::template rebind<char>::other>;

    short* genome;
    int genomeSize;
    float fitness;
    float robot_count;

    explicit SharedBinaryGenome(shortallocator alloc) : genome(nullptr), genomeSize(0), fitness(-1), robot_count(-1) { }

    SharedBinaryGenome(short* genome, int genomeSize, float fitness, float robot_count, shortallocator alloc) :
            genome(alloc.allocate(genomeSize).get()),
            genomeSize(genomeSize),
            fitness(fitness),
            robot_count(robot_count) {

        std::copy(genome, genome + genomeSize, this->genome);
    }

    inline void copyArray(short array1[], short array2[], int n) {

    }
};

namespace shared {
    using segment                      = inter::managed_shared_memory; // or managed_shared_memory
    using segment_manager              = segment::segment_manager;

    template <typename T> using alloc  = inter::allocator<T, segment_manager>;
    template <typename T> using vector = inter::vector<T, alloc<T>>;

    using BinaryGenome = SharedBinaryGenome; // shared memory version
}

class BNGenomeSharedMemory {

#define SHARED_MEM_NAME "CustomGenomeSharedMemory"

private:
    //Remove shared memory on construction and destruction
    struct shm_remove
    {
        shm_remove() { inter::shared_memory_object::remove(SHARED_MEM_NAME); }
        ~shm_remove(){ inter::shared_memory_object::remove(SHARED_MEM_NAME); }
    } remover;


private:
    shared::segment segment;
    shared::BinaryGenome* genomes;
    int* workSlices;

public:
    BNGenomeSharedMemory(int genomes, int workers) :
        genomes(nullptr),
        segment(inter::open_or_create, SHARED_MEM_NAME, 65536)
    {
        this->genomes = segment.find_or_construct<shared::BinaryGenome>("genomes")[genomes](shared::BinaryGenome(segment.get_segment_manager()));
        this->workSlices = segment.find_or_construct<int>("work_slices")[workers * 2](0);
    }

    void putGenome(int index, short* genome, int genomeSize, float fitness, float robotCount) {
        auto data = shared::BinaryGenome { genome, genomeSize, fitness, robotCount, segment.get_segment_manager() };
        this->genomes[index] = data;
    }

    void updateGenomeEvaluation(int index, float fitness, float robotCount) {
        this->genomes[index].fitness = fitness;
        this->genomes[index].robot_count = robotCount;
    }

    shared::BinaryGenome& getGenome(int index) {
        return this->genomes[index];
    }

    void putWorkSlice(int index, int from, int size) {
        this->workSlices[index] = from;
        this->workSlices[index + 1] = size;
    }

    void getWorkSlice(int index, int* from, int* size) {
        *from = this->workSlices[index];
        *size = this->workSlices[index + 1];
    }
};

#endif //SWARM_GEN_BNGENOMESHAREDMEMORY_H
