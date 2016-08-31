#include <stdio.h>

int datastack[15] = {25,0,100,-50,50,-100,75,0,50,-75,0,-50,100,0,50};
int buffptr = 0;		// Buffer pointer: Do not trace real time rainflow if it is lower than 3 (No peak/valley detected)
int flag = 0;			// Flag to determine rainflow counting process: 0 = None; 1 = Minima; 2 = Maxima
int extrema_buff[15];
int maxima_buff[100];
int minima_buff[100];
int maxima_ptr = 0;
int minima_ptr = 0;

double cycle_buff[15];
int cycle_ptr = 0;

int diff_buff[15];
int diff_ptr = 0;

int mean_buff[15];
int mean_ptr = 0;

int extrema_detection(int pt0, int pt1, int pt2);
void online_rainflow_max(int rf_flag);

// Additional notes: sizeof(a)/sizeof(a[0]) could be used to determine array size


int main(void)
{
	int i = 0;
	int len = sizeof(datastack)/sizeof(datastack[0]);
	int status1 = 0;
	int pre_status = 0;
	
			
	printf("Test Online Rainflow Algorithm Here\r\n");
	printf("Array size: %d\r\n", len); 
	
	for (i = 1; i < len-1; i++)
	{
		int pt0 = datastack[i-1];
		int pt1 = datastack[i];
		int pt2 = datastack[i+1];
		
		status1 = extrema_detection(pt0,pt1,pt2);
		
		if ((status1 == 0));
		else if ((status1 == 1)) {minima_buff[minima_ptr++] = datastack[i]; flag = 1;}
		else if ((status1 == 2)) {maxima_buff[maxima_ptr++] = datastack[i]; flag = 2;}
		
		if ((status1 >= 3) && ((i+2) <= len))		// Check for next instance only if i is still within range
		{
			int pt3 = datastack[i+2];
			if (status1 == 3)
			{
				if (pt3 > pt2) {minima_buff[minima_ptr++] = datastack[i]; flag = 1;}		// It is a minimal point
				else if (pt3 == pt2) pre_status = 3;										// Store value for next instance checking
				else if (pt3 < pt2); 														// Not an extrema
			}
			
			else if (status1 == 4)
			{
				if (pre_status == 3) minima_buff[minima_ptr++] = datastack[i]; 		// If there is a bottom left corner, it is a minimal point
				flag = 1;
			}
			
			else if (status1 == 5)
			{
				if (pt3 > pt2);																// Not an extrema
				else if (pt3 == pt2) pre_status = 5;										// Store value for next instance checking 
				else if (pt3 < pt2) {maxima_buff[maxima_ptr++] = datastack[i]; flag = 2;}	// It is a maximal point
			}
			
			else if (status1 == 6)
			{
				if (pre_status == 5) maxima_buff[maxima_ptr++] = datastack[i]; 		// If there is a top left corner, it is a maximal point
				flag = 2;
			}
		}
		
		printf("Maximum pointer: %d\tMinimum pointer: %d\tFlag:%d\r\n",maxima_ptr,minima_ptr,flag);
		
		if ((flag > 0)&&(((maxima_ptr>=1)&&(minima_ptr>=2))||((maxima_ptr>=2)&&(minima_ptr>=1))))
		{
			printf("Couting cycles...\r\n");
			online_rainflow_max(flag);
			flag = 0;
		}
		
	}
	
	printf("Data in maxima buffer: ");
	for (i = 0; i < maxima_ptr; i++)
	{
		printf("%d ", maxima_buff[i]);
	}
	
	printf("\r\nData in minima buffer: ");
	for (i = 0; i < minima_ptr; i++)
	{
		printf("%d ", minima_buff[i]);
	}
	
	printf("\r\nData in diff buffer: ");
	for (i = 0; i < diff_ptr; i++)
	{
		printf("%d ", diff_buff[i]);
	}
	
	printf("\r\nData in mean buffer: ");
	for (i = 0; i < mean_ptr; i++)
	{
		printf("%d ", mean_buff[i]);
	}
	
	printf("\r\nData in cycle buffer: ");
	for (i = 0; i < cycle_ptr; i++)
	{
		printf("%f ", cycle_buff[i]);
	}
	
	//if datastack[0] < 
		
	return 0;
}

int extrema_detection(int pt0, int pt1, int pt2)	
{
	// Function to check extrema: 
	//		0 = No extrema;
	//		1 = Minima;
	//		2 = Maxima;
	//		3 = Possile Minima? (Bottom Left corner)
	//		4 = Possible Minima? (Bottom Right corner)
	//		5 = Possible Maxima? (Top Left corner)
	//		6 = Possible Maxima? (Top right corner)
		
	int range0_1;
	int range1_2;
		
	if ((pt1==pt0)&&(pt1==pt2)) return 0;
	else if ((pt1<pt0)&&(pt1<pt2)) return 1;
	else if ((pt1>pt0)&&(pt1>pt2)) return 2;
	else if ((pt1<pt0)&&(pt1==pt2)) return 3;
	else if ((pt1==pt0)&&(pt1<pt2)) return 4;
	else if ((pt1>pt0)&&(pt1==pt2)) return 5;
	else if ((pt1==pt0)&&(pt1>pt2)) return 6;
	else return 0;
}

void online_rainflow_max(int rf_flag)
{	
	int diff = 0;
	double mean = 0;
	
	if(rf_flag == 1)
	{
		do
		{
			if (minima_buff[minima_ptr-1] <= minima_buff[minima_ptr-2]) // Compare new minima value with old minima value
			{
				if (maxima_ptr == 1)
				{
					// Half cycle
					printf("Half cycle: %d - %d\r\n",maxima_buff[maxima_ptr-1],minima_buff[minima_ptr-2]);
					diff = maxima_buff[maxima_ptr-1] - minima_buff[minima_ptr-2];
					mean = minima_buff[minima_ptr-2] + diff/2;
					diff_buff[diff_ptr++] = diff;
					mean_buff[mean_ptr++] = mean;
					//TLC
					
					minima_buff[minima_ptr-2] = minima_buff[minima_ptr-1];		// Shift stack left
					minima_buff[minima_ptr-1] = 0;
					minima_ptr -= 1;
					
					cycle_buff[cycle_ptr++] = 0.5;
				}
				
				//Check!!
				else if (maxima_ptr > 1)
				{
					// Full cycle
					printf("Full cycle: %d - %d\r\n",maxima_buff[maxima_ptr-1],minima_buff[minima_ptr-2]);
					diff = maxima_buff[maxima_ptr-1] - minima_buff[minima_ptr-2];
					mean = minima_buff[minima_ptr-2] + diff/2;
					diff_buff[diff_ptr++] = diff;
					mean_buff[mean_ptr++] = mean;
					//TLC
					
					maxima_buff[maxima_ptr-1] = 0;
					maxima_ptr -= 1;
					
					minima_buff[minima_ptr-2] = minima_buff[minima_ptr-1];
					minima_buff[minima_ptr-1] = 0;
					minima_ptr -= 1;
					
					cycle_buff[cycle_ptr++] = 1;
				}
			}
			else break;
		} while (minima_ptr > 1);
	}
	
	if(rf_flag == 2)
	{
		do
		{
			if (maxima_buff[maxima_ptr-1] >= maxima_buff[maxima_ptr-2]) // Compare new maxima value with old minima value
			{
				if (minima_ptr == 1)
				{
					// Half cycle
					printf("Half cycle: %d - %d\r\n",maxima_buff[maxima_ptr-2],minima_buff[minima_ptr-1]);
					diff = maxima_buff[maxima_ptr-2] - minima_buff[minima_ptr-1];
					mean = minima_buff[minima_ptr-2] + diff/2;
					diff_buff[diff_ptr++] = diff;
					mean_buff[mean_ptr++] = mean;
					//TLC
					
					maxima_buff[maxima_ptr-2] = maxima_buff[maxima_ptr-1];		// Shift stack left
					maxima_buff[maxima_ptr-1] = 0;
					maxima_ptr -= 1;
					//TLC
					
					cycle_buff[cycle_ptr++] = 0.5;
				}
				
				else if (minima_ptr > 1)
				{
					// Full cycle
					printf("Full cycle: %d - %d\r\n",maxima_buff[maxima_ptr-2],minima_buff[minima_ptr-1]);
					diff = maxima_buff[maxima_ptr-2] - minima_buff[minima_ptr-1];
					mean = minima_buff[minima_ptr-1] + diff/2;
					diff_buff[diff_ptr++] = diff;
					mean_buff[mean_ptr++] = mean;
					//TLC
					
					minima_buff[minima_ptr-1] = 0;
					minima_ptr -= 1;
					
					maxima_buff[maxima_ptr-2] = maxima_buff[maxima_ptr-1];
					maxima_buff[maxima_ptr-1] = 0;
					maxima_ptr -= 1;
					
					cycle_buff[cycle_ptr++] = 1;
					
				}
			}
			else break;
		} while (maxima_ptr > 1);
	}
}