#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<string.h>
#include<assert.h>
#include"defines.h"

extern t_log* logger;

t_paquete* armar_paquete(t_buffer* buffer, int codigo_operacion);

int crear_conexion(char* ip, char* puerto);
void liberar_conexion(int socket_cliente);
int iniciar_servidor(char *sPuerto);
int esperar_cliente(int Socket);
void Enviar_Info(void* info, int size,int operacion,int socket);
int Recibir_Info(void** Info, int size, int socket);


t_Prueba* serializar_prueba(t_buffer* buffer);
t_buffer* armar_buffer_prueba(t_Prueba* info, int size);

t_Proceso* serializar_proceso(t_buffer* buffer);
t_buffer* armar_buffer_proceso(t_Proceso* info, int size);

t_Registros* serializar_contexto(t_buffer* buffer);
t_buffer* armar_buffer_contexto(t_Registros* info, int size);
