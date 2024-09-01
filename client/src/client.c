#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();
	if(logger == NULL){
		terminar_programa(conexion, logger, config);
	}

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	log_info(logger, "Hola! Soy un log");


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();
	if(config == NULL){
		log_error(logger, "No se pudo iniciar config");
	}	

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

		valor = leer_config(config, "CLAVE", TYPE_STRING);
	ip = leer_config(config, "IP", TYPE_STRING);
	puerto = leer_config(config, "PUERTO", TYPE_STRING);

	// Loggeamos el valor de config

	log_info(logger, "Valor de CLAVE: %s", valor);
	log_info(logger, "Valor de IP: %s", ip);
	log_info(logger, "Valor de PUERTO: %s", puerto);


	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion); 
	// while(1){

	// }
	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;

	nuevo_logger = log_create("./tp0.log", "loger", true, LOG_LEVEL_INFO);

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;

	nuevo_config = config_create("./cliente.config");

	return nuevo_config;
}

void* leer_config(t_config* config, char* clave, t_type tipo){
	if(!config_has_property(config, clave)){
		//log_error(logger, "Error: la clave %s no se encuentra en la configuracion. \n", clave);
		return NULL;
	}

	switch (tipo)
	{
	case TYPE_INT:
		return (void*)(intptr_t)config_get_int_value(config, clave);
	case TYPE_STRING:
		return (void*)config_get_string_value(config, clave);
	default:
		//log_error(logger, "Error: tipo no soportado. \n");
		return NULL;
	}
}

void leer_consola(t_log* logger)
{
    char* leido = NULL;

    // La primera te la dejo de yapa
    leido = readline("> ");
    if (leido == NULL) {
        log_error(logger, "Error leyendo de la consola.");
        return;
    }

    // El resto, las vamos leyendo y logueando hasta recibir un string vacío
    while (strcmp(leido, "") != 0) {
        log_info(logger, "Leido de la consola: %s", leido);
        free(leido);  // Free the previously allocated memory
        leido = readline("> ");
        if (leido == NULL) {
            log_error(logger, "Error leyendo de la consola.");
            return;
        }
    }

    // ¡No te olvides de liberar las lineas antes de regresar!
    free(leido);  // Free the last line read, which is an empty string
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido = NULL;
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	leido = readline("> ");
    if (leido == NULL) {
        // log_error(logger, "Error leyendo de la consola.");
        return;
    }

    // El resto, las vamos leyendo y logueando hasta recibir un string vacío
    while (strcmp(leido, "") != 0) {
        agregar_a_paquete(paquete, leido, strlen(leido)+1);
        leido = readline("> ");
        if (leido == NULL) {
            // log_error(logger, "Error leyendo de la consola.");
            return;
        }
    }

	enviar_paquete(paquete, conexion);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(leido);
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	  liberar_conexion(conexion);
	  log_info(logger, "Programa terminado");
	  log_destroy(logger);
	  config_destroy(config);
	  abort();
}
