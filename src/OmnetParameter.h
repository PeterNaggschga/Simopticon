#ifndef SIMOPTICON_OMNETPARAMETER_H
#define SIMOPTICON_OMNETPARAMETER_H


#include "Parameter.h"

/**
 * A parameter for the OMNeT++ environment.
 */
class OMNeTParameter : public Parameter {
    private:
        string definition;

    public:
        /**
         * Creates an OMNeTParameter.
         * @param min Minimum Value of the parameter.
         * @param max Maximum Value of the parameter.
         * @param val Current Value of the parameter.
         * @param definition Format of the parameter definition in .ini file
         * @param unit Unit of the parameter.
         */
        OMNeTParameter(double min, double max, double val, string definition, string unit="");

        /**
         * Creates an OMNeTParameter.
         * @param min Minimum Value of the parameter.
         * @param max Maximum Value of the parameter.
         * @param definition Format of the parameter definition in .ini file
         * @param unit Unit of the parameter.
         */
        OMNeTParameter(double min, double max, string definition, string unit="");

        const string &getDefinition() const;

        void setDefinition(const string &definition);
};


#endif //SIMOPTICON_OMNETPARAMETER_H
