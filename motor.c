/*	 __ ___  __
	|_   |  (_
	|__  |  __)

	MIT License

	Copyright (c) 2024	École de technologie supérieure

	La présente autorise, de façon libre et gratuite, à toute personne obtenant
	une copie de ce programme et des fichiers de documentation associés (le "Programme"),
	de distribuer le Programme sans restriction, y compris sans limitation des droits
	d'utiliser, copier, modifier, fusionner, publier, distribuer, sous-autoriser ou vendre
	des copies du Programme, et de permettre aux personnes à qui le Programme est fourni
	d'en faire autant, aux conditions suivantes.

	Le copyright précédent et cette autorisation doivent être distribués dans toute
	copie entière ou substantielle de ce Programme.

	Le Programme est fourni en l'état, sans garantie d'aucune sorte, explicite ou
	implicite, y compris les garanties de commercialisation ou d'adaptation dans
	un but particulier et l'absence de contrefaçon. En aucun cas les auteurs ou
	ayants droit ne seront tenus responsables de réclamations, dommages ou autres,
	que ce soit dans une action de nature contractuelle, préjudiciable ou autres façons,
	découlant de, hors ou en connexion avec le Programme ou l'utilisation ou autres
	modifications du Programme.
*/
/**
	\file motor.c
	\brief Permet de contrôler la vitesse et la direction d'un moteur connecté à
	un pont en H
	\author --- Jean-nicolas de Broeck ---
	\author --- VOS NOMS ---
	\author --- VOS NOMS ---
	\author --- VOS NOMS ---
	\author --- VOS NOMS ---
	\date   --- LA DATE ---
*/

// Inclusions _________________________________________________________________

#include "motor.h"

#include <avr/io.h>


// Définition de fonctions publiques __________________________________________

void pwm0_init(void){

	// Configuration des broches de la modulation de largeur d'impulsion en sortie
	DDRB = set_bits(DDRB, 0b00011111);
	// Choix du mode de comparaison pour le comparateur A: 
	
	TCCR0A = set_bit(TCCR0A, COM0A1);
	TCCR0A = clear_bit(TCCR0A, COM0A0);
	// "Clear on Compare Match when up-counting. Set on Compare Match when down-counting."
	TCCR0A = set_bit(TCCR0A, WGM00);
	TCCR0B = clear_bit(TCCR0B, WGM02);
	TCCR0A = clear_bit(TCCR0A, WGM01);
	
	// Choix du mode de comparaison pour le comparateur B: 
	TCCR0A = set_bit(TCCR0A, COM0B1);
	TCCR0A = clear_bit(TCCR0A, COM0B0);
	// "Clear on Compare Match when up-counting. Set on Compare Match when down-counting."
	
	// Choix du mode du compteur :  "PWM phase correct (avec valeur TOP égale à 255)"
	
	
	// Choix de l'horloge interne sans facteur de division de fréquence
	TCCR0B = clear_bits(TCCR0B, 0b00000110);
	TCCR0B = set_bit(TCCR0B, CS00);
}

void pwm0_ctrl_PB3(uint8_t compare_value){

	// Choix du rapport cyclique en fixant la valeur de comparaison
	OCR0A = compare_value;
	
}


void pwm0_ctrl_PB4(uint8_t compare_value){

	// Choix du rapport cyclique en fixant la valeur de comparaison
	OCR0B = compare_value;
	
}


void motor_init(void){
	
	// Cette fonction permet de faire toutes les initialisations nécessaires à l’utilisation 
	// des moteurs connectés à des ponts en H (les 2 roues)
	// Pour le moment, vous n'avez que le moteur gauche donc vous devez faire les 
	// initialisations pour le moteur gauche seulement : je viens de rajouter 1 et 3
	DDRB = set_bits(DDRB, 0b00011110);
	
	// Configuration de la broche de direction (PH du H-bridge) du pont en h du moteur de gauche en sortie
	
	// Initialisation du PWM 0 pour le contrôle des moteurs de gauche et droite
	pwm0_init();
	
	
	// Mise à 0 de la vitesse initiale du moteur de gauche
	
}

void motor_ctrl_right(float speed){
	// RAPPEL : La vitesse reçue en paramètre est entre -100 et +100
	
	// Déclaration des variables
	float temp;
	int rapport_cyclique = 0;
	
	
	if (speed>0)
	{
		PORTB = set_bit(PORTB, PB1);
		
		if (speed>100)
		{
			temp = 255.0;
		}
		else{
			temp = 2.45*speed+15;
		}
	}
	
	// Conversion du paramètre speed en un rapport cyclique entre 0 et 255
	// Contrôle de la broche de direction pour le sens horaire
	
	//quelle broche??
	
	if (speed<0)
	{
		PORTB = clear_bit(PORTB, PB1);
		if (speed<-100)
		{
			temp = 255.0;
		}
		else{
			temp = -2.45*speed+15;
		}
		
	}
	// Si la vitesse (speed) est égale ou inférieur à -100,
	// Ajustement du rapport cyclique à sa valeur maximale (255)
	// Contrôle de la broche de direction pour le sens anti-horaire
	
	// Si la vitesse (speed) est entre 0 et -100,
	// Conversion du paramètre speed en un rapport cyclique entre 0 et 255
	// Contrôle de la broche de direction pour le sens anti-horaire
	
	// Si la vitesse (speed) est nulle,
	if (speed == 0)
	{
		rapport_cyclique = 127;
	}
	// Ajustement du rapport cyclique à sa valeur minimale (0)
	
	// Envoi du rapport cyclique au moteur de droite (doit être entre 0 et 255)
	rapport_cyclique = temp;
	pwm0_ctrl_PB3(rapport_cyclique);

	}


void motor_ctrl_left(float speed){
	
	
	// RAPPEL : La vitesse reçue en paramètre est entre -100 et +100
	
	// Déclaration des variables
	float temp;
	int rapport_cyclique = 0;
	
	
	if (speed>0)
	{
		PORTB = set_bit(PORTB, PB2);
		
		if (speed>100)
		{
			temp = 255.0;
		}
		else{
			temp = 2.45*speed+15;
		}
	}
	
	// Conversion du paramètre speed en un rapport cyclique entre 0 et 255
	// Contrôle de la broche de direction pour le sens horaire
	
	//quelle broche??
	
	if (speed<0)
	{
		PORTB = clear_bit(PORTB, PB2);
		if (speed<-100)
		{
			temp = 255.0;
		}
		else{
			temp = -2.45*speed+15;
		}
		
	}
	// Si la vitesse (speed) est égale ou inférieur à -100,
	// Ajustement du rapport cyclique à sa valeur maximale (255)
	// Contrôle de la broche de direction pour le sens anti-horaire
	
	// Si la vitesse (speed) est entre 0 et -100,
	// Conversion du paramètre speed en un rapport cyclique entre 0 et 255
	// Contrôle de la broche de direction pour le sens anti-horaire
	
	// Si la vitesse (speed) est nulle,
	if (speed == 0)
	{
		rapport_cyclique = 127;
	}
	// Ajustement du rapport cyclique à sa valeur minimale (0)
	
	// Envoi du rapport cyclique au moteur de droite (doit être entre 0 et 255)
	rapport_cyclique = temp;
	pwm0_ctrl_PB4(rapport_cyclique);

}