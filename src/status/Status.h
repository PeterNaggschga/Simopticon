#ifndef SIMOPTICON_STATUS_H
#define SIMOPTICON_STATUS_H

#include <string>

using namespace std;

/**
 * This package provides functionality for command line output to keep the user updated about the optimization state and progress.
 * @defgroup status
 */

/**
 * An interface defining functions for status updates on configuration and progress of a class.
 * Used for creation of a StatusBar. Overriding the defined methods is not mandatory but recommended.
 * @ingroup status
 */
class Status {
protected:
    /**
     * Default message returned by Status::getStatus and Status::getStatusBar if the implementing class does not override the respective function.
     */
    const static string NO_STATUS_SUPPORT;
    /**
     * Default message returned by Status::getName if the implementing class does not override the function.
     */
    const static string NO_NAME;

public:
    /**
     * Returns a string representing the name of the implementing component in natural language.
     * @return A string containing the name of the component.
     */
    [[nodiscard]] virtual string getName();

    /**
     * Returns a string representing the current state of the implementing component.
     * May contain values of class members or other meaningful information. The returned string is always visible in StatusBar.
     * @return A string containing the state of the component.
     */
    [[nodiscard]] virtual string getStatus();

    /**
     * Returns a string representing the current progress of the calculations of the implementing component.
     * The returned string is visible in StatusBar, when the component is actively calculating something.
     * Must not exceed one console line!
     * @return A string containing the progress of a calculation.
     */
    [[nodiscard]] virtual string getStatusBar();
};

#endif //SIMOPTICON_STATUS_H
