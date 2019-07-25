#include <stdio.h>
#include <string.h>
#include <math.h>
#include "esp_system.h"
#include "kidbright32.h"
#include "driver/uart.h"
#include "soc/uart_struct.h"
#include "ultrasonic.h"

// #include "soc/timer_group_struct.h"
// #include "soc/timer_group_reg.h"

Ultrasonic_callback _callback;
Ultrasonic_callback _didEnter;

bool didEnter_enable = true;

int _previous_distance = -1;
int _distance = -1;

ultrasonic::ultrasonic() {
  polling_ms = ultrasonic_POLLING_MS;
}

void ultrasonic_initial(const ultrasonic_sensor_t *dev)
{
    gpio_set_direction(dev->trigger_pin, GPIO_MODE_OUTPUT);
    gpio_set_direction(dev->echo_pin, GPIO_MODE_INPUT);

    //gpio_set_level(dev->trigger_pin, 0);
};

esp_err_t ultrasonic_measure_cm(const ultrasonic_sensor_t *dev, uint32_t max_distance, uint32_t *distance)
{
    if (!distance)
        return ESP_ERR_INVALID_ARG;

    portENTER_CRITICAL(&mux);

    // Ping: Low for 2..4 us, then high 10 us
    gpio_set_level(dev->trigger_pin, 0);
    ets_delay_us(TRIGGER_LOW_DELAY);
    gpio_set_level(dev->trigger_pin, 1);
    ets_delay_us(TRIGGER_HIGH_DELAY);
    gpio_set_level(dev->trigger_pin, 0);

    // Previous ping isn't ended
    if (gpio_get_level(dev->echo_pin))
        RETURN_CRTCAL(mux, ESP_ERR_ULTRASONIC_PING);

    // Wait for echo
    uint32_t start = get_time_us();
    while (!gpio_get_level(dev->echo_pin))
    {
        if (timeout_expired(start, PING_TIMEOUT))
            RETURN_CRTCAL(mux, ESP_ERR_ULTRASONIC_PING_TIMEOUT);
    }

    // got echo, measuring
    uint32_t echo_start = get_time_us();
    uint32_t time = echo_start;
    uint32_t meas_timeout = echo_start + max_distance * ROUNDTRIP;
    while (gpio_get_level(dev->echo_pin))
    {
        time = get_time_us();
        if (timeout_expired(echo_start, meas_timeout))
            RETURN_CRTCAL(mux, ESP_ERR_ULTRASONIC_ECHO_TIMEOUT);
    }
    portEXIT_CRITICAL(&mux);

    *distance = (time - echo_start) / ROUNDTRIP;

    return ESP_OK;
};

void ultrasonic_start(void *pvParamters)
{

    ultrasonic_sensor_t sensor;
    memset(&sensor, 0, sizeof(sensor));

    sensor.trigger_pin = TRIGGER_GPIO;
    sensor.echo_pin = ECHO_GPIO;

    ultrasonic_initial(&sensor);

    while (true)
    {
        uint32_t distance;
        esp_err_t res = ultrasonic_measure_cm(&sensor, MAX_DISTANCE_CM, &distance);

        // TIMERG0.wdt_wprotect=TIMG_WDT_WKEY_VALUE;
        // TIMERG0.wdt_feed=1;
        // TIMERG0.wdt_wprotect=0;

        if (res != ESP_OK)
        {
            printf("Error: ");
            switch (res)
            {
                case ESP_ERR_ULTRASONIC_PING:
                    printf("Cannot ping (device is in invalid state)\n");
                    break;
                case ESP_ERR_ULTRASONIC_PING_TIMEOUT:
                    printf("Ping timeout (no device found)\n");
                    break;
                case ESP_ERR_ULTRASONIC_ECHO_TIMEOUT:
                    printf("Echo timeout (i.e. distance too big)\n");
                    break;
                default:
                    printf("%d\n", res);
            }
        }
        else
            printf("Distance: %d cm, %.02f m\n", distance, distance / 100.0);
            _distance = distance;

            if (distance < 0) {
                continue;
            }

            if (_callback) {
              _callback();
            }

            if ((distance < 10 && _didEnter) && didEnter_enable ) {
                _didEnter();
                didEnter_enable = false;
            }

            if (distance > 10) {
                didEnter_enable = true;
            }

        vTaskDelay(150 / portTICK_PERIOD_MS);
    }
};

void ultrasonic::init(void) {
    initialized = true;
    xTaskCreate(ultrasonic_start, "ultrasonic_start", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL);
}

int ultrasonic::prop_count(void) {
  // not supported
  return 0;
}

bool ultrasonic::prop_name(int index, char *name) {
  // not supported
  return false;
}

bool ultrasonic::prop_unit(int index, char *unit) {
  // not supported
  return false;
}

bool ultrasonic::prop_attr(int index, char *attr) {
  // not supported
  return false;
}

bool ultrasonic::prop_read(int index, char *value) {
  // not supported
  return false;
}

bool ultrasonic::prop_write(int index, char *value) {
  // not supported
  return false;
}

void ultrasonic::process(Driver *drv) {
//  char buffer[64];
//  sprintf(buffer, "%lldms\n", (esp_timer_get_time() / 1000));
//  uart_write_bytes(UART_NUM_0, (const char*)buffer, strlen(buffer));
}

char* ultrasonic::random() {
  return "nat-random";
}

void ultrasonic::callback(Ultrasonic_callback statement) {
  _callback = statement;
}

void ultrasonic::didEnter(Ultrasonic_callback statement) {
    _didEnter = statement;
}

int ultrasonic::distance(void) {
  return _distance;
}

