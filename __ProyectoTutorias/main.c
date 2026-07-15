#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estudiantes.h"
#include "docentes.h"
#include "tutorias.h"

static void limpiarPantalla() {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

void menuGestionTutoriasAdmin() {
	int opcion = 0;
	do {
		printf("\n--- GESTION DE TUTORIAS (ADMIN) ---\n");
		printf("1. Crear Tutoria\n2. Cancelar Tutoria\n3. Activar Tutoria\n4. Editar Tutoria\n");
		printf("5. Consultar Tutorias por Estudiante\n6. Volver al Menu Administrador\nSeleccione una opcion: ");
		if (scanf("%d", &opcion) != 1) {
			while (getchar() != '\n');
			opcion = 0;
		}

		switch (opcion) {
		case 1: adminCrearTutoria(); break;
		case 2: adminCancelarTutoria(); break;
		case 3: adminActivarTutoria(); break;
		case 4: adminEditarTutoria(); break;
		case 5: consultarTutoriasPorFiltro(); break;
		case 6: limpiarPantalla(); break;
		}
	} while (opcion != 6);
}

void menuAdministrador() {
	int opcion = 0;
	do {
		printf("\n--- MENU ADMINISTRADOR ---\n");
		printf("1. Registrar Estudiante\n2. Listar Estudiantes\n3. Modificar Estudiante\n4. Eliminar Estudiante\n");
		printf("5. Registrar Docente\n6. Listar Docentes\n7. Modificar Docente\n8. Eliminar Docente\n");
		printf("9. Gestionar Tutorias (Crear/Cancelar/Activar/Editar)\n");
		printf("10. Generar Informes .TXT\n11. Cerrar Sesion\nSeleccione una opcion: ");
		if (scanf("%d", &opcion) != 1) {
			while (getchar() != '\n');
			opcion = 0;
		}
		
		switch(opcion) {
		case 1: registrarEstudiante(); break;
		case 2: listarEstudiantes(); break;
		case 3: modificarEstudiante(); break;
		case 4: eliminarEstudiante(); break;
		case 5: registrarDocente(); break;
		case 6: listarDocentes(); break;
		case 7: modificarDocente(); break;
		case 8: eliminarDocente(); break;
		case 9: menuGestionTutoriasAdmin(); break;
		case 10:
			guardarEstudiantesTXT();
			guardarDocentesTXT();
			guardarTutoriasTXT();
			printf("Informes .txt generados exitosamente.\n");
			break;
		case 11: limpiarPantalla(); guardarDocentesBaseDatos(); guardarEstudiantesBaseDatos(); break;
		}
	} while (opcion != 11);
}


void menuDocenteRol(int idx) {
	limpiarPantalla();
	int opcion = 0;
	do {
		printf("\n--- MENU DOCENTE: %s ---\n", listaDocentes[idx].nombreCompleto);
		printf("1. Ver mi Horario de Tutorias\n2. Modificar Notas de Estudiantes\n3. Cerrar Sesion\nSeleccione una opcion: ");
		if (scanf("%d", &opcion) != 1) { while (getchar() != '\n'); opcion = 0; }
		
		switch(opcion) {
		case 1: verHorarioDocente(listaDocentes[idx].codigoDocente); break;
		case 2: modificarNotasDocente(listaDocentes[idx].codigoDocente); break;
		case 3: limpiarPantalla(); guardarDocentesBaseDatos(); break;
		}
	} while (opcion != 3);
}


void menuEstudianteRol(int idx) {
	limpiarPantalla();
	int opcion = 0;
	do {
		printf("\n--- MENU ESTUDIANTE: %s ---\n", listaEstudiantes[idx].nombreCompleto);
		printf("1. Solicitar Tutoria (Carrera/Materia y Horario)\n2. Revisar mis Notas Propias\n3. Cerrar Sesion\nSeleccione una opcion: ");
		if (scanf("%d", &opcion) != 1) { while (getchar() != '\n'); opcion = 0; }
		
		switch(opcion) {
		case 1: agendarTutoria(listaEstudiantes[idx].cedula); break;
		case 2: estudianteVerNotas(listaEstudiantes[idx].cedula); break;
		case 3: limpiarPantalla(); guardarEstudiantesBaseDatos(); break;
		}
	} while (opcion != 3);
}


int login() {
	char user[50], pass[50];
	int autenticado = 0;
	
	printf("--- INICIO DE SESION ---\n");
	printf("Usuario (o escriba 'salir' para terminar el programa): "); scanf("%s", user);
	
	if (strcmp(user, "salir") == 0) {
		return 1;
	}
	
	printf("Contrasena: "); scanf("%s", pass);
	
	if (strcmp(user, "admin") == 0 && strcmp(pass, "admin") == 0) {
		autenticado = 1;
		menuAdministrador();
	} else {
	
		int i;
		for(i=0; i<contadorDocentes; i++) {
			if(strcmp(user, listaDocentes[i].codigoDocente) == 0 && strcmp(pass, listaDocentes[i].clave) == 0) {
				autenticado = 1;
				menuDocenteRol(i);
				break;
			}
		}

		if(!autenticado) {
			for(i=0; i<contadorEstudiantes; i++) {
				if(strcmp(user, listaEstudiantes[i].cedula) == 0 && strcmp(pass, listaEstudiantes[i].clave) == 0) {
					autenticado = 1;
					menuEstudianteRol(i);
					break;
				}
			}
		}
	}
	if(!autenticado) printf("Acceso denegado.\n");
	return 0;
}

int main() {
	cargarEstudiantesBaseDatos();
	cargarDocentesBaseDatos();
	cargarTutoriasBaseDatos();
	int salir = 0;
	while (!salir) {
		salir = login();
	}
	printf("Programa finalizado. Hasta pronto.\n");
	guardarTutoriasBaseDatos();
	return 0;
}
