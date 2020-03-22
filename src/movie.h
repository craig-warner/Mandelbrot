/*
 * Mandelbrot Movie Structures 
 *
 * Written by Craig Warner.
 */


#define MAX_VECTORS	100

typedef struct                       /**** structure ****/
    {
    double 	deltax[MAX_VECTORS];  /* Direction of movement */
    double 	deltay[MAX_VECTORS];  /* Direction of movement */
    unsigned int dir_duration[MAX_VECTORS]; /* Frames in direction */
    double 	 shrink[MAX_VECTORS];     /* Pixel shrink between frames */
    unsigned int index;               /* Current direction */
    unsigned int cur_duration;        /* Current duration */
    unsigned int num_frames; 	      /* Number of frames */	
    unsigned int starting_frame;      /* First Frame to Generate*/	
} CMovie_Trajectory;

#define DIR_UP_LEFT 	0
#define DIR_UP_MID 	1
#define DIR_UP_RIGHT 	2
#define DIR_MID_LEFT 	3
#define DIR_MID_MID 	4
#define DIR_MID_RIGHT 	5
#define DIR_DOWN_LEFT 	6
#define DIR_DOWN_MID 	7
#define DIR_DOWN_RIGHT 	8
