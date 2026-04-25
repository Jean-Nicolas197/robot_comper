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
	\file joystick.h
	\brief Permet de faire une lecture de la valeur analogique et du bouton du joystick
	\author --- VOS NOMS ---
	\author --- VOS NOMS ---
	\author --- VOS NOMS ---
	\author --- VOS NOMS ---
	\author --- VOS NOMS ---
	\date   --- LA DATE ---
*/

// Inclusions _________________________________________________________________

#include "joystick.h"

#include <avr/io.h>


// Définition de fonctions publiques __________________________________________

void adc_init(void){
	
	// Désactivation des fonctions numériques des broches du port A qui servent d'entrées analogiques
	
	// Configuration en entrée des broches du port A qui servent d'entrées analogiques
	
	// Sélection de la référence de tension: la tension d'alimentation

	// Choix de format du résultat de conversion: shift à droite pour une conversion entre 0 et 1023

	// Choix du facteur de division de l'horloge
	
	// Activation le CAN
	
}

uint16_t adc_read(uint8_t channel){

	// Sélection de la broche d'entrée à convertir (selon la valeur passée en paramètre)
	ADMUX = write_bits(ADMUX, 0b00011111, channel);
	
	// Démarrage d'une conversion

	// Attente de la fin de conversion

	// Lecture et renvoi du résultat qui est stocké dans le registre ADC
	// RAPPEL : La fonction retourne une valeur d'ADC entre 0 et 1023
	// (Le return 0 est placé ici temporairement pour permettre au code de compiler correctement. Vous devez modifier cette ligne.)
	return 0; 
}


void joystick_init(void){
	
	// Initialisation de l'ADC pour faire la lecture de la position verticale et horizontale du joystick
	
	// Configuration de la broche du bouton (click) du joystick en entrée
	
	// Activation la pull-up du bouton (click) du joystick
	
}

float joystick_get_vertical(void){
	
	// Déclarations des variables
	
	// Lecture de la position verticale du joystick (entre 0 et 1023)
	
	// Conversion de la position verticale du joystick (entre 0 et 1023)
	// en une valeur normalisée entre -100 et +100
	
	// RAPPEL : La fonction retourne la position verticale du joystick entre -100 et +100
	// (Le return 0 est placé ici temporairement pour permettre au code de compiler correctement. Vous devez modifier cette ligne.)
	return 0;
}

float joystick_get_horizontal(void){
	
	// Déclarations des variables
	
	// Lecture de la position horizontale du joystick (entre 0 et 1023)
	
	// Conversion de la position verticale du joystick (entre 0 et 1023)
	// en une valeur normalisée entre -100 et +100
	
	// RAPPEL : La fonction retourne la position horizontale du joystick entre -100 et +100
	// (Le return 0 est placé ici temporairement pour permettre au code de compiler correctement. Vous devez modifier cette ligne.)
	return 0;
}

bool joystick_get_button_state(void){
	
	// (Le return 0 est placé ici temporairement pour permettre au code de compiler correctement. Vous devez modifier cette ligne.)
	return 0;
}

bool joystick_get_click(void){
	
	// Ceci est une variable statique qui conserve sa valeur entre les appels à la fonction
	static bool previous_state = false;
	
	// À chaque fois que la fonction est appelée, on va cherche l'état actuel du bouton
	bool current_state = joystick_get_button_state();
	
	// On crée une variable pour stocker si le bouton vient d'être enfoncé.
	// Par défaut, on considère que ce n'est pas le cas
	bool click_state = false;
	
	// Si le bouton n'était pas enfoncé la dernière fois et que maintenant il l'est
	if(!previous_state && current_state){
		click_state = true;
	}
	
	// On sauvergarde l'état actuel pour le prochain appel avant de quitter la fonction
	previous_state = current_state;
	
	return click_state;
}