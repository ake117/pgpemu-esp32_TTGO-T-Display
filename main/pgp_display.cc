/*
 * Copyright 2020 James Hawtin oolon@ankh.org
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "sdkconfig.h"
#ifdef CONFIG_TFT_PREDEFINED_DISPLAY_TYPE
#include "tftspi.h"
#include "tft.h"

#include "pgp_display.h"
#include "pgp_images.h"
#include "pgp_utils.h"

#define SPI_BUS TFT_HSPI_HOST

void pgp_display_init(void)
{
  esp_err_t ret;
  spi_lobo_device_handle_t spi;

  tft_max_rdclock = 8000000;
  TFT_PinsInit();

  spi_lobo_bus_config_t buscfg;
  zero((uint8_t *)&buscfg, sizeof(spi_lobo_bus_config_t));
  buscfg.miso_io_num=PIN_NUM_MISO;                              // set SPI MISO pin
  buscfg.mosi_io_num=PIN_NUM_MOSI;                              // set SPI MOSI pin
  buscfg.sclk_io_num=PIN_NUM_CLK;                               // set SPI CLK pin
  buscfg.quadwp_io_num=-1;
  buscfg.quadhd_io_num=-1;
  buscfg.max_transfer_sz = 6*1024;
  
  spi_lobo_device_interface_config_t devcfg;
  zero((uint8_t *)&devcfg,sizeof(spi_lobo_device_interface_config_t));
  devcfg.clock_speed_hz=8000000;                // Initial clock out at 8 MHz
  devcfg.mode=0;                                // SPI mode 0
  devcfg.spics_io_num=-1;                       // we will use external CS pin
  devcfg.spics_ext_io_num=PIN_NUM_CS;           // external CS pin
  devcfg.flags=LB_SPI_DEVICE_HALFDUPLEX;        // ALWAYS SET  to HALF DUPLEX MODE!! for display spi
  
  vTaskDelay(500 / portTICK_RATE_MS);
  ret=spi_lobo_bus_add_device(SPI_BUS, &buscfg, &devcfg, &spi);
  assert(ret==ESP_OK);
  printf("SPI: display device added to spi bus (%d)\r\n", SPI_BUS);
  tft_disp_spi = spi;
  TFT_display_init();
  tft_gray_scale = 0;
  TFT_setRotation(3);
  TFT_setFont(DEJAVU18_FONT, NULL);
  
#ifdef TFT_START_COLORS_INVERTED
    TFT_invertDisplay(1);
#endif

	// Display splash screen
    TFT_jpg_image(CENTER, CENTER, 0, NULL, PGPEMU_SPLASH, PGPEMU_SPLASH_LEN);
	tft_fg = TFT_ORANGE;
	//
	// TFT_print("ESP32", 8, 47);
	vTaskDelay(2500 / portTICK_RATE_MS);
	TFT_fillWindow(TFT_BLACK);

}
#else

void pgp_display_init(void)
{

}

#endif
