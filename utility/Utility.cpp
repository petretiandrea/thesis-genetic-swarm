//
// Created by Andrea Petreti on 04/01/2021.
//

#include "Utility.h"
#include <stdexcept>
#include <boost/range.hpp>
#include <boost/range/algorithm.hpp>

vector<bool> utility::generateBinaryVector(int size, rnd::Random& rnd, float bias) {
    vector<bool> v(size);
    boost::range::generate(v, [&rnd, bias](){
        return rnd.next() <= bias;
    });
    return v;
}


int utility::boolToInt(bool value, bool invert) { return invert ? (!value ? 1 : 0) : (value ? 1 : 0); }