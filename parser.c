#include "anlexico.c"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define CANT_COMP_LEX 10


//funciones BNF de la gramatica
void json();
void element();
void array();
void array_p();
void element_list();
void element_list_p();
void object();
void object_p();
void attribute();
void attributes_list();
void attributes_list_p();
void attribute_name();
void attribute_value(int []);
void match(int);
void error_s(int []);
int accept=1;


void json(){
	//primero de element
	int c_primero[] = {L_LLAVE, L_CORCHETE, '\0'};
	//siguiente de element
	int c_siguiente[] = {COMA, R_CORCHETE, R_LLAVE, '\0'};
	//synchset de sincronizacion
	element(c_siguiente);
}

//puede ser un object o un array
void element(int syncset[]){
    int c_primero[] = {L_CORCHETE,L_LLAVE,'\0'};
    int c_siguiente[] = {COMA,R_CORCHETE,R_LLAVE, '\0'};
    //check_input(c_primero,syncset);

    if(t.compLex == L_LLAVE)
        object(c_siguiente);
    else if(t.compLex == L_CORCHETE)
        array(c_siguiente);

    //check_input(c_siguiente, c_primero);
}

void array(int synchset[]){
	int c_primero[] = {L_CORCHETE, '\0'};
	int c_siguiente[] = {COMA, R_CORCHETE, R_LLAVE, '\0'};

	if (t.compLex == L_CORCHETE){
		match(L_CORCHETE);
		array_p(c_siguiente);
	}

}

void array_p(int synchset[]){
	int c_primero[] = {L_LLAVE, L_CORCHETE, R_CORCHETE, '\0'};
	int c_siguiente[] = {COMA, R_CORCHETE, R_LLAVE, '\0'};

	if ((t.compLex == L_LLAVE) || (t.compLex == L_CORCHETE)){
		
		element_list(c_siguiente);
		match(R_CORCHETE);
	}
	else if(t.compLex == R_CORCHETE){
		match(R_CORCHETE);
		element_list(c_siguiente);
	}
}

void element_list(int synchset[]){
	int c_primero[] = {L_LLAVE, L_CORCHETE, '\0'};
	int c_siguiente[] = {R_CORCHETE, '\0'};

	if((t.compLex == L_LLAVE) || (t.compLex == L_CORCHETE)){
		element(c_siguiente);
		element_list_p(c_siguiente);
	}
}

void element_list_p(int synchset[]){
	int c_primero[] = {COMA,'\0'};
	int c_siguiente[] = {R_CORCHETE, '\0'};

	if (t.compLex == COMA){
		match(COMA);
		element(c_siguiente);
		element_list_p(c_siguiente);
	}

}

void object(int synchset[]){
	int c_primero[] = {L_LLAVE, '\0'};
	int c_siguiente[] = {COMA, R_CORCHETE, R_LLAVE, '\0'};
	//check_input(c_primero, synchset);

	if (t.compLex == L_LLAVE) {
		match(L_LLAVE);
		object_p(c_siguiente);
	}
	//check_input(c_siguiente, c_primero);
}

void object_p(int synchset[]){
	int c_primero[] = {STRING, R_LLAVE, '\0'};
	int c_siguiente[] = {COMA, R_CORCHETE, R_LLAVE, '\0'};

	if (t.compLex == STRING){
		attributes_list(synchset);
		match(R_LLAVE);
	} else if (t.compLex == R_LLAVE){
		match(R_LLAVE);
	}
}

void attributes_list(int synchset[]){
	int c_primero[] = {STRING, '\0'};
	int c_siguiente[] = {R_LLAVE, '\0'};

	if (t.compLex == STRING){
		attribute(c_siguiente);
		attributes_list_p(c_siguiente);
	}

}

void attributes_list_p(int synchset[]){
	int c_primero[] = {COMA, '\0'};
	int c_siguiente[] = {R_LLAVE, '\0'};

	if (t.compLex == COMA){
		match(COMA);
		attribute(c_siguiente);
		attributes_list_p(c_siguiente);
	}
}

void attribute(int synchset[]){
	int c_primero[] = {STRING, '\0'};
	int c_siguiente[] = {COMA, '\0'};

	if (t.compLex == STRING){
		attribute_name(c_siguiente);
		match(DOS_PUNTOS);
		attribute_value(c_siguiente);
	}
}

void attribute_name(int synchset[]){
	int c_primero[] = {STRING, '\0'};
	int c_siguiente[] = {COMA, '\0'};

	if (t.compLex == STRING){
		match(STRING);
	}
}

void attribute_value(int synchset[]){
	int c_primero[] = {L_LLAVE, L_CORCHETE, STRING,
		NUMBER, PR_TRUE, PR_FALSE, PR_NULL, '\0'};
	int c_siguiente[] = {COMA, '\0'};

	if (t.compLex == L_LLAVE){
		element(c_siguiente);
	}
	else if (t.compLex == L_CORCHETE) {
		element(c_siguiente);
	}
	else if (t.compLex == STRING){
		match(STRING);
	}
	else if (t.compLex == NUMBER){
		match(NUMBER);
	}
	else if (t.compLex == PR_TRUE){
		match(PR_TRUE);
	}
	else if (t.compLex == PR_FALSE){
		match(PR_FALSE);
	}
	else if (t.compLex == PR_NULL){
		match(PR_NULL);
	}

}
//llama al siguiente elemento
void match(int expectedToken){
    if(t.compLex == expectedToken){
        sigLex();
    }
}


void check_input(int c_primero[], int c_siguiente[]){

    int synchset[]={CANT_COMP_LEX};
    int i=0;
    if(t.compLex == EOF) return;

    //mientras no sea el fin del array c_primero
    while(c_primero[i] != '\0') 
    {
        if(t.compLex == c_primero[i])
            return;
        i++;
    }
    i=0;    
    while(c_primero[i] != '\0'){ 
        synchset[i] = c_primero[i];
        i++;
    }
    int j=0;

    //
    while(c_siguiente[j] != '\0'){
        synchset[i] = c_siguiente[j];
        i++;
        j++;
    }
    error_s(synchset);
}

void error_s(int synch[]){
    accept=0;
   	printf("\nError en la linea: %d. Se encontro %s.\n", numLinea, t.lexema);
	int i = 0;
    while(t.compLex != synch[i] && t.compLex != EOF){   
        if (synch[i] == '\0')
        {
            sigLex();
            i=0;        
        }
        i++;
    }
    sigLex();
    return;    	   
}

int main(int argc,char* args[]){
	//como imprimir un \n
	if(argc > 1)
	{
		if (!(archivo=fopen(args[1],"rt")))
		{
			printf("Archivo no encontrado.\n");
			exit(1);
		}
		sigLex();
        json();
        if(accept){
        	printf("No se encontraron errores sintacticos.\n");
		}
		fclose(archivo);
	}
	else
	{
		printf("Debe pasar como parametro el path al archivo fuente.\n");
		exit(1);
	}

	return 0;
}