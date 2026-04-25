#ifndef LCD_H_INCLUDED
#define LCD_H_INCLUDED
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
	\file
	\brief Driver pour un affichage LCD piloté par un HD44780
	\author Iouri Savard Colbert
	\date 28 avril 2014 Création du fichier
	\date 18 septembre 2024 Ajout de la redirection de stdout vers le LCD
	\date 19 septembre 2024 Meilleure gestion du \n
	\date 4 octobre 2024 Ajout du mode 4 bit pour libérer les broches du i2c

	Ce module est divisé en deux sous-modules :
    - hd44780
    - lcd


    hd44780 :

    Ce sous- module fournit des fonctions pour accomplir les fonctionnalités
    matérielle fournies par le circuit du LCD.  Autrement dit, elles permettent à un
    utilisateur à ne pas avoir à générer manuellement des codes binaires pour faire
    fonctionner ce dernier. Les fonctions de ce module sont très rapides
    et peu gourmandes en mémoire. Par contre, elles sont limitées en
    fonctionnalité.  Les deux limitations majeures sont l'absence de relation
    avec l'affichage réel et l'impossibilité de connaître la position actuelle
    du curseur. Ces fonctionnalités sont ajoutées par le module englobant "lcd".


    lcd :

    Permet d'accomplir les mêmes tâches que le pilote directement, mais pallie
    par logiciel aux limitations majeures de celui-ci.  Cet ajout
	de fonctionnalité se fait en échange d'un léger coût en temps de traitement
	et en utilisation de mémoire.  Ceci dit, ces deux derniers restent minimes.
	Toute personne voulant contrôler l'affichage du LCD devrait passer par ce
	sous-module et non pas par le pilote de hd44780.


    Utilisation des sous-modules :

    De façon à fonctionner correctement, les fonctions des sous-modules ne
    peuvent être intermélangées. Donc, un code qui aurait besoin d'une fonctionnalité
    du sous-module LCD une seule fois  doit quand même utiliser
	LCD pour toutes les autres opérations, aussi simples soient elles.

*/

// Includes ___________________________________________________________________

#include "utils.h"	



// ----------------------------------------------------------------------------
// Sous Module HD44780 (Accès direct, mais difficile au LCD)
//-----------------------------------------------------------------------------


// Switches ___________________________________________________________________

/**
    \brief Switch qui permet d'utiliser le HD44780 sur seulement 4 bits

    Si la switch est définie, seulement 4 bits sont utilisé pour le port de data
*/
#define HD44780_ENABLE_4BIT_OPERATION


/**
    \brief Switch qui utilise la ram de caractère pour les accents français

    Si la switch est définit, les accents français seront correctement affichés au
	détriment de perdre la possibilité de définir des caractères personnalisés
*/
#define HD44780_USE_CGRAM_FOR_FRENCH_ACCENT


// Defines et typedef _________________________________________________________

/**
    \brief HD44780_DATA_PORT Définit quel port est utilisé pour le data du LCD
	\brief HD44780_DATA_DDR Définit le registre pour contrôler la direction du data du LCD
	\brief HD44780_DATA_DDR Définit le registre pour lire le data provenant du LCD
	\brief HD44780_DATA_SHIFT Définit le décalage du data port (seulement pertinent pour le mode 4 bits)
*/
#define HD44780_DATA_PORT		PORTC
#define HD44780_DATA_DDR		DDRC
#define HD44780_DATA_PIN		PINC
#ifdef HD44780_ENABLE_4BIT_OPERATION
	#define HD44780_DATA_SHIFT	4
#endif

/**
    \brief Définit quel port est utilisé pour la broche enable du LCD
	\brief Définit le registre pour contrôler la direction de la broche enable du LCD
	\brief Définit le numéro de la broche enable du LCD
*/
#define HD44780_E_PORT   PORTA
#define HD44780_E_DDR    DDRA
#define HD44780_E_SHIFT  7

/**
    \brief Définit quel port est utilisé pour la broche read/write du LCD
	\brief Définit le registre pour contrôler la direction de la broche read/write du LCD
    \brief Définit le numéro de la broche read/write du LCD
*/
#define HD44780_RW_PORT   PORTC
#define HD44780_RW_DDR    DDRC
#define HD44780_RW_SHIFT  3

/**
    \brief Définit quel port est utilisé pour la broche register select du LCD
	\brief Définit le registre pour contrôler la direction de la broche register select du LCD
    \brief Définit le numéro de la broche du register select du LCD
*/

#define HD44780_RS_PORT   PORTC
#define HD44780_RS_DDR    DDRC
#define HD44780_RS_SHIFT  2

/**
    \sa hd44780_shift_cursor(hd44780_shift_e shift)
*/
typedef enum{

    HD44780_SHIFT_RIGHT,
    HD44780_SHIFT_LEFT,

}hd44780_shift_e;


/* Void datasheet (https://www.sparkfun.com/datasheets/LCD/HD44780.pdf) p.17 */

/**
    \brief Définit le caractère représentant une flèche vers la droite
*/
#define HD44780_CHAR_RIGHT_ARROW			0x7E

/**
    \brief Définit le caractère représentant une flèche vers la gauche
*/
#define HD44780_CHAR_LEFT_ARROW				0x7F

/**
    \brief Définit le caractère représentant le symbole °
*/
#define HD44780_CHAR_DEGREE					0xDF

/**
    \brief Définit le caractère représentant la lettre grecque alpha
*/
#define HD44780_CHAR_ALPHA					0xE0

/**
    \brief Définit le caractère 'Ä'
*/
#define HD44780_CHAR_TREMA_A				0xE1

/**
    \brief Définit le caractère représentant la lettre grecque beta
*/
#define HD44780_CHAR_BETA					0xE2

/**
    \brief Définit le caractère représentant la lettre grecque espilon
*/
#define HD44780_CHAR_EPSILON				0xE3

/**
    \brief Définit le caractère représentant la lettre grecque mu
*/
#define HD44780_CHAR_MU						0xE4

/**
    \brief Définit le caractère représentant la lettre grecque sigma
*/
#define HD44780_CHAR_SIGMA					0xE5

/**
    \brief Définit le caractère représentant la lettre grecque rho
*/
#define HD44780_CHAR_RHO					0xE6

/**
    \brief Définit le caractère d'une autre représentation de la lettre g qui descend plus bas dans l'affichage
*/
#define HD44780_CHAR_LOW_G					0xE7

/**
    \brief Définit le caractère représentant le symbole racine carrée
*/
#define HD44780_CHAR_SQUARE_ROOT			0xE8

/**
    \brief Définit le caractère représentant le symbole exposant -1
*/
#define HD44780_CHAR_SUPERSCRIPT_MINUS_ONE	0xE9

/**
    \brief Définit le caractère d'une autre représentation de la lettre j qui descend plus bas dans l'affichage
*/
#define HD44780_CHAR_LOW_J					0xEA

/**
    \brief Définit le caractère représentant le symbole exposant X
*/
#define HD44780_CHAR_SUPERSCRIPT_X			0xEB

/**
    \brief Définit le caractère représentant le cent
*/
#define HD44780_CHAR_CENT_SYMBOL			0xEC

/**
    \brief Définit le caractère représentant la livre sterling
*/
#define HD44780_CHAR_POUND_SYMBOL			0xED

/**
    \brief Définit le caractère représentant la lettre n avec un ~
*/
#define HD44780_CHAR_TILDE_N				0xEE

/**
    \brief Définit le caractère représentant la lettre ö
*/
#define HD44780_CHAR_TREMA_O				0xEF

/**
    \brief Définit le caractère d'une autre représentation de la lettre p qui descend plus bas dans l'affichage
*/
#define HD44780_CHAR_LOW_P					0xF0

/**
    \brief Définit le caractère d'une autre représentation de la lettre q qui descend plus bas dans l'affichage
*/
#define HD44780_CHAR_LOW_Q					0xF1

/**
    \brief Définit le caractère représentant la lettre grecque theta
*/
#define HD44780_CHAR_THETA					0xF2

/**
    \brief Définit le caractère représentant le symbole exposant infini
*/
#define HD44780_CHAR_INFINITY				0xF3

/**
    \brief Définit le caractère représentant la lettre grecque omega majuscule
*/
#define HD44780_CHAR_OMEGA					0xF4

/**
    \brief Définit le caractère représentant la lettre Ü
*/
#define HD44780_CHAR_TREMA_U				0xF5

/**
    \brief Définit le caractère représentant le symbole de la sommation
*/
#define HD44780_CHAR_SUMMATION				0xF6

/**
    \brief Définit le caractère représentant la lettre grecque pi
*/
#define HD44780_CHAR_PI						0xF7

/**
    \brief Définit le caractère représentant le symbole de la moyenne
*/
#define HD44780_CHAR_AVERAGE				0xF8

/**
    \brief Définit le caractère d'une autre représentation de la lettre y qui descend plus bas dans l'affichage
*/
#define HD44780_CHAR_LOW_Y					0xF9

/**
    \brief Définit le caractère représentant le symbole de la division
*/
#define HD44780_CHAR_DIVIDE					0xFD

#ifdef HD44780_USE_CGRAM_FOR_FRENCH_ACCENT
	#define HD44780_CHAR_GRAVE_A			0x00
	#define HD44780_CHAR_CEDILLA_C			0x01
	#define HD44780_CHAR_GRAVE_E			0x02
	#define HD44780_CHAR_ACUTE_E			0x03
	#define HD44780_CHAR_CIRCUMFLEX_E		0x04
	#define HD44780_CHAR_TREMA_E			0x05
	#define HD44780_CHAR_TREMA_I			0x06
	#define HD44780_CHAR_GRAVE_U			0x07
#else
	#define HD44780_CHAR_GRAVE_A			'a'	// Pas d'accent
	#define HD44780_CHAR_CEDILLA_C			'c'	// Pas d'accent
	#define HD44780_CHAR_GRAVE_E			'e'	// Pas d'accent
	#define HD44780_CHAR_ACUTE_E			'e'	// Pas d'accent
	#define HD44780_CHAR_CIRCUMFLEX_E		'e'	// Pas d'accent
	#define HD44780_CHAR_TREMA_E			'e'	// Pas d'accent
	#define HD44780_CHAR_TREMA_I			'i'	// Pas d'accent
	#define HD44780_CHAR_GRAVE_U			'u'	// Pas d'accent
#endif


// Déclaration de fonctions ___________________________________________________

/**
    \brief Fait l'initialisation de base du contrôleur LCD.  C'est-à-dire
    l'initialisation, régler la config pour 16 caractères X 2 rangées, effacer
    l'écran et mettre le curseur au début.
    \param[in]  increment   Si == true, le texte avance vers la droite
    \param[in]  cursor      Si == true, un curseur est affiché
    \param[in]  blink       Si == true, la case du curseur clignote
    \return Rien
*/
void hd44780_init(bool increment, bool cursor, bool blink);

/**
    \brief Efface tous les caractères sur l'écran et remet le curseur au début
    \return Rien
*/
void hd44780_clear_display();

/**
    \brief Définit le sens dans lequel les caractères sont insérés.  À moins
    d'être bien spécial le sens normal est incrémental et ne devrait pas changer
	souvent. Cette fonction est utilisée par la fonction qui insère des caractères.
    \param[in]  increment   Si == true, le texte avance vers la droite
    \return Rien
*/
void hd44780_set_entry_mode(bool increment);

/**
    \brief Permet de contrôler certains paramètres de l'affichage
    \param[in]  display Si == true, l'affichage est allumée
    \param[in]  cursor  Si == true, un curseur est affiché
    \param[in]  blink   Si == true, la case du curseur clignote
    \return Rien
*/
void hd44780_set_display_control(bool display, bool cursor, bool blink);

/**
    \brief Permet de déplacer le curseur dans la mémoire du HD44780.
    \param[in]  row La rangée de 0 à 1
	\param[in]  col La colonne de 0 à 39    
    \return Rien

    Cette fonction permet de faire abstraction des adresses en mémoire qui sont
    un peu batardes et d'aller directement à une position dans l'affichage.
    Rien n'empêche d'aller mettre un caractère à l'extérieur de la zone
    affichable, en autant que les limites des paramètres soient respectées
*/
void hd44780_set_cursor_position(uint8_t row, uint8_t col);

/**
    \brief Permet de déplacer le curseur d'une position à gauche ou à droite.
    \param[in]  shift   La direction du shift
    \return Rien
*/
void hd44780_shift_cursor(hd44780_shift_e shift);

/**
    \brief Écrit un seul caractère à l'endroit actuel du curseur.
    \param[in]  character   Le caractère ASCII à afficher.
    \return Rien

    Au retour, le curseur aura été déplacé d'une position dans le sens déterminé à l'aide
    de hd44780_set_entry_mode.
*/
void hd44780_write_char(char character);

#ifndef HD44780_USE_CGRAM_FOR_FRENCH_ACCENT
/**
	\Attention Cette fonction est seulement disponible si la CGRAM n'est pas utilisée par afficher les accents
    \brief Définit un caratère personnalisé qui peut par la suite être utilisé par hd44780_write_char();
    \param[in]  slot			L'endroit en mémoire où enregister le caractère [0 à 7]
	\param[in]  bitmap_array	Un pointeur vers un tableau d'exactement 8 bytes qui représentent le masque du caractère personnalisé
    \return Rien	
*/
void hd44780_write_cgram(uint8_t slot, const uint8_t* bitmap_array);
#endif


// ----------------------------------------------------------------------------
// Sous Module LCD (Accès indirect, mais plus facile au LCD)
//-----------------------------------------------------------------------------

// Switch _____________________________________________________________________

/**
    \brief Switch qui permet d'utiliser le LCD comme stdout pour printf

    Si la switch est définit, printf imprime sur l'écran LCD
*/
#define LCD_REDIRECT_STDOUT_TO_LCD


// Defines et typedef _________________________________________________________

/**
    \brief Définit le nombre de colonnes du LCD
*/
#define LCD_NB_COL 16

/**
    \brief Définit le nombre de rangées du LCD
*/
#define LCD_NB_ROW 2

/**
    \sa lcd_shift_cursor(lcd_shift_e shift)
*/
typedef enum{

    LCD_SHIFT_RIGHT,
    LCD_SHIFT_LEFT,
    LCD_SHIFT_UP,
    LCD_SHIFT_DOWN,
	LCD_SHIFT_END,
	LCD_SHIFT_START,
	LCD_SHIFT_TOP,
	LCD_SHIFT_BOTTOM

}lcd_shift_e;


// Déclaration de fonctions ___________________________________________________

/**
    \brief Fait l'initialisation du LCD.
    \return Rien

	Cette fonction doit préalablement être appelée avant d'utiliser les autres
	fonctions du module.
*/
void lcd_init(void);

/**
    \brief Efface l'écran du LCD et retourne le curseur à la position 0,0.
	\return Rien
	
	Il n'est pas réellement possible "d'effacer" l'écran du LCD. Bien que
	la fonctionnalité soit offerte par le hd44780, en réalité la fiche technique
	nous apprend que le LCD ne fait que remplacer tous les caractères de l'écran
	par des espaces. C'est pour cette raison que cette fonction est relativement
	lente. Ce serait une mauvaise idée de faire un appel à celle-ci dans une boucle
	avec des délais critiques.
*/
void lcd_clear_display(void);

/**
    \brief Efface le restant de la ligne à partir de la position actuelle du curseur
    \return Rien
*/
void lcd_clean_line(void);

/**
    \brief Déplace le curseur sur une rangée en gardant la position de la colonne inchangée
    \param[in] row La rangée (0 ou 1)
	\return Rien
	
	Il y a 16 colonnes et 2 rangées. Si le paramètre dépasse les dimensions
	de l'écran, la demande est ignorée.
*/
void lcd_set_cursor_row(uint8_t row);

/**
    \brief Déplace le curseur sur une colonne en gardant la position de la rangée inchangée
    \param[in]  col La colonne (0 à 15)
    \return Rien
	
	Il y a 16 colonnes et 2 rangées. Si le paramètre dépasse les dimensions
	de l'écran, la demande est ignorée.
*/
void lcd_set_cursor_col(uint8_t col);

/**
    \brief Déplace le curseur à un endroit précis dans l'écran
    \param[in]  row La rangée (0 ou 1)
	\param[in]  col La colonne (0 à 15)

	Par exemple, lcd_set_cursor_position(1, 3) : amène le curseur
	à la position du X

         0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1
         0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        | | | | | | | | | | | | | | | | | 0
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        | | | |X| | | | | | | | | | | | | 1
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	Il y a 16 colonnes et 2 rangées. Si les paramètres dépassent les dimensions
	de l'écran, la demande est ignorée.
*/
void lcd_set_cursor_position(uint8_t row, uint8_t col);

/**
    \brief Déplace le curseur d'une seule case dans la position spécifiée
    \param[in]  shift   Une des valeurs de la liste suivante
	Les valeurs possibles pour shift sont :

    - LCD_SHIFT_RIGHT	: Déplace le curseur d'une case à droite
    - LCD_SHIFT_LEFT	: Déplace le curseur d'une case à gauche
    - LCD_SHIFT_UP		: Déplace le curseur d'une case en haut
    - LCD_SHIFT_DOWN	: Déplace le curseur d'une case en bas
    - LCD_SHIFT_END		: Déplace le curseur à la dernière colonne de la ligne (mais reste sur la même rangé)
    - LCD_SHIFT_START	: Déplace le curseur à la première colonne de la ligne (mais reste sur la même rangé)
    - LCD_SHIFT_TOP		: Déplace le curseur sur la première ligne (mais reste dans la même colonne)
    - LCD_SHIFT_BOTTOM	: Déplace le curseur sur la dernière ligne (mais reste dans la même colonne)

	Les 4 premières options se comportent comme si l'écran du LCD était une boucle dans les deux directions.
	Par exemple :

	Si le curseur est à la position du X

         0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1
         0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        | | | | | | | | | | | | | | | |X| 0
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        | | | | | | | | | | | | | | | | | 1
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	Et que l'appel suivant est fait : `lcd_shift_cursor(LCD_SHIFT_RIGHT);`

	Le curseur va se retrouver à la position suivante:

         0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1
         0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        |X| | | | | | | | | | | | | | | | 0
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        | | | | | | | | | | | | | | | | | 1
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
void lcd_shift_cursor(lcd_shift_e shift);


/**
    \brief Permet de changer le style du curseur après l'initialisation du LCD
    \param[in]  is_visible	Si == true le curseur est visible, si == false le curseur est masqué
	\param[in]  is_blinking	Si == true le curseur clignote, si == false le curseur est inerte
    \return Rien
*/
void lcd_set_cursor_style(bool is_visible, bool is_blinking);


/**
    \brief Écrit un seul caractère ASCII à la position du curseur sur le LCD
    \param[in]  character Le caractère à afficher
	\return Rien

	Pour afficher des caractères spéciaux, il faut utiliser les définitions du type :
	HD44780_CHAR_xxx
	
	Deux caractères de contrôle sont supportés :
		- "\n" : change de rangé et retourne le curseur à la colonne 0
		- "\r" : retourne le curseur à la colonne 0 sans changer de ligne
*/
void lcd_write_char(char character);

/**
    \brief Écrit une string à la position du curseur sur le LCD.
    \param[in] string La string à afficher
	\return Rien

	Pour que cette fonction produise le résultat attendu, il faut absolument
	que la string soit terminée par un caractère nul ('\0'). Le caractère nul
	n'est pas envoyé au LCD.

	Par exemple :

	`lcd_write_string("Hello World");` produira le résultat suivant :

         0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1
         0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        |H|e|l|l|o| |W|o|r|l|d| | | | | | 0
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        | | | | | | | | | | | | | | | | | 1
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	La fonction déplace automatiquement le curseur au début de la ligne suivante
	si la string à afficher est trop longue.

	Par exemple :

	`lcd_write_string("Hello World ABCDEF");` produira le résultat suivant :

         0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1
         0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        |H|e|l|l|o| |W|o|r|l|d| |A|B|C|D| 0
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        |E|F| | | | | | | | | | | | | | | 1
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	La fonction sait gérer deux caractère de contrôle; "\n" et "\r"

	"\n" Déplace le curseur d'une ligne et la retourne à la colonne 0

	Par exemple :

	`lcd_write_string("Hello\nWorld");` produira le résultat suivant :

         0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1
         0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        |H|e|l|l|o| | | | | | | | | | | | 0
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        |W|o|r|l|d| | | | | | | | | | | | 1
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	"\r" Renvois le curseur au début de la ligne en cours. Son comportement est identique
	à faire l'appel suivant : lcd_shift_cursor(LCD_SHIFT_START);

	Par exemple :

	`lcd_write_string("Hello World\rABC");` produira le résultat suivant :

         0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1
         0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        |A|B|C|l|o| |W|o|r|l|d| | | | | | 0
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        | | | | | | | | | | | | | | | | | 1
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
void lcd_write_string(const char* string);

/**
	\brief Affiche sur l'écran LCD une représentation en caractères ASCII d'un nombre non signé
	\param[in] u				Le nombre non signé à afficher
	\param[in] min_int_digit	Le nombre minimal de chiffres à utiliser
	\return Rien
	
	Si le nombre de chiffres nécéssaire pour un affichage correcte dépasse la valeur passée en paramètre,
	celle-ci est ignorée et le nombre qui produit un affichage correct est utilisé.
	
	Si la valeur passée en paramètre est supérieure au nombre nécéssaire, les espaces en trop sont bourrés de 0.

	Par exemple :

	`lcd_write_uint16(123, 1);` produira le résultat suivant :

	0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1
	0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|1|2|3| | | | | | | | | | | | | | 0
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	| | | | | | | | | | | | | | | | | 1
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	
	`lcd_write_uint16(123, 5);` produira le résultat suivant :

	0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1
	0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|0|0|1|2|3| | | | | | | | | | | | 0
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	| | | | | | | | | | | | | | | | | 1
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	
	Cette fonction est prévue pour des cas super spécifiques qui se sont produits durant le développement du projet.
	De façon quasi catégorique, il est mieux d'utiliser printf() avec le formatage désiré
*/

void lcd_write_uint16(uint16_t u, int8_t min_int_digit);

/**
	\brief Affiche sur l'écran LCD une représentation en caractères ASCII d'un nombre signé
	\param[in] i				Le nombre signé à afficher
	\param[in] min_int_digit	Le nombre minimal de chiffres à utiliser
	\return Rien
	
	Si le nombre de chiffres nécéssaire pour un affichage correcte dépasse la valeur passée en paramètre,
	celle-ci est ignorée et le nombre qui produit un affichage correct est utilisé.
	
	Si la valeur passée en paramètre est supérieure au nombre nécéssaire, les espaces en trop sont bourrés de 0.

	Par exemple :

	`lcd_write_uint16(123, 1);` produira le résultat suivant :

	0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1
	0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|1|2|3| | | | | | | | | | | | | | 0
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	| | | | | | | | | | | | | | | | | 1
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	
	`lcd_write_uint16(123, 5);` produira le résultat suivant :

	0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1
	0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|0|0|1|2|3| | | | | | | | | | | | 0
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	| | | | | | | | | | | | | | | | | 1
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	
	Cette fonction est prévue pour des cas super spécifiques qui se sont produits durant le développement du projet.
	De façon quasi catégorique, il est mieux d'utiliser printf() avec le formatage désiré
*/
void lcd_write_int16(int16_t i, int8_t min_int_digit);

/**
TODO
*/
void lcd_write_float(float f, int8_t min_int_digit, int8_t nb_frac_digit,  bool show_positive_sign);


#endif // LCD_H_INCLUDED
