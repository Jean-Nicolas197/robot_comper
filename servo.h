#ifndef SERVO_H_INCLUDED
#define SERVO_H_INCLUDED
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
	\file servo.h
	\brief Module permetteant de contrôler un servomoteur
	\author Iouri Savard Colbert
	\date 11 janvier 2025 Création du fichier
	\date 30 janvier 2026 Modification pour prendre de 0 à 100
*/

// Inclusions _________________________________________________________________

#include "utils.h"


// Constantes globales publiques ______________________________________________

#define SERVO_INITIAL_RATIO_PD4 50
#define SERVO_INITIAL_RATIO_PD5 50


// Déclaration de fonctions publiques _________________________________________

/**
    \brief Fait l'initialisation nécéssaire pour contrôler un servomoteur. En
	appelant cette fonction, le timer 1 ainsi que les broches PD4 et PD5 deviennent
	utilisées exclusivement par ce module.
    \return Rien
*/
void servo_init(void);

/**
    \brief Change l'angle d'un servomoteur qui serait connecté à PD4. Le paramètre
	ratio doit inclusivement être compris entre 0 et 100. O représente l'angle minimal
	et 100 représente l'angle maximal. Toute valeur entre les deux extrêmes représente
	un angle intermédiaire proportionel à la valeur passée. Une valeur inférieure à 0
	est interprètée comme étant 0. Une valeur supérieure à 100 est interprètée comme 100.
	\param[in]  ratio   Une valeur dans l'interval [0., 100.] réprésentant l'angle désiré
    \return Rien
*/
void servo_ctrl_PD4(float ratio);

/**
    \brief Change l'angle d'un servomoteur qui serait connecté à PD4. Le paramètre
	ratio doit inclusivement être compris entre 0 et 100. O représente l'angle minimal
	et 100 représente l'angle maximal. Toute valeur entre les deux extrêmes représente
	un angle intermédiaire proportionel à la valeur passée. Une valeur inférieure à 0
	est interprètée comme étant 0. Une valeur supérieure à 100 est interprètée comme 100.
	\param[in]  ratio   Une valeur dans l'interval [0., 100.] réprésentant l'angle désiré
    \return Rien
*/
void servo_ctrl_PD5(float ratio);


#endif /* SERVO_H_INCLUDED */