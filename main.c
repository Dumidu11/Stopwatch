#include<avr/io.h>
#include<avr/interrupt.h>
#include <util/delay.h>


#define SSD_DATA PORTC
#define SSD_DATA_DDR DDRC

#define SSD_DIG_DDR DDRA
#define SSD_DIG PORTA

#define dig_delay _delay_ms(1)
#define dig_delay1 _delay_ms(10)

#define switch_DDR DDRD
#define Switch_PORT PORTD
#define Switch_PIN PIND

#define DIG0 0
#define DIG1 1
#define DIG2 2
#define DIG3 3
#define DIG4 4
#define DIG5 5
#define DIG6 6
#define DIG7 7

void init_ssd(void);
void ssd_display(void);
void split_data( unsigned int i, unsigned int k,unsigned int n,unsigned int m);

unsigned char ssd[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
unsigned char ssd_dig[]={0, 0, 0, 0};

volatile unsigned char data = 0xff;
volatile int i =0,k=0,n=0,m=0,button_prs=1;


int main(void){

    init_ssd();
	SSD_DATA_DDR=0xff;
	TIMSK=1<<OCIE0;
	OCR0=124;
	
	
	switch_DDR &=~(1<<DIG2);
	Switch_PIN&=~(1<<DIG2); //D bouncing
	
	GICR |=1<<INT0 | 1<<INT1;
    MCUCR|=1<<ISC01 | 1<<ISC00 | 1<<ISC11 | 1<<ISC10;//ricing  edge
	sei();
	
	
	while(1){
		
			split_data(i,k,n,m);
			ssd_display();	
	    }
     }

ISR(TIMER0_COMP_vect){
   
   
    if( i>=1000){
   
		i=0; //milisecond
		
	    if(k>=60){
		  
		    k=0; //second
			
			if(n>=60){
			
			   n=0; //minite
			   
			   if(m>=24){
			   
			      m=0;
			   }
			   
			   else{
			   
			    m++;
			   }
			}
			
			else{
			
			  n++;
			
			}
		  
		}
		
		else{
		
			k++;
		}
	
	}
	
	else{
	
       i++; 	
  }
	
		
}

ISR(INT0_vect){

	if(button_prs ==0){
	    
		TCCR0=1<<CS01 | 1<<WGM01 |1<<FOC0;//mode set amd prescaler set
		button_prs =1;
	}
	else{
	
		button_prs =0;
		TCCR0 &=~(1<<CS01);
	}	
}

ISR(INT1_vect){

    i=0;//milisec
	k=0;//sec
	n=0;//minite
	m=0;//hour

}


void init_ssd(void){
  
	  SSD_DATA_DDR = 0xff;
	  SSD_DIG_DDR = SSD_DIG_DDR | 1<<DIG0 | 1<<DIG1 | 1<<DIG2 | 1<<DIG3| 1<<DIG4| 1<<DIG5| 1<<DIG6 | 1<<DIG7;
	  SSD_DATA = 0x00;
	  SSD_DIG &=~(1<<DIG0 | 1<<DIG1 | 1<<DIG2 | 1<<DIG3| 1<<DIG4| 1<<DIG5| 1<<DIG6| 1<<DIG7);
	  
}

void ssd_display(void){

	SSD_DATA=ssd[ssd_dig[0]]|0x80;
	//PORTA=ssd[5];
	SSD_DIG=1<<DIG4;
	dig_delay;
	SSD_DIG&=~(1<<DIG4);
	SSD_DATA=  (1<<DIG7);
 
	SSD_DATA=ssd[ssd_dig[1]];
	//PORTA=ssd[4];
	SSD_DIG=1<<DIG5;
	 dig_delay;
	 SSD_DIG&=~(1<<DIG5);
	
	SSD_DATA=ssd[ssd_dig[2]]|0x80;
	//PORTA=ssd[5];
	SSD_DIG=1<<DIG6;
	  dig_delay;
	  SSD_DIG&=~(1<<DIG6);
	  SSD_DATA=  (1<<DIG7);
	 
	SSD_DATA=ssd[ssd_dig[3]];
	//PORTA=ssd[9];
	SSD_DIG=1<<DIG7;
	 dig_delay;
	 SSD_DIG&=~(1<<DIG7);
 
}

void split_data( unsigned int i, unsigned int k,unsigned int n,unsigned int m) {
	
	if(n<10){
	
		ssd_dig[3]=i/100;
		ssd_dig[2]=k%10;
		ssd_dig[1]=k/10;
		ssd_dig[0]=n%10;
	
	}
	
	else{
	
	    ssd_dig[3]= k/10;
		ssd_dig[2]=n%10;
		ssd_dig[1]=n/10;
		ssd_dig[0]=m/10;
		
	}
	
}


