//
// Created by andrea on 05/01/21.
//

#ifndef SWARM_GEN_EVALUATIONFUNCTIONS_H
#define SWARM_GEN_EVALUATIONFUNCTIONS_H

#include <cmath>

/**
 * All evaluation function used for evaluate the performance in simulation
 */
namespace evaluation {

    typedef std::function<double(double, double, double)> EvaluationFunction;

    // 1 / ( 1 + e^(-A*(B-x)) )
    /**
     *
     * @param a control the slope
     * @param b control x axis center flex
     * @return
     */
    inline std::function<double(double)> parametric_sigmoid(double slope, double center) {
        return [slope, center](double x) {
            return 1.0 / (1 + exp(-slope *(center - x)));
        };
    }

    /**
     * The max of one circle
     **/
    inline EvaluationFunction max_of_circle() {
        return [](double total, double a, double b){
            auto norm_a = a / total;
            auto norm_b = a / total;
            return (norm_a > norm_b) ? norm_a : norm_b;
        };
    }

    inline EvaluationFunction max_parametric_sigmoid(double slope, double center) {
        std::function<double(double)> sigmoid = evaluation::parametric_sigmoid(slope, center);
        return [sigmoid](double total, double a, double b) {
            double sigmoid_a = sigmoid(a / total);
            double sigmoid_b = sigmoid(b / total);
            return (sigmoid_a > sigmoid_b) ? sigmoid_a : sigmoid_b;
        };
    }

    inline EvaluationFunction difference() {
        return [](double total, double a, double b){
            return std::abs(a - b) / total;
        };
    }
}

#endif //SWARM_GEN_EVALUATIONFUNCTIONS_H
