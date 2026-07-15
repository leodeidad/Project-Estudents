#include <stdio.h>
#include <stdlib.h>
#include "estudiantes.h"
#include "docentes.h"
#include <string.h>
#include <ctype.h>


#ifndef VALIDACIONES_H
#define VALIDACIONES_H
#include <stdio.h>
#include <ctype.h>
#include <string.h>

void leerSoloNumeros(char *buffer, int maxLen, const char *mensaje) {
	int valido;
	do {
		valido = 1;
		printf("%s", mensaje);
		scanf("%s", buffer);
		for(int i = 0; buffer[i] != '\0'; i++) {
			if (!isdigit(buffer[i])) { valido = 0; break; }
		}
		if (!valido) printf("Error: Solo se permiten numeros.\n");
	} while (!valido);
}

void leerSoloLetras(char *buffer, int maxLen, const char *mensaje) {
	int valido;
	do {
		valido = 1;
		printf("%s", mensaje);
		while(getchar() != '\n'); 
		fgets(buffer, maxLen, stdin);
		buffer[strcspn(buffer, "\n")] = 0;
		for(int i = 0; buffer[i] != '\0'; i++) {
			if (!isalpha(buffer[i]) && buffer[i] != ' ') { valido = 0; break; }
		}
		if (!valido) printf("Error: Solo se permiten letras.\n");
	} while (!valido);
}
#endif



Estudiante listaEstudiantes[100];
int contadorEstudiantes = 0;

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

// Lee una linea completa (permite espacios, ej. "Ana Garcia") de forma segura.
// Debe llamarse siempre despues de un scanf("%s", ...) previo, ya que primero
// descarta el salto de linea pendiente de esa lectura anterior.
static void leerNombreCompleto(char *buffer, int maxLen, const char *mensaje) {
	printf("%s", mensaje);
	while (getchar() != '\n');
	fgets(buffer, maxLen, stdin);
	buffer[strcspn(buffer, "\n")] = '\0';
}

int compararCedulasEstudiantes(char texto1[], char texto2[]) {
	int i = 0;
	while (texto1[i] != '\0' || texto2[i] != '\0') {
		if (texto1[i] != texto2[i]) {
			return 0;
		}
		i = i + 1;
	}
	return 1;
}

int validarCorreo(char correo[]) {
	int i = 0;
	int tieneArroba = 0;
	int tienePunto = 0;
	while (correo[i] != '\0') {
		if (correo[i] == '@') {
			tieneArroba = 1;
		}
		if (tieneArroba == 1 && correo[i] == '.') {
			tienePunto = 1;
		}
		i = i + 1;
	}
	return (tieneArroba && tienePunto);
}

int buscarEstudiantePorCedula(char cedula[]) {
	int i = 0;
	for (i = 0; i < contadorEstudiantes; i = i + 1) {
		if (compararCedulasEstudiantes(listaEstudiantes[i].cedula, cedula) == 1) {
			return 1;
		}
	}
	return 0;
}

int validarCedulaFormato(char cedula[]) {
	int longi = 0;
	while (cedula[longi] != '\0') {
		longi = longi + 1;
	}
	if (longi != 10) {
		return 0;
	}
	int i = 0;
	for (i = 0; i < 10; i = i + 1) {
		if (cedula[i] < '0' || cedula[i] > '9') {
			return 0;
		}
	}
	return 1;
}

// Muestra los docentes cuya area de conocimiento coincide con la carrera indicada
// y obliga a seleccionar uno de ellos. Devuelve 1 si se asigno correctamente,
// 0 si no hay docentes disponibles para esa carrera (no se puede continuar).
int asignarDocenteObligatorio(char carrera[], char docenteAsignadoOut[]) {
	int indicesDisponibles[100];
	int totalDisponibles = 0;
	int i = 0;

	for (i = 0; i < contadorDocentes; i = i + 1) {
		if (strcmp(listaDocentes[i].areaConocimiento, carrera) == 0) {
			indicesDisponibles[totalDisponibles] = i;
			totalDisponibles = totalDisponibles + 1;
		}
	}

	if (totalDisponibles == 0) {
		printf("Error: No hay docentes registrados para la carrera '%s'.\n", carrera);
		printf("Debe registrar primero un docente de esa area antes de poder registrar estudiantes.\n");
		return 0;
	}

	printf("\nDocentes disponibles para %s:\n", carrera);
	for (i = 0; i < totalDisponibles; i = i + 1) {
		int idx = indicesDisponibles[i];
		printf("%d. %s (Codigo: %s) - Bloque: %d\n", i + 1,
			   listaDocentes[idx].nombreCompleto, listaDocentes[idx].codigoDocente,
			   listaDocentes[idx].horarioBloque);
	}

	int opDocente = 0;
	printf("Seleccione el docente tutor a asignar (numero de la lista): ");
	while (scanf("%d", &opDocente) != 1 || opDocente < 1 || opDocente > totalDisponibles) {
		while (getchar() != '\n');
		printf("Opcion invalida. Ingrese un numero entre 1 y %d: ", totalDisponibles);
	}

	strcpy(docenteAsignadoOut, listaDocentes[indicesDisponibles[opDocente - 1]].codigoDocente);
	return 1;
}

void registrarEstudiante() {
	char cedulaTemporal[20];
	limpiarPantalla();
	printf("--- Registrar Estudiante ---\n");
	
	int cedulaValida = 0;
	while (cedulaValida == 0) {
		printf("Ingrese cedula (10 digitos numericos): ");
		scanf("%s", cedulaTemporal);
		if (validarCedulaFormato(cedulaTemporal) == 1) {
			cedulaValida = 1;
		} else {
			printf("Error: La cedula debe tener exactamente 10 digitos numericos.\n");
		}
	}
	
	if (buscarEstudiantePorCedula(cedulaTemporal) == 1) {
		printf("Error: Esa cedula ya esta registrada.\n");
		esperarEnter();
		return;
	}
	
	int pos = contadorEstudiantes;
	int i = 0;
	while (cedulaTemporal[i] != '\0') {
		listaEstudiantes[pos].cedula[i] = cedulaTemporal[i];
		i = i + 1;
	}
	listaEstudiantes[pos].cedula[i] = '\0';
	
	leerNombreCompleto(listaEstudiantes[pos].nombreCompleto, 100, "Ingrese nombre completo: ");
	
	printf("Ingrese una contrasena para el estudiante: ");
	scanf("%s", listaEstudiantes[pos].clave);
	
	int opCarrera = 0;
	printf("Seleccione la carrera:\n1. Ing en Software\n2. Ing en Computacion\n3. Ing en Sistemas\nOpcion: ");
	while (scanf("%d", &opCarrera) != 1 || opCarrera < 1 || opCarrera > 3) {
		while (getchar() != '\n');
		printf("Opcion invalida. Ingrese 1, 2 o 3: ");
	}
	
	if (opCarrera == 1) strcpy(listaEstudiantes[pos].carrera, "Ing en Software");
	else if (opCarrera == 2) strcpy(listaEstudiantes[pos].carrera, "Ing en Computacion");
	else strcpy(listaEstudiantes[pos].carrera, "Ing en Sistemas");

	// Asignacion obligatoria de docente tutor segun la carrera seleccionada.
	// Si no hay docentes disponibles para esa carrera, se cancela el registro.
	if (asignarDocenteObligatorio(listaEstudiantes[pos].carrera, listaEstudiantes[pos].docenteAsignado) == 0) {
		esperarEnter();
		return;
	}

	printf("Ingrese el semestre (0 al 9): ");
	while (scanf("%d", &listaEstudiantes[pos].nivelSemestre) != 1 || 
		   listaEstudiantes[pos].nivelSemestre < 0 || 
		   listaEstudiantes[pos].nivelSemestre > 9) {
		
		while (getchar() != '\n');
		printf("Entrada invalida. Ingrese semestre en numeros (0 al 9): ");
	}	
	int correoValido = 0;
	while (correoValido == 0) {
		printf("Ingrese correo electronico: ");
		scanf("%s", listaEstudiantes[pos].correoElectronico);
		correoValido = validarCorreo(listaEstudiantes[pos].correoElectronico);
		if (correoValido == 0) {
			printf("Correo invalido. Intente de nuevo.\n");
		}
	}
	
	listaEstudiantes[pos].notas[0] = 0.0;
	listaEstudiantes[pos].notas[1] = 0.0;
	
	contadorEstudiantes = contadorEstudiantes + 1;
	printf("Estudiante registrado exitosamente.\n");
	esperarEnter();
}

void listarEstudiantes() {
	limpiarPantalla();
	if (contadorEstudiantes == 0) {
		printf("No hay estudiantes registrados en el sistema.\n");
		esperarEnter();
		return;
	}
	printf("--- Lista de Estudiantes ---\n");
	int i = 0;
	for (i = 0; i < contadorEstudiantes; i = i + 1) {
		printf("Cedula: %s | Nombre: %s | Carrera: %s | Semestre: %d | Correo: %s | Docente Asignado: %s | Nota 1: %.2f | Nota 2: %.2f\n",
			   listaEstudiantes[i].cedula, listaEstudiantes[i].nombreCompleto, listaEstudiantes[i].carrera,
			   listaEstudiantes[i].nivelSemestre, listaEstudiantes[i].correoElectronico,
			   listaEstudiantes[i].docenteAsignado,
			   listaEstudiantes[i].notas[0], listaEstudiantes[i].notas[1]);
	}
	esperarEnter();
}

void modificarEstudiante() {
	char cedulaBuscada[15];
	limpiarPantalla();
	printf("--- Modificar Estudiante ---\n");
	leerSoloNumeros(cedulaBuscada, 14, "Ingrese la cedula del estudiante a modificar: ");
	
	int i, encontrado = 0;
	for (i = 0; i < contadorEstudiantes; i++) {
		if (strcmp(listaEstudiantes[i].cedula, cedulaBuscada) == 0) {
			encontrado = 1;
			printf("Estudiante encontrado: %s\n", listaEstudiantes[i].nombreCompleto);
			
			// Usamos nuestra funcion de validacion de letras
			leerSoloLetras(listaEstudiantes[i].nombreCompleto, 100, "Ingrese nuevo nombre completo: ");
			
			int opCarrera;
			printf("Seleccione la nueva carrera:\n1. Ing en Software\n2. Ing en Computacion\n3. Ing en Sistemas\nOpcion: ");
			scanf("%d", &opCarrera);
			
			// Usamos strcpy en lugar de asignar letra por letra
			if (opCarrera == 1) strcpy(listaEstudiantes[i].carrera, "Ing en Software");
			else if (opCarrera == 2) strcpy(listaEstudiantes[i].carrera, "Ing en Computacion");
			else strcpy(listaEstudiantes[i].carrera, "Ing en Sistemas");
			
			// Al cambiar de carrera, el docente tutor debe volver a asignarse
			// de forma obligatoria para mantener la coherencia del sistema.
			if (asignarDocenteObligatorio(listaEstudiantes[i].carrera, listaEstudiantes[i].docenteAsignado) == 0) {
				printf("No se pudo completar la modificacion: no hay docente disponible para la nueva carrera.\n");
				esperarEnter();
				return;
			}
			
			// Validacion de numeros para el semestre
			char tempSemestre[5];
			leerSoloNumeros(tempSemestre, 2, "Ingrese nuevo semestre (1-10): ");
			listaEstudiantes[i].nivelSemestre = atoi(tempSemestre);
			
			printf("Estudiante modificado correctamente.\n");
			break;
		}
	}
	if (!encontrado) printf("Error: Estudiante no encontrado.\n");
	esperarEnter();
}

void eliminarEstudiante() {
	char cedulaBuscada[20];
	limpiarPantalla();
	printf("--- Eliminar Estudiante ---\n");
	printf("Ingrese la cedula del estudiante a eliminar: ");
	scanf("%s", cedulaBuscada);
	
	int i = 0;
	int j = 0;
	int encontrado = 0;
	for (i = 0; i < contadorEstudiantes; i = i + 1) {
		if (compararCedulasEstudiantes(listaEstudiantes[i].cedula, cedulaBuscada) == 1) {
			encontrado = 1;
			for (j = i; j < contadorEstudiantes - 1; j = j + 1) {
				listaEstudiantes[j] = listaEstudiantes[j + 1];
			}
			contadorEstudiantes = contadorEstudiantes - 1;
			printf("Estudiante eliminado exitosamente del sistema.\n");
			break;
		}
	}
	if (encontrado == 0) {
		printf("Error: Estudiante no encontrado.\n");
	}
	esperarEnter();
}

void guardarEstudiantesTXT() {
	FILE *archivo = fopen("listado_estudiantes.txt", "w");
	if (archivo == NULL) return;
	fprintf(archivo, "=========================================================================\n");
	fprintf(archivo, "                        LISTADO DE ESTUDIANTES                           \n");
	fprintf(archivo, "=========================================================================\n");
	int i = 0;
	for (i = 0; i < contadorEstudiantes; i = i + 1) {
		fprintf(archivo, "Cedula: %s | Nombre: %s | Carrera: %s | Semestre: %d | Correo: %s | Docente Asignado: %s\n",
				listaEstudiantes[i].cedula, listaEstudiantes[i].nombreCompleto,
				listaEstudiantes[i].carrera, listaEstudiantes[i].nivelSemestre,
				listaEstudiantes[i].correoElectronico, listaEstudiantes[i].docenteAsignado);
	}
	fclose(archivo);
	
	FILE *archivoNotas = fopen("notas_estudiantes.txt", "w");
	if (archivoNotas == NULL) return;
	fprintf(archivoNotas, "=========================================================================\n");
	fprintf(archivoNotas, "                        REPORTE DE NOTAS ACADEMICAS                      \n");
	fprintf(archivoNotas, "=========================================================================\n");
	for (i = 0; i < contadorEstudiantes; i = i + 1) {
		fprintf(archivoNotas, "Estudiante: %s | Cedula: %s | Nota 1: %.2f | Nota 2: %.2f\n",
				listaEstudiantes[i].nombreCompleto, listaEstudiantes[i].cedula,
				listaEstudiantes[i].notas[0], listaEstudiantes[i].notas[1]);
	}
	fclose(archivoNotas);
}

void guardarEstudiantesBaseDatos() {
	FILE *archivo = fopen("estudiantes_db.txt", "w");
	if (archivo == NULL) {
		printf("Error al guardar la base de datos de estudiantes.\n");
		return;
	}
	for (int i = 0; i < contadorEstudiantes; i++) {
		fprintf(archivo, "%s;%s;%s;%s;%s;%d;%s;%.2f;%.2f\n",
				listaEstudiantes[i].cedula,
				listaEstudiantes[i].nombreCompleto,
				listaEstudiantes[i].clave,
				listaEstudiantes[i].carrera,
				listaEstudiantes[i].docenteAsignado,
				listaEstudiantes[i].nivelSemestre,
				listaEstudiantes[i].correoElectronico,
				listaEstudiantes[i].notas[0],
				listaEstudiantes[i].notas[1]);
	}
	fclose(archivo);
}

// NUEVA FUNCIÓN: Carga la base de datos de estudiantes al iniciar el programa
void cargarEstudiantesBaseDatos() {
	FILE *archivo = fopen("estudiantes_db.txt", "r");
	if (archivo == NULL) {
		contadorEstudiantes = 0;
		return;
	}
	char linea[400];
	contadorEstudiantes = 0;
	while (fgets(linea, sizeof(linea), archivo) != NULL && contadorEstudiantes < 100) {
		linea[strcspn(linea, "\n")] = '\0';
		
		char *cedula = strtok(linea, ";");
		char *nombre = strtok(NULL, ";");
		char *clave = strtok(NULL, ";");
		char *carrera = strtok(NULL, ";");
		char *docente = strtok(NULL, ";");
		char *semestreStr = strtok(NULL, ";");
		char *correo = strtok(NULL, ";");
		char *nota1Str = strtok(NULL, ";");
		char *nota2Str = strtok(NULL, ";");
		
		if (cedula && nombre && clave && carrera && docente && semestreStr && correo && nota1Str && nota2Str) {
			int pos = contadorEstudiantes;
			strcpy(listaEstudiantes[pos].cedula, cedula);
			strcpy(listaEstudiantes[pos].nombreCompleto, nombre);
			strcpy(listaEstudiantes[pos].clave, clave);
			strcpy(listaEstudiantes[pos].carrera, carrera);
			strcpy(listaEstudiantes[pos].docenteAsignado, docente);
			listaEstudiantes[pos].nivelSemestre = atoi(semestreStr);
			strcpy(listaEstudiantes[pos].correoElectronico, correo);
			listaEstudiantes[pos].notas[0] = atof(nota1Str);
			listaEstudiantes[pos].notas[1] = atof(nota2Str);
			contadorEstudiantes++;
		}
	}
	fclose(archivo);
}
