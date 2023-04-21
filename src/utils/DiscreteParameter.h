#ifndef SIMOPTICON_DISCRETEPARAMETER_H
#define SIMOPTICON_DISCRETEPARAMETER_H


#include "Parameter.h"

/**
 * A container representing a parameter with discrete values.
 */
class DiscreteParameter : public Parameter {
    private:
        int times;
        double step;
        double offset = 0;


    public:
        /**
         * Creates a DiscreteParameter. Its value is represented as \p times * \p step + \p offset.
         * @param min Minimum value of the parameter.
         * @param max Maximum value of the parameter.
         * @param times Initial value of \p times.
         * @param step Size of steps.
         * @param offset Initial value of the \p offset.
         * @param unit Unit of the parameter.
         */
        DiscreteParameter(double min, double max, int times, double step, double offset=0, const string& unit="");

        /**
         * Creates a DiscreteParameter.
         * @param min Minimum value of the parameter.
         * @param max Maximum value of the parameter.
         * @param step Size of steps.
         * @param unit Unit of the parameter.
         */
        DiscreteParameter(double min, double max, double step, const string& unit="");

        /**
         * Creates a DiscreteParameter.
         * @param min Minimum value of the parameter.
         * @param max Maximum value of the parameter.
         * @param val Initial value of the parameter.
         * @param step Size of steps.
         * @param unit Unit of the parameter.
         */
        DiscreteParameter(double min, double max, double val, double step, const string& unit="");

        int getTimes() const;

        void setTimes(int times);

        double getStep() const;

        double getOffset() const;

        void setOffset(double offset);

        double getVal() const override;

        void setVal(double val) override;
};


#endif //SIMOPTICON_DISCRETEPARAMETER_H
