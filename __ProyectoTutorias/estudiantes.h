#ifndef ESTUDIANTES_H
#define ESTUDIANTES_H

typedef struct {
	char cedula[15];
	char clave[20];
	char nombreCompleto[100];
	char carrera[50];
	int nivelSemestre;
	char correoElectronico[70];
	float notas[2];
	char docenteAsignado[15];
} Estudiante;

extern Estudiante listaEstudiantes[100];
extern int contadorEstudiantes;

void registrarEstudiante();
void listarEstudiantes();
int buscarEstudiantePorCedula(char cedula[]);
int compararCedulasEstudiantes(char texto1[], char texto2[]);
int validarCorreo(char correo[]);
void modificarEstudiante();
void eliminarEstudiante();
void guardarEstudiantesTXT();

#endif
