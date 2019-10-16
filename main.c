/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.145.0
        Device            :  PIC32MM0256GPM064
    The generated drivers are tested against the following:
        Compiler          :  XC32
        MPLAB 	          :  MPLAB X v5.25
*/

/*
    (c) 2019 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/


#include "mcc_generated_files/system.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "delay.h"
#include "ili9341.h"
#include "mcc_generated_files/fatfs/ff.h"
#include "mcc_generated_files/fatfs/diskio.h"
#include "mcc_generated_files/fatfs/integer.h"


FATFS FatFs;		/* File system object */
FIL File[2];	    /* File objects */
TCHAR Buff[16];    /* Working buffer */
UINT s1,s2;
FRESULT res; 

void DrawBitmapRGB565_withSDcard(const TCHAR* nazwa_pliku);
void read_error_handle(FRESULT err);
void open_error_handle(FRESULT err);

int main(void)
{
    uint16_t data;
    // initialize the device
    SYSTEM_Initialize();
    ili9341_init(); // init LCD
    setOrientation(PORTRAIT_REV); //PORTRAIT, LANDSCAPE, PORTRAIT_REV, LANDSCAPE_REV
    putString("Test ILI9341", 30, 30, 2, YELLOW); 
    
    /*Mount device*/	
	if(f_mount(&FatFs, "", 0)!=FR_OK) {
		/*error handling*/
        putString("f_mount error!", 1, 60, 2, RED); 
		while(1);
	}
    else {
        putString("f.mount OK.!", 1, 60, 2, GREEN); 
        delayMs(1000);
    }
    
    /*Initialize drive*/
	DSTATUS driveStatus = disk_initialize(0);
    if(!driveStatus){
       putString("disk init OK.!", 1, 90, 2, GREEN); 
       delayMs(1000);
    }
    else {
       /*error handling*/
       putString("disk init error", 1, 90, 2, RED);
       putString("error number :", 1, 120, 2, RED); 
       putInteger(driveStatus, 180, 120, 2, YELLOW); 
       while(1);
    }
       
     DrawBitmapRGB565_withSDcard("fiat.bmp");
        
    /*Unmount device*/
        f_mount(0, "", 0); 
          
        while (1)
    {
       
    }
   
}


void DrawBitmapRGB565_withSDcard(const TCHAR* nazwa_pliku)
{
uint32_t i = 0, j = 0, liczba_pikseli = 0, liczba_bajtow =0;
uint16_t piksel;
uint8_t temp[4];
UINT ile_bajtow;
FIL plik;

setOrientation(LANDSCAPE_REV); //PORTRAIT, LANDSCAPE, PORTRAIT_REV, LANDSCAPE_REV
setWindow(0, 0, 320 - 1, 240 - 1); //LCD ILI9341 320x240
/*Otwarcie do odczytu pliku bitmapy*/
res = f_open (&plik, nazwa_pliku, FA_READ);
open_error_handle(res);
/*Opuszczenie dwoch pierwszych bajtow*/
res = f_read (&plik, &temp[0], 2, &ile_bajtow);
read_error_handle(res);
/*rozmiar pliku w bajtach*/
res = f_read (&plik, (uint8_t*) &liczba_bajtow, 4, &ile_bajtow);
read_error_handle(res);
/*Opuszczenie 4 bajtow*/
res = f_read (&plik, &temp[0], 4, &ile_bajtow);
read_error_handle(res);
/*Odczytanie przesuniecia (offsetu) od poczatku pliku do poczatku bajtow opisujacych obraz*/
res = f_read (&plik, (uint8_t*) &i, 4, &ile_bajtow);
read_error_handle(res);
/*Opuszczenie liczby bajtow od aktualnego miejsca do poczatku danych obrazu, wartosc 14, bo odczytane zostalo juz z pliku 2+4+4+4=14 bajtow*/
for(j = 0; j < (i - 14); j++)
res = f_read (&plik, &temp[0], 1, &ile_bajtow);
read_error_handle(res);
/*Liczba pikseli obrazu = (rozmiar pliku - offset)/2 bajty na piksel*/
liczba_pikseli = (liczba_bajtow - i)/2;

/*Odczyt bajtow z karty SD i wyslanie danych do LCD*/
for(i = 0; i < liczba_pikseli; i++){
    res = f_read (&plik, (uint8_t*) &piksel, 2, &ile_bajtow);
    read_error_handle(res);
    sendData16(piksel);
}

/*Close file*/ 
res = f_close (&plik);
}

void read_error_handle(FRESULT err){
   if(err) {
         /*error handling*/
         putString("read file error", 1, 120, 2, RED);
         while(1);
            } 
}

void open_error_handle(FRESULT err){
   if(err) {
         /*error handling*/
         putString("open file error", 1, 120, 2, RED);
         while(1);
            } 
}


              