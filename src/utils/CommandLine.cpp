/**
 * @file
 * In this file, the implementation of the CommandLine class is defined.
 */

#include "CommandLine.h"

#include <array>
#include <memory>
#include <stdexcept>

std::unique_ptr<std::string> CommandLine::exec(std::string cmd) {
    std::array<char, 128> buffer{};
    cmd += " 2>&1";
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("Pipeline execution failed!");
    }
    std::string result;
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return std::make_unique<std::string>(result);
}
