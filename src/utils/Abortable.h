#ifndef SIMOPTICON_ABORTABLE_H
#define SIMOPTICON_ABORTABLE_H


/**
 * A simple interface for classes that encapsulate abortable processes.
 * @ingroup utils
 */
class Abortable {
protected:
    /**
     * Defines if the process has been abortet i.e. Abortable::abort has been called.
     */
    bool aborted = false;

public:
    /**
     * Sets Abortable::aborted to @a true.
     */
    virtual void abort();

};


#endif //SIMOPTICON_ABORTABLE_H
