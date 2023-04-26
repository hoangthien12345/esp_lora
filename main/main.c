

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"

#include "lora.h"
#include "wifi.h"
#include "http_sever_app.h"


uint8_t buf[32];

void task_tx(void *p)
{
   for(;;) {
      vTaskDelay(pdMS_TO_TICKS(5000));
      printf("transmitter\n");
      lora_send_packet((uint8_t*)"Hello", 5);
      printf("packet sent...\n");
   }
}
void task_rx(void *p)
{
   int x;
   for(;;) {
      lora_receive();    // put into receive mode
      while(lora_received()) {
         x = lora_receive_packet(buf, sizeof(buf));
         buf[x] = 0;
         printf("Received: %s\n", buf);
         lora_receive();
      }
      vTaskDelay(1);
   }
}

void app_main()
{
   lora_init();
   lora_set_bandwidth(7);
   lora_set_spreading_factor(7);
   lora_set_frequency(433e6);
   lora_enable_crc();
   lora_set_preamble_length(10);
   xTaskCreate(&task_tx, "task_tx", 2048, NULL, 5, NULL);
   // xTaskCreate(&task_rx, "task_rx", 2048, NULL, 5, NULL);

   //  //Initialize NVS
   //  esp_err_t ret = nvs_flash_init();
   //  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
   //    ESP_ERROR_CHECK(nvs_flash_erase());
   //    ret = nvs_flash_init();
   //  }
   //  ESP_ERROR_CHECK(ret);
   //  wifi_init_sta();
   //  start_webserver();

}