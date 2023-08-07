#ifndef SIMOPTICON_PLEXESIMULATIONRUNNER_H
#define SIMOPTICON_PLEXESIMULATIONRUNNER_H

/**
 * @dir
 * In this directory, classes of the plexe module are defined and implemented.
 */

/**
 * @file
 * In this file, the header of the PlexeSimulationRunner class is defined.
 */

#include "../SimulationRunner.h"
#include "ConfigEditor.h"

#include <list>
#include <mutex>

/**
 * This module extends SimulationRunner to interface with the Plexe framework to enable the optimization of platooning controllers.
 * @defgroup plexe plexe
 * @ingroup runner
 */

/**
 * A class capable of starting platooning simulations in the <a href="https://plexe.car2x.org/">Plexe</a> framework with given Parameter combinations.
 * @ingroup plexe
 */
class PlexeSimulationRunner : public SimulationRunner,
                              public Multithreaded<std::pair<std::filesystem::path, std::pair<std::string, unsigned int>>, bool> {
private:
    /**
     * Number of repetitions per Parameter combination and scenario in #SCENARIOS.
     * Translates to repeat setting in `omnetpp.ini`. Can be set in configuration.
     */
    const unsigned int REPEAT;
    /**
     * Scenarios that are simulated per Parameter combination.
     * Should not invoke a GUI (e.g. pick BrakingNoGui instead of Braking). Can be set in configuration.
     */
    const std::vector<std::string> SCENARIOS;

    /**
     * ConfigEditor used for automatically creating `.ini` files with given Parameter settings.
     */
    ConfigEditor editor;

    /**
     * Identifier for each simulated Parameter combination.
     * Is incremented when new Parameter combination is simulated. Used for unique directory names for result files.
     */
    size_t runNumber = 0;
    /**
     * Threadlock to prevent race conditions on concurrent access of #runNumber.
     */
    std::mutex runNumberLock;

    /**
     * Returns an unique number which can be used to identify the results of a certain Parameter combination.
     * Returned value is only unique for one optimization process.
     * Basically increments #runNumber and returns value before incrementation.
     * @return An unique number used for discerning results of different runs.
     */
    size_t getRunId();

    /**
     * Runs simulations for the given Parameter combination.
     * Creates a new `.ini` file for the Parameter combination.
     * Parallelizes the execution of different scenarios (see #SCENARIOS) and their repetitions (see #REPEAT) using Multithreaded class.
     * Parallelized function is defined in #work(std::pair< std::filesystem::path, std::pair< std::basic_string< char >, unsigned int >>).
     * @param run: Parameter combination to be simulated.
     * @return A pair containing the path to the result files and OMNeT++-Run-IDs of the executed simulations.
     */
    std::pair<std::filesystem::path, std::set<runId>> work(parameterCombination run) override;

    /**
     * Executes one run of a Parameter combination (meaning repetition @a k of scenario @a c).
     * Runs command for starting Plexe and returns after execution is done.
     * @param arg: A triple containing the path to the `.ini` defining the parameters, the scenario name and the repetition number.
     * @return A boolean defining whether the execution ran without throwing exceptions.
     */
    bool work(std::pair<std::filesystem::path, std::pair<std::basic_string<char>, unsigned int>> arg) override;

public:
    /**
     * Creates PlexeSimulationRunner which cannot use more than the given number of threads.
     * Number of repetitions, scenarios to be simulated and the ConfigEditor must also be defined.
     * The new PlexeSimulationRunner uses @f$ t = \min(\mathit{threads}, \mathit{repeat} \cdot \mathit{size}(\mathit{scenarios})) @f$ concurrent threads for parallelization of #work(std::pair< std::filesystem::path, std::pair< std::basic_string< char >, unsigned int >>).
     * For the parallelization of #work(parameterCombination) @f$ t' = \lfloor \mathit{threads} \div t \rfloor @f$ concurrent threads are used.
     * @param threads: Maximum number of threads to be used.
     * @param repeat: Number of repetitions per Parameter combination and scenario.
     * @param scenarios: Scenarios to be simulated per Parameter combination.
     * @param editor: ConfigEditor to be used.
     */
    PlexeSimulationRunner(unsigned int threads, unsigned int repeat, std::vector<std::string> scenarios,
                          ConfigEditor editor);

    std::string getName() override;

    std::string getStatus() override;

    std::string getStatusBar() override;

};


#endif //SIMOPTICON_PLEXESIMULATIONRUNNER_H
