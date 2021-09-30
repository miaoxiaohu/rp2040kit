#ifndef __INCLUDES_H
#define __INCLUDES_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "pico/stdio_usb.h"
#include "hardware/watchdog.h"
#include "hardware/flash.h"


// Drivers
#include "led.h"
#include "dut.h"
#include "icp.h"
#include "program.h"
#include "tmp117.h"

// Task
#include "task_usb.h"


#endif  // __INCLUDES_H
