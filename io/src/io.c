#include "io.h"
void start()
{
	config = config_create("io.config");
	if (config == NULL)
	{
		logger = log_create("io.log","io",true,log_level);
		log_error(logger,"No hay archivo config");
		abort();
	}
	else
	{
		ip_kernel = config_get_string_value(config,"IP_KERNEL");
		puerto_kernel = config_get_string_value(config,"PUERTO_KERNEL");
		log_level = log_level_from_string(config_get_string_value(config,"LOG_LEVEL"));
	}

	logger = log_create("io.log","io",true,log_level);
	log_trace(logger,"puerto_kernel : %s",puerto_kernel);
	log_trace(logger,"ip_kernel     : %s",ip_kernel);
}

int main(int argc, char *argv[]){
	start();

	if(argc > 1)
	{
		nombre_io = argv[1];
		log_trace(logger,"Nombre I/O : %s",nombre_io);
	}
	else
	{
		nombre_io = "Anonimo";
		log_warning(logger,"Falta definir Nombre de I/O, se usa : %s",nombre_io);
	}



	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip_kernel, puerto_kernel);
	log_trace(logger,"conexion creada %i",conexion);


	// Enviamos al servidor  : nombre_io 
	enviar_mensaje(nombre_io,conexion);
	size_t bytes;
	int32_t result;


	while (true)
	{

		bytes = recv(conexion, &result, sizeof(int32_t), MSG_WAITALL);

		if (result == 0) {
			// Handshake OK
			log_info(logger,"conexion rechazada %i",conexion);
			break;
		} else {
			// proceso recibido
			log_info(logger,"conexion rechazada %i",conexion);
			usleep(result);
			log_info(logger,"conexion rechazada %i",conexion);
			enviar_mensaje("FIN",conexion);
		}

	}
	// Armamos y enviamos el paquete
	// paquete(conexion);

	terminar_programa(conexion, logger, config);
}


void leer_consola(t_log* logger)
{
	char* leido;
	while (1)
	{
		leido = readline("> ");

		if (leido) 
			add_history(leido);

		if (!strcmp(leido,"")) 
			break;

		log_info(logger,"valor leido %s",leido);
		free(leido);
	}
}

void leer_consola2(t_paquete* paquete)
{
	char* leido;
	int tamanio;
	while (1)
	{
		leido = readline("> ");

		if (leido) 
			add_history(leido);

		if (!strcmp(leido,"")) 
			break;
		tamanio = strlen(leido);
		agregar_a_paquete(paquete,leido,tamanio);
		free(leido);
	}
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete;

	paquete = crear_paquete();
	leer_consola2(paquete);
	enviar_paquete(paquete,conexion);
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	config_destroy(config);	
	log_destroy(logger);
}
