#ifndef SIMOPTICON_ABORTABLE_H
#define SIMOPTICON_ABORTABLE_H


class Abortable {
protected:
    bool aborted = false;

public:
    virtual void abort();

};


#endif //SIMOPTICON_ABORTABLE_H
