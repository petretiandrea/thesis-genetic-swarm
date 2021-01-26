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

    explicit SharedBinaryGenome() : genome(nullptr), genomeSize(0), fitness(-1), robot_count(-1) { }
    explicit SharedBinaryGenome(shortallocator alloc, int genomeSize) : genome(alloc.allocate(genomeSize).get()), genomeSize(0), fitness(-1), robot_count(-1) { }

    /*SharedBinaryGenome(short* genome, int genomeSize, float fitness, float robot_count, shortallocator alloc) :
            genome(alloc.allocate(genomeSize).get()),
            genomeSize(genomeSize),
            fitness(fitness),
            robot_count(robot_count) {


    }*/
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

    static unsigned long calculate_mem(int individuals, int genomeSize, int workers) {
        auto total = (sizeof(SharedBinaryGenome) * individuals) +
                 (sizeof(short) * genomeSize * individuals) +
                 sizeof(int) * 2 * workers;

        return (unsigned long) total + (0.33 * total);
    }

private:
    shared::segment segment;
    shared::BinaryGenome* genomes;
    int* workSlices;

public:
    BNGenomeSharedMemory(int genomes, int genomeSize, int workers) :
        genomes(nullptr),
        segment(inter::open_or_create, SHARED_MEM_NAME, calculate_mem(genomes, genomeSize, workers))
    {
        this->genomes = segment.find_or_construct<shared::BinaryGenome>("genomes")[genomes](shared::BinaryGenome());
        this->workSlices = segment.find_or_construct<int>("work_slices")[workers * 2](0);
        std:: cout << "Free shared memory " << segment.get_free_memory() << std::endl;
    }

    void putGenome(int index, short* genome, int genomeSize, float fitness, float robotCount) {
        if(this->genomes[index].genome == nullptr) {
            this->genomes[index] = shared::BinaryGenome(segment.get_segment_manager(), genomeSize);
        }
        this->genomes[index].fitness = fitness;
        this->genomes[index].robot_count = robotCount;
        this->genomes[index].genomeSize = genomeSize;
        std::copy(genome, genome + genomeSize, this->genomes[index].genome);
    }

    void updateGenomeEvaluation(int index, float fitness, float robotCount) {
        this->genomes[index].fitness = fitness;
        this->genomes[index].robot_count = robotCount;
    }

    shared::BinaryGenome& getGenome(int index) {
        return this->genomes[index];
    }

    void putWorkSlice(int index, int from, int size) {
        this->workSlices[index * 2] = from;
        this->workSlices[(index * 2) + 1] = size;
    }

    void getWorkSlice(int index, int* from, int* size) {
        *from = this->workSlices[index * 2];
        *size = this->workSlices[(index * 2) + 1];
    }
};

#endif //SWARM_GEN_BNGENOMESHAREDMEMORY_H
