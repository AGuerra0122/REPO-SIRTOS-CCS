/**********************************************************************************************************************
  FileName:        main.c  / Proyect: IO_POINTERS_nortos
  Dependencies:    msp.h, stdint.h
  Processor:       MSP432
  Board:           MSP432P401R
  Program version: CCS V10 TI
  Company:         TecNM /IT Chihuahua
  Description:     UTILIZACION DE APUNTADORES A REGISTROS DE PERIFERICO GPIO.
  Authors:         ALFREDO CHACON
  Created on:      6 mar. 2021
  Updated:         11/2021
  Nota:            Ejemplo basado en IO_POINTERS al cual se incorpora el Simplelink y la carpeta de Drivers
 ************************************************************************************************************************/
//
//  Descripcion; Toggle RGB LED P2.0 P2.1 P2.2 dentro de un loop sensando  PUSH BOTTOM P1.4
//  ACLK = n/a, MCLK = SMCLK = default DCO
//
//                MSP432P4xx
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |             P2.0|-->LED ROJO
//            |             P2.1|-->LED VERDE
//            |             P2.2|-->LED AZUL
//            |                 |
//            |             P1.4|<--PUSH BOTON
//
//
//  TecNM /IT Chihuahua
//  Marzo 2021
//  Built with Code Composer Studio v10.4

/************************************************************************************************
 * * Copyright (C) 2021 by Alfredo Chacon - TecNM /IT Chihuahua
 *
 * Se permite la redistribución, modificación o uso de este software en formato fuente o binario
 * siempre que los archivos mantengan estos derechos de autor.
 * Los usuarios pueden modificar esto y usarlo para aprender sobre el campo de software embebido.
 * Alfredo Chacon y el TecNM /IT Chihuahua no son responsables del mal uso de este material.
 *************************************************************************************************/
#include "DRIVERS/BSP.h"
/*****************************************************************************
     * Function: MAIN
     * Preconditions: NINGUNA.
     * Overview:
     * Input: NINGUNA.
     * Output: NINGUNA
     *
 *****************************************************************************/
int main(void)
{
    /************************************************
       DECLARACION DE VARIABLES
    ************************************************/
    volatile uint32_t i;
    uint8_t bandera = 0;

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // DETIENE EL TIMER DEL WATCHDOG
    /************************************************
        CONFIGURACION DE GPIO
    ************************************************/
    GPIO_setPinEntradaconPullUp(GPIO_PORT_P1,GPIO_PIN4);                 //PIN P1.4 COMO ENTRADA
    GPIO_setPinSalida(GPIO_PORT_P1, GPIO_PIN0);                          // Set P1.0 COMO SALIDA  "ESTO UTILIZA EL DRIVER DECLARADO EN BSP.h"
    GPIO_setPinSalida(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 |GPIO_PIN2);   // CONFIGURA PINES 2.0,2.1,2.2  COMO SALIDA (LEDS RGB)"
    GPIO_setPinBajo(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 |GPIO_PIN2);      // APAGADOS
    while(1)
    {
        GPIO_setPinAlto(GPIO_PORT_P1, GPIO_PIN0);
        if ( Gpio_Pin_in(0x10) != 1)                // ENTRA AL PRESIONAR EL SWITCH DEL PUERTO P1.4 (CUANDO ESTA EN BAJO)
                {
                GPIO_setPinBajo(GPIO_PORT_P1, GPIO_PIN0);
                    if(bandera == FALSE)
                        {
                            bandera = TRUE;                                 //BANDERA EN ALTO
                            while (bandera==TRUE){
                                GPIO_setPinBajo(GPIO_PORT_P2, GPIO_PIN2);    //APAGA BLUE
                                GPIO_setPinAlto(GPIO_PORT_P2,GPIO_PIN0);    //ENCIENDE RED
                                for(i=RETARDO; i>0 ;i--);
                                GPIO_setPinBajo(GPIO_PORT_P2, GPIO_PIN0);    //APAGA RED
                                GPIO_setPinAlto(GPIO_PORT_P2,GPIO_PIN1);    //ENCIENDE GREEN
                                for(i=RETARDO; i>0 ;i--);
                                GPIO_setPinBajo(GPIO_PORT_P2, GPIO_PIN1);    //APAGA GREEN
                                GPIO_setPinAlto(GPIO_PORT_P2,GPIO_PIN2);    //ENCIENDE BLUE
                                for(i=RETARDO; i>0 ;i--);
                             if ( Gpio_Pin_in(0x10) != 1) bandera=FALSE;    // ENTRA CUANDO PRESIONAMOS BOTON DE P1.4
                             while( Gpio_Pin_in(0x10) != 1);                //MIENTRAS ESTE PRESIONADO EL BOTON
                            }
                        }
                 GPIO_setPinBajo(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 |GPIO_PIN2);  //APAGA TODOS
                }
        //for(i=RETARDO; i>0 ;i--);
        _delay_cycles(500000);
    }
}



