#ifndef IO_H_
#define IO_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include<readline/history.h>

#include "utils.h"

// variables config
int conexion;
char* ip_kernel;
char* puerto_kernel;
t_log_level log_level;

t_log* logger;
t_config* config;

// Nombre IO
char* nombre_io;



void leer_consola(t_log*);
void paquete(int);
void terminar_programa(int, t_log*, t_config*);

#endif /* IO_H_ */
