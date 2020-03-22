#include "mandel.h"
#include "bmp.h"
#include "assert.h"
 
int create_bmp();
void create_infoheader();
void create_bits();
void create_bits_tile();
RGBQUAD gen_rgb();

int create_bmp (struct_image *image,char *file_name)
{
BITMAPINFO info;
BYTE *bits;
int num_bytes;
int iBytesPerLine;
int iBytesPerLineInFile;

	/* Info header Creation */
	create_infoheader(image,&(info.bmiHeader));
	/* Create bits*/ 
	iBytesPerLine = (image->pixels_per_image_side * 3);
	iBytesPerLineInFile = iBytesPerLine + (iBytesPerLine % 4);	
	num_bytes = iBytesPerLineInFile * image->pixels_per_image_side;
	bits = (unsigned char *) malloc(num_bytes);
	create_bits(bits,image);
	/* Save File */
	SaveDIBitmap (file_name,&info,bits);
}


void create_infoheader(struct_image *image, BITMAPINFOHEADER *infoheader)
{
	
    /* Size of info header */
    infoheader->biSize = 40;
    /* Width of image */
    infoheader->biWidth = (int) image->pixels_per_image_side;
    /* Height of image */
    infoheader->biHeight = (int) image->pixels_per_image_side; 
    /* Number of color planes */
    infoheader->biPlanes = 1;         
    /* Number of bits per pixel */
    infoheader->biBitCount = 24;       
    /* Type of compression to use */
    infoheader->biCompression = BI_RGB;   
    /* Size of image data, 0 is no compression */
    infoheader->biSizeImage = 0;  
    /* X pixels per meter */
    infoheader->biXPelsPerMeter = 0; 
    /* Y pixels per meter */
    infoheader->biYPelsPerMeter = 0;  
    /* Number of colors used */
    infoheader->biClrUsed = 0;        /* No color pallete*/
    /* Number of important colors */
    infoheader->biClrImportant = 0;   /* No color pallete*/
}

void create_bits(BYTE *bits,struct_image *image)
{
int itx,ity;
int ipy;
BYTE *cur_bit;
struct_tile *pt_cur,*pt_line_start;
int bits_per_color;


	bits_per_color = image->bits_per_color;
	cur_bit = bits;
	for(ity=0;ity<image->tiles_per_side;ity++) {
		pt_line_start = (image->tiles + (image->tiles_per_side * ity));
		for(ipy=0;ipy<image->tiles->pixels_per_tile_side;ipy++) {
			for(itx=0;itx<image->tiles_per_side;itx++) {
		  		pt_cur=pt_line_start + itx;  
				create_bits_tile(cur_bit,pt_cur,ipy,
					bits_per_color);		
				cur_bit += 	
					(image->tiles->pixels_per_tile_side*3);
			}
		}
	}
}

void create_bits_tile(BYTE *bits,struct_tile *tile,int ipy,int bits_per_color)
{
int ipx;
RGBQUAD p; 
BYTE *pb;
	
	pb=bits;
	for (ipx=0;ipx<tile->pixels_per_tile_side;ipx++) {
		p = gen_rgb(tile->tile_image[ipx][ipy],bits_per_color);
		*pb= p.rgbBlue; 
		pb++;
		*pb = p.rgbGreen;
		pb++;
		*pb = p.rgbRed;
		pb++;
	}
}

RGBQUAD gen_rgb(unsigned int pixel,int bits_per_color)
{
unsigned int mask;
unsigned int pad;
RGBQUAD p; 

	if((bits_per_color>8) || (bits_per_color<1)) {
		assert(0);
	}
	else {
		mask = (0x1<<bits_per_color) - 1;
		pad = (8-bits_per_color);
		p.rgbRed = (unsigned char) ((pixel & mask)<<pad);
		
		p.rgbBlue = (unsigned char) 
			(((pixel>>bits_per_color) & mask)<<pad); 

		p.rgbGreen = (unsigned char) 
			(((pixel>>(bits_per_color*2)) & mask)<<pad);
	}
	return (p);
}


