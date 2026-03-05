#include "app.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main()
{
    app_init();

    while (1)
    {
        app_loop();
    }
}