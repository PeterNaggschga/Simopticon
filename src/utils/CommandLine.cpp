#include "CommandLine.h"

#include <array>
#include <memory>

unique_ptr<string> CommandLine::exec(string cmd) {
    array<char, 128> buffer{};
    cmd += " 2>&1";
    unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("Pipeline execution failed!");
    }
    string result;
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return std::make_unique<string>(result);
}
