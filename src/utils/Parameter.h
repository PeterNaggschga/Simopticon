#ifndef SIMOPTICON_PARAMETER_H
#define SIMOPTICON_PARAMETER_H

#include <string>

using namespace std;

/**
 * A container representing a Parameter.
 */
class Parameter {
    private:
        double min;
        double max;
        string unit;

    public:
        /**
         * Creates a Parameter.
         * @param min Minimum Value of the parameter.
         * @param max Maximum Value of the parameter.
         * @param unit Unit of the parameter.
         */
        Parameter(double min, double max, const string& unit="");

        double getMin() const;

        void setMin(double min);

        double getMax() const;

        void setMax(double max);

        virtual double getVal() const = 0;

        virtual void setVal(double val) = 0;

        const string &getUnit() const;

        void setUnit(const string &unit);
};


#endif //SIMOPTICON_PARAMETER_H
