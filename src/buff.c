#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/buff.h"

// ------initialisation taille realocation------

//return the pointer to allocated buffer
void * buff_init_buffer(size_t buffer_length){
  return
    malloc(buffer_length);
};

//return un pointer sur l'ancien vecteur de pointer par buffer
//la fonction ne libere pas l'ancienne zone memoire
void * buff_rinit_buffer(void * buffer, size_t new_length){
  if( buff_empty(buffer) )
    return
      NULL;

  void * tmp_buffer= buffer;
  buffer = buff_init_buffer( new_length );
  return
    tmp_buffer;
};

//return old_buffer on success, or NULL if new buffer is empty
void * buff_rebind_buffer( void * old_buffer, void * new_buffer){
  if( buff_empty(new_buffer) )
    return 
      NULL;

  void * tmp_buffer= old_buffer;
  old_buffer=new_buffer;

  return
    tmp_buffer;
};

int buff_empty( void *buffer){
  return
    buffer == NULL;
};


// ------ operation sur la forme du tableau ------

//retourne un tableau de pointer sur chaque zone ou null si buffer et vide
// ou si la position est out of scope
void** buff_split( void * buffer, size_t buffer_length, size_t position ){
  if(
     buff_empty( buffer )|| 
     position > buffer_length || 
     position == buffer_length ||
     position == 0 
    )
    return 
      NULL;
      
  void * buffer1=NULL; 
  buffer1 += position;
  size_t buffer1_length= buffer_length - position;
  buffer1= realloc( buffer1, buffer1_length );   

  void * buffer0 = realloc ( buffer0, position );

  void **buffer_container = buff_init_buffer ( sizeof( void **));  
  buffer_container[0]=buffer0;
  buffer_container[1]=buffer1;

  return
    buffer_container;
};

//retourne un tableau egale a buffa + buffb avec ou non un delimiter
void * buff_agregate( void * buffer_1, void* buffer_2, size_t length_b1, size_t length_b2 ){
  size_t length_agr = length_b1 + length_b2;
  void * agregate = buff_init_buffer ( length_agr );
  if (agregate == NULL )
    return
      NULL;

  void * ptr_buffer2 = agregate;
  ptr_buffer2 += length_b2;

  buff_cpy( agregate, buffer_1, length_agr, length_b1, length_b1 );
  buff_cpy( ptr_buffer2, buffer_2, length_b1, length_b2, length_b2 );

  return
    agregate;
};

//retourne un tableau bourrer a gauche ou a droite
void * buff_feed_left(void *buffer, size_t buffer_length, int feeding_char, size_t count){
  if (buff_empty ( buffer ))
    return
      NULL;

  void * feeding_buffer= buff_init_buffer ( count );
  buff_set( feeding_buffer, count, feeding_char, count );

  return
    buff_agregate( feeding_buffer, buffer, count, buffer_length);
};

void * buff_feed_right(void *buffer, size_t buffer_length, int feeding_char, size_t count){

  if (buff_empty ( buffer ))
    return
      NULL;

  void * feeding_buffer= buff_init_buffer ( count );
  buff_set( feeding_buffer, count,  feeding_char, count );

  return
    buff_agregate( buffer, feeding_buffer, buffer_length ,count);
};

void * buff_feed(void *buffer, size_t buffer_length,  char feeding_char, BUFF_ALIGNEMENT_e ALIGN_DIRECTION, size_t count){
  switch(ALIGN_DIRECTION)
  {
    case BUFF_LEFT_ALG :
      buff_feed_left(buffer, buffer_length, feeding_char, count); 
      break;

    case BUFF_RIGHT_ALG :
      buff_feed_right(buffer, buffer_length, feeding_char, count); 
      break;
    default :
      break;        
  }
};

void * buff_truncat_left( void * buffer, size_t buffer_length , size_t count){
  if( 
     buff_empty(buffer) ||
     count > buffer_length ||
     count == 0 
    )
    return
      NULL;

  void * ptr_buffer = buffer;  
  ptr_buffer += count;  

  size_t trunc_buffer_length = buffer_length - count ;
  void * trunc_buffer= buff_init_buffer( trunc_buffer_length );

  buff_cpy( trunc_buffer, buffer, buffer_length, trunc_buffer_length, trunc_buffer_length );

  return
    trunc_buffer;
}

void * buff_truncat_right( void * buffer, size_t buffer_length , size_t count){
  if( 
     buff_empty(buffer) ||
     count > buffer_length ||
     count == 0 
    )
    return
      NULL;

  size_t trunc_buffer_length = buffer_length - count ;
  void * trunc_buffer= buff_init_buffer( trunc_buffer_length );

  buff_cpy( trunc_buffer, buffer, buffer_length, trunc_buffer_length, trunc_buffer_length );

  return
    trunc_buffer;
}

void * buff_truncat( void * buffer, size_t buffer_length , BUFF_ALIGNEMENT_e TRUNC_DIRECTION, size_t count){
  switch(TRUNC_DIRECTION)
  {
    case BUFF_LEFT_ALG :
      buff_truncat_left( buffer, buffer_length , count);
      break;

    case BUFF_RIGHT_ALG :
      buff_truncat_right( buffer, buffer_length , count);
      break;
    default :
      break;        
  }
  
} 

// ------- operation sur les donner du tableau --------

void * buff_bitwise_not(  void * buffer , size_t length_buffer  ){
  if ( buff_empty(buffer) || !length_buffer )
    return
      NULL;
  
  void * not_buffer = buff_dup( buffer, length_buffer );
  char * ptr_not = not_buffer;
  char * ptr_buffer = buffer;
  for ( size_t i = 0; i < length_buffer; i++ )
  { 
    *ptr_not= ~*ptr_buffer;  

    ++ptr_not;
    ++ptr_buffer;
  } 

  return
    not_buffer;
};


void * buff_bitwise_or( void * buffer_1, void * buffer_2, size_t length_b1, size_t length_b2 ){
  if( 
      buff_empty(buffer_1) || !length_b1 || 
      buff_empty(buffer_2) || !length_b2 
    )
      return
        NULL;
        
  size_t smallest_length;     
  if( length_b1 == length_b2 || length_b1 > length_b2 )
     smallest_length = length_b2;     
  if( length_b2 > length_b1 )
     smallest_length = length_b1;
   
  void * or_buffer = buff_init_buffer( smallest_length );
  char * ptr_or = or_buffer;
  char * ptr_buffer1 = buffer_1;
  char * ptr_buffer2 = buffer_2;
  for ( size_t i = 0; i < smallest_length; i++ )
  { 
    *ptr_or= ( *ptr_buffer1 | *ptr_buffer2 );  
  
    ++ptr_or;
    ++ptr_buffer1;
    ++ptr_buffer2;
  } 
  
  return
    or_buffer;
};
void * buff_bitwise_and( void * buffer_1, void * buffer_2, size_t length_b1, size_t length_b2 ){
  if( 
      buff_empty(buffer_1) || !length_b1 || 
      buff_empty(buffer_2) || !length_b2 
    )
      return
        NULL;
        
  size_t smallest_length;     
  if( length_b1 == length_b2 || length_b1 > length_b2 )
     smallest_length = length_b2;     
  if( length_b2 > length_b1 )
     smallest_length = length_b1;
   
  void * and_buffer = buff_init_buffer( smallest_length );
  char * ptr_and = and_buffer;
  char * ptr_buffer1 = buffer_1;
  char * ptr_buffer2 = buffer_2;
  for ( size_t i = 0; i < smallest_length; i++ )
  { 
    *ptr_and= ( *ptr_buffer1 & *ptr_buffer2 );  
  
    ++ptr_and;
    ++ptr_buffer1;
    ++ptr_buffer2;
  } 
  
  return
    and_buffer;
};

void * buff_bitwise_orExclusive( void * buffer_1, void * buffer_2, size_t length_b1, size_t length_b2 ){
  if( 
      buff_empty(buffer_1) || !length_b1 || 
      buff_empty(buffer_2) || !length_b2 
    )
      return
        NULL;
        
  size_t smallest_length;     
  if( length_b1 == length_b2 || length_b1 > length_b2 )
     smallest_length = length_b2;     
  if( length_b2 > length_b1 )
     smallest_length = length_b1;
   
  void * xor_buffer = buff_init_buffer( smallest_length );
  char * ptr_xor = xor_buffer;
  char * ptr_buffer1 = buffer_1;
  char * ptr_buffer2 = buffer_2;
  for ( size_t i = 0; i < smallest_length; i++ )
  { 
    *ptr_xor= ( *ptr_buffer1 ^ *ptr_buffer2 );  
  
    ++ptr_xor;
    ++ptr_buffer1;
    ++ptr_buffer2;
  } 
  
  return
    xor_buffer;
};
void * buff_bitwise_shiftleft( void * buffer , size_t length_buffer , size_t shifting_count ){
      
  void * trunc_buffer = buff_truncat_left( buffer, length_buffer , shifting_count);
  void * feed_buffer =  buff_feed_left( trunc_buffer, length_buffer, 0, shifting_count);

  return
    feed_buffer;
};

void * buff_bitwise_shiftright( void * buffer , size_t length_buffer , size_t shifting_count ){    

  void * trunc_buffer = buff_truncat_right( buffer, length_buffer , shifting_count);
  void * feed_buffer =  buff_feed_right( trunc_buffer, length_buffer, 0, shifting_count);

  return
    feed_buffer;
};

//------ operation d'edition ---------------------
// rechercher de character
// renvoie un pointer sur la position de chr
void * buff_chr(void * buffer, size_t length_buf, int chr){
  return
    memchr( buffer, chr, length_buf);
};

// copie de donner d'un buffer a un autre
void * buff_cpy(void * buffer, void * from, size_t length_b1,size_t length_b2,size_t count){
  size_t smallest_length;     
  if( length_b1 == length_b2 || length_b1 > length_b2 )
     smallest_length = length_b2;     
  if( length_b2 > length_b1 )
     smallest_length = length_b1;
  if( count > smallest_length )
    return
      NULL;
      
  return
    memcpy( buffer, from, count );
};

// duplication de buffer
// renvoie un buffer qui est la copie de buffer
void * buff_dup( void * buffer, size_t length_buffer ){
  void * new_buffer = buff_init_buffer( length_buffer );
  return
    buff_cpy( new_buffer, buffer, length_buffer, length_buffer, length_buffer);
};

// operation d'eciture
// renvoie le buffer modifier avec le chracter
void * buff_set(void * buffer, size_t length_buffer, int character, size_t count){
  if( buff_empty( buffer ) )
    return
      NULL;
  if( count > length_buffer )
    return
      NULL;
      
  return
    memset( buffer, character, count);
};

// operation de deplacement de curseurr
// renvoie un pointeur a la position "position" du buffer
void * buff_get( void * buffer, size_t length_buffer, size_t position ){
  if( buff_empty( buffer ) )
    return
      NULL;
  if( position > length_buffer )
    return
      NULL;

  void * ptr = buffer;
  return
    ptr += position;
};

//operation de comparaison
int buff_cmp( void * buff1, void * buff2, size_t length_b1, size_t length_b2, size_t count){
  size_t smallest_length;     
  if( buff_empty( buff1 ) || buff_empty( buff2 ) )
    return
      -3;
  if( length_b1 == 0 || length_b2 == 0 || count == 0 )
    return
      -2;
  if( length_b1 == length_b2 || length_b1 > length_b2 )
     smallest_length = length_b2;     
  if( length_b2 > length_b1 )
     smallest_length = length_b1;
  if( count > smallest_length )
    return
      -1;

  return
    memcmp( buff1, buff2, count);  
};
