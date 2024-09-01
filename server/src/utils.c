#include"utils.h"

t_log* logger;

int iniciar_servidor(void)
{
	// Quitar esta línea cuando hayamos terminado de implementar la funcion
	// assert(!"no implementado!");

	int socket_servidor;
	int err;
	struct addrinfo hints, *servinfo, *p;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	err = getaddrinfo(NULL, PUERTO, &hints, &servinfo);
	if (err != 0) {
        // log_error(logger, "Error in getaddrinfo: %s", gai_strerror(err));
        // return -1;
		handle_error("Error in getaddrinfo", NULL, -1);
    }

	// Creamos el socket de escucha del servidor
	socket_servidor = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    if (socket_servidor == -1) {
        handle_error("Error creating socket", servinfo, -1);
    }

	// Asociamos el socket a un puerto
	int optval = 1;
    err = setsockopt(socket_servidor, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    if (err == -1) {
        handle_error("Error setting socket options", servinfo, socket_servidor);
    }

	err = bind(socket_servidor, servinfo->ai_addr, servinfo->ai_addrlen);
    if (err == -1) {
        handle_error("Error binding socket", servinfo, socket_servidor);
    }

	// Escuchamos las conexiones entrantes
	err = listen(socket_servidor, SOMAXCONN);
    if (err == -1) {
        handle_error("Error listening on socket", servinfo, socket_servidor);
    }

	freeaddrinfo(servinfo);
	log_trace(logger, "Listo para escuchar a mi cliente");

	return socket_servidor;
}

int esperar_cliente(int socket_servidor)
{
	// Quitar esta línea cuando hayamos terminado de implementar la funcion
	// assert(!"no implementado!");

	// Aceptamos un nuevo cliente
	int socket_cliente;
	// while(1){
	// 	socket_cliente = accept(socket_servidor, NULL, NULL);
	// 	if(socket_cliente == -1){
	// 		handle_error("Error acepting the client", NULL, socket_servidor);
	// 	}
	// 	log_info(logger, "Se conecto un cliente!");
	// }
	socket_cliente = accept(socket_servidor, NULL, NULL);
	if(socket_cliente == -1){
		handle_error("Error acepting the client", NULL, socket_servidor);
	}
	log_info(logger, "Se conecto un cliente!");

	return socket_cliente;
}

int recibir_operacion(int socket_cliente)
{
	int cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0)
		return cod_op;
	else
	{
		close(socket_cliente);
		return -1;
	}
}

void* recibir_buffer(int* size, int socket_cliente)
{
	void * buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

void recibir_mensaje(int socket_cliente)
{
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);
	log_info(logger, "Me llego el mensaje %s", buffer);
	free(buffer);
}

t_list* recibir_paquete(int socket_cliente)
{
	int size;
	int desplazamiento = 0;
	void * buffer;
	t_list* valores = list_create();
	int tamanio;

	buffer = recibir_buffer(&size, socket_cliente);
	while(desplazamiento < size)
	{
		memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
		desplazamiento+=sizeof(int);
		char* valor = malloc(tamanio);
		memcpy(valor, buffer+desplazamiento, tamanio);
		desplazamiento+=tamanio;
		list_add(valores, valor);
	}
	free(buffer);
	return valores;
}

void handle_error(const char* message, struct addrinfo* servinfo, int socket_fd) {
    // Free resources and handle the error
    if (servinfo) {
        freeaddrinfo(servinfo);
    }
    if (socket_fd != -1) {
        close(socket_fd);
    }
    // Call the existing error_show function
    error_show(message, servinfo, socket_fd);
	abort();
}