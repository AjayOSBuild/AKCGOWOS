#include <iostream>
#include "package_cli.h"

int main(int argc, char* argv[]) {
    // For standalone testing without real args, simulate a 'list' command
    if (argc < 2) {
        std::cout << "[Test Mode] Simulating 'nexus-pkg list' execution:\n";
        char* mock_argv[] = {argv[0], (char*)"list"};
        PackageCli cli;
        return cli.Execute(2, mock_argv);
    }

    PackageCli cli;
    return cli.Execute(argc, argv);
}
