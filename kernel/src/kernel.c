#include "kernel.h"

void start()
{
	config = config_create("kernel.config");
	if (config == NULL)
	{
		logger = log_create("kernel.log","kernel",true,log_level);
		log_error(logger,"No hay archivo config");
		abort();
	}
	else
	{
		puerto_escucha_io = config_get_string_value(config,"PUERTO_ESCUCHA_IO");
		log_level = log_level_from_string(config_get_string_value(config,"LOG_LEVEL"));

		logger = log_create("kernel.log","kernel",true,log_level);
		log_trace(logger,"puerto_escucha_io : %s",puerto_escucha_io);
	}
}

int main(void) {
	start();

	int server_fd = iniciar_servidor(puerto_escucha_io);

	int cliente_fd;
	while (1) {
		while (!cliente_fd)
		{
			cliente_fd = esperar_cliente(server_fd);
		}
		log_info(logger,"se conecto un cliente");

		t_list* lista;
		while (1) {
			int cod_op = recibir_operacion(cliente_fd);
			if (cod_op == -1)
			{
				log_error(logger, "el cliente se desconecto. esperando otro cliente");
				cliente_fd = 0;
				break; // exit while 
			}

			if (cod_op == CLOSE_CONNECTION)
			{
				log_info(logger, "El cliente cerro la coneccion, esperando");
				break; // exit while 
			}
			switch (cod_op) {
			case MENSAJE:
				recibir_mensaje(cliente_fd);
				break;
			case PAQUETE:
				lista = recibir_paquete(cliente_fd);
				log_info(logger, "Me llegaron los siguientes valores:");
				list_iterate(lista, (void*) iterator);
				break;
			case -1:
				log_error(logger, "el cliente se desconecto. Terminando servidor");
				break; //return EXIT_FAILURE;
			default:
				log_warning(logger,"Operacion desconocida. No quieras meter la pata");
				break;
			}
		}
	}
	return EXIT_SUCCESS;
}

void iterator(char* value) {
	log_info(logger,"%s", value);
}


