#include <16F648A.h>
#use delay(clock=4000000)
#fuses NOWDT, INTRC, NOPUT, NOPROTECT, NOLVP, NOMCLR

int const LOOKUPTABLE[4] = {0x09,0x03,0x06,0x0C};
int const DIR[2]={1,3};
int const SPEED[4]={40,20,10,5};

struct pin{
	int sensors:5;		//Port A
	
	int unused:3;

	int Output1:4;		//Port B
	int Output2:4;
};

struct pin IO_data;
struct pin IO_status;

#byte IO_data = 0x05
#byte IO_status = 0x85
int TableIndex_motor1=0;
int TableIndex_motor2=0;
int c1=0, c2=0;
int speed_setting;
int sensor_array[5];
int c3=0;

#int_RTCC
void Timer0_isr()
{
	c1++;		// Delay Counters
	c2++;
}

void move_forward() {

	
	if(c1>=10)
    {IO_data.Output1 = LOOKUPTABLE[TableIndex_motor1];
 	TableIndex_motor1 = ((TableIndex_motor1+1)%4);
	c1=0;
				}
				

	if(c2>=10)
    {IO_data.Output2 = LOOKUPTABLE[TableIndex_motor2];
   	TableIndex_motor2 = ((TableIndex_motor2-1)%4);
    c2=0;
				}

				
}


void turn_left_fast() {

	if(c1>=20)  
	{
	IO_data.Output1 = LOOKUPTABLE[TableIndex_motor1];
  	TableIndex_motor1 = ((TableIndex_motor1+1)%4);
	c1=0;
	}


	if(c2>=20)
    {
	IO_data.Output2 = LOOKUPTABLE[TableIndex_motor2];
    TableIndex_motor2 = ((TableIndex_motor2+1)%4);
    c2=0;
	}



}


void turn_right_fast() {

	if(c1>=20)  
	{
	IO_data.Output1 = LOOKUPTABLE[TableIndex_motor1];
  	TableIndex_motor1 = ((TableIndex_motor1-1)%4);
	c1=0;
	}


	if(c2>=20)
    {
	IO_data.Output2 = LOOKUPTABLE[TableIndex_motor2];
    TableIndex_motor2 = ((TableIndex_motor2-1)%4);
    c2=0;
	}
}


void turn_left_slow() {

    for (c3=0; c3<5; c3++)
    {
        if(c2>=20)
        {
            IO_data.Output2 = LOOKUPTABLE[TableIndex_motor2]; 
            TableIndex_motor2 = ((TableIndex_motor2-1)%4);
            c2=0;
        }
    }

	
}


void turn_right_slow() {

    for (c3=0; c3<5; c3++)
 	{
        if(c1>=20)
        {
            IO_data.Output1 = LOOKUPTABLE[TableIndex_motor1];
            TableIndex_motor1 = ((TableIndex_motor1+1)%4);
            c1=0;
        }	
    }
}

/*void update_sensor_array() {
	
	sensor_array[0]=IO_status.sensor1;
	sensor_array[1]=IO_status.sensor2;
	sensor_array[2]=IO_status.sensor3;
	sensor_array[3]=IO_status.sensor4;
	sensor_array[4]=IO_status.sensor5;
}*/

void main(){

	IO_status.Output1 = 0b0000;	//Sets motor 1 as output TRIS
	IO_status.Output2 = 0b0000;	//Sets motor 2 as output TRIS
	IO_status.sensors = 0b11111;	//Sets sensor 1 as input TRIS

	enable_interrupts(INT_RTCC);
	enable_interrupts(GLOBAL);

	
	
	while(1)
	{
		setup_timer_0(RTCC_INTERNAL|RTCC_DIV_4);
		
	/*	update_sensor_array();*/

			
			//Straight

            if (IO_data.sensors==0b00100)
                move_forward();
			

			//Turn Left

     		if (IO_data.sensors==0b00010)
                turn_left_slow();
			
       		if (IO_data.sensors==0b00110)
            	turn_left_slow();

      		if (IO_data.sensors==0b00111)
            	turn_left_slow();
			            
     		if (IO_data.sensors==0b00011)
                turn_left_slow();

			if (IO_data.sensors==0b00001)
                turn_left_slow();

			if (IO_data.sensors==0b00101)
                turn_left_slow();

			//Turn Right

            if (IO_data.sensors==0b01000)
                turn_right_slow();  

            if (IO_data.sensors==0b01100)
             	turn_right_slow();
  	
            if (IO_data.sensors==0b11100)
             	turn_right_slow();
  			
            if (IO_data.sensors==0b11000)
                turn_right_slow();
			
            if (IO_data.sensors==0b10000)
                turn_right_slow();  

            if (IO_data.sensors==0b10100)
                turn_right_slow();  


	}
}