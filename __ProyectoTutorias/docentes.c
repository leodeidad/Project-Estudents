#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "docentes.h"

Docente listaDocentes[100];
int contadorDocentes = 0;

static void limpiarPantalla() {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

static void esperarEnter() {
	printf("\nPresione Enter para continuar...");
	while (getchar() != '\n');
	getchar();
	limpiarPantalla();
}


static void leerNombreCompleto(char *buffer, int maxLen, const char *mensaje) {
	printf("%s", mensaje);
	while (getchar() != '\n');
	fgets(buffer, maxLen, stdin);
	buffer[strcspn(buffer, "\n")] = '\0';
}

int compararCodigosDocentes(char texto1[], char texto2[]) {
	int i = 0;
	while (texto1[i] != '\0' || texto2[i] != '\0') {
		if (texto1[i] != texto2[i]) {
			return 0;
		}
		i = i + 1;
	}
	return 1;
}

int buscarDocentePorCodigo(char codigo[]) {
	int i = 0;
	for (i = 0; i < contadorDocentes; i = i + 1) {
		if (compararCodigosDocentes(listaDocentes[i].codigoDocente, codigo) == 1) {
			return 1;
		}
	}
	return 0;
}

void registrarDocente() {
	char codigoTemporal[20];
	limpiarPantalla();
	printf("--- Registrar Docente ---\n");
	printf("Ingrese codigo de docente: ");
	scanf("%s", codigoTemporal);
	
	if (buscarDocentePorCodigo(codigoTemporal) == 1) {
		printf("Error: Este codigo de docente ya existe.\n");
		esperarEnter();
		return;
	}
	
	int pos = contadorDocentes;
	int i = 0;
	while (codigoTemporal[i] != '\0') {
		listaDocentes[pos].codigoDocente[i] = codigoTemporal[i];
		i = i + 1;
	}
	listaDocentes[pos].codigoDocente[i] = '\0';
	
	leerNombreCompleto(listaDocentes[pos].nombreCompleto, 100, "Ingrese nombre completo: ");
	
	printf("Ingrese una contrasena para el docente: ");
	scanf("%s", listaDocentes[pos].clave);
	
	int opArea = 0;
	printf("Seleccione Area de Conocimiento:\n1. Ing en Software\n2. Ing en Computacion\n3. Ing en Sistemas\nOpcion: ");
	while (scanf("%d", &opArea) != 1 || opArea < 1 || opArea > 3) {
		while (getchar() != '\n');
		printf("Opcion invalida. Ingrese 1, 2 o 3: ");
	}
	
	
	if (opArea == 1) {
		strcpy(listaDocentes[pos].areaConocimiento, "Ing en Software");
	} else if (opArea == 2) {
		strcpy(listaDocentes[pos].areaConocimiento, "Ing en Computacion");
	} else {
		strcpy(listaDocentes[pos].areaConocimiento, "Ing en Sistemas");
	}
	
	printf("Seleccione bloque de horario propio:\n1. 07:00 a 09:00\n2. 09:00 a 11:00\n3. 11:00 a 13:00\nOpcion: ");
	while (scanf("%d", &listaDocentes[pos].horarioBloque) != 1 || listaDocentes[pos].horarioBloque < 1 || listaDocentes[pos].horarioBloque > 3) {
		while (getchar() != '\n');
		printf("Opcion invalida. Ingrese 1, 2 o 3: ");
	}
	
	contadorDocentes = contadorDocentes + 1;
	printf("Docente registrado exitosamente.\n");
	esperarEnter();
}

void listarDocentes() {
	limpiarPantalla();
	if (contadorDocentes == 0) {
		printf("No hay docentes registrados.\n");
		esperarEnter();
		return;
	}
	printf("--- Lista de Docentes ---\n");
	int i = 0;
	for (i = 0; i < contadorDocentes; i = i + 1) {
		printf("Codigo: %s | Nombre: %s | Area: %s | Bloque Horario: %d ",
			   listaDocentes[i].codigoDocente, listaDocentes[i].nombreCompleto,
			   listaDocentes[i].areaConocimiento, listaDocentes[i].horarioBloque);
		if (listaDocentes[i].horarioBloque == 1) printf("(07:00-09:00)\n");
		else if (listaDocentes[i].horarioBloque == 2) printf("(09:00-11:00)\n");
		else printf("(11:00-13:00)\n");
	}
	esperarEnter();
}

void modificarDocente() {
	char codigoBuscado[20];
	limpiarPantalla();
	printf("--- Modificar Docente ---\n");
	printf("Ingrese el codigo del docente a modificar: ");
	scanf("%s", codigoBuscado);
	
	int i = 0;
	int encontrado = 0;
	for (i = 0; i < contadorDocentes; i = i + 1) {
		if (compararCodigosDocentes(listaDocentes[i].codigoDocente, codigoBuscado) == 1) {
			encontrado = 1;
			printf("Docente encontrado: %s\n", listaDocentes[i].nombreCompleto);
			leerNombreCompleto(listaDocentes[i].nombreCompleto, 100, "Ingrese nuevo nombre completo: ");
			
			int opArea = 0;
			printf("Seleccione nueva Area de Conocimiento:\n1. Ing en Software\n2. Ing en Computacion\n3. Ing en Sistemas\nOpcion: ");
			while (scanf("%d", &opArea) != 1 || opArea < 1 || opArea > 3) {
				while (getchar() != '\n');
				printf("Invalido. Ingrese 1, 2 o 3: ");
			}
			
			
			if (opArea == 1) {
				strcpy(listaDocentes[i].areaConocimiento, "Ing en Software");
			} else if (opArea == 2) {
				strcpy(listaDocentes[i].areaConocimiento, "Ing en Computacion");
			} else {
				strcpy(listaDocentes[i].areaConocimiento, "Ing en Sistemas");
			}
			
			printf("Seleccione nuevo bloque horario (1: 7-9, 2: 9-11, 3: 11-13): ");
			while (scanf("%d", &listaDocentes[i].horarioBloque) != 1 || listaDocentes[i].horarioBloque < 1 || listaDocentes[i].horarioBloque > 3) {
				while (getchar() != '\n');
				printf("Invalido. Ingrese 1, 2 o 3: ");
			}
			printf("Registro de docente modificado.\n");
			break;
		}
	}
	if (encontrado == 0) {
		printf("Error: Docente no encontrado.\n");
	}
	esperarEnter();
}

void eliminarDocente() {
	char codigoBuscado[20];
	limpiarPantalla();
	printf("--- Eliminar Docente ---\n");
	printf("Ingrese el codigo del docente a eliminar: ");
	scanf("%s", codigoBuscado);
	
	int i = 0;
	int j = 0;
	int encontrado = 0;
	for (i = 0; i < contadorDocentes; i = i + 1) {
		if (compararCodigosDocentes(listaDocentes[i].codigoDocente, codigoBuscado) == 1) {
			encontrado = 1;
			for (j = i; j < contadorDocentes - 1; j = j + 1) {
				listaDocentes[j] = listaDocentes[j + 1];
			}
			contadorDocentes = contadorDocentes - 1;
			printf("Docente eliminado del sistema.\n");
			break;
		}
	}
	if (encontrado == 0) {
		printf("Error: Docente no encontrado.\n");
	}
	esperarEnter();
}

void guardarDocentesTXT() {
	FILE *archivo = fopen("listado_docentes.txt", "w");
	if (archivo == NULL) return;
	fprintf(archivo, "=========================================================================\n");
	fprintf(archivo, "                        LISTADO DE DOCENTES                              \n");
	fprintf(archivo, "=========================================================================\n");
	int i = 0;
	for (i = 0; i < contadorDocentes; i = i + 1) {
		fprintf(archivo, "Codigo: %s | Nombre: %s | Area: %s | Bloque: %d\n",
				listaDocentes[i].codigoDocente, listaDocentes[i].nombreCompleto,
				listaDocentes[i].areaConocimiento, listaDocentes[i].horarioBloque);
	}
	fclose(archivo);
}


void guardarDocentesBaseDatos() {
	FILE *archivo = fopen("docentes_db.txt", "w");
	if (archivo == NULL) {
		printf("Error al guardar la base de datos de docentes.\n");
		return;
	}
	
	for (int i = 0; i < contadorDocentes; i++) {

		fprintf(archivo, "%s;%s;%s;%s;%d\n",
				listaDocentes[i].codigoDocente,
				listaDocentes[i].nombreCompleto,
				listaDocentes[i].clave,
				listaDocentes[i].areaConocimiento,
				listaDocentes[i].horarioBloque);
	}
	fclose(archivo);
}


void cargarDocentesBaseDatos() {
	FILE *archivo = fopen("docentes_db.txt", "r");
	if (archivo == NULL) {

		contadorDocentes = 0;
		return;
	}
	
	char linea[300];
	contadorDocentes = 0;
	
	while (fgets(linea, sizeof(linea), archivo) != NULL && contadorDocentes < 100) {

		linea[strcspn(linea, "\n")] = '\0';
		

		char *codigo = strtok(linea, ";");
		char *nombre = strtok(NULL, ";");
		char *clave = strtok(NULL, ";");
		char *area = strtok(NULL, ";");
		char *bloqueStr = strtok(NULL, ";");
		
		if (codigo && nombre && clave && area && bloqueStr) {
			int pos = contadorDocentes;
			strcpy(listaDocentes[pos].codigoDocente, codigo);
			strcpy(listaDocentes[pos].nombreCompleto, nombre);
			strcpy(listaDocentes[pos].clave, clave);
			strcpy(listaDocentes[pos].areaConocimiento, area);
			listaDocentes[pos].horarioBloque = atoi(bloqueStr);
			contadorDocentes++;
		}
	}
	fclose(archivo);
}
