#ifndef CLIENT_H_
#define CLIENT_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>

#include "utils.h"

typedef enum {
    TYPE_INT,
    TYPE_STRING,
    TYPE_LONG,
    TYPE_DOBLE,
    TYPE_ARRAY
} t_type;


t_log* iniciar_logger(void);
t_config* iniciar_config(void);
void leer_consola(t_log*);
void paquete(int);
void terminar_programa(int, t_log*, t_config*);
void* leer_config(t_config* config, char* clave, t_type tipo);


#endif /* CLIENT_H_ */
