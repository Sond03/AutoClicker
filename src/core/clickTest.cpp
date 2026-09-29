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
#include <thread>

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

int autoclick_cli(double cps, int seconds_to_play, int how_long_to_play){
    const double second = 1000;
    cps = second/cps;
    std::cout << "MS:" << cps << std::endl;
    return 0;
}

int autoclicker_base(){
    int milliseconds_pause = 50;
    std::cout << "how many ms do you want the clicks to be inbetween eachother\n(it will play in 5s)" << std::endl;
    std::cin >> milliseconds_pause;

    std::this_thread::sleep_for(std::chrono::seconds(5));
    struct libevdev_uinput *mouse = create_virtual_mouse();

    auto now = std::chrono::steady_clock::now;
    using namespace std::chrono_literals;
    auto work_duration = 1s; // TODO: add a specification on how long it will repeat
    auto start = now();
    while ( (now() - start) < work_duration) {
        // std::this_thread::sleep_for(std::chrono::milliseconds(500));

        libevdev_uinput_write_event(mouse, EV_KEY, BTN_RIGHT, 1);
        libevdev_uinput_write_event(mouse, EV_SYN, SYN_REPORT, 0);

        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds_pause));

        libevdev_uinput_write_event(mouse, EV_KEY, BTN_RIGHT, 0);
        libevdev_uinput_write_event(mouse, EV_SYN, SYN_REPORT, 0);
    };

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    libevdev_uinput_destroy(mouse);
    return 0;
}


int main(){
    autoclick_cli(22.2, 1, 1);

    return 0;
}


