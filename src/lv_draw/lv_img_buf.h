/**
 * @file lv_img_buf.h
 *
 */

#ifndef LV_IMG_BUF_H
#define LV_IMG_BUF_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdbool.h>
#include "../lv_misc/lv_color.h"


/*********************
 *      DEFINES
 *********************/
/*If image pixels contains alpha we need to know how much byte is a pixel*/
#if LV_COLOR_DEPTH == 1 || LV_COLOR_DEPTH == 8
#define LV_IMG_PX_SIZE_ALPHA_BYTE 2
#elif LV_COLOR_DEPTH == 16
#define LV_IMG_PX_SIZE_ALPHA_BYTE 3
#elif LV_COLOR_DEPTH == 32
#define LV_IMG_PX_SIZE_ALPHA_BYTE 4
#endif


#define LV_IMG_BUF_SIZE_TRUE_COLOR(w, h) ((LV_COLOR_SIZE / 8) * w * h)
#define LV_IMG_BUF_SIZE_TRUE_COLOR_CHROMA_KEYED(w, h) ((LV_COLOR_SIZE / 8) * w * h)
#define LV_IMG_BUF_SIZE_TRUE_COLOR_ALPHA(w, h) (LV_IMG_PX_SIZE_ALPHA_BYTE * w * h)

/*+ 1: to be sure no fractional row*/
#define LV_IMG_BUF_SIZE_ALPHA_1BIT(w, h) ((((w / 8) + 1) * h))
#define LV_IMG_BUF_SIZE_ALPHA_2BIT(w, h) ((((w / 4) + 1) * h))
#define LV_IMG_BUF_SIZE_ALPHA_4BIT(w, h) ((((w / 2) + 1) * h))
#define LV_IMG_BUF_SIZE_ALPHA_8BIT(w, h) ((w * h))

/*4 * X: for palette*/
#define LV_IMG_BUF_SIZE_INDEXED_1BIT(w, h) (LV_IMG_BUF_SIZE_ALPHA_1BIT(w, h) + 4 * 2)
#define LV_IMG_BUF_SIZE_INDEXED_2BIT(w, h) (LV_IMG_BUF_SIZE_ALPHA_2BIT(w, h) + 4 * 4)
#define LV_IMG_BUF_SIZE_INDEXED_4BIT(w, h) (LV_IMG_BUF_SIZE_ALPHA_4BIT(w, h) + 4 * 16)
#define LV_IMG_BUF_SIZE_INDEXED_8BIT(w, h) (LV_IMG_BUF_SIZE_ALPHA_8BIT(w, h) + 4 * 256)

/**********************
 *      TYPEDEFS
 **********************/

/*Image color format*/
enum {
    LV_IMG_CF_UNKNOWN = 0,

    LV_IMG_CF_RAW,              /**< Contains the file as it is. Needs custom decoder function*/
    LV_IMG_CF_RAW_ALPHA,        /**< Contains the file as it is. The image has alpha. Needs custom decoder
                                   function*/
    LV_IMG_CF_RAW_CHROMA_KEYED, /**< Contains the file as it is. The image is chroma keyed. Needs
                                   custom decoder function*/

    LV_IMG_CF_TRUE_COLOR,              /**< Color format and depth should match with LV_COLOR settings*/
    LV_IMG_CF_TRUE_COLOR_ALPHA,        /**< Same as `LV_IMG_CF_TRUE_COLOR` but every pixel has an alpha byte*/
    LV_IMG_CF_TRUE_COLOR_CHROMA_KEYED, /**< Same as `LV_IMG_CF_TRUE_COLOR` but LV_COLOR_TRANSP pixels
                                          will be transparent*/

    LV_IMG_CF_INDEXED_1BIT, /**< Can have 2 different colors in a palette (always chroma keyed)*/
    LV_IMG_CF_INDEXED_2BIT, /**< Can have 4 different colors in a palette (always chroma keyed)*/
    LV_IMG_CF_INDEXED_4BIT, /**< Can have 16 different colors in a palette (always chroma keyed)*/
    LV_IMG_CF_INDEXED_8BIT, /**< Can have 256 different colors in a palette (always chroma keyed)*/

    LV_IMG_CF_ALPHA_1BIT, /**< Can have one color and it can be drawn or not*/
    LV_IMG_CF_ALPHA_2BIT, /**< Can have one color but 4 different alpha value*/
    LV_IMG_CF_ALPHA_4BIT, /**< Can have one color but 16 different alpha value*/
    LV_IMG_CF_ALPHA_8BIT, /**< Can have one color but 256 different alpha value*/

    LV_IMG_CF_RESERVED_15,              /**< Reserved for further use. */
    LV_IMG_CF_RESERVED_16,              /**< Reserved for further use. */
    LV_IMG_CF_RESERVED_17,              /**< Reserved for further use. */
    LV_IMG_CF_RESERVED_18,              /**< Reserved for further use. */
    LV_IMG_CF_RESERVED_19,              /**< Reserved for further use. */
    LV_IMG_CF_RESERVED_20,              /**< Reserved for further use. */
    LV_IMG_CF_RESERVED_21,              /**< Reserved for further use. */
    LV_IMG_CF_RESERVED_22,              /**< Reserved for further use. */
    LV_IMG_CF_RESERVED_23,              /**< Reserved for further use. */

    LV_IMG_CF_USER_ENCODED_0,          /**< User holder encoding format. */
    LV_IMG_CF_USER_ENCODED_1,          /**< User holder encoding format. */
    LV_IMG_CF_USER_ENCODED_2,          /**< User holder encoding format. */
    LV_IMG_CF_USER_ENCODED_3,          /**< User holder encoding format. */
    LV_IMG_CF_USER_ENCODED_4,          /**< User holder encoding format. */
    LV_IMG_CF_USER_ENCODED_5,          /**< User holder encoding format. */
    LV_IMG_CF_USER_ENCODED_6,          /**< User holder encoding format. */
    LV_IMG_CF_USER_ENCODED_7,          /**< User holder encoding format. */
};
typedef uint8_t lv_img_cf_t;


/**
 * LittlevGL image header
 */
typedef struct
{

    /* The first 8 bit is very important to distinguish the different source types.
     * For more info see `lv_img_get_src_type()` in lv_img.c */
    uint32_t cf : 5;          /* Color format: See `lv_img_color_format_t`*/
    uint32_t always_zero : 3; /*It the upper bits of the first byte. Always zero to look like a
                                 non-printable character*/

    uint32_t reserved : 2; /*Reserved to be used later*/

    uint32_t w : 11; /*Width of the image map*/
    uint32_t h : 11; /*Height of     the image map*/
} lv_img_header_t;


/** Image header it is compatible with
 * the result from image converter utility*/
typedef struct
{
    lv_img_header_t header;
    uint32_t data_size;
    const uint8_t * data;
} lv_img_dsc_t;


typedef struct {
    lv_color_t res_color;
    lv_opa_t res_opa;

    const void * src;
    lv_coord_t src_w;
    lv_coord_t src_h;
    lv_coord_t pivot_x;
    lv_coord_t pivot_y;
    int32_t pivot_x_256;
    int32_t pivot_y_256;
    lv_img_dsc_t img_dsc;
    int32_t sinma;
    int32_t cosma;
    int16_t angle;
    lv_color_t color;
    lv_img_cf_t cf;

    uint8_t chroma_keyed :1;
    uint8_t has_alpha :1;
    uint8_t native_color :1;

}lv_img_rotate_dsc_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/



/**
 * Get the color of an image's pixel
 * @param dsc an image descriptor
 * @param x x coordinate of the point to get
 * @param y x coordinate of the point to get
 * @param color the color of the image. In case of `LV_IMG_CF_ALPHA_1/2/4/8` this color is used.
 * Not used in other cases.
 * @return color of the point
 */
lv_color_t lv_img_buf_get_px_color(lv_img_dsc_t * dsc, lv_coord_t x, lv_coord_t y, lv_color_t color);


/**
 * Get the alpha value of an image's pixel
 * @param dsc pointer to an image descriptor
 * @param x x coordinate of the point to set
 * @param y x coordinate of the point to set
 * @return alpha value of the point
 */
lv_opa_t lv_img_buf_get_px_alpha(lv_img_dsc_t * dsc, lv_coord_t x, lv_coord_t y);


void lv_img_buf_rotate_init(lv_img_rotate_dsc_t * dsc, int16_t angle, const void * src, lv_coord_t src_w, lv_coord_t src_h,
                        lv_img_cf_t cf, lv_coord_t pivot_x, lv_coord_t pivot_y, lv_color_t color);

bool lv_img_buf_get_px_rotated(lv_img_rotate_dsc_t * dsc, lv_coord_t x, lv_coord_t y);

/**
 * Set the color of a pixel of an image. The alpha channel won't be affected.
 * @param dsc pointer to an image descriptor
 * @param x x coordinate of the point to set
 * @param y x coordinate of the point to set
 * @param c color of the point
 */
void lv_img_buf_set_px_color(lv_img_dsc_t * dsc, lv_coord_t x, lv_coord_t y, lv_color_t c);

/**
 * Set the alpha value of a pixel of an image. The color won't be affected
 * @param dsc pointer to an image descriptor
 * @param x x coordinate of the point to set
 * @param y x coordinate of the point to set
 * @param opa the desired opacity
 */
void lv_img_buf_set_px_alpha(lv_img_dsc_t * dsc, lv_coord_t x, lv_coord_t y, lv_opa_t opa);

/**
 * Set the palette color of an indexed image. Valid only for `LV_IMG_CF_INDEXED1/2/4/8`
 * @param dsc pointer to an image descriptor
 * @param id the palette color to set:
 *   - for `LV_IMG_CF_INDEXED1`: 0..1
 *   - for `LV_IMG_CF_INDEXED2`: 0..3
 *   - for `LV_IMG_CF_INDEXED4`: 0..15
 *   - for `LV_IMG_CF_INDEXED8`: 0..255
 * @param c the color to set
 */
void lv_img_buf_set_palette(lv_img_dsc_t * dsc, uint8_t id, lv_color_t c);

/**
 * Free an allocated image buffer
 * @param dsc image buffer to free
 */
void lv_img_buf_free(lv_img_dsc_t *dsc);

/**
 * Get the memory consumption of a raw bitmap, given color format and dimensions.
 * @param w width
 * @param h height
 * @param cf color format
 * @return size in bytes
 */
uint32_t lv_img_buf_get_img_size(lv_coord_t w, lv_coord_t h, lv_img_cf_t cf);



/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_IMG_BUF_H*/
