#ifndef TUTORIAS_H
#define TUTORIAS_H

typedef struct {
	char codigoTutoria[15];
	char cedulaEstudiante[15];
	char codigoDocente[15];
	char fecha[12];
	int bloqueHorario;
	char tema[100];
	int estado;
} Tutoria;

extern Tutoria listaTutorias[100];
extern int contadorTutorias;

// Solicitud de tutoria por parte del estudiante autenticado.
// El estudiante ya no elige el docente (se usa su docente asignado);
// solo elige Carrera/Materia (tema) y Horario (bloque + fecha).
void agendarTutoria(char cedulaEstudiante[]);

int verificarCruceHorario(char codigoDocente[], char fecha[], int bloque);
void consultarTutoriasPorFiltro();
void guardarTutoriasTXT();

void verHorarioDocente(char codigoDocente[]);
void modificarNotasDocente(char codigoDocente[]);
void estudianteVerNotas(char cedulaEstudiante[]);

// Gestion administrativa centralizada: crear, cancelar, activar y editar
// tutorias es una funcion EXCLUSIVA del perfil Administrador.
void adminCrearTutoria();
void adminCancelarTutoria();
void adminActivarTutoria();
void adminEditarTutoria();

#endif
