#pragma once
#include <string>

class Options {
    public:
        Options(int argc, char *argv[]);

        double clicks_per_second = 10.0;
        int start_delay_seconds = 5;
        int duration_seconds = 5;
        std::string click_type = "left";
};

