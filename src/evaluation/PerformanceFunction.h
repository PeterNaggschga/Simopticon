#ifndef SIMOPTICON_PERFORMANCEFUNCTION_H
#define SIMOPTICON_PERFORMANCEFUNCTION_H

/**
 * An Interface for a function evaluating the performance of a simulation run.
 */
class PerformanceFunction {
    public:
        /**
         * Evaluates the last simulation run.
         * @return A double value representing the error (the lower the better).
         */
        virtual double evaluateOutput() = 0;

};


#endif //SIMOPTICON_PERFORMANCEFUNCTION_H
