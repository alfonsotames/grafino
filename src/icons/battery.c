
/*******************************************************************************
* name: battery
*
* preset name: sumotoy_font
* data block size: 8 bit(s), uint8_t
* RLE compression enabled: no
* conversion type: Monochrome, Edge 144
* bits per pixel: 1
*
* preprocess:
*  main scan direction: top_to_bottom
*  line scan direction: forward
*  inverse: yes
* ------------------------------------------------------------------------------
* Created by a custom template of LCD-Image-Converter for .s.u.m.o.t.o.y.
* Icon template version: 1.31
* Note: Used for B&W images as icons
* ------------------------------------------------------------------------------
*******************************************************************************/
#if !defined(SUMO_ICN_LIB_battery)
#define SUMO_ICN_LIB_battery

#include <stdlib.h>





static const _smCharType image_data_battery[42]
#if defined(_FORCE_PROGMEM__)
 PROGMEM 
#endif
= {0x00, 0x00, 0x00, 0x77, 0xff, 0xff, 0xf7, 0x7f, 0xff, 0xff, 0x77, 0xff, 0xff, 0xf0, 0x7f, 0xff, 0xff, 0x67, 0xff, 0xff, 0xf6, 0x7f, 0xff, 0xff, 0x67, 0xff, 0xff, 0xf6, 0x7f, 0xff, 0xff, 0x07, 0xff, 0xff, 0xf7, 0x7f, 0xff, 0xff, 0x70, 0x00, 0x00, 0x07};

static const tIcon battery = { image_data_battery, 28, 12, 42, RLE_no }; 



#endif



