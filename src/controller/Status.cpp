#include "Status.h"


const string Status::NO_STATUS_SUPPORT = "Component doesn't support status updates!";

const string Status::NO_NAME = "No name given";

string Status::getStatus() {
    return NO_STATUS_SUPPORT;
}

string Status::getStatusBar() {
    return NO_STATUS_SUPPORT;
}

string Status::getName() {
    return NO_NAME;
}
