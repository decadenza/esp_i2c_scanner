#include <driver/i2c.h>
#include "esp_log.h"

// SECTION I2C settings.
#define I2C_SDA         4
#define I2C_SCL         16
#define I2C_FREQ_HZ     100000UL
#define I2C_TIMEOUT_MS  100
#define I2C_PORT_NUM    0
// !SECTION

static const char* TAG = "main";

void app_main(void)
{   
    esp_err_t err = ESP_OK;

    // ANCHOR Configuration.
    i2c_config_t i2c_conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_SDA,
        .scl_io_num = I2C_SCL,
        .sda_pullup_en = GPIO_PULLUP_DISABLE, // Internal pullups are around 50k.
        .scl_pullup_en = GPIO_PULLUP_DISABLE, // Internal pullups are around 50k.
        .master.clk_speed = I2C_FREQ_HZ,
        .clk_flags = I2C_SCLK_SRC_FLAG_FOR_NOMAL,
    };

    err = i2c_param_config(0, &i2c_conf);
    if(err != ESP_OK) {
        ESP_LOGE(TAG, "Failed i2c configuration: %s", esp_err_to_name(err));
        abort();
        };
        
    err = i2c_driver_install(0, i2c_conf.mode, 0, 0, 0);
    if(err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to install i2c driver: %s", esp_err_to_name(err));
        abort();
    };

    // ANCHOR Iterative device search.
    for (uint8_t addr = 1; addr < 127; addr++) {

        err = ESP_OK;

        ESP_LOGD(TAG, "Scanning address 0x%2x...", (unsigned int) addr);
        
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        if (!cmd) err = ESP_FAIL;

        err = i2c_master_start(cmd);
        if (err != ESP_OK) goto end;

        // first, send device address (indicating write) & register to be read
        err = i2c_master_write_byte(cmd, ( addr << 1 ), true);
        if (err != ESP_OK) goto end;

        // No need to send any payload.

        err = i2c_master_stop(cmd);
        if (err != ESP_OK) goto end;

        err = i2c_master_cmd_begin(I2C_PORT_NUM, cmd, pdMS_TO_TICKS(I2C_TIMEOUT_MS));
        
    end:
        if (cmd) i2c_cmd_link_delete(cmd);
        if(err == ESP_OK) {
            ESP_LOGI(TAG, "\tFound device at address 0x%2x\n", (unsigned int) addr);
        }
    }

}
