/*
 * robot.c
 *
 * Created: 2026-03-09 4:08:04 PM
 * Author : jndeb
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>


#include "utils.h"
#include "lcd.h"
#include "joystick.h"
#include "motor.h"
#include "uart.h"
#include "pid.h"
#include "servo.h"

int communication(int *x, int *y, int *joystick, int *speed_mode, int *slider);
void lab4C_exercice_3_et_4(void);
void servo();



int main(void){
	
	// Mettre la broche du reset du module Wifi en sortie
	DDRA = set_bit(DDRA, PA6);
	// Mettre la broche du reset du module Wifi à 0 pour forcer un reset
	PORTA = clear_bit(PORTA, PA6);
	// Attendre 200 ms
	_delay_ms(200);
	// Mettre la broche du reset du module Wifi à 1 pour forcer un démarrage
	PORTA = set_bit(PORTA, PA6);
	// Initialisation du joystick pour faire une lecture du bouton

	joystick_init();

	// Initialisation des moteurs

	motor_init();

	// Initialisation du UART0

	uart0_init();

	// Initialisation du module de calcul de PID

	pid_init();
	servo_init();
	
	// Activation des interruptions
	sei();
	
	lab4C_exercice_3_et_4();
}


void lab4C_exercice_3_et_4(void){

	
	
	

	// Gains de l'exercice 3

	//const float K_P = 0.6366;

	//const float K_I = 0.0;

	//const float K_D = 0.0;

	//const float FIX_RATE = 0.0;

	// Gains de l'exercice 4

	const float K_P = 3.15;

	const float K_I = 9;

	const float K_D = 0.12;

	const float FIX_RATE = 0.05;

	pid_set_gains(K_P, K_I, K_D);

	pid_set_fix_rate(FIX_RATE);

	// Activation des interruptions
	lcd_init();
	lcd_clear_display();
	int speed_controller_x=0;
	int speed_controller_y=0;
	int speed_mode=3;
	int joystick_state=0;
	int slider=0;
	while(true){
		
		float speed_modes = speed_mode*0.1;
		/*
		controller_speed = communication();
		printf("%d", controller_speed);
		motor_ctrl_left(controller_speed);
		motor_ctrl_right(controller_speed);
		*/
		
		// Si un échantillon est prêt pour le calcul du PID
		
		if(pid_is_new_data_ready()){

			// Obtention de l'angle actuel

			float theta_rad = pid_get_current_angle();
			float theta_deg = theta_rad * 57;
			int theta_int = (int)(theta_deg* 100);
			// Calcul du PID
			
			float speed = pid_tic(0, theta_rad);
			
			
			communication(&speed_controller_x, &speed_controller_y, &joystick_state, &speed_mode, &slider);
			
			if(joystick_state){
				pid_reset();
			}
			
			float servo = (0.0537*slider) + 35;
			
			lcd_clear_display();
			
			printf("%d",slider);
			
			motor_ctrl_right(1.15*(-speed- speed_modes*(speed_controller_y-0.5*speed_controller_x)));
			motor_ctrl_left((-speed- speed_modes*(speed_controller_y+0.5*speed_controller_x)));
			
			servo_ctrl_PD5(servo);
			
			
		

			// Conversion de l'angle final en radian en un angle en degrés pour affichage

			
			
			
			/***************************************************************************/

		}

		// Si un clic est détecté sur le joysitck

		if(joystick_get_click()){

			// Remettre le module PID dans son état initial

			pid_reset();

		}

	}

}





int communication(int *x, int *y, int *joystick, int *speed_mode, int *slider){
			int joystick_state;
			int speed_controller_x;
			int speed_controller_y;
			int speed_number;
			int slider_received;
		// S'il y a réception d'une nouvelle ligne
			
			char ligne[100];
			
			// Récupérer la ligne
			uart0_get_line(ligne,100);
			
			// Convertir le contenu de la ligne en une valeur entière avec sscanf
			sscanf(ligne, "x=%d y=%d j=%d s=%d r=%d",&speed_controller_x, &speed_controller_y, &joystick_state, &speed_number, &slider_received);
			
			// return la vitesse du moteur gauche
			*x = speed_controller_x;
			*y = speed_controller_y;
			*joystick = joystick_state;
			*speed_mode = speed_number;
			*slider = slider_received;

		// ATTENTION! Il ne devrait y avoir aucun délai dans la boucle de réception.
	
}

void servo(){
	
}