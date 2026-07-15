#ifndef DOCENTES_H
#define DOCENTES_H

typedef struct {
	char codigoDocente[15];
	char clave[20];
	char nombreCompleto[100];
	char areaConocimiento[50];
	int horarioBloque;
} Docente;

extern Docente listaDocentes[100];
extern int contadorDocentes;

void registrarDocente();
void listarDocentes();
int buscarDocentePorCodigo(char codigo[]);
void modificarDocente();
void eliminarDocente();
void guardarDocentesTXT();

#endif
