#ifndef SIMOPTICON_CONTINUOUSPARAMETER_H
#define SIMOPTICON_CONTINUOUSPARAMETER_H


#include "Parameter.h"

/**
 * A container representing a parameter with continuous values.
 */
class ContinuousParameter : public Parameter {
    private:
        double val;

    public:
        /**
         * Creates a ContinuousParameter.
         * @param min Minimum value of the parameter.
         * @param max Maximum value of the parameter.
         * @param val Initial value of the parameter.
         * @param unit Unit of the parameter.
         */
        ContinuousParameter(double min, double max, double val, const string& unit="");

        /**
         * Creates a ContinuousParameter.
         * @param min Minimum value of the parameter.
         * @param max Maximum value of the parameter.
         * @param unit Unit of the parameter.
         */
        ContinuousParameter(double min, double max, const string& unit="");

        double getVal() const override;

        void setVal(double val) override;
};


#endif //SIMOPTICON_CONTINUOUSPARAMETER_H
