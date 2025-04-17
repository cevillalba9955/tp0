#ifndef KERNEL_H_
#define KERNEL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include <commons/config.h>
#include "utils.h"

void iterator(char* value);

// variables config
char* puerto_escucha_io;



t_log_level log_level;
t_log* logger;
t_config* config;


#endif /* KERNEL_H_ */

