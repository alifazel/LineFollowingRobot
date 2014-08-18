#include <16F648A.h>
#use delay(clock=4000000)
#fuses NOWDT, INTRC, NOPUT, NOPROTECT, NOLVP, NOMCLR



   static int const LUTBL[4] = {0x09,0x03,0x06,0x0C};   
   int DIR1=1, DIR2=1;
   int Index1, Index2;
   int i1=1, i2=1;
   int speed1, speed2;
   int const Speeds[4] = {10, 30, 60, 100};
   int d;
   


struct Motor_pin_map 
{
boolean Forward1: 1;      
int Speed1: 2;         // for motor1 speed, using 2 switches  
boolean Forward2: 1;      
int Speed2: 2;       // for motor2 speed, using 2 switches
int unused: 2;
int Motor1: 4;   // output pin of the motor
int Motor2: 4;   
       
};

struct Motor_pin_map MotorPort;
struct Motor_pin_map MotorPortDirection;
#byte MotorPort = 0x05
#byte MotorPortDirection = 0x85


#int_RTCC
void Timer0_isr()
{
   speed1 = Speeds[MotorPort.Speed1];     // choose a speed for motor1
   speed2 = Speeds[MotorPort.Speed2];     // choose a speed for motor2
   
   i1++;   
   if(i1==speed1)
   {
     Index1 = (Index1+DIR1)%4;
     i1=0;
   }

   i2++;   
   if(i2==speed2)
   {
     Index2 = (Index2+DIR2)%4;
     i2=0;
   }

}


int Direction(boolean Forward,int D)   // function to control the motor directions

{
   if(Forward==0)
      d=1;
   if(Forward==1)
      d=-1;   

   return d;
}


void main()
{
   

   MotorPortDirection.Motor1 = 0x0;      
   MotorPortDirection.Motor2 = 0x0;
   MotorPortDirection.Forward1 = 0b1;   
   MotorPortDirection.Speed1 = 0b11;   
   MotorPortDirection.Forward2 = 0b1;   
   MotorPortDirection.Speed2 = 0b11;   
   setup_timer_0(RTCC_INTERNAL | RTCC_DIV_4);   //generates interrupt every 1.024ms
   enable_interrupts(INT_RTCC);
   enable_interrupts(GLOBAL);   





while(1)
{
   DIR1 = Direction(MotorPort.Forward1, DIR1);   //determines the direction of motor1
   DIR2 = Direction(MotorPort.Forward2, DIR2);   //determines the direction of motor2

   MotorPort.Motor1=LUTBL[Index1];
   MotorPort.Motor2=LUTBL[Index2];

}
}