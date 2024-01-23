
#define  PORTA    (*(volatile unsigned char *)0x3B)
#define  DDRA    (*(volatile unsigned char *)0x3A)
#define  PINA    (*(volatile unsigned char *)0x39)

#define  PORTB    (*(volatile unsigned char *)0x38)
#define  DDRB    (*(volatile unsigned char *)0x37)
#define  PINB    (*(volatile unsigned char *)0x36)

#define  PORTC    (*(volatile unsigned char *)0x35)
#define  DDRC    (*(volatile unsigned char *)0x34)
#define  PINC    (*(volatile unsigned char *)0x33)

#define  PORTD    (*(volatile unsigned char *)0x32)
#define  DDRD    (*(volatile unsigned char *)0x31)
#define  PIND    (*(volatile unsigned char *)0x30)

#define  F_CPU   8000000
#include <util/delay.h>

#define SET_BIT(reg,bit) (reg|=(1<<bit))
#define CLR_BIT(reg,bit) (reg&=~(1<<bit))
#define READ_BIT(reg,bit) ((reg>>bit)&1)

void SEVEN_SEGMENT_DISPLAY_MLX (char n)
{
	const char arr[10]={0X7E,0X0C,0XB6,0X9E,0XCC,0XDA,0XFA,0X0E,0XFF,0XDE};
	SET_BIT(PORTC,6);
	PORTA=arr[n%10];
	CLR_BIT(PORTC,7);
	_delay_ms(2); // << 5
	
	SET_BIT(PORTC,7);
	PORTA=arr[n/10];
	CLR_BIT(PORTC,6);
	_delay_ms(1); // << 1
	
}

void finish (void)
{
	char x;
	for (x=0;x<4;x++)
	{
		SET_BIT(PORTC,7);
		SET_BIT(PORTC,6);
		PORTA=0X7E;
		SET_BIT(PORTC,5);
		PORTB=0xff;
		_delay_ms(200);
		
		CLR_BIT(PORTC,7);
		CLR_BIT(PORTC,6);
		CLR_BIT(PORTC,5);
		PORTB=0;
		_delay_ms(120);
		
	}
	
}

void BUTTON_DELAY (char i)
{
	for(char x=0;x<20;x++)
	{
		_delay_ms(1);
		SEVEN_SEGMENT_DISPLAY_MLX(i);
		_delay_ms(1);
		SEVEN_SEGMENT_DISPLAY_MLX(i);
		_delay_ms(1);
		SEVEN_SEGMENT_DISPLAY_MLX(i);
		_delay_ms(1);
		SEVEN_SEGMENT_DISPLAY_MLX(i);
	}
}


char increament (char i)
{
	char x,new1=i/10,new2=i%10;
	
	while(1)
	{
		SEVEN_SEGMENT_DISPLAY_MLX(i);
		
		for (x=0;x<200;x++)
		{
			if(READ_BIT(PIND,2)==0)
			{
				BUTTON_DELAY(i);
				
				new1++;
				if(new1==10)
				new1=0;
			}
			if(READ_BIT(PIND,3)==0)
			{
				BUTTON_DELAY(i);
				
				new2++;
				if(new2==10)
				new2=0;
			}
			
			i=(new1*10)+new2;
			
			if(READ_BIT(PIND,4)==0)
			{
				BUTTON_DELAY(i);
				
				return i;
			}
		}
	}
	
}

int main(void)
{
	unsigned char flag=0;
	DDRA=0xff;
	DDRB=0xff;
	DDRD=0;
	PORTD=0xff; // << pullup resistor
	DDRC=0xff;
	PORTC=0;
	char x,i=30,new_num=i;
	
	while(1)
	{
		for (x=0;x<200;x++)
		{
			if(READ_BIT(PIND,4)==0)
			{
				BUTTON_DELAY(i);
				
				new_num=increament(i);
				i=new_num;
			}
			
			
			SEVEN_SEGMENT_DISPLAY_MLX(i);
			
			
			if(READ_BIT(PIND,2)==0)
			{
				BUTTON_DELAY(i);
				
				flag++;
			}
			if(READ_BIT(PIND,3)==0)
			{
				BUTTON_DELAY(i);
			
				i=new_num;
			}
			
			
		}
		if(flag%2==0)
		{
			
			i--;
			
			if(i==0)
			{
				finish();
				
				i=30;
				i=new_num;
			}
			
		}
		
	}
}

