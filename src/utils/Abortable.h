#ifndef SIMOPTICON_ABORTABLE_H
#define SIMOPTICON_ABORTABLE_H


/**
 * A simple interface for classes that encapsulate abortable processes.
 * @ingroup utils
 */
class Abortable {
protected:
    /**
     * Defines if the process has been aborted, i.e. #abort has been called.
     */
    bool aborted = false;

public:
    /**
     * Sets #aborted to @a true.
     */
    virtual void abort();

};


#endif //SIMOPTICON_ABORTABLE_H
