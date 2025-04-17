#include "server.h"

void start()
{
	logger = log_create("server.log", "Servidor", 1, LOG_LEVEL_DEBUG);
}

int main(void) {
	start();

	int server_fd = iniciar_servidor();
	log_info(logger, "Servidor listo para recibir al cliente");

	int cliente_fd;
	while (1) {
		while (!cliente_fd)
		{
			cliente_fd = esperar_cliente(server_fd);
		}

		t_list* lista;
		while (1) {
			int cod_op = recibir_operacion(cliente_fd);
			if (cod_op == -1)
			{
				log_error(logger, "el cliente se desconecto. esperando otro cliente");
				cliente_fd = 0;
				break; // exit while 
			}

			if (cod_op == -1)
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
