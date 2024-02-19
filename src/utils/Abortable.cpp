/**
 * @file
 * In this file, the implementation of the Abortable class is defined.
 * @author Per Natzschka
 */

#include "Abortable.h"

void Abortable::abort() {
    aborted = true;
}
