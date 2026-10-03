// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <math.h>

#define DEFAULT_PORT 8080
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) 
{
	int sock, new_socket;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	char buffer[BUFFER_SIZE] = {0};
	char option;	

	int port, num1, num2, suma, resta, multiplicacio;
	float divisio;
	char *endptr;

	if (argc == 1) {
		// Cap argument -> port per defecte
		port = DEFAULT_PORT;
		printf("Cap port indicat. Utilitzant valor per defecte: %d\n", port);
	}
	else if (argc == 2) {
		errno = 0;
		port = strtol(argv[1], &endptr, 10);

		// Comprovacions
		if (errno != 0 || *endptr != '\0' || port < 1 || port > 65535) {
			fprintf(stderr, "Error: el port ha de ser un enter entre 1 i 65535.\n");
			exit(EXIT_FAILURE);
		}
	}
	else {
		fprintf(stderr, "Ús: %s [port]\n", argv[0]);
		exit(EXIT_FAILURE);
	}


	// Crear el socket
	// Afegiu comentari explicant els arguments
	
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("Error en crear el socket");
		exit(EXIT_FAILURE);
	}


	// Afegiu comentari explicant què són aquests 3 paràmetres,
	// per què s'utilitzen htons() i htonl(),
	// i per què s'utilitzen els valors que hi ha

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);	// Per a INADDR_ANY no faria falta htonl(), però li posem per coherència
	address.sin_port = htons(port);

	// Enllaçar el socket al port especificat
	// Afegiu comentari explicant els arguments
	if (bind(sock, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("Error en fer el bind");
		close(sock);
		exit(EXIT_FAILURE);
	}

	// Escoltar connexions entrants
	// Afegiu comentari explicant els arguments
	
	if (listen(sock, 3) < 0) {
		perror("Error en escoltar");
		close(sock);
		exit(EXIT_FAILURE);
	}

	printf("Servidor en funcionament, esperant connexions...\n");

	while (1) {
		// Acceptar connexions de clients
		// Afegiu comentari explicant els arguments i per què hi ha i cal new_socket si ja tenim sock
		
		if ((new_socket = accept(sock, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
			perror("Error en acceptar la connexió");
			close(sock);
			exit(EXIT_FAILURE);
		}

		 while (1) {
			memset(buffer, 0, BUFFER_SIZE);
			
			// Llegir missatge del client
			if (recv(new_socket, buffer, BUFFER_SIZE, 0) <= 0) {
				printf("Client desconnectat (PID: %d)\n", getpid());
				break;
			}

			// Aquí haureu d'implementar l'anàlisi de la cadena rebuda per saber l'operació,
			// i si n'hi ha els arguments, executar-la i tornar el(s) resultat(s
			option=buffer[0];  // Suposant que l'opció és el primer caràcter del missatge rebut
			switch (option)
					{
					case '1':
						printf("Missatge rebut del client (PID: %d): %s\n", getpid(), buffer);
						send(new_socket, "Missatge rebut\n", strlen("Missatge rebut\n"), 0);
						break;
					case '6':
						printf("Tancant connexió amb el client (PID: %d)...\n", getpid());
						send(new_socket, "Connexió tancada\n", strlen("Connexió tancada\n"), 0);
						close(new_socket);
						break;	
												
						// Llegir resposta del servidor
						// Afegiu un control d'errors al recv()
						// Afegiu comentari explicant què fa i per què s'utilitza memset
						// Afegiu comentari explicant els arguments de la crida a recv()

					case '2':
						buffer[0] = ' '; // Eliminar l'opció del missatge per poder fer sscanf correctament	
						suma = 0;
						printf("Missatge rebut del client (PID: %d): %s\n", getpid(), buffer);
						// Suposant que el missatge conté dos números separats per ';'
						sscanf(buffer, "%d;%d", &num1, &num2);
						suma = num1 + num2;
						sprintf(buffer, "La suma de %d i %d és %d\n", num1, num2, suma);
						send(new_socket, buffer, strlen(buffer), 0);
						break;

					case '3':
						buffer[0] = ' '; // Eliminar l'opció del missatge per poder fer sscanf correctament
						resta = 0;
						printf("Missatge rebut del client (PID: %d): %s\n", getpid(), buffer);
						// Suposant que el missatge conté dos números separats per ';'
						sscanf(buffer, "%d;%d", &num1, &num2);
						resta = num1 - num2;
						sprintf(buffer, "La resta de %d i %d és %d\n", num1, num2, resta);
						send(new_socket, buffer, strlen(buffer), 0);
						break;

					case '4':
						buffer[0] = ' '; // Eliminar l'opció del missatge per poder fer sscanf correctament
						multiplicacio = 0;
						printf("Missatge rebut del client (PID: %d): %s\n", getpid(), buffer);
						// Suposant que el missatge conté dos números separats per ';'
						sscanf(buffer, "%d;%d", &num1, &num2);
						multiplicacio = num1 * num2;
						sprintf(buffer, "La multiplicació de %d i %d és %d\n", num1, num2, multiplicacio);
						send(new_socket, buffer, strlen(buffer), 0);
						break;

					case '5':
						buffer[0] = ' '; // Eliminar l'opció del missatge per poder fer sscanf correctament
						divisio = 0.00; 
						printf("Missatge rebut del client (PID: %d): %s\n", getpid(), buffer);
						// Suposant que el missatge conté dos números separats per ';'
						sscanf(buffer, "%d;%d", &num1, &num2);
						if (num2 != 0) {
							divisio = (float)num1 / num2;
							sprintf(buffer, "La divisió de %d i %d és %.2f\n", num1, num2, divisio);
						}else {
							sprintf(buffer, "Error: Divisió per zero no permesa.\n");
						}
						send(new_socket, buffer, strlen(buffer), 0);
						break;

					default:
						printf("Opció invàlida\n");
						break;
					}
				
			// A continuació hi ha el codi corresponent a l'opció d'Enviar missatge, amb el retorn d'una cadena fixa,
			// i la de Sortida de la connexió/bucle si és el cas.
			// Modifiqueu la cadena de retorn per tal que sigui la que diu l'enunciat.

		}

		close(new_socket); // Tancar la connexió amb el client
	}

	close(sock);
	return 0;
}