/* 
 * Mandelbrot Include File 
 *
 */

/*
 * Defines
 */
#define MAX_PIXELS 4096 
#define MAX_TILES 16 

/*
 * Structures
 */

struct struct_tile_def {
	unsigned pixels_per_tile_side;
	double min_x;
	double min_y;
	double length;
	unsigned num_iterations;
	unsigned tile_image[MAX_PIXELS][MAX_PIXELS];	
};
#define struct_tile struct struct_tile_def 

struct struct_image_def {
	unsigned pixels_per_image_side;
	unsigned tiles_per_side;
	unsigned log2_tiles_per_side;
	double min_x;
	double min_y;
	double length;
	unsigned num_iterations;
	unsigned bits_per_color;
	struct_tile* tiles;	
};
#define struct_image struct struct_image_def 

