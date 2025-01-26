/* 
 * Create Mandelbrot Movie 
 */
#include <math.h>
#include <stdio.h>
#include <argp.h>
#include "movie.h"
#include "mandel.h"
/*
 * Functions
 */
void create_tiles();
void color_image();
int create_bmp();

int initialize_image (); 
int initialize_trajectory ();
int create_frames();
int adjust_image();
int adjust_tiles();
int adjust_range();

/*
 * Mandelbrot <frame> <log2_tiles_per_side> <pixels_per_image_side>
 * %Mandelbrot <frame> <log2_tiles_per_side> <pixels_per_image_side> [--text]
 */

const char *argp_program_version = "Mandelbrot 1.0";
const char *argp_program_bug_address= "cgwarner2014@gmail.com";
struct arguments
{
  unsigned int frame; 					// Frame number ARG1
  unsigned int log2_tiles_per_side; 	// Log2 of tiles per side ARG2
  unsigned int pixels_per_image_side;	// Pixels per image side ARG3
  int text; 							// Text output -text
  int verbose; 							// Verbose output -v
};

/*
   OPTIONS.  Field 1 in ARGP.
   Order of fields: {NAME, KEY, ARG, FLAGS, DOC}.
*/
static struct argp_option options[] =
{
  {"verbose", 'v', 0, 0, "Produce verbose output"},
  {"text", 't', 0, 0, "Text output"},
  {"frame",   'f', 0, 0, "Frame Number"},
  {"log2_tiles_per_side",   'l', 0, 0, "Log2 of Tiles Per Side"},
  {"pixels_per_image_side",   'p', 0, 0, "Pixels Per Image Side"},
  {0}
};

static error_t parse_opt ( int key, char *arg, struct argp_state *state) {
  struct arguments *arguments = state->input;
  switch (key) {
	case 'v':
	  arguments->verbose=1; // Verbose output
	  break;
	case 't':
	  arguments->text=1; // Text output
	  break;
	case ARGP_KEY_ARG:
	  if (state->arg_num >= 3) {
		argp_usage(state);
	  }
	  switch (state->arg_num) {
		case 0:
		  arguments->frame = atoi(arg);
		  break;
		case 1:
		  arguments->log2_tiles_per_side = atoi(arg);
		  break;
		case 2:
		  arguments->pixels_per_image_side = atoi(arg);
		  break;
	  }
	  break;
	case ARGP_KEY_END:
	  if (state->arg_num < 3) {
		argp_usage(state);
	  }
	  break;
	default:
	  return ARGP_ERR_UNKNOWN;
  }
  return 0;
}


/*
   ARGS_DOC. Field 3 in ARGP.
   A description of the non-option command-line arguments
     that we accept.
*/
static char args_doc[] = "<frame number> <log2_tiles_per_side> <pixels_per_image_side> [-t] [-v]";

/*
  DOC.  Field 4 in ARGP.
  Program documentation.
*/
static char doc[] = "Mandelbrot -- A program to draw Mandelbrot images.";
/*
   The ARGP structure itself.
*/
static struct argp argp = {options, parse_opt, args_doc, doc};

/*
   The main function.
   Notice how now the only function call needed to process
   all command-line options and arguments nicely
   is argp_parse.
*/

main(int argc,char *argv[])	{
struct_image image;
CMovie_Trajectory trajectory;
struct arguments arguments;

/* Argp Default values*/
	arguments.frame = 0;
	arguments.log2_tiles_per_side = 2;
	arguments.pixels_per_image_side = 128;
	arguments.text = 0;
	arguments.verbose= 0;

 	/* Where the argument parsing magic happens */
  	argp_parse (&argp, argc, argv, 0, 0, &arguments);

	if (arguments.verbose) {
		printf ("Frame Number = %d\n", arguments.frame);
		printf ("Log2 of Tiles Per Side = %d\n", arguments.log2_tiles_per_side);
		printf ("Pixels Per Image Side = %d\n", arguments.pixels_per_image_side);
		printf ("Text output = %s\n", arguments.text ? "yes" : "no");
	}

	initialize_trajectory (&trajectory,arguments.frame);
	initialize_image(&image,arguments.log2_tiles_per_side,arguments.pixels_per_image_side);
	//system("mkdir frames");
	create_frames("frames",&image,&trajectory);
}

int initialize_image (struct_image *image,unsigned int log2_tiles_per_side, unsigned int pps) 
{
	image->pixels_per_image_side = pps; 
	image->tiles_per_side = (1<<log2_tiles_per_side);
	image->log2_tiles_per_side = log2_tiles_per_side;
	image->min_x = -1;
	image->min_y = -1.5;
	image->length = 3; 
	image->bits_per_color = 5;
	image->num_iterations = (1<<(image->bits_per_color*3));

	create_tiles(image);
}

int initialize_trajectory (CMovie_Trajectory *t,unsigned int starting_frame)
{
	t->deltax[0]= 0.01;
	t->deltay[0]= 0.02;
	t->dir_duration[0]=100;
	t->shrink[0] = 0.01;

	t->deltax[1]= 0.02;
	t->deltay[1]= -0.01;
	t->dir_duration[1]=60;
	t->shrink[1] = 0.01;

	t->deltax[2]=0.00;
	t->deltay[2]=-0.001;
	t->dir_duration[2]=40;
	t->shrink[2] = 0.04;

	t->deltax[3]=0.0001;
	t->deltay[3]=-0.000;
	t->dir_duration[3]=12;
	t->shrink[3] = 0.10;

	t->deltax[4]=0.0004;
	t->deltay[4]=0.000;
	t->dir_duration[4]=12;
	t->shrink[4] = 0.10;

	t->deltax[5]=0.0002;
	t->deltay[5]=0.000;
	t->dir_duration[5]=6;
	t->shrink[5] = 0.10;

	t->deltax[6]=0.00001;
	t->deltay[6]=0.00001;
	t->dir_duration[6]=70;
	t->shrink[6] = 0.10;
	
	t->index= 0;
	t->cur_duration = 0;

	t->num_frames = 1;
	t->starting_frame = starting_frame;
}

int create_frames(char *pch_directory, struct_image *image, 
		CMovie_Trajectory *ptTraj)
{
char file_name[80];
unsigned int iFrame;
unsigned int iNum_frames;
unsigned int iStartFrame;

	iNum_frames=ptTraj->num_frames;
	iStartFrame=ptTraj->starting_frame; 
	for(iFrame=0;iFrame<iStartFrame;iFrame++) {
		adjust_image(image,ptTraj);
	}
	//printf("Info:Skipped Frames\n");		
	for(iFrame=iStartFrame;iFrame<(iStartFrame+iNum_frames);iFrame++) {
		if(iFrame != iStartFrame) {
			adjust_image(image,ptTraj);
		}
		color_image(image);
		sprintf(file_name,"%s/m%05d.bmp",
				pch_directory,iFrame);
		create_bmp(image,file_name);
		//printf("Frame %d Created\n",iFrame);		
	}
}

int adjust_image(struct_image *piImage,CMovie_Trajectory *ptTraj)
{
unsigned int iDir;
double dDeltax;
double dDeltay;
double dShrink;

	/* Get direction and adjust Trajectory */	
	dDeltax= ptTraj->deltax[ptTraj->index];
	dDeltay= ptTraj->deltay[ptTraj->index];
	dShrink= ptTraj->shrink[ptTraj->index];
	ptTraj->cur_duration++;
	if(ptTraj->cur_duration == ptTraj->dir_duration[ptTraj->index]) {
		ptTraj->index++;
		ptTraj->cur_duration=0;
	}
	/* Adjust Image */
	adjust_range(dDeltax,dDeltay,dShrink,
	&(piImage->min_x),&(piImage->min_y),&(piImage->length));
	/* Adjust Each Tile */
	adjust_tiles(piImage);
}

int adjust_tiles(struct_image* piImage)
{
int itx,ity;
struct_tile* pt_cur;
double dtile_length;
double dtile_min_x;
double dtile_min_y;


	dtile_length = piImage->length / 
			((double)piImage->tiles_per_side);

	
	pt_cur = piImage->tiles;
	for(ity=0;ity<piImage->tiles_per_side;ity++) {
		for(itx=0;itx<piImage->tiles_per_side;itx++) {
			if((itx==0)&&(ity==0)) {
				dtile_min_y = piImage->min_y; 
			}
			else if(itx==0) {
				dtile_min_y += dtile_length; 
			}
			if(itx==0) {
				dtile_min_x = piImage->min_x; 
			}
			else {
				dtile_min_x += dtile_length; 
			}
			pt_cur->length = dtile_length;
			pt_cur->min_x= dtile_min_x;
			pt_cur->min_y= dtile_min_y;
			pt_cur++;
		}
	}
}

int adjust_range(double dDeltax,double dDeltay,double dShrink, 
	double *pdMin_x, double *pdMin_y, double *pdLength)
{

	*pdLength = (*pdLength * (1-dShrink));
	*pdMin_x += dDeltax;
	*pdMin_y += dDeltay;
}
	
