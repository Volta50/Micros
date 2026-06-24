/* 
 * File:   LibLCDXC8.h
 * Author: Robin
 *
 * Created on 2 de septiembre de 2018, 08:15 PM
 * Modificado: funciones vacias implementadas (MensajeLCD_Var, FijaCursorLCD, etc.)
 */

#ifndef LIBLCDXC8_H
#define	LIBLCDXC8_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif
#include<xc.h>
#ifndef _XTAL_FREQ
#define _XTAL_FREQ 20000000
#endif
#ifndef Datos
#define Datos LATD	//El puerto de conexión de los datos el cual se puede cambiar
#endif
#ifndef RS
#define RS LATD0	//Los pines de control al LCD los cuales se
#endif
#ifndef E
#define E LATD1	//pueden cambiar
#endif

unsigned char interfaz=8;

unsigned char InvierteNibbleAlto(unsigned char a){
    unsigned char n = (a >> 4) & 0x0F;
    unsigned char r = 0;
    if(n & 0x01) r |= 0x08;
    if(n & 0x02) r |= 0x04;
    if(n & 0x04) r |= 0x02;
    if(n & 0x08) r |= 0x01;
    return (unsigned char)((r << 4) | (a & 0x0F));
}

void ConfiguraLCD(unsigned char);
void RetardoLCD(unsigned char);
void EnviaDato(unsigned char);
void InicializaLCD(void);
void HabilitaLCD(void);
void BorraLCD(void);
void CursorAInicio(void);
void ComandoLCD(unsigned char);
void EscribeLCD_c(unsigned char);
void EscribeLCD_n8(unsigned char, unsigned char);
void EscribeLCD_n16(unsigned int, unsigned char);
void EscribeLCD_d(double, unsigned char, unsigned char);
void MensajeLCD_Var(char *);
void DireccionaLCD(unsigned char);
void FijaCursorLCD(unsigned char,unsigned char);
void DesplazaPantallaD(void);
void DesplazaPantallaI(void);
void DesplazaCursorD(void);
void DesplazaCursorI(void);

void ConfiguraLCD(unsigned char a){
	if(a==4 | a ==8)
		interfaz=a;	
}
void EnviaDato(unsigned char a){
	if(interfaz==4){
		unsigned char dato;
		dato = InvierteNibbleAlto(a);
		Datos=(Datos & 0b00001111) | (dato & 0b11110000);
		HabilitaLCD();
		RetardoLCD(1);
		dato = InvierteNibbleAlto((unsigned char)(a<<4));
		Datos=(Datos & 0b00001111) | (dato & 0b11110000);
		//HabilitaLCD();
		//RetardoLCD(1);
	}else if(interfaz==8){
		Datos=a;
	}	
}
void InicializaLCD(void){
//Función que inicializa el LCD caracteres
	RS=0;
	if(interfaz==4)
		Datos=(Datos & 0b00001111) | (InvierteNibbleAlto(0x30) & 0xF0);
	else	
		Datos=0x3F;
	HabilitaLCD();
	RetardoLCD(1);
	if(interfaz==4)
		Datos=(Datos & 0b00001111) | (InvierteNibbleAlto(0x30) & 0xF0);
	else	
		Datos=0x3F;
	HabilitaLCD();
	RetardoLCD(3);
	if(interfaz==4)
		Datos=(Datos & 0b00001111) | (InvierteNibbleAlto(0x30) & 0xF0);
	else	
		Datos=0x3F;
	HabilitaLCD();
	RetardoLCD(4);
	if(interfaz==4){
		Datos=(Datos & 0b00001111) | (InvierteNibbleAlto(0x20) & 0xF0);
		HabilitaLCD();
		RetardoLCD(4);
		EnviaDato(0x2F);
		HabilitaLCD();
		RetardoLCD(4);
	}
	BorraLCD();
	EnviaDato(0xF);
	HabilitaLCD();
	RetardoLCD(4);	
}
void HabilitaLCD(void){
//Función que genera los pulsos de habilitación al LCD 	
	E=1;
	__delay_us(40);
    //Delay1TCY();
	E=0;
}
void BorraLCD(void){
//Función que borra toda la pantalla	
	RS=0;
	EnviaDato(0x01);
	HabilitaLCD();
	RetardoLCD(2);
}
void CursorAInicio(void){
//Función que lleva el cursor a la primera posición (Home).
//Comando 0x02 del HD44780.
	RS=0;
	EnviaDato(0x02);
	HabilitaLCD();
	RetardoLCD(2);     // Home necesita ~1.64 ms igual que Clear
}	
void ComandoLCD(unsigned char a){
//Función que envia cualquier comando al LCD
	RS=0;
	if(a==1)
		BorraLCD();
	else if((a&0b11111110)==2)	
		CursorAInicio();
	else{	
		EnviaDato(a);
		HabilitaLCD();
		RetardoLCD(4);
	}		
}	
void EscribeLCD_c(unsigned char a){
//Función que escribe un caracter en la pantalla
//a es un valor en codigo ascii
//Ejemplo EscribeLCD_c('A');
	RS=1;
	EnviaDato(a);
	HabilitaLCD();
	RetardoLCD(4);
}
void EscribeLCD_n8(unsigned char a,unsigned char b){
//Función que escribe un número positivo de 8 bits en la pantalla
//a es el número a escribir, el cual debe estar en el rango de 0 a 255
//b es el número de digitos que se desea mostrar empezando desde las unidades
//Ejemplo EscribeLCD_n8(204,3);	
    unsigned char centena,decena,unidad;
	RS=1;
	switch(b){
		case 1: unidad=a%10;
				EnviaDato(unidad+48);
				HabilitaLCD();
				RetardoLCD(4);
				break;
		case 2:	decena=(a%100)/10;
				unidad=a%10;
				EnviaDato(decena+48);
				HabilitaLCD();
				RetardoLCD(4);
				EnviaDato(unidad+48);
				HabilitaLCD();
				RetardoLCD(4);
				break;
		case 3: centena=a/100;
				decena=(a%100)/10;
				unidad=a%10;
				EnviaDato(centena+48);
				HabilitaLCD();
				RetardoLCD(4);
				EnviaDato(decena+48);
				HabilitaLCD();
				RetardoLCD(4);
				EnviaDato(unidad+48);
				HabilitaLCD();
				RetardoLCD(4);
				break;
		default: break;
	}
}
void EscribeLCD_n16(unsigned int a,unsigned char b){
//Función que escribe un número positivo de 16 bits en la pantalla
//a es el número a escribir, el cual debe estar en el rango de 0 a 65535
//b es el número de digitos que se desea mostrar empezando desde las unidades
//Ejemplo EscribeLCD_n16(12754,5);	
    unsigned char decena,unidad;
	unsigned int centena,millar;
    RS=1;
	switch(b){
		case 1: unidad=a%10;
                EscribeLCD_c(unidad+48);
				break;
		case 2:	decena=(a%100)/10;
				unidad=a%10;
				EscribeLCD_c(decena+48);
                EscribeLCD_c(unidad+48);
				break;
		case 3: centena=(a%1000)/100;
                decena=(a%100)/10;
				unidad=a%10;
                EscribeLCD_c(centena+48);
				EscribeLCD_c(decena+48);
                EscribeLCD_c(unidad+48);
				break;
		case 4: millar=(a%10000)/1000;
                centena=(a%1000)/100;
                decena=(a%100)/10;
				unidad=a%10;
                EscribeLCD_c(millar+48);
				EscribeLCD_c(centena+48);
				EscribeLCD_c(decena+48);
                EscribeLCD_c(unidad+48);
				break;
		case 5: EscribeLCD_c(a/10000 +48);
				millar=(a%10000)/1000;
                centena=(a%1000)/100;
                decena=(a%100)/10;
				unidad=a%10;
                EscribeLCD_c(millar+48);
				EscribeLCD_c(centena+48);
				EscribeLCD_c(decena+48);
                EscribeLCD_c(unidad+48);
				break;
		default: break;
	}	
}
void EscribeLCD_d(double num, unsigned char digi, unsigned char digd){
//Función que escribe un número con decimales.
//num: número a mostrar; digi: dígitos de la parte entera;
//digd: dígitos de la parte decimal.
//Ejemplo EscribeLCD_d(3.1416, 1, 4);  -> "3.1416"
	unsigned long entero;
	unsigned long fraccion;
	unsigned char i;
	unsigned long factor;

	RS=1;
	if(num < 0){
		EscribeLCD_c('-');
		num = -num;
	}

	entero = (unsigned long)num;

	// Parte entera con la cantidad de dígitos pedida (digi)
	// Construimos el divisor 10^(digi-1)
	factor = 1;
	for(i=1; i<digi; i++) factor *= 10;
	while(factor > 0){
		EscribeLCD_c((unsigned char)((entero / factor) % 10) + 48);
		factor /= 10;
	}

	if(digd > 0){
		EscribeLCD_c('.');
		// Calculamos 10^digd para extraer la fracción
		factor = 1;
		for(i=0; i<digd; i++) factor *= 10;
		fraccion = (unsigned long)((num - (double)entero) * (double)factor + 0.5);

		// Imprimimos la fracción con ceros a la izquierda
		factor /= 10;
		while(factor > 0){
			EscribeLCD_c((unsigned char)((fraccion / factor) % 10) + 48);
			factor /= 10;
		}
	}
}
void MensajeLCD_Var(char* a){
//Función que escribe una cadena de caracteres variable en la pantalla
//a es una cadena de caracteres guardada en una variable *char
//Ejemplo: char aux[]="Hola"; MensajeLCD_Var(aux);
	while(*a){
		EscribeLCD_c((unsigned char)(*a));
		a++;
	}
}
void DireccionaLCD(unsigned char a){
//Función que ubica el cursor en una posición especificada
//a debe ser una dirección de 8 bits valida de la DDRAM o la CGRAM	
	RS=0;
	EnviaDato(a);
	HabilitaLCD();
	RetardoLCD(4);
}
void FijaCursorLCD(unsigned char fila,unsigned char columna){
//Función que ubica el cursor en una fila y columna especificadas
//fila: 1 a 4 ; columna: 1 en adelante.
//Calcula la dirección DDRAM según el mapa estándar del HD44780.
	unsigned char dir;
	switch(fila){
		case 1:  dir = 0x00; break;
		case 2:  dir = 0x40; break;
		case 3:  dir = 0x14; break;   // 20 columnas: fila3 inicia en 0x14
		case 4:  dir = 0x54; break;   // fila4 inicia en 0x54
		default: dir = 0x00; break;
	}
	dir += (columna - 1);
	DireccionaLCD(0x80 | dir);        // 0x80 = set DDRAM address
}
void DesplazaPantallaD(void){
//Función que desplaza una sola vez la pantalla a la derecha	
  RS=0;
  EnviaDato(0x1C);                    // Display shift right
  HabilitaLCD();
  RetardoLCD(4);	
}
void DesplazaPantallaI(void){
//Función que desplaza una sola vez la pantalla a la izquierda
	RS=0;
	EnviaDato(0x18);                   // Display shift left
	HabilitaLCD();
	RetardoLCD(4);
}
void DesplazaCursorD(void){
//Función que desplaza una sola vez el cursor a la derecha
	RS=0;
	EnviaDato(0x14);                   // Cursor shift right
	HabilitaLCD();
	RetardoLCD(4);
}
void DesplazaCursorI(void){
//Función que desplaza una sola vez el cursor a la izquierda
	RS=0;
	EnviaDato(0x10);                   // Cursor shift left
	HabilitaLCD();
	RetardoLCD(4);
}		
void RetardoLCD(unsigned char a){
	switch(a){
		case 1: __delay_ms(15);
                //Delay100TCYx(38); //Retardo de mas de 15 ms
				break;
		case 2: __delay_ms(1);
                __delay_us(640);
                //Delay10TCYx(41); //Retardo de mas de 1.64 ms
				break;
		case 3: __delay_us(100);
                //Delay10TCYx(3);	//Retardo de mas de 100 us
				break;
		case 4: __delay_us(40);
                //Delay10TCYx(1); //Retardo de mas de 40 us
				break;
		default:
				break;
	}
}
#endif	/* LIBLCDXC8_H */
