#include "stm32f446XX.h"

void turnLedOn();
void turnLedOff();
void blink();
void delay(int delay);
void delayWithCounter(int a);
void output_compare();
void timerDelayInSecond(int a);

int overflowCounter=0;
int limit=0;

int main(){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	
	//enable timer1 clock in APB2
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	
	//LED in A5 as gpio output
	GPIOA->MODER &= ~(3<<10);
	GPIOA->MODER |= (1<<10);
	GPIOA->OTYPER &=~(1<<5);
	GPIOA->OSPEEDR &=~ (3<<10);
	GPIOA->OSPEEDR |= (3<<10);
	GPIOA->PUPDR &=~ (3<<10);
	
	//disable tim1
	TIM1->CR1 &= ~(TIM_CR1_CEN);
	//clear SR
	TIM1->SR = 0x0;
	//set the prescaler
	TIM1->PSC = 39999;
	//set auto reload
	TIM1->ARR = (1000-1);
	//enable counter
	TIM1->CR1 |= (TIM_CR1_CEN);
	//enable timer interrupt in DIER
	TIM1->DIER |=(1<<0);
	
	//enable interrupt
	NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
	
	
	while(1){
		turnLedOff();
		//delayWithCounter(200);
		timerDelayInSecond(1);
		turnLedOn();
		//delayWithCounter(200);
		timerDelayInSecond(1);
	}
}


void timerDelayInSecond(int a){
		overflowCounter=0;
	while(overflowCounter != a);
	
}

void TIM1_UP_TIM10_IRQHandler(){
	if((TIM1->SR & TIM_SR_UIF)!=0){
		overflowCounter++;
	}
	TIM1->SR &= ~TIM_SR_UIF;
}


void delayWithCounter(int a){
	
	//disable tim1
	TIM1->CR1 &= ~(TIM_CR1_CEN);
	//clear SR
	TIM1->SR = 0x0;
	//clear counter
	TIM1->CNT = 0x0;
	//set the prescaler
	TIM1->PSC = 39999;
	//set auto reload
	TIM1->ARR = (1000-1);
	//enable counter
	TIM1->CR1 |= (TIM_CR1_CEN);
	while((TIM1->SR & TIM_SR_UIF)==0){
		//blink(1);
	}
}

void blink(int a){
	turnLedOn();
	delay(a);
	turnLedOff();
	delay(a);
}
void turnLedOn(){
		GPIOA->BSRR |= (1<<5);
}
void turnLedOff(){
	  GPIOA->BSRR |= (1<<(5+16));
}
void delay(int delay){
	for(int i=0; i<delay*1000000; i++);
}

void output_compare(){ //needs further work
	//select PA8 as alt function 1 as its tim1 ch1 output
	GPIOA->MODER &= ~(3<<16);
	GPIOA->MODER |= (3<<16);
	GPIOA->OTYPER &=~(1<<8);
	GPIOA->OSPEEDR &=~ (3<<16);
	GPIOA->OSPEEDR |= (3<<16);
	GPIOA->PUPDR &=~ (3<<16);
	GPIOA->AFR[1]|= (1<<0);
	

//	//set the prescaler
//	TIM1->PSC = 39999;
//	
//	//set auto reload
//	TIM1->ARR = (2000-1);

	//set repetiton counter TIMx_RCR
		//toDo
	
//	//set compare/capture to toggle led in alternate function
//	//TIMx_CCR1
//	//putting a value in this register will compare it
//	//with the timer and if matched then led will toggle
//	//put any value
//		TIM1->CCR1=500;
//		
//	//enable main output enable(moe) in BDTR
//	TIM1->BDTR |= TIM_BDTR_MOE;
//	
//	//reset then select toggle in CCMR1
//	TIM1->CCMR1 &= ~(TIM_CCMR1_OC1M);
//	TIM1->CCMR1 |= (3<<TIM_CCMR1_OC1M_Pos);
//	
//	//select output polarity
//	TIM1->CCER &=~(TIM_CCER_CC1NP);
//	
//	//enable capture compare in TIMx_CCER
//		TIM1->CCER |= TIM_CCER_CC1NE;
		
		
//	//enable tim1(CEN=1) int first bit and dir=0, cms=00
//	TIM1->CR1 = 0x1;
	
}