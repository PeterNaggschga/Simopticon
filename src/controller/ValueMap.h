#ifndef SIMOPTICON_VALUEMAP_H
#define SIMOPTICON_VALUEMAP_H

/**
 * @file
 * In this file, the header of the ValueMap class is defined.
 */

#include "../Types.h"
#include "../ComparisonFunctions.h"
#include "../parameters/Parameter.h"

#include <map>
#include <vector>
#include <list>
#include <memory>
#include <set>
#include <mutex>

/**
 * A container managing a map data structure that maps Parameter combinations to their respective found values.
 * The class manages concurrent access using the #operationsLock.
 * Running median calculation is supported by using sets #upperValues and #lowerValues.
 * Values are inserted into the data structure at once when #updateMap is called.
 * Before that they are saved in #tba to avoid unnecessary costly insertion operations.
 * @ingroup controller
 */
class ValueMap {
private:
    /**
     * Threadlock to avoid damage to the data structure when concurrent threads access it.
     */
    mutex operationsLock;

    /**
     * Greater half of the values in #values. Same size as or one element more than #lowerValues.
     */
    set<functionValue *, CmpPtrFunctionvalue> upperValues;
    /**
     * Lesser half of the values in #values. Same size as or one element less than #upperValues.
     */
    set<functionValue *, CmpPtrFunctionvalue> lowerValues;

    /**
     * Number of entries to be printed as best values at the end of the optimization process.
     * Can be configured in main config.
     */
    const unsigned int topEntries;
    /**
     * Set of pairs of the best Parameter combinations and their respective values.
     * Contains not more than #topEntries entries.
     */
    set<pair<const vector<shared_ptr<Parameter>>, functionValue>, CmpPairVectorSharedParameterFunctionvalue> topVals;

    /**
     * Actual map that contains Parameter combinations and their respective values.
     */
    map<vector<shared_ptr<Parameter>>, functionValue, CmpVectorSharedParameter> values;

    /**
     * Entries that have been added since last #updateMap.
     * Will be inserted into #values, #upperValues and #lowerValues when #updateMap is called.
     */
    list<pair<vector<shared_ptr<Parameter>>, functionValue>> tba;

    /**
     * Takes all values in #tba, adds them to #lowerValues or #upperValues and inserts them into #values.
     * #lowerValues and #upperValues are sorted as is required by their constraints.
     * Afterwards #tba is cleared.
     */
    void updateMap();

    /**
     * Inserts a single value into #values and into #lowerValues or #upperValues depending on @a set argument.
     * @param val: Parameter combination and respective value to be inserted. 
     * @param set: Set that value is inserted in. Either #lowerValues or #upperValues.
     */
    void addValue(const pair<vector<shared_ptr<Parameter>>, functionValue> &val,
                  set<functionValue *, CmpPtrFunctionvalue> &set);

public:
    /**
     * Creates a ValueMap.
     * @param topEntries: Value to be assigned to #topEntries.
     */
    explicit ValueMap(unsigned int topEntries = 10);

    /**
     * Returns the value saved at the given Parameter combination. If no value is present, an exception is thrown.
     * Triggers #updateMap.
     * @param params: Parameter combination to which the value is requested. 
     * @return The value saved in #values at the given Parameter combination.
     */
    [[nodiscard]] functionValue query(const vector<shared_ptr<Parameter>> &params);

    /**
     * Adds the given Parameter combination and value to #tba.
     * @param params: Parameter combination to be added.
     * @param val: Value to be added.
     */
    void insert(const vector<shared_ptr<Parameter>> &params, functionValue val);

    /**
     * Checks if a value has been recorded at the given Parameter combination.
     * Triggers #updateMap.
     * @param cords: Parameter combination that is checked. 
     * @return A boolean value that represents if the value is known.
     */
    [[nodiscard]] bool isKnown(const vector<shared_ptr<Parameter>> &cords);

    /**
     * Checks if the given Parameter combination is to be found in #topVals.
     * Triggers #updateMap.
     * @param cords: Parameter combination that is checked. 
     * @return A boolean value that represents if the value is one of the best #topEntries entries in #values.
     */
    [[nodiscard]] bool isTopValue(const vector<shared_ptr<Parameter>> &cords);

    /**
     * Returns the whole #values member.
     * Triggers #updateMap.
     * @return A map reference to #values.
     */
    const map<vector<shared_ptr<Parameter>>, functionValue, CmpVectorSharedParameter> &getValues();

    /**
     * Returns the median of all values using #lowerValues and #upperValues.
     * If no values have been added, 0 is returned.
     * Triggers #updateMap.
     * @return A value representing the median of all values.
     */
    [[nodiscard]] functionValue getMedian();

    /**
     * Returns the number of inserted values. Values in #tba are included.
     * @return An integral representing the number of inserted values.
     */
    [[nodiscard]] size_t getSize() const;

    /**
     * Returns the best #topEntries entries that are saved in #topVals.
     * Triggers #updateMap.
     * @return A list of the best #topEntries Parameter combinations and their respective values.
     */
    [[nodiscard]] list<pair<vector<shared_ptr<Parameter>>, functionValue>> getTopVals();
};


#endif //SIMOPTICON_VALUEMAP_H
