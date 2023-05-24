#include <stdint.h>
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <iostream>>

/*
@brief		combine two yuv frame together
@param[in]		frame1     a bigger yuv frame
@param[in]		w1		the width of the frame1
@param[in]		h1		the height of the frame1
@param[in]		frame2		a smaller yuv frame   
@param[in]		w2		the width of the frame2
@param[in]		h2		the height of the frame2
@param[in]		off_x, off_y		the location of the top left coner of the frame2 on frame1   
*/

void Combine(char *frame1, int w1, int h1, char *frame2, int w2, int h2, int off_x, int off_y)
{
    int size1 = w1 * h1;
    int size2 = w2 * h2;
 
    char *y1 = frame1;
    char *u1 = y1 + size1;
    char *v1 = u1 + size1 / 4;
 
    char *y2 = frame2;
    char *u2 = y2 + size2;
    char *v2 = u2 + size2 / 4;
 
    int nOff = 0;
    for (int i = 0; i < h2; i++){
        nOff = w1 * (off_y + i) + off_x; 
        memcpy(y1 + nOff, y2 + w2 * i, w2);
    }
    for (int j = 0; j < h2 / 2; j++){
        nOff = (w1 / 2) * (off_y / 2 + j) + off_x / 2;
        memcpy(u1 + nOff, u2 + w2 / 2 * j, w2 / 2);
        memcpy(v1 + nOff, v2 + w2 / 2 * j, w2 / 2);
    }
}
 
int main(int argc, char **argv)
{
	// set how many times the program runs
	int test_num = 1;
	int total_time = 0;
	for(int i = 0; i != test_num; i++){
		// the timer
		DWORD time_start, time_end;
		time_start = GetTickCount();
		
		// open two input files and one output file
	    FILE *file1 = fopen("background.yuv", "rb");
	    FILE *file2 = fopen("1288_592.yuv", "rb");
	    FILE *file3 = fopen("1288_592-.yuv", "ab+");
	    
		// set the resolution of each input frame
		int w1 = 1920;
	    int h1 = 1080;
	    int w2 = 1288;
	    int h2 = 592;
	    int size1 = w1 * h1;
	    int size2 = w2 * h2;
	 
	    char *frame1 = (char *)malloc(size1 * 3 / 2);
	    char *frame2 = (char *)malloc(size2 * 3 / 2);
	     memset(frame1, 0, size1);
	     memset(frame2, 0, size2);
	 
        fread(frame1, 1, size1 * 3 / 2, file1);
        fread(frame2, 1, size2 * 3 / 2, file2);
 
        Combine(frame1, w1, h1, frame2, w2, h2, 0, 0);
        fwrite(frame1, 1, size1 * 3 / 2, file3);

	    fclose(file1);
	    fclose(file2);
	    fclose(file3);
	    
	    // timer
	    time_end = GetTickCount();
		total_time += 	time_end - time_start;
		printf("%d\n", time_end - time_start);
	}
	printf("average: %d", total_time / test_num);
	
}
