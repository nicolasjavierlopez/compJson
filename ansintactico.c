#include "anlexico.c"
#include <stdio.h>

//funciones BNF de la gramatica
void json();
void element(int []);
void array(int []);
void array_p(int []);
void element_list(int []);
void element_list_p(int []);
void object(int []);
void object_p(int []);
void attribute(int []);
void attributes_list(int []);
void attributes_list_p(int []);
void attribute_name(int []);
void attribute_value(int []);

//funciones de control
void match(int);

void check_input(int [], int []);

void scanto(int []);

void error_();

//estado de aceptacion para el fuente
int accept=1;


//raiz de la gramatica
void json(){
	int c_siguiente[] = {COMA, R_CORCHETE, R_LLAVE, '\0'};
	//synchset de sincronizacion
	element(c_siguiente);
}

//puede ser un object o un array
void element(int synchset[]){
    int c_primero[] = {L_CORCHETE,L_LLAVE,'\0'};
    int c_siguiente[] = {COMA,R_CORCHETE,R_LLAVE, '\0'};

    check_input(c_primero, synchset);

    if(t.compLex == L_LLAVE)
        object(c_siguiente);
    else if(t.compLex == L_CORCHETE)
        array(c_siguiente);

    check_input(c_siguiente, c_primero);
}

void array(int synchset[]){
	int c_primero[] = {L_CORCHETE, '\0'};
	int c_siguiente[] = {COMA, R_CORCHETE, R_LLAVE, '\0'};

	check_input(c_primero, synchset);

	if (t.compLex == L_CORCHETE){
		match(L_CORCHETE);
		array_p(c_siguiente);
	}

	check_input(c_siguiente, c_primero);

}

void array_p(int synchset[]){
	int c_primero[] = {L_LLAVE, L_CORCHETE, R_CORCHETE, '\0'};

	//PROBLEMA CON L_CORCHETE
	int c_siguiente[] = {COMA, R_CORCHETE, R_LLAVE, L_CORCHETE, '\0'};

	check_input(c_primero, synchset);

	if ((t.compLex == L_LLAVE) || (t.compLex == L_CORCHETE)){
		
		element_list(c_siguiente);
		match(R_CORCHETE);
	}
	else if(t.compLex == R_CORCHETE){
		match(R_CORCHETE);
		
	}

	check_input(c_siguiente, c_primero);
}

void element_list(int synchset[]){
	int c_primero[] = {L_LLAVE, L_CORCHETE, '\0'};
	int c_siguiente[] = {R_CORCHETE, '\0'};

	check_input(c_primero, synchset);

	if((t.compLex == L_LLAVE) || (t.compLex == L_CORCHETE)){
		element(c_siguiente);
		element_list_p(c_siguiente);
	}

	check_input(c_siguiente, c_primero);
}

void element_list_p(int synchset[]){

	if(t.compLex == R_CORCHETE)
       return;

	int c_primero[] = {COMA,'\0'};
	int c_siguiente[] = {R_CORCHETE, '\0'};

	check_input(c_primero, synchset);

	

	if (t.compLex == COMA){
		match(COMA);
		element(c_siguiente);
		element_list_p(c_siguiente);
	}

	check_input(c_siguiente, c_primero);
}

void object(int synchset[]){
	int c_primero[] = {L_LLAVE, '\0'};
	int c_siguiente[] = {COMA, R_CORCHETE, R_LLAVE, '\0'};
	
	check_input(c_primero, synchset);

	if (t.compLex == L_LLAVE) {
		match(L_LLAVE);
		object_p(c_siguiente);
	}
	
	check_input(c_siguiente, c_primero);
}

void object_p(int synchset[]){
	int c_primero[] = {STRING, R_LLAVE, '\0'};

	//problema con STRING
	int c_siguiente[] = {COMA, R_CORCHETE, R_LLAVE, STRING, '\0'};

	check_input(c_primero, synchset);

	if (t.compLex == STRING){
		attributes_list(synchset);
		match(R_LLAVE);
	} else if (t.compLex == R_LLAVE){
		match(R_LLAVE);
	}

	check_input(c_siguiente, c_primero);
}

void attributes_list(int synchset[]){
	int c_primero[] = {STRING, '\0'};
	int c_siguiente[] = {R_LLAVE, '\0'};

	check_input(c_primero, synchset);

	if (t.compLex == STRING){
		attribute(c_siguiente);
		attributes_list_p(c_siguiente);
	}

	check_input(c_siguiente, c_primero);

}

void attributes_list_p(int synchset[]){

	if (t.compLex == R_LLAVE)
        return;

	int c_primero[] = {COMA, '\0'};
	int c_siguiente[] = {R_LLAVE, '\0'};

	check_input(c_primero, synchset);


	if (t.compLex == COMA){
		match(COMA);
		attribute(c_siguiente);
		attributes_list_p(c_siguiente);
	}

	check_input(c_siguiente, c_primero);
}

void attribute(int synchset[]){
	int c_primero[] = {STRING, '\0'};
	int c_siguiente[] = {COMA, R_LLAVE, '\0'};

	check_input(c_primero, synchset);

	if (t.compLex == STRING){
		attribute_name(c_siguiente);
		match(DOS_PUNTOS);
		attribute_value(c_siguiente);
	}

	check_input(c_siguiente, c_primero);
}

void attribute_name(int synchset[]){
	int c_primero[] = {STRING, '\0'};
	int c_siguiente[] = {DOS_PUNTOS, '\0'};

	check_input(c_primero, synchset);

	if (t.compLex == STRING)
		match(STRING);

	check_input(c_siguiente, c_primero);
}

void attribute_value(int synchset[]){
	int c_primero[] = {L_LLAVE, L_CORCHETE, STRING,
		NUMBER, PR_TRUE, PR_FALSE, PR_NULL, '\0'};
	int c_siguiente[] = {COMA, R_LLAVE, '\0'};

	check_input(c_primero, synchset);

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

	check_input(c_siguiente, c_primero);

}

//llama al siguiente elemento de la pila
void match(int expectedToken){
    if(t.compLex == expectedToken)
        sigLex();
}


void check_input(int c_primero[], int c_siguiente[]){

    int i=0;
    if(t.compLex == EOF)
    	return;

    //entra cuando no hay errores
    while(c_primero[i] != '\0') 
    {
        if(t.compLex == c_primero[i])
            return;
        i++;
    }
 
 	i=0;    
    
    int synchset[]={1}; //acumulador
    
    while(c_primero[i] != '\0'){ 
        synchset[i] = c_primero[i];
        i++;
    }
    int j=0;

    while(c_siguiente[j] != '\0'){
        synchset[i] = c_siguiente[j];
        i++; //aumenta i
        j++; //aumenta j
    }

    scanto(synchset);

}


//si no esta en el synchset llama a sigLex() e imprimer errores del check input
void scanto(int synch[]){
    error_();
	int i = 0;
    while(t.compLex != synch[i] && t.compLex != EOF){   
        if (synch[i] == '\0'){
            sigLex();
            i=0;        
        }
        i++;
    }
    //el sigLex que falta luego de salir
    sigLex();
    return;    	   
}

/*muestra los errores en pantalla*/
void error_(){
	accept=0;
   	printf("\nError: Linea %d. Se encontro %s.\n", numLinea, t.lexema);
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
		else {
			printf("Parser finalizado con errores.\n");
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