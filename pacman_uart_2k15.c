#include<stdio.h>
#include<SerialStream.h>

using namespace LibSerial;

int matrix[33][33];
int ledmatrix[16][16];
int input[6]; // 1 on, 0 off
SerialStream serial_stream;

void takeInput(){
	char inp;
	serial_stream >> inp;
	int i;
	for(i=0;i<6;i++)
	{
 		input[i] = (inp & (1 << (5-i))) >> (5-i);
	}
}
void toLedMatrix(){
	int i,j;
	for (i=1;i<33;i+=2)
	{
        	for(j=1;j<33;j+=2)
       		{
			ledmatrix[i/2][j/2] = matrix[i][j];
			printf("%d ", ledmatrix[i/2][j/2]);
                }
		printf("\n");
        }
}

void portInit()
{
	serial_stream.Open("/dev/ttyACM2");
	serial_stream.SetBaudRate(SerialStreamBuf::BAUD_9600);
	serial_stream.SetCharSize( SerialStreamBuf::CHAR_SIZE_8 ) ;
	serial_stream.SetNumOfStopBits(1) ;
	serial_stream.SetParity( SerialStreamBuf::PARITY_NONE ) ;
	serial_stream.SetFlowControl( SerialStreamBuf::FLOW_CONTROL_NONE ) ;
	if(!serial_stream.good())
	{
	    printf("Error in opening port - Try sudo \n");
	}
}

void writeFrame()
{
   char byteStream1[32];
   char byteStream2[32];
   char byteStream4[32];
   int i, j, k;
   k = 0;
   //Setting up bytestreams to write
   for (i=0;i<16;i++)
   {
      for (j=0;j<16;j++)
      {
          int val = ledmatrix[i][j];
          if (val == 1)
          {
              byteStream1[k] = byteStream1[k] | (1 << (7-(j%8)));
              byteStream2[k] = byteStream2[k] & (~(1 << (7-(j%8))));
              byteStream4[k] = byteStream4[k] & (~(1 << (7-(j%8))));
          }
          if (val == 2)
          {
              byteStream1[k] = byteStream1[k] & (~(1 << (7-(j%8))));
              byteStream2[k] = byteStream2[k] | (1 << (7-(j%8)));
              byteStream4[k] = byteStream4[k] & (~(1 << (7-(j%8))));
          }
          if (val == 4)
          {
              byteStream1[k] = byteStream1[k] & (~(1 << (7-(j%8))));
              byteStream2[k] = byteStream2[k] & (~(1 << (7-(j%8))));
              byteStream4[k] = byteStream4[k] | (1 << (7-(j%8)));
          }
          if (j == 7)
             k++;
      }
   }
   //Writing bytestreams to port now.
   char ack;
   serial_stream << 'S';
   printf("Wrote S\n");
   do
   {
      serial_stream >> ack;
   }
   while(ack != 'K');

   printf("K received\n");
   for (i=0; i<16; i++){
       serial_stream << byteStream1[2*i];
       printf("Wrote %c\n", byteStream1[2*i]);
       serial_stream << byteStream1[2*i+1];
       printf("Wrote %c\n", byteStream1[2*i+1]);
       serial_stream << byteStream2[2*i];
       printf("Wrote %c\n", byteStream2[2*i]);
       serial_stream << byteStream2[2*i+1];
       printf("Wrote %c\n", byteStream2[2*i+1]);
       serial_stream << byteStream4[2*i];
       printf("Wrote %c\n", byteStream4[2*i]);
       serial_stream << byteStream4[2*i+1];
       printf("Wrote %c\n", byteStream4[2*i+1]);
   }
}

void create_maze()
{  // 8 is free space
    // 5 is obstruction


   int n=0,m=0,i;
  // initialize to 8, free space
  for (m=0;m<33;m++)
{
  for(n=0;n<33;n++)
  {

    matrix[m][n]=8;
  }
}


   for(n=0;n<33;n++)// initialization of boundary condition
  {
    matrix[0][n]=5;
    matrix[32][n]=5;
    matrix[n][0]=5;
    matrix[n][32]=5;
   }




//horizontal obstacles

for(i=1;i<9;i++)
{
  matrix[4][i]=5;
  matrix[8][i]=5;
  if(i>=4)
  {
    matrix[20][i]=5;
    matrix[24][i]=5;
    matrix[28][i]=5;
  }

}

for(i=12;i<17;i++)
  { matrix[8][i]=5;
    matrix[20][i]=5;
  }
for(i=20;i<29;i++)
{  matrix[8][i]=5;
   matrix[16][i]=5;
   matrix[28][i]=5;
   if(i>=24)
    matrix[24][i]=5;
}
for(i=12;i<25;i++)
   matrix[12][i]=5;
for(i=28;i<33;i++)
{  matrix[12][i]=5;
   matrix[20][i]=5;
   matrix[24][i]=5;
}
for(i=8;i<13;i++)
  { matrix[16][i]=5;
    matrix[24][i]=5;
  }
for(i=16;i<21;i++)
{
  matrix[24][i]=5;
}

// vertical obstacles
for(i=0;i<5;i++)
{
  matrix[i][16]=5;
}
for(i=4;i<9;i++)
{
  matrix[i][12]=5;
  matrix[i][20]=5;
  matrix[i][28]=5;
}
for(i=8;i<13;i++)
{
  matrix[i][8]=5;
  matrix[i][24]=5;
}
for(i=12;i<21;i++)
{
  matrix[i][4]=5;
  matrix[i][12]=5;
  matrix[i][20]=5;
  if(i>=16)
    matrix[i][8]=5;
}
for(i=20;i<25;i++)
{
  matrix[i][20]=5;
  matrix[i][24]=5;
}
for(i=24;i<29;i++)
{
  matrix[i][4]=5;
  matrix[i][12]=5;
  matrix[i][16]=5;
}
for(i=28;i<33;i++)
{
  matrix[i][12]=5;
  matrix[i][20]=5;
}
for (m=0;m<33;m++) //initialize to food
{
  for(n=0;n<33;n++)
  {
    if(((m%2)==1)&&((n%2)==1))
    matrix[m][n]=4;
  }
}
matrix[31][1]=1;// initiatize to red
matrix[31][31]=2;//initialize to blue

}

int digits[70][4] = {{1,1,1,1}, {1,0,0,1}, {1,0,0,1}, {1,0,0,1}, {1,0,0,1}, {1,0,0,1},{1,1,1,1},{0,0,0,1},{0,0,0,1},{0,0,0,1},{0,0,0,1},{0,0,0,1},{0,0,0,1},{0,0,0,1},{1,1,1,1},{0,0,0,1}, {0,0,0,1}, {1,1,1,1}, {1,0,0,0}, {1,0,0,0}, {1,1,1,1},{1,1,1,1},{0,0,0,1}, {0,0,0,1}, {1,1,1,1}, {0,0,0,1}, {0,0,0,1}, {1,1,1,1},{1,0,0,1},{1,0,0,1}, {1,0,0,1}, {1,1,1,1}, {0,0,0,1}, {0,0,0,1}, {0,0,0,1},    {1,1,1,1}, {1,0,0,0}, {1,0,0,0}, {1,1,1,1}, {0,0,0,1}, {0,0,0,1}, {1,1,1,1},{1,1,1,1}, {1,0,0,0}, {1,0,0,0}, {1,1,1,1}, {1,0,0,1}, {1,0,0,1}, {1,1,1,1},{1,1,1,1}, {0,0,0,1}, {0,0,0,1},{0,0,0,1},{0,0,0,1},{0,0,0,1},{0,0,0,1},{1,1,1,1}, {1,0,0,1}, {1,0,0,1}, {1,1,1,1}, {1,0,0,1}, {1,0,0,1}, {1,1,1,1}, {1,1,1,1}, {1,0,0,1}, {1,0,0,1}, {1,1,1,1}, {0,0,0,1}, {0,0,0,1}, {0,0,0,1}};


void display_score(int a, int b) {
     int i,j,temp,row,column,c,d,k,m;
 // make matrix zero
	for(i=0;i<16;i++)
	  for(j=0;j<16;j++)
		{matrix[i][j]=0;
		}
     for(j=0;j<2;j++){
        c = (j==0)?a:b;
        for(i=0;i<3;i++) {
            d = c%10;
            c = c/10;
            row = j==0?0:9;
            column = 11-5*i;
            temp = column;
            for(k=7*d;k<7*d+7;k++) {
                column = temp;
                for(m=0;m<4;m++){
                    matrix[row][column] = digits[k][m];
                    column++;
                }
                row++;
            }
        }
     }
}

void centre(int *m,int *n,int *dir, char p, int *score)
{

   if(((*dir)==0)&&(matrix[(*m)-1][(*n)]==8))
    {
     (*m)-=2;
     (*dir)=0;
    }
    else if(((*dir)==1)&&(matrix[(*m)][(*n)+1]==8))
      { (*n)+=2;
       (*dir)=1;
     }
   else if(((*dir)==2)&&(matrix[(*m)+1][(*n)]==8))
      { (*m)+=2;
        (*dir)=2;
     }
     else if(((*dir)==3)&&(matrix[(*m)][(*n)-1]==8))
     {   (*n)-=2;
        (*dir)=3;
      }

    if(matrix[(*m)][(*n)]==4)
      {
	if(p=='r')
        matrix[(*m)][(*n)]=1; //red ate food
	else
	matrix[(*m)][(*n)]=2; //blue ate food

	(*score)++;
      }
}

void right(int *m,int *n,int *dir, char p,int *score)
{
   if(((*dir)==0)&&(matrix[(*m)][(*n)+1]==8))
   {
     (*n)+=2;
     (*dir)=1;
    }
    else if(((*dir)==1)&&(matrix[(*m)+1][(*n)]==8))
      { (*m)+=2;
       (*dir)=2;
     }
     else if(((*dir)==2 )&&(matrix[(*m)][(*n)-1]==8))
       { (*n)-=2;
        (*dir)=3;
      }
    else if(((*dir)==3)&&(matrix[(*m)-1][(*n)]==8))
     { (*m)-=2;
      (*dir)=0;
     }
    if(matrix[(*m)][(*n)]==4)
      {
       if(p=='r')
        matrix[(*m)][(*n)]=1; //red ate food
	else
	matrix[(*m)][(*n)]=2; //blue ate food

	(*score)++;
      }
}

void left(int *m,int *n,int *dir, char p, int *score)
{
   if(((*dir)==0)&&(matrix[(*m)][(*n)-1]==8))
     {
     (*n)-=2;
     (*dir)=3;
    }
    else if(((*dir)==1)&&(matrix[(*m)-1][(*n)]==8))
       {
	(*m)-=2;
       (*dir)=0;
     }
     else if(((*dir)==2 )&&(matrix[(*m)][(*n)+1]==8))
        {
        (*n)+=2;
        (*dir)=2;
      }

    else if(((*dir)== 3)&&(matrix[(*m)+1][(*n)]==8))
     {
	 (*m)+=2;
      (*dir)=2;
     }
    if(matrix[(*m)][(*n)]==4)
      {
        if(p=='r')
        matrix[(*m)][(*n)]=1; //red ate food
	else
	matrix[(*m)][(*n)]=2; //blue ate food

	(*score)++;
      }
}



int main()
{
int red_mv; // where it wants to move
int blue_mv;
int red_dir=0,blue_dir=0; // previous direction
int red_score=1,blue_score=1;
int rm=31,rn=1,bm=31,bn=31; //current positions
int food=14;
int m, n;
FILE *maze;
create_maze();
portInit(); //Initialize serial ports.
while(food>=1)
{
   printf("enter red dir\n");
   scanf("%d",&red_mv);
   if (red_mv==2)
   {
	centre(&rm,&rn,&red_dir,'r',&red_score);
   }
   else if (red_mv==1)
   {
	right(&rm,&rn,&red_dir,'r',&red_score);
   }
   else if (red_mv==4)
   {
	left(&rm,&rn,&red_dir,'r',&red_score);
   }
   printf("enter blue dir\n");
   scanf("%d",&blue_mv);
   if (blue_mv==2)
   {
	centre(&bm,&bn,&blue_dir,'b',&blue_score);
   }
   else if (blue_mv==1)
   {
        right(&bm,&bn,&blue_dir,'b',&blue_score);
   }
   else if (blue_mv==4)
   {
       left(&bm,&bn,&blue_dir,'b',&blue_score);
   }
   food--;
   toLedMatrix(); // Convert to 16x16 matrix for sending
   writeFrame(); // Write stuff to serial
}

maze=fopen("maze_file","w");
for (m=0;m<33;m++)
{
  for(n=0;n<33;n++)
  {
    fprintf(maze,"%d  ",matrix[m][n]);
s  }
  fprintf(maze,"\n");
}
printf("Red score is %d\n",red_score);
printf("Blue score is %d\n",blue_score);
return 0;

}