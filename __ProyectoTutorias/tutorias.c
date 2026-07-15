#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tutorias.h"
#include "estudiantes.h"
#include "docentes.h"

Tutoria listaTutorias[100];
int contadorTutorias = 0;

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

int compararTextosTutorias(char texto1[], char texto2[]) {
	int i = 0;
	while (texto1[i] != '\0' || texto2[i] != '\0') {
		if (texto1[i] != texto2[i]) {
			return 0;
		}
		i = i + 1;
	}
	return 1;
}

int verificarCruceHorario(char codigoDocente[], char fecha[], int bloque) {
	int i = 0;
	for (i = 0; i < contadorTutorias; i = i + 1) {
		if (listaTutorias[i].estado == 1) {
			if (compararTextosTutorias(listaTutorias[i].codigoDocente, codigoDocente) == 1 &&
				compararTextosTutorias(listaTutorias[i].fecha, fecha) == 1 &&
				listaTutorias[i].bloqueHorario == bloque) {
				return 1;
			}
		}
	}
	return 0;
}

int validarFechaYLimite(char fecha[], int *d, int *m, int *a) {
	int i = 0;
	while (fecha[i] != '\0') i++;
	if (i != 10) return 0;
	if (fecha[2] != '-' || fecha[5] != '-') return 0;
	for (i = 0; i < 10; i++) {
		if (i != 2 && i != 5) {
			if (fecha[i] < '0' || fecha[i] > '9') return 0;
		}
	}
	*d = (fecha[0] - '0') * 10 + (fecha[1] - '0');
	*m = (fecha[3] - '0') * 10 + (fecha[4] - '0');
	*a = (fecha[6] - '0') * 1000 + (fecha[7] - '0') * 100 + (fecha[8] - '0') * 10 + (fecha[9] - '0');

	if (*m < 1 || *m > 12 || *d < 1 || *d > 31) return 0;

	if (*a < 2026) return 0;
	if (*a == 2026) {
		if (*m < 6) return 0;
		if (*m == 6) {
			if (*d < 24) return 0;
		}
	}
	return 1;
}

// Genera el siguiente codigo de tutoria (TUT<numero>) para la posicion dada.
static void generarCodigoTutoria(int pos, char codigoOut[]) {
	int num = pos + 1;
	codigoOut[0] = 'T'; codigoOut[1] = 'U'; codigoOut[2] = 'T';
	if (num < 10) {
		codigoOut[3] = (char)(num + '0');
		codigoOut[4] = '\0';
	} else {
		codigoOut[3] = (char)((num / 10) + '0');
		codigoOut[4] = (char)((num % 10) + '0');
		codigoOut[5] = '\0';
	}
}

// --------------------------------------------------------------------------
// SOLICITUD DE TUTORIA (rol Estudiante)
// El estudiante ya NO elige al docente: se usa automaticamente el docente
// que le fue asignado obligatoriamente al momento de su registro.
// El estudiante solo puede elegir Carrera/Materia (tema) y Horario (bloque+fecha).
// --------------------------------------------------------------------------
void agendarTutoria(char cedulaEstudiante[]) {
	limpiarPantalla();
	printf("--- Solicitar Tutoria ---\n");

	int idxEst = -1;
	int i = 0;
	for (i = 0; i < contadorEstudiantes; i = i + 1) {
		if (compararCedulasEstudiantes(listaEstudiantes[i].cedula, cedulaEstudiante) == 1) {
			idxEst = i;
			break;
		}
	}

	if (idxEst == -1) {
		printf("Error: No se encontro su perfil de estudiante.\n");
		esperarEnter();
		return;
	}

	char codigoDocente[20];
	strcpy(codigoDocente, listaEstudiantes[idxEst].docenteAsignado);

	int idxDocente = -1;
	for (i = 0; i < contadorDocentes; i = i + 1) {
		if (compararTextosTutorias(listaDocentes[i].codigoDocente, codigoDocente) == 1) {
			idxDocente = i;
			break;
		}
	}

	if (idxDocente == -1) {
		printf("Error: Su docente asignado ya no se encuentra registrado. Contacte al administrador.\n");
		esperarEnter();
		return;
	}

	printf("Carrera: %s\n", listaEstudiantes[idxEst].carrera);
	printf("Docente asignado: %s (Codigo: %s)\n", listaDocentes[idxDocente].nombreCompleto, codigoDocente);

	char fechaTemporal[20];
	int d = 0, m = 0, a = 0;
	int fechaValida = 0;
	while (fechaValida == 0) {
		printf("Ingrese fecha (DD-MM-AAAA) posterior al 24-06-2026: ");
		scanf("%s", fechaTemporal);
		if (validarFechaYLimite(fechaTemporal, &d, &m, &a) == 1) {
			fechaValida = 1;
		} else {
			printf("Error: Formato invalido o fecha anterior a la permitida (24-06-2026).\n");
		}
	}

	int bloque = 0;
	printf("Seleccione Horario disponible (segun bloque de su docente asignado):\n");
	printf("1. 07:00 a 09:00\n2. 09:00 a 11:00\n3. 11:00 a 13:00\nSeleccion: ");
	while (scanf("%d", &bloque) != 1 || bloque < 1 || bloque > 3) {
		while (getchar() != '\n');
		printf("Opcion invalida (1-3): ");
	}

	if (listaDocentes[idxDocente].horarioBloque != bloque) {
		printf("Error: Su docente asignado no atiende en ese bloque horario. Su bloque es: %d\n",
			   listaDocentes[idxDocente].horarioBloque);
		esperarEnter();
		return;
	}

	if (verificarCruceHorario(codigoDocente, fechaTemporal, bloque) == 1) {
		printf("Error: El docente ya tiene otra tutoria agendada en ese mismo bloque y fecha.\n");
		esperarEnter();
		return;
	}

	int pos = contadorTutorias;
	strcpy(listaTutorias[pos].cedulaEstudiante, cedulaEstudiante);
	strcpy(listaTutorias[pos].codigoDocente, codigoDocente);
	strcpy(listaTutorias[pos].fecha, fechaTemporal);
	listaTutorias[pos].bloqueHorario = bloque;

	printf("Ingrese la Materia o tema central a tratar (use_guiones): ");
	scanf("%s", listaTutorias[pos].tema);

	generarCodigoTutoria(pos, listaTutorias[pos].codigoTutoria);
	listaTutorias[pos].estado = 1;

	printf("Tutoria solicitada con el codigo: %s\n", listaTutorias[pos].codigoTutoria);
	contadorTutorias = contadorTutorias + 1;
	esperarEnter();
}

void consultarTutoriasPorFiltro() {
	char cedulaBuscada[20];
	limpiarPantalla();
	printf("--- Consulta de Tutorias ---\n");
	printf("Ingrese la cedula del estudiante: ");
	scanf("%s", cedulaBuscada);

	int i = 0;
	int coincidencias = 0;
	printf("\nResultados:\n");
	for (i = 0; i < contadorTutorias; i = i + 1) {
		if (compararTextosTutorias(listaTutorias[i].cedulaEstudiante, cedulaBuscada) == 1) {
			coincidencias = coincidencias + 1;
			printf("Codigo: %s | Docente: %s | Fecha: %s | Bloque: %d | Tema: %s | Estado: ",
				   listaTutorias[i].codigoTutoria, listaTutorias[i].codigoDocente,
				   listaTutorias[i].fecha, listaTutorias[i].bloqueHorario, listaTutorias[i].tema);
			if (listaTutorias[i].estado == 1) printf("Activa\n");
			else printf("Cancelada\n");
		}
	}
	if (coincidencias == 0) {
		printf("No se encontraron tutorias para la cedula ingresada.\n");
	}
	esperarEnter();
}

void verHorarioDocente(char codigoDocente[]) {
	limpiarPantalla();
	int i = 0;
	int count = 0;
	printf("--- Tus Tutorias Asignadas ---\n");
	for (i = 0; i < contadorTutorias; i = i + 1) {
		if (compararTextosTutorias(listaTutorias[i].codigoDocente, codigoDocente) == 1 && listaTutorias[i].estado == 1) {
			count = count + 1;
			printf("Cod: %s | Estudiante: %s | Fecha: %s | Bloque: %d | Tema: %s\n",
				   listaTutorias[i].codigoTutoria, listaTutorias[i].cedulaEstudiante,
				   listaTutorias[i].fecha, listaTutorias[i].bloqueHorario, listaTutorias[i].tema);
		}
	}
	if (count == 0) printf("No posees tutorias activas agendadas.\n");
	esperarEnter();
}

void modificarNotasDocente(char codigoDocente[]) {
	char cedulaEst[20];
	limpiarPantalla();
	printf("--- Modificar Notas de Estudiante ---\n");
	printf("Ingrese la cedula del estudiante: ");
	scanf("%s", cedulaEst);

	int i = 0;
	int flag = 0;
	for (i = 0; i < contadorTutorias; i = i + 1) {
		if (compararTextosTutorias(listaTutorias[i].codigoDocente, codigoDocente) == 1 &&
			compararTextosTutorias(listaTutorias[i].cedulaEstudiante, cedulaEst) == 1) {
			flag = 1;
			break;
		}
	}

	if (flag == 0) {
		printf("Error: No puedes modificar notas de este estudiante si no registra tutorias contigo.\n");
		esperarEnter();
		return;
	}

	int idxEst = -1;
	for (i = 0; i < contadorEstudiantes; i = i + 1) {
		if (compararCedulasEstudiantes(listaEstudiantes[i].cedula, cedulaEst) == 1) {
			idxEst = i;
			break;
		}
	}

	if (idxEst != -1) {
		printf("Estudiante: %s\n", listaEstudiantes[idxEst].nombreCompleto);
		printf("Ingrese la Nota 1: ");
		while (scanf("%f", &listaEstudiantes[idxEst].notas[0]) != 1) {
			while (getchar() != '\n');
			printf("Invalido. Ingrese nota numerica: ");
		}
		printf("Ingrese la Nota 2: ");
		while (scanf("%f", &listaEstudiantes[idxEst].notas[1]) != 1) {
			while (getchar() != '\n');
			printf("Invalido. Ingrese nota numerica: ");
		}
		printf("Notas guardadas exitosamente en el perfil.\n");
	}
	esperarEnter();
}

void estudianteVerNotas(char cedulaEstudiante[]) {
	limpiarPantalla();
	int i = 0;
	for (i = 0; i < contadorEstudiantes; i = i + 1) {
		if (compararCedulasEstudiantes(listaEstudiantes[i].cedula, cedulaEstudiante) == 1) {
			printf("--- Tus Notas Guardadas ---\n");
			printf("Nota 1: %.2f\n", listaEstudiantes[i].notas[0]);
			printf("Nota 2: %.2f\n", listaEstudiantes[i].notas[1]);
			esperarEnter();
			return;
		}
	}
	esperarEnter();
}


void guardarTutoriasTXT() {
	FILE *archivo = fopen("reporte_horarios_tutorias.txt", "w");
	if (archivo == NULL) return;
	fprintf(archivo, "=========================================================================\n");
	fprintf(archivo, "                     HORARIOS GENERALES DE TUTORIAS                      \n");
	fprintf(archivo, "=========================================================================\n");
	int i = 0;
	for (i = 0; i < contadorTutorias; i = i + 1) {
		fprintf(archivo, "Codigo: %s | Estudiante: %s | Docente: %s | Fecha: %s | Bloque: %d | Estado: %s\n",
				listaTutorias[i].codigoTutoria, listaTutorias[i].cedulaEstudiante,
				listaTutorias[i].codigoDocente, listaTutorias[i].fecha, listaTutorias[i].bloqueHorario,
				(listaTutorias[i].estado == 1 ? "Activa" : "Cancelada"));
	}
	fclose(archivo);
}

void guardarTutoriasBaseDatos() {
	FILE *archivo = fopen("tutorias_db.txt", "w");
	if (archivo == NULL) {
		printf("Error al guardar la base de datos de tutorias.\n");
		return;
	}
	
	for (int i = 0; i < contadorTutorias; i++) {
		// Guardamos todos los atributos estructuradamente de la tutoría
		fprintf(archivo, "%s;%s;%s;%s;%d;%s;%d\n",
				listaTutorias[i].codigoTutoria,
				listaTutorias[i].cedulaEstudiante,
				listaTutorias[i].codigoDocente,
				listaTutorias[i].fecha,
				listaTutorias[i].bloqueHorario,
				listaTutorias[i].tema,
				listaTutorias[i].estado);
	}
	fclose(archivo);
}

// NUEVA FUNCIÓN: Carga la base de datos de tutorías al iniciar el programa
void cargarTutoriasBaseDatos() {
	FILE *archivo = fopen("tutorias_db.txt", "r");
	if (archivo == NULL) {
		contadorTutorias = 0;
		return;
	}
	
	char linea[400];
	contadorTutorias = 0;
	
	while (fgets(linea, sizeof(linea), archivo) != NULL && contadorTutorias < 100) {
		linea[strcspn(linea, "\n")] = '\0';
		
		char *codigo = strtok(linea, ";");
		char *cedula = strtok(NULL, ";");
		char *docente = strtok(NULL, ";");
		char *fecha = strtok(NULL, ";");
		char *bloqueStr = strtok(NULL, ";");
		char *tema = strtok(NULL, ";");
		char *estadoStr = strtok(NULL, ";");
		
		if (codigo && cedula && docente && fecha && bloqueStr && tema && estadoStr) {
			int pos = contadorTutorias;
			strcpy(listaTutorias[pos].codigoTutoria, codigo);
			strcpy(listaTutorias[pos].cedulaEstudiante, cedula);
			strcpy(listaTutorias[pos].codigoDocente, docente);
			strcpy(listaTutorias[pos].fecha, fecha);
			listaTutorias[pos].bloqueHorario = atoi(bloqueStr);
			strcpy(listaTutorias[pos].tema, tema);
			listaTutorias[pos].estado = atoi(estadoStr);
			contadorTutorias++;
		}
	}
	fclose(archivo);
}
// --------------------------------------------------------------------------
// GESTION ADMINISTRATIVA CENTRALIZADA (rol Administrador)
// Crear, cancelar, activar y editar tutorias es exclusivo del Administrador.
// --------------------------------------------------------------------------

void adminCrearTutoria() {
	char cedulaEstudiante[20];
	char codigoDocente[20];
	char fechaTemporal[20];
	int bloque = 0;
	int d = 0, m = 0, a = 0;

	limpiarPantalla();
	printf("--- [ADMIN] Crear Tutoria ---\n");
	printf("Ingrese la cedula del estudiante: ");
	scanf("%s", cedulaEstudiante);

	if (buscarEstudiantePorCedula(cedulaEstudiante) == 0) {
		printf("Error: El estudiante no esta registrado.\n");
		esperarEnter();
		return;
	}

	printf("Ingrese el codigo del docente: ");
	scanf("%s", codigoDocente);

	if (buscarDocentePorCodigo(codigoDocente) == 0) {
		printf("Error: El docente no esta registrado.\n");
		esperarEnter();
		return;
	}

	int idxDocente = -1;
	int i = 0;
	for (i = 0; i < contadorDocentes; i = i + 1) {
		if (compararTextosTutorias(listaDocentes[i].codigoDocente, codigoDocente) == 1) {
			idxDocente = i;
			break;
		}
	}

	int fechaValida = 0;
	while (fechaValida == 0) {
		printf("Ingrese fecha (DD-MM-AAAA) posterior al 24-06-2026: ");
		scanf("%s", fechaTemporal);
		if (validarFechaYLimite(fechaTemporal, &d, &m, &a) == 1) {
			fechaValida = 1;
		} else {
			printf("Error: Formato invalido o fecha anterior a la permitida (24-06-2026).\n");
		}
	}

	printf("Seleccione bloque horario para este docente:\n");
	printf("1. 07:00 a 09:00\n2. 09:00 a 11:00\n3. 11:00 a 13:00\nSeleccion: ");
	while (scanf("%d", &bloque) != 1 || bloque < 1 || bloque > 3) {
		while (getchar() != '\n');
		printf("Opcion invalida (1-3): ");
	}

	if (listaDocentes[idxDocente].horarioBloque != bloque) {
		printf("Error: El docente no atiende en ese bloque horario.\n");
		esperarEnter();
		return;
	}

	if (verificarCruceHorario(codigoDocente, fechaTemporal, bloque) == 1) {
		printf("Error: El docente ya tiene otra tutoria agendada en ese mismo bloque y fecha.\n");
		esperarEnter();
		return;
	}

	int pos = contadorTutorias;
	strcpy(listaTutorias[pos].cedulaEstudiante, cedulaEstudiante);
	strcpy(listaTutorias[pos].codigoDocente, codigoDocente);
	strcpy(listaTutorias[pos].fecha, fechaTemporal);
	listaTutorias[pos].bloqueHorario = bloque;

	printf("Ingrese el tema central (use_guiones): ");
	scanf("%s", listaTutorias[pos].tema);

	generarCodigoTutoria(pos, listaTutorias[pos].codigoTutoria);
	listaTutorias[pos].estado = 1;

	printf("Tutoria creada con el codigo: %s\n", listaTutorias[pos].codigoTutoria);
	contadorTutorias = contadorTutorias + 1;
	esperarEnter();
}

// Busca una tutoria por codigo y devuelve su indice, o -1 si no existe.
static int buscarTutoriaPorCodigo(char codigoBuscado[]) {
	int i = 0;
	for (i = 0; i < contadorTutorias; i = i + 1) {
		if (compararTextosTutorias(listaTutorias[i].codigoTutoria, codigoBuscado) == 1) {
			return i;
		}
	}
	return -1;
}

void adminCancelarTutoria() {
	char codigoBuscado[20];
	limpiarPantalla();
	printf("--- [ADMIN] Cancelar Tutoria ---\n");
	printf("Ingrese el codigo de la tutoria (ej: TUT1): ");
	scanf("%s", codigoBuscado);

	int idx = buscarTutoriaPorCodigo(codigoBuscado);
	if (idx == -1) {
		printf("Error: No se encontro ninguna tutoria con ese codigo.\n");
		esperarEnter();
		return;
	}

	listaTutorias[idx].estado = 2;
	printf("Tutoria %s cancelada correctamente.\n", listaTutorias[idx].codigoTutoria);
	esperarEnter();
}

void adminActivarTutoria() {
	char codigoBuscado[20];
	limpiarPantalla();
	printf("--- [ADMIN] Activar Tutoria ---\n");
	printf("Ingrese el codigo de la tutoria (ej: TUT1): ");
	scanf("%s", codigoBuscado);

	int idx = buscarTutoriaPorCodigo(codigoBuscado);
	if (idx == -1) {
		printf("Error: No se encontro ninguna tutoria con ese codigo.\n");
		esperarEnter();
		return;
	}

	if (verificarCruceHorario(listaTutorias[idx].codigoDocente, listaTutorias[idx].fecha, listaTutorias[idx].bloqueHorario) == 1) {
		printf("Error: Existe un cruce de horario con otra tutoria activa del mismo docente.\n");
		esperarEnter();
		return;
	}

	listaTutorias[idx].estado = 1;
	printf("Tutoria %s activada correctamente.\n", listaTutorias[idx].codigoTutoria);
	esperarEnter();
}

void adminEditarTutoria() {
	char codigoBuscado[20];
	limpiarPantalla();
	printf("--- [ADMIN] Editar Tutoria ---\n");
	printf("Ingrese el codigo de la tutoria a editar (ej: TUT1): ");
	scanf("%s", codigoBuscado);

	int idx = buscarTutoriaPorCodigo(codigoBuscado);
	if (idx == -1) {
		printf("Error: No se encontro ninguna tutoria con ese codigo.\n");
		esperarEnter();
		return;
	}

	printf("Tutoria encontrada. Estudiante: %s | Docente: %s | Fecha: %s | Bloque: %d | Tema: %s\n",
		   listaTutorias[idx].cedulaEstudiante, listaTutorias[idx].codigoDocente,
		   listaTutorias[idx].fecha, listaTutorias[idx].bloqueHorario, listaTutorias[idx].tema);

	char fechaTemporal[20];
	int d = 0, m = 0, a = 0;
	int fechaValida = 0;
	while (fechaValida == 0) {
		printf("Ingrese nueva fecha (DD-MM-AAAA) posterior al 24-06-2026: ");
		scanf("%s", fechaTemporal);
		if (validarFechaYLimite(fechaTemporal, &d, &m, &a) == 1) {
			fechaValida = 1;
		} else {
			printf("Error: Formato invalido o fecha anterior a la permitida (24-06-2026).\n");
		}
	}

	int bloque = 0;
	printf("Seleccione nuevo bloque horario:\n1. 07:00 a 09:00\n2. 09:00 a 11:00\n3. 11:00 a 13:00\nSeleccion: ");
	while (scanf("%d", &bloque) != 1 || bloque < 1 || bloque > 3) {
		while (getchar() != '\n');
		printf("Opcion invalida (1-3): ");
	}

	strcpy(listaTutorias[idx].fecha, fechaTemporal);
	listaTutorias[idx].bloqueHorario = bloque;

	printf("Ingrese nuevo tema central (use_guiones): ");
	scanf("%s", listaTutorias[idx].tema);

	printf("Tutoria %s editada correctamente.\n", listaTutorias[idx].codigoTutoria);
	esperarEnter();
}
