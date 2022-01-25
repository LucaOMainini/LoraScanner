/*
 * LoraScanner.c
 *
 *  Created on: 16 gen 2022
 *      Author: Luca
 */

#include "LoraScanner.h"
#include "sys_app.h"
#include "string.h"
#include "stdio.h"
#include "subghz_phy_app.h"


extern UART_HandleTypeDef huart2;

void LoraAnalyzer(void){
	while(1){
		if(RssiScan()==1){
			RssiPlot();
		}
	}
}
uint8_t RssiScan(void){
	uint8_t ret=0;
	if(!Radio.IsChannelFree(RF_FREQUENCY,125000,-105,500)){
		for(int i=0;i<=NUMBER_OF_POINTS;i++){
			Radio.SetChannel(FREQ_START+i*FREQ_STEP);
			Radio.Rx(1);
			Spectrum[i]=Radio.Rssi(MODEM_LORA)-LORA_SENSITIVITY;
			//if(Spectrum[i]>THREASHOLD){ret=1;}
			ret=1;
		}
	}
	return ret;
}
void RssiPlot(void){
#ifdef PLOT
	{
		//APP_LOG(TS_OFF,VLEVEL_H, "PLOT\r\n\r\n[dBm] ");
		HAL_UART_Transmit(&huart2,(uint8_t*)"PLOT\r\n\r\n[dBm] ",14,500);
		int i=0;
		int lim=PLOT_SIZE/6;
		int dF=(FREQ_FINISH-FREQ_START)/lim;
		while(i<lim){
			char s[6]={'\0'};
			sprintf(s,"%d.%d ",(FREQ_START+dF*i)/1000000,(FREQ_START+dF*i)%1000000/100000);
			//APP_LOG(TS_OFF,VLEVEL_H, "%d.%d ",(FREQ_START+dF*i)/1000000,(FREQ_START+dF*i)%1000000/100000);
			HAL_UART_Transmit(&huart2,s,6,500);
			i++;
		}
		//APP_LOG(TS_OFF,VLEVEL_H, "\r\n");
		HAL_UART_Transmit(&huart2,(uint8_t*)"\r\n",2,500);
	}
	int max=arrayMax(Spectrum,sizeof(Spectrum)/sizeof(Spectrum[0]));
	int min=arrayMin(Spectrum,sizeof(Spectrum)/sizeof(Spectrum[0]));
	int jump=(NUMBER_OF_POINTS/PLOT_SIZE);
	for(int k=0;k<(max-min);k++){
		if(max-k>-100){
			//APP_LOG(TS_OFF,VLEVEL_H, "% d",max-k);
			char s[4];
			sprintf(s," %d",max-k);
			HAL_UART_Transmit(&huart2,s,4,500);
		}else{
			//APP_LOG(TS_OFF,VLEVEL_H, "%d",max-k);
			char s[4];
			sprintf(s,"%d",max-k);
			HAL_UART_Transmit(&huart2,s,4,500);
		}
		//for(int i=0;1<PLOT_SIZE;i++){
			char string[PLOT_SIZE+5]= " ";
			for(int j=0;j<PLOT_SIZE;j++){
				int16_t x=arrayMax(Spectrum+j*jump,jump);
				//int16_t x=Spectrum[j];
				if(x>=max-k){strcat(string,"*");}
				else{strcat(string," ");}
			}
			strcat(string,"\r\n");
			//APP_LOG(TS_OFF,VLEVEL_H, string);
			HAL_UART_Transmit(&huart2,(uint8_t*)string,strlen(string),500);
			//HAL_Delay(80);
		//}
	}
#else
	for(int i=0;i<=NUMBER_OF_POINTS;i++){
		APP_LOG(TS_OFF,VLEVEL_H, "%d,%d\r\n",(FREQ_START+i*FREQ_STEP)/1000,Spectrum[i]);
		//APP_LOG(TS_OFF,VLEVEL_H, "%d\r\n",Spectrum[i]);
		HAL_Delay(5);
	}
#endif
	//HAL_Delay(500);
}


int16_t arrayMax(int16_t *arr, int n){
	int16_t max=arr[0];
	//for(int i=0;i<sizeof(arr) / sizeof(arr[0]);i++){
	for(int i=0;i<n;i++){
			if(arr[i]>max){max=arr[i];}
	}
	return max;
}
int16_t arrayMin(int16_t *arr, int n){
	int16_t min=arr[0];
	//for(int i=0;i<sizeof(arr) / sizeof(arr[0]);i++){
	for(int i=0;i<n;i++){
		if(arr[i]<min){min=arr[i];}
	}

	return min;
}
int16_t arrayMean(int16_t *arr, int n){
	int mean=0;
	for(int i=0;i<n;i++){
		mean=mean+arr[i];
	}
	mean=mean/n;
	return (int16_t)mean;
}
