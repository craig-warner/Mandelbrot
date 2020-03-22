/* 
 * Create Mandelbrot Movie 
 */
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
 * Functions
 */

main()
{
struct_image image;
CMovie_Trajectory trajectory;

	initialize_trajectory (&trajectory);
	initialize_image(&image);
	system("mkdir frames");
	create_frames("frames",&image,&trajectory);
}

int initialize_image (struct_image *image) 
{
	image->pixels_per_image_side = 1000; 
	image->tiles_per_side = 4;
	image->log2_tiles_per_side = 2;
	image->min_x = -1;
	image->min_y = -1.5;
	image->length = 3; 
	image->bits_per_color = 5;
	image->num_iterations = (1<<(image->bits_per_color*3));

	create_tiles(image);
}

int initialize_trajectory (CMovie_Trajectory *t)
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

	t->num_frames = 300;
	t->starting_frame = 0;
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
	printf("Info:Skipped Frames\n");		
	for(iFrame=iStartFrame;iFrame<iNum_frames;iFrame++) {
		if(iFrame != iStartFrame) {
			adjust_image(image,ptTraj);
		}
		color_image(image);
		sprintf(file_name,"%s/m%05d.bmp",
				pch_directory,iFrame);
		create_bmp(image,file_name);
		printf("Frame %d Created\n",iFrame);		
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
	
