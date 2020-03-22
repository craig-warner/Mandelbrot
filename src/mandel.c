/* 
 * Mandelbrot with pthreads 
 *
 * Typical View:
 *	X=-1 -> +2
 *	Y= -1.5->+1.5
 *
 * f(x)=x^2-u
 * 	Where x is a complex number and so is u.
 * 
 *  	u is the current position on the grid
 * 	
 * 	color is determined by how fast the function
 * 	a converges.  If the function converges, paint
 * 	black otherwise assign a color.
 * 	
 * 	How many iterations should be attempted?
 * 
 */
#include <pthread.h>
#include "mandel.h" 
/*
 * Defines
 */
#define MAX_PIXELS 1024 
#define MAX_TILES 16 

/* 
 * Functions
 */
void create_tiles();
void color_image();
void color_tile();
void print_image();
void print_tile_line();
unsigned get_color ();
int create_bmp();

/*
 * Functions
 */
void create_tiles(struct_image* image)
{
int itx,ity;
unsigned upixels_per_tile_side;
unsigned num_tiles;
struct_tile* pt_cur;
double dtile_length;
double dtile_min_x;
double dtile_min_y;

	num_tiles = image->tiles_per_side * 
			image->tiles_per_side;
	
	image->tiles = 
	(struct_tile*) malloc(sizeof(struct_tile)*num_tiles);

	upixels_per_tile_side = image->pixels_per_image_side >> 
			image->log2_tiles_per_side;

	dtile_length = image->length / 
			((double)image->tiles_per_side);

	
	pt_cur = image->tiles;
	for(ity=0;ity<image->tiles_per_side;ity++) {
		for(itx=0;itx<image->tiles_per_side;itx++) {
			if((itx==0)&&(ity==0)) {
				dtile_min_y = image->min_y; 
			}
			else if(itx==0) {
				dtile_min_y += dtile_length; 
			}
			if(itx==0) {
				dtile_min_x = image->min_x; 
			}
			else {
				dtile_min_x += dtile_length; 
			}
			pt_cur->length = dtile_length;
			pt_cur->min_x= dtile_min_x;
			pt_cur->min_y= dtile_min_y;
			pt_cur->pixels_per_tile_side =
				upixels_per_tile_side;
			pt_cur->num_iterations =
				image->num_iterations;
			pt_cur++;
		}
	}
}

void color_image(struct_image* image)
{
int ity,itx;
struct_tile* pt_cur;
unsigned ut; 
unsigned unum_threads;

pthread_t *pthe_threads;
pthread_t *pcur_thread; 

	unum_threads = 
		image->tiles_per_side * image->tiles_per_side;

	pthe_threads = (pthread_t *) malloc(unum_threads * 
				sizeof (pthread_t));

	pt_cur = image->tiles;
	pcur_thread = pthe_threads;
	for(ity=0;ity<image->tiles_per_side;ity++) {
		for(itx=0;itx<image->tiles_per_side;itx++) {
			pthread_create(pcur_thread,NULL,
				color_tile, pt_cur);
			pt_cur++;
			pcur_thread++;
		}
	}

	pcur_thread = pthe_threads;
	for(ut=0;ut<unum_threads;ut++) {
		pthread_join (*pcur_thread,NULL);
		pcur_thread++;
	}
}

void color_tile (struct_tile* tile) {
double c, di;
double increment;
unsigned ix,iy;
	
	increment = ((double) tile->length /
			((double)tile->pixels_per_tile_side));
	for (iy=0; iy<tile->pixels_per_tile_side; iy++) {
		for(ix=0;ix<tile->pixels_per_tile_side;ix++) {
			if(ix == 0) {
				c=tile->min_x; 
			}
			else {
				c=c + increment; 
			}
			if((ix == 0) && (iy == 0)) {
				di=tile->min_y; 
			}
			else if (ix == 0) {
				di=di + increment; 
			}

			tile->tile_image[ix][iy] = 
				get_color (c,di,tile->num_iterations); 
		}
	}
	pthread_exit(0);
}  

void print_image (struct_image* image) 
{
unsigned itx,ity;
unsigned iy;
unsigned upixels_per_tile_side;  
struct_tile* pt_cur;

	upixels_per_tile_side = image->tiles->pixels_per_tile_side;
	for(ity=0;ity<image->tiles_per_side;ity++) {
		for (iy=0; iy<upixels_per_tile_side; iy++) {
			pt_cur=((image->tiles) + 
				((image->tiles_per_side)*ity));
			for (itx=0; itx<image->tiles_per_side; itx++) {
				print_tile_line(pt_cur,iy);
				pt_cur++;
			}
			printf("\n");
		}
			
	}
}
			
void print_tile_line (struct_tile* tile,unsigned iy) 
{
char text_color[8] = {'.','-','=','!','^','(','@','%'};
unsigned ix;
unsigned dot;

	for (ix=0; ix<tile->pixels_per_tile_side; ix++) {
		dot=tile->tile_image[ix][iy];	
		dot=(dot % 8);
		printf ("%c",text_color[dot]);
	}
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
