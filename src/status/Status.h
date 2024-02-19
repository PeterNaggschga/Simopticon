#ifndef SIMOPTICON_STATUS_H
#define SIMOPTICON_STATUS_H

/**
 * @dir
 * In this directory, classes of the status module are defined and implemented.
 */

/**
 * @file
 * In this file, the header of the Status class is defined.
 */

#include <string>

/**
 * This module provides functionality for command line output to keep the user updated about the optimization state and progress.
 * @defgroup status status
 */

/**
 * An interface defining functions for status updates on configuration and progress of a class.
 * Used for creation of a StatusBar. Overriding the defined methods is not mandatory but recommended.
 * @ingroup status
 * @author Per Natzschka
 */
class Status {
protected:
    /**
     * Default message returned by #getStatus and #getStatusBar if the implementing class does not override the respective function.
     */
    const static std::string NO_STATUS_SUPPORT;
    /**
     * Default message returned by #getName if the implementing class does not override the function.
     */
    const static std::string NO_NAME;

public:
    /**
     * Returns a string representing the name of the implementing component in natural language.
     * @return A string containing the name of the component.
     */
    [[nodiscard]] virtual std::string getName();

    /**
     * Returns a string representing the current state of the implementing component.
     * May contain values of class members or other meaningful information. The returned string is always visible in StatusBar.
     * @return A string containing the state of the component.
     */
    [[nodiscard]] virtual std::string getStatus();

    /**
     * Returns a string representing the current progress of the calculations of the implementing component.
     * The returned string is visible in StatusBar, when the component is actively calculating something.
     * Must not exceed one console line!
     * @return A string containing the progress of a calculation.
     */
    [[nodiscard]] virtual std::string getStatusBar();
};

#endif //SIMOPTICON_STATUS_H
