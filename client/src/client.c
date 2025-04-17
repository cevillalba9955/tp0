#include "client.h"

void start()
{
	logger = iniciar_logger();
	config = iniciar_config();
	if (config == NULL)
	{
		log_info(logger,"No hay archivo config");
		abort();
	}
	ip = config_get_string_value(config,"IP");
	puerto = config_get_string_value(config,"PUERTO");
	clave = config_get_string_value(config,"CLAVE");

	// Loggeamos el valor de config
	log_info(logger,"el valor ip es %s",ip);
	log_info(logger,"el valor puerto es %s",puerto);
	log_info(logger,"el valor clave es %s",clave);

}

int main(void)
{
	start();
	


	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);
	log_info(logger,"conexion creada %i",conexion);
	// Enviamos al servidor el valor de CLAVE como mensaje

	size_t bytes;

	int32_t handshake = 1;
	int32_t result;

	bytes = send(conexion, &handshake, sizeof(int32_t), 0);
	bytes = recv(conexion, &result, sizeof(int32_t), MSG_WAITALL);

	if (result == 0) {
		// Handshake OK
		enviar_mensaje("hola servidor",conexion);
	} else {
		// Handshake ERROR
		log_info(logger,"conexion rechazada %i",conexion);

	}



	// Armamos y enviamos el paquete
	paquete(conexion);


	terminar_programa(conexion, logger, config);
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;
	char* archivo_log = "client.log";
	char* process_log = "Cliente ";
	nuevo_logger = log_create(archivo_log,process_log,true,LOG_LEVEL_INFO);
	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;
	nuevo_config = config_create("cliente.config");
	return nuevo_config;
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
