#include <16F648A.h>
#use delay(clock=4000000)
#fuses NOWDT, INTRC, NOPUT, NOPROTECT, NOLVP, NOMCLR

int const LOOKUPTABLE[4] = {0B1001,0B1100,0B0110,0B0011};
int const DIR[2]={1,3};
int const SPEED[4]={40,20,10,5};

struct pin{
	int sensor1:1;		//Port A
	int sensor2:1;
	int sensor3:1;
	int sensor4:1;
	int sensor5:1;
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
int counter=0;
int speed_setting;



#int_RTCC
void Timer0_isr()
{
	counter++;		// Delay Counters
}

void move_forward() {

    IO_data.Output1 = LOOKUPTABLE[TableIndex_motor1];
    IO_data.Output2 = LOOKUPTABLE[TableIndex_motor2];
    TableIndex_motor1 = ((TableIndex_motor1+1)%4);
    TableIndex_motor2 = ((TableIndex_motor2+1)%4);
    counter=0;
}


void turn_left_fast() {

    IO_data.Output1 = LOOKUPTABLE[TableIndex_motor1];
    IO_data.Output2 = LOOKUPTABLE[TableIndex_motor2];
    TableIndex_motor1 = ((TableIndex_motor1-1)%4);
    TableIndex_motor2 = ((TableIndex_motor2+1)%4);
    counter=0;
}


void turn_right_fast() {

    IO_data.Output1 = LOOKUPTABLE[TableIndex_motor1];
    IO_data.Output2 = LOOKUPTABLE[TableIndex_motor2];
    TableIndex_motor1 = ((TableIndex_motor1+1)%4);
    TableIndex_motor2 = ((TableIndex_motor2-1)%4);
    counter=0;
}


void turn_left_slow() {

    IO_data.Output2 = LOOKUPTABLE[TableIndex_motor2];
    TableIndex_motor2 = ((TableIndex_motor2+1)%4);
    counter=0;
}


void turn_right_slow() {

    IO_data.Output1 = LOOKUPTABLE[TableIndex_motor1];
    TableIndex_motor1 = ((TableIndex_motor1+1)%4);
    counter=0;

}

void update_sensor_array() {
	
	sensor_array[0]=IO_status.sensor1;
	sensor_array[1]=IO_status.sensor2;
	sensor_array[2]=IO_status.sensor3;
	sensor_array[3]=IO_status.sensor4;
	sensor_array[4]=IO_status.sensor5;
}

void main(){

	IO_status.Output1 = 0b0000;	//Sets motor 1 as output TRIS
	IO_status.Output2 = 0b0000;	//Sets motor 2 as output TRIS
	IO_status.sensor1 = 0b1;	//Sets sensor 1 as input TRIS
	IO_status.sensor2 = 0b1;	//Sets sensor 2 as input TRIS
	IO_status.sensor3 = 0b1;	//Sets sensor 3 as input TRIS
	IO_status.sensor4 = 0b1;	//Sets sensor 4 as input TRIS
	IO_status.sensor5 = 0b1;	//Sets sensor 5 as input TRIS
	enable_interrupts(INT_RTCC);
	enable_interrupts(GLOBAL);

	int sensor_array[5];
	
	while(1)
	{
		setup_timer_0(RTCC_INTERNAL|RTCC_DIV_4);
		
		update_sensor_array();

        if (counter==5)
        {
            if ((sensor_array==(0,0,1,0,0)))
                move_forward();
        }
        
        if (counter==10)
        {
            if ((sensor_array==(0,1,0,0,0)))
                turn_left_fast();

            if ((sensor_array==(0,0,0,1,0)))
                turn_right_fast();
        }
        
            
        if (counter==20)
        {
            if ((sensor_array==(1,0,0,0,0)))
                turn_left_slow();
        
            if ((sensor_array==(0,1,0,0,0)))
                turn_right_slow();
        }

	}
}