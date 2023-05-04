#include <array>
#include <memory>
#include "CommandLine.h"

const string CommandLine::SOURCE = "/home/petern/programming/simopticon/";    //TODO: dynamisch machen

string CommandLine::exec(const char *cmd) {
    array<char, 128> buffer{};
    string result;
    unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("Pipeline execution failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}
