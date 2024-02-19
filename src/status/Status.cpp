/**
 * @file
 * In this file, the implementation of the Status class is defined.
 * @author Per Natzschka
 */

#include "Status.h"


const std::string Status::NO_STATUS_SUPPORT = "Component doesn't support status updates!";

const std::string Status::NO_NAME = "No name specified";

std::string Status::getStatus() {
    return NO_STATUS_SUPPORT;
}

std::string Status::getStatusBar() {
    return NO_STATUS_SUPPORT;
}

std::string Status::getName() {
    return NO_NAME;
}
