#ifndef __ultrasonic_H__
#define __ultrasonic_H__

#include "driver.h"
#include "device.h"
#include <driver/gpio.h>
#include <sys/time.h>

typedef void(*Ultrasonic_callback)();

#define ultrasonic_POLLING_MS         1000

#define ESP_ERR_ULTRASONIC_PING         0x200
#define ESP_ERR_ULTRASONIC_PING_TIMEOUT 0x201
#define ESP_ERR_ULTRASONIC_ECHO_TIMEOUT 0x202

#define TRIGGER_LOW_DELAY 4
#define TRIGGER_HIGH_DELAY 10
#define PING_TIMEOUT 6000
#define ROUNDTRIP 58

#define MAX_DISTANCE_CM 500 // 5m max
#define TRIGGER_GPIO GPIO_NUM_18
#define ECHO_GPIO GPIO_NUM_19

static inline uint32_t get_time_us()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_usec;
}

#define timeout_expired(start, len) ((uint32_t)(get_time_us() - (start)) >= (len))

#define RETURN_CRTCAL(MUX, RES) do { portEXIT_CRITICAL(&MUX); return RES; } while(0)

static portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

typedef struct
{
    gpio_num_t trigger_pin;
    gpio_num_t echo_pin;
} ultrasonic_sensor_t;

class ultrasonic : public Device {
    private:

    public:
        // constructor
        ultrasonic();
        // override
        void init(void);
        void process(Driver *drv);
        int prop_count(void);
        bool prop_name(int index, char *name);
        bool prop_unit(int index, char *unit);
        bool prop_attr(int index, char *attr);
        bool prop_read(int index, char *value);
        bool prop_write(int index, char *value);
        // method
        char* random();
        void callback(Ultrasonic_callback statement);
        void didEnter(Ultrasonic_callback statement);
        int distance(void);
};

#endif
