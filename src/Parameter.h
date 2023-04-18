#ifndef SIMOPTICON_PARAMETER_H
#define SIMOPTICON_PARAMETER_H

#include <string>
#include <stdexcept>
#include <utility>

using namespace std;

/**
 * A container representing a Parameter.
 */
class Parameter {
    private:
        double min;
        double max;
        double val;
        string unit;

    public:
        /**
         * Creates a Parameter.
         * @param min Minimum Value of the parameter.
         * @param max Maximum Value of the parameter.
         * @param val Current Value of the parameter.
         * @param unit Unit of the parameter.
         */
        Parameter(double min, double max, double val, string unit="");

        /**
         * Creates a Parameter.
         * @param min Minimum Value of the parameter.
         * @param max Maximum Value of the parameter.
         * @param unit Unit of the parameter.
         */
        Parameter(double min, double max, string unit="");

        double getMin() const;

        void setMin(double min);

        double getMax() const;

        void setMax(double max);

        double getVal() const;

        void setVal(double val);

        const string &getUnit() const;

        void setUnit(const string &unit);
};


#endif //SIMOPTICON_PARAMETER_H
