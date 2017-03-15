#include<stdio.h>
#include<wiringPi.h>
int main()
{
    wiringPiSetup();
    
    pinMode(8,OUTPUT);
    pinMode(9,INPUT); 
    
    digitalWrite(8,LOW);
    digitalWrite(8,HIGH);
    delayMicroseconds(10);
    digitalWrite(8,LOW);
    
    int vol = digitalRead(9);
    while(vol == 0)
    {
	  vol = digitalRead(9);
	  
	}
	printf("start\n");	
	while(vol == 1)
	{
	  vol = digitalRead(9);
	  
	}
	printf("end\n");
    
    return(0);
}
