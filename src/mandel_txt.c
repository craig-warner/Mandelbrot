/*
 * Mandelbrot with Text
 *
 * Typical View:
 *  X=-1 -> +2
 *  Y= -1.5->+1.5
 *
 * f(x)=x^2-u
 *   Where x is a complex number and so is u.
 *
 *    u is the current position on the grid
 *
 *   color is determined by how fast the function
 *   a converges.  If the function converges, paint
 *   black otherwise assign a color.
 *
 *   How many iterations should be attempted?
 *
 */
#include "mandel_txt.h"

/*
 * Functions
 */
void color_image();
void print_image();
void print_image_line();
unsigned get_color ();
void initialize_image ();


void color_image(struct_txt_image* image) {
double c, di;
double increment;
unsigned ix,iy;

  increment = ((double) image->length /
      ((double)image->pixels_per_side));
  for (iy=0; iy<image->pixels_per_side; iy++) {
    for(ix=0;ix<image->pixels_per_side;ix++) {
      if(ix == 0) {
        c=image->min_x;
      }
      else {
        c=c + increment;
      }
      if((ix == 0) && (iy == 0)) {
        di=image->min_y;
      }
      else if (ix == 0) {
        di=di + increment;
      }

      image->image[ix][iy] =
        get_color (c,di,image->num_iterations);
    }
  }
}

void print_image (struct_txt_image* image)
{
unsigned itx,ity;
unsigned iy;

  for (iy=0; iy<image->pixels_per_side; iy++) {
    print_image_line(image,iy);
  }
}

void print_image_line (struct_txt_image* image,unsigned iy)
{
char text_color[8] = {'.','-','=','!','^','(','@','%'};
unsigned ix;
unsigned dot;

  for (ix=0; ix<image->pixels_per_side; ix++) {
    dot=image->image[ix][iy];
    dot=(dot % 8);
    printf ("%c",text_color[dot]);
  }
  printf("\n");
}

unsigned get_color (double c, double di,unsigned num_iterations) {

unsigned i;
double a, bi;
double new_a, new_bi;
double threshold;

  threshold=1000.0;
  /* printf("c:%5.3f di:%5.3f\n",c,di); */
  for (i=0;i<num_iterations; i++) {
    if(i == 0) {
      a=c;
      bi=di;
    }
    else {
       // (a+bi)(a+bi)-(c+di) = a^2 - b^2 -c + 2*a*bi -di
      new_a = (a*a - bi*bi - c);
      new_bi = (2*a*bi-di);
      a=new_a;
      bi=new_bi;
    }
    /* printf("%d: %5.3f\n",i,a); */
    if(a>threshold) {
      return(i);
    }
  }
}

void initialize_image (struct_txt_image *image)
{
  image->pixels_per_side = MAX_PIXELS;
  image->min_x = -1;
  image->min_y = -1.5;
  image->length = 3;
  image->bits_per_color = 4;
  image->num_iterations = (1<<(image->bits_per_color*3));
}

main() {

  struct_txt_image img;
  initialize_image (&img);
  color_image (&img);
  print_image (&img);
}
