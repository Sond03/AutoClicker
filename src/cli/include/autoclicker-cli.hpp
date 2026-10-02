#pragma once
#include <string>

struct libevdev_uinput *create_virtual_mouse(void);
int autoclick_cli(double cps, int seconds_to_start, int run_seconds, std::string click);

:
