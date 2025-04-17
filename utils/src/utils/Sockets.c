#include"Sockets.h"

int iniciar_servidor(char* sPuerto)
{
	int socket_servidor,err;

	struct addrinfo hints, *servinfo, *p;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(NULL, sPuerto, &hints, &servinfo);

	// Creamos el socket de escucha del servidor
	socket_servidor = socket(servinfo->ai_family,
                         servinfo->ai_socktype,
                         servinfo->ai_protocol);


	// Asociamos el socket a un puerto
	err = bind(socket_servidor, servinfo->ai_addr, servinfo->ai_addrlen);

	// Escuchamos las conexiones entrantes

	err = listen(socket_servidor, SOMAXCONN);

	freeaddrinfo(servinfo);
	//log_trace(logger, "Listo para escuchar a mi cliente");

	return socket_servidor;
}

int esperar_cliente(int socket_servidor)
{
	// Aceptamos un nuevo cliente
	int socket_cliente = accept(socket_servidor, NULL, NULL);

//	log_info(logger, "Se conecto un cliente!");

	return socket_cliente;
}

int crear_conexion(char *ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	// Ahora vamos a crear el socket.
	int socket_cliente = 0;
	socket_cliente = socket(server_info->ai_family,
                         server_info->ai_socktype,
                         server_info->ai_protocol);
	
	// Ahora que tenemos el socket, vamos a conectarlo
	int err = connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen);

	freeaddrinfo(server_info);

	return socket_cliente;
}

t_buffer* armar_buffer_prueba(t_Prueba* info, int size) {
    t_buffer* buffer = malloc(size);
 
    buffer->size = sizeof(int) * 2 + info->largoCadena;
    buffer->offset = 0;
    buffer->stream = malloc(buffer->size);
 
    char* stream = buffer->stream;
    int offset = 0;

    memcpy(stream + offset, &info->Valor, sizeof(int));
    offset += sizeof(int);
    memcpy(stream + offset, &info->largoCadena, sizeof(int));
    offset += sizeof(int);
    memcpy(stream + offset, info->Cadena, info->largoCadena);
    
    return buffer;
}

t_buffer* armar_buffer_proceso(t_Proceso* info, int size) {
    t_buffer* buffer = malloc(size);
 
    buffer->size = sizeof(int) * 4;
    buffer->offset = 0;
    buffer->stream = malloc(buffer->size);
 
    char* stream = buffer->stream;
    int offset = 0;

    memcpy(stream + offset, &info->TID, sizeof(int));
    offset += sizeof(int);
    memcpy(stream + offset, &info->PID, sizeof(int));
    offset += sizeof(int);
    memcpy(stream + offset, &info->resultado, sizeof(int));
    offset += sizeof(int);
    memcpy(stream + offset, &info->Tipo, sizeof(int));
    offset += sizeof(int);
    
    return buffer;
}

t_buffer* armar_buffer_contexto(t_Registros* info, int size) {
    t_buffer* buffer = malloc(size);
 
    buffer->size = sizeof(uint8_t) * 5 + sizeof(uint32_t) * 7;
    buffer->offset = 0;
    buffer->stream = malloc(buffer->size);
 
    char* stream = buffer->stream;
    int offset = 0;

    memcpy(stream + offset, &info->PID,sizeof(uint8_t));
    offset += sizeof(int);
    memcpy(stream + offset, &info->PC, sizeof(uint32_t));
    offset += sizeof(int);
    memcpy(stream + offset, &info->AX, sizeof(uint8_t));
    offset += sizeof(int);
    memcpy(stream + offset, &info->BX, sizeof(uint8_t));
    offset += sizeof(int);
    memcpy(stream + offset, &info->CX, sizeof(uint8_t));
    offset += sizeof(int);
    memcpy(stream + offset, &info->DX, sizeof(uint8_t));
    offset += sizeof(int);
    memcpy(stream + offset, &info->EAX, sizeof(uint32_t));
    offset += sizeof(int);
    memcpy(stream + offset, &info->EBX, sizeof(uint32_t));
    offset += sizeof(int);
    memcpy(stream + offset, &info->ECX, sizeof(uint32_t));
    offset += sizeof(int);
    memcpy(stream + offset, &info->EDX, sizeof(uint32_t));
    offset += sizeof(int);
    memcpy(stream + offset, &info->SI, sizeof(uint32_t));
    offset += sizeof(int);
    memcpy(stream + offset, &info->DI, sizeof(uint32_t));
    offset += sizeof(int);            
    return buffer;
}

t_paquete* armar_paquete(t_buffer* buffer, int codigo_operacion) {
    t_paquete* paquete = malloc(sizeof(t_paquete));
 
    paquete->codigo_operacion = codigo_operacion;
    paquete->buffer = buffer;

    return paquete;
}

void Enviar_Info(void* info, int size,int operacion,int socket) {
    // Armar el buffer
    t_buffer* buffer = armar_buffer_prueba(info,size);

    // Armar el paquete
    t_paquete* paquete = armar_paquete(buffer, operacion);

    // Preparar el stream a enviar
    void* a_enviar = malloc(buffer->size + sizeof(int) * 2);

    int offset = 0;
    memcpy(a_enviar + offset, &paquete->codigo_operacion, sizeof(int));
    offset += sizeof(int);
    memcpy(a_enviar + offset, &paquete->buffer->size, sizeof(int));
    offset += sizeof(int);
    memcpy(a_enviar + offset, paquete->buffer->stream, paquete->buffer->size);

    // Enviar los datos
    send(socket, a_enviar, buffer->size + sizeof(int) * 2, 0);

    // Liberar memoria
    free(a_enviar);
    free(paquete->buffer->stream);
    free(paquete->buffer);
    free(paquete);
}

int Recibir_Info(void** Info, int size, int socket) {
    t_paquete* paquete = malloc(sizeof(t_paquete));

    paquete->buffer = malloc(sizeof(t_buffer));

    // Primero recibimos el código de operación
    recv(socket, &(paquete->codigo_operacion), sizeof(int), 0);

    // Después ya podemos recibir el buffer. Primero su tamaño seguido del contenido
    recv(socket, &(paquete->buffer->size), sizeof(int), 0);

    paquete->buffer->stream = malloc(paquete->buffer->size);

	recv(socket, paquete->buffer->stream, paquete->buffer->size, 0);

    // Ahora en función del código recibido procedemos a deserializar el resto
    switch (paquete->codigo_operacion) {
        case PRUEBA: {
            t_Prueba* stPrueba = serializar_prueba(paquete->buffer);
            *Info = stPrueba;
            break;
        }
        case PROCESO: {
            t_Proceso* stProceso = serializar_proceso(paquete->buffer);
            *Info = stProceso;
            break;
        } 
        case CONTEXTO: {
            t_Registros* stRegistros = serializar_contexto(paquete->buffer);
            *Info = stRegistros;
            break;
        }                
        default:
            fprintf(stderr, "Código de operación desconocido: %d\n", paquete->codigo_operacion);
            free(paquete->buffer->stream);
            free(paquete->buffer);
            free(paquete);
            return -1;
    }

    // Liberar memoria de paquete y buffer (excepto el stream que se ha transferido)
    free(paquete->buffer->stream);
    free(paquete->buffer);
    free(paquete);

    return 0;
}

t_Prueba* serializar_prueba(t_buffer* buffer) {
    t_Prueba* prueba = malloc(sizeof(t_Prueba));
 
    void* stream = buffer->stream;
    // Deserializamos los campos que tenemos en el buffer
    memcpy(&(prueba->Valor), stream, sizeof(int));
    stream += sizeof(int);
    memcpy(&(prueba->largoCadena), stream, sizeof(int));
    stream += sizeof(int);

    prueba->Cadena = malloc(prueba->largoCadena);
    memcpy(prueba->Cadena, stream, prueba->largoCadena);

    return prueba;
}
t_Proceso* serializar_proceso(t_buffer* buffer) {
    t_Proceso* proceso = malloc(sizeof(t_Proceso));
 
    void* stream = buffer->stream;
    // Deserializamos los campos que tenemos en el buffer
    memcpy(&(proceso->TID), stream, sizeof(int));
    stream += sizeof(int);
    memcpy(&(proceso->PID), stream, sizeof(int));
    stream += sizeof(int);
    memcpy(&(proceso->resultado), stream, sizeof(int));
    stream += sizeof(int);
    memcpy(&(proceso->Tipo), stream, sizeof(int));
    stream += sizeof(int);
    return proceso;
}
t_Registros* serializar_contexto(t_buffer* buffer) {
    t_Registros* registro = malloc(sizeof(t_Registros));
    void* stream = buffer->stream;
    // Deserializamos los campos que tenemos en el buffer
    memcpy(&(registro->PID), stream, sizeof(uint8_t));
    stream += sizeof(int);
    memcpy(&(registro->PC), stream, sizeof(uint32_t));
    stream += sizeof(int);
    memcpy(&(registro->AX), stream, sizeof(uint8_t));
    stream += sizeof(int);
    memcpy(&(registro->BX), stream, sizeof(uint8_t));
    stream += sizeof(int);
    memcpy(&(registro->CX), stream, sizeof(uint8_t));
    stream += sizeof(int);
    memcpy(&(registro->DX), stream, sizeof(uint8_t));
    stream += sizeof(int);
    memcpy(&(registro->EAX), stream, sizeof(uint32_t));
    stream += sizeof(int);    
    memcpy(&(registro->EBX), stream, sizeof(uint32_t));
    stream += sizeof(int);    
    memcpy(&(registro->ECX), stream, sizeof(uint32_t));
    stream += sizeof(int);            
    memcpy(&(registro->EDX), stream, sizeof(uint32_t));
    stream += sizeof(int);    
    memcpy(&(registro->SI), stream, sizeof(uint32_t));
    stream += sizeof(int);    
    memcpy(&(registro->DI), stream, sizeof(uint32_t));
    stream += sizeof(int);                
    return registro;
}