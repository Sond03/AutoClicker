#include <chrono>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <libevdev/libevdev.h>
#include <libevdev/libevdev-uinput.h>
#include <fcntl.h>
#include <dirent.h>
#include <linux/input-event-codes.h>
#include <ostream>
#include <print>
#include <string>
#include <thread>

const std::string RED = "\033[31m";
const std::string RESET = "\033[0m";

struct libevdev_uinput *create_virtual_mouse(void){
    struct libevdev *dev = libevdev_new();
    if (!dev) {
        std::cerr << "failed to make virtual mouse on function libevdev_new()" << std::endl;       
        exit(1);
    }

    libevdev_set_name(dev, "virtualMouse");
    libevdev_enable_event_type(dev, EV_KEY);
    libevdev_enable_event_code(dev, EV_KEY, BTN_LEFT, NULL);
    libevdev_enable_event_code(dev, EV_KEY, BTN_RIGHT, NULL);

    struct libevdev_uinput *uinput_dev = NULL;

    int err = libevdev_uinput_create_from_device(dev, LIBEVDEV_UINPUT_OPEN_MANAGED, &uinput_dev);

    libevdev_free(dev);

    if (err < 0) {
        std::cerr << "Error creating uinput device: " << strerror(-err) << std::endl;
        exit(-err);
    }

    return uinput_dev;
}

int autoclick_cli(double cps, int seconds_to_start, int run_seconds, std::string click){
    struct libevdev_uinput *mouse = create_virtual_mouse();

    using namespace std::chrono_literals;
    auto cps_ms = 1000ms/cps;
    std::this_thread::sleep_for(std::chrono::seconds(seconds_to_start));

    unsigned int button_code = BTN_LEFT;
    if (click == "right" || click == "RIGHT" || click == "r") {
        button_code = BTN_RIGHT;
    } else if (click == "left" || click == "LEFT" || click == "l") {
        button_code = BTN_LEFT;
    } else {
        std::cerr << "Unknown click mode '" << click << "', defaulting to left click." << std::endl;
    }

    auto end_time = std::chrono::steady_clock::now() + std::chrono::seconds(run_seconds);
    while ( std::chrono::steady_clock::now() < end_time) {
        libevdev_uinput_write_event(mouse, EV_KEY, button_code, 1);
        libevdev_uinput_write_event(mouse, EV_SYN, SYN_REPORT, 0);

        std::this_thread::sleep_for(cps_ms);

        libevdev_uinput_write_event(mouse, EV_KEY, button_code, 0);
        libevdev_uinput_write_event(mouse, EV_SYN, SYN_REPORT, 0);
    
    }
    libevdev_uinput_destroy(mouse);
    return 0;
}

int main(int argc, char *argv[]){
    if (argc < 5) {
        std::println(std::cerr, "Usage: {} <clicks per second> <start delay(s)> <run time(s)> <left/right>", argv[0]);
        return EXIT_FAILURE;
    }
    double cps = std::stod(argv[1]);
    if (cps == 0) {
        std::println(std::cerr, "{}Error:{} Invalid argument for <clicks per second>. Value must be a positive value above 0.", RED, RESET);
        return EXIT_FAILURE;
    }
    autoclick_cli(cps, std::stoi(argv[2]), std::stoi(argv[3]), argv[4]);
    return EXIT_SUCCESS;
}


