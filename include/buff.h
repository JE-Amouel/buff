#ifndef _BUFF_H_
#define _BUFF_H_

#include <stdarg.h>
#include <stddef.h>

typedef 
enum
buff_direction
  {
    BUFF_RTL_DIR,
    BUFF_LTR_DIR,
    BUFF_PTL_DIR,
    BUFF_PTR_DIR,
  }
BUFF_DIRECTION_e;

typedef 
enum
buff_alignement
  {
    BUFF_LEFT_ALG = 0x1 ,
    BUFF_RIGHT_ALG = 0x2,
  }
BUFF_ALIGNEMENT_e;

// ------initialisation taille realocation------

//return the pointer to allocated buffer
void * buff_init_buffer(size_t buffer_length);

//return un pointer sur l'ancien vecteur de pointrt par buffer
//la fonction ne libere pas l'ancienne zone memoire
void * buff_rinit_buffer(void * buffer, size_t new_length);

//return old_buffer on success, else return old_buffer
void * buff_rebind_buffer( void * old_buffer, void * new_buffer);

int buff_empty( void *buffer);


// ------ operation sur la forme du tableau ------

//retourne un tableau de pointer sur chaque zone
void** buff_split( void * buffer, size_t buffer_length, size_t position );

//retourne un tableau egale a buffa + buffb avec ou non un delimiter
void * buff_agregate( void * buffer_1, void *buffer_2, size_t length_buffer1, size_t length_buffer2 );

//retourne un tableau bourrer a gauche ou a droite
void * buff_feed(void *buffer, size_t buffer_length,  char feeding_char, BUFF_ALIGNEMENT_e ALIGN_DIRECTION, size_t count);

 
// ------- operation sur les donner du tableau --------

void * buff_bitwise_not(  void * buff , size_t length_buffer  );
void * buff_bitwise_or( void * buffer_1, void * buffer_2, size_t length_b1, size_t length_b2 );
void * buff_bitwise_and( void * buffer_1, void * buffer_2, size_t length_b1, size_t length_b2 );

void * buff_bitwise_orExclusive( void * buffer_1, void * buffer_2, size_t length_b1, size_t length_b2 );
void * buff_bitwise_andExclusive( void * buffer_1, void * buffer_2, size_t length_b1, size_t length_b2 );

void * buff_bitwise_shiftleft( void * buffer , size_t length_buffer , size_t shifting_count );
void * buff_bitwise_shiftright( void * buffer , size_t length_buffer , size_t shifting_count ); 
// ------ operation de recherche dans le tableau ------

//renvoie le nombre de fois que pattern a eter trouver
// buff_search_nbr(void * buffer, char * pattern);

//renvoie le pointer vers la premiere occurence de pattern
// buff_search_ptr(void * buffer, char * pattern);

//------ operation d'edition ---------------------
// rechercher de character
// renvoie un pointer sur la position de chr
void * buff_chr(void * buffer, size_t length_buf, int chr);

// copie de donner d'un buffer a un autre
void * buff_cpy(void * buffer, void * from, size_t length_b1,size_t length_b2,size_t count);

// duplication de buffer
// renvoie un buffer qui est la copie de buffer
void * buff_dup( void * buffer, size_t length_buffer );

// operation d'eciture
// renvoie le buffer modifier avec le chracter
void * buff_set(void * buffer, size_t length_buffer, int character, size_t count);

// operation de deplacement de curseurr
// renvoie un pointeur a la position "position" du buffer
void * buff_get( void * buffer, size_t length_buffer, size_t position);

//operation de comparaison
int buff_cmp( void * buff1, void * buff2, size_t length_b1, size_t length_b2, size_t count);

#endif
