/*
 * Mandelbrot Text Include File
 *
 */

/*
 * Defines
 */
//#define MAX_PIXELS 1024
#define MAX_PIXELS 32

/*
 * Structures
 */

struct struct_txt_image_def {
  unsigned pixels_per_side;
  double min_x;
  double min_y;
  double length;
  unsigned num_iterations;
  unsigned bits_per_color;
  unsigned image[MAX_PIXELS][MAX_PIXELS];
};
#define struct_txt_image struct struct_txt_image_def
