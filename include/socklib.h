#pragma once
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
//#include <error.h>
#include <string.h>
#include <stdio.h>

/* Une partie nÃ©cessaire pour utiliser les sockets sous linux et windows */
#if defined (WIN32)
#include <winsock2.h>
#elif defined (linux)
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

//#define MSG_NOSIGNAL -1 //TODO: celle-là devrait être définie dans <sys/socket.h> ???

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

/**
 * @brief Cree une socket d'attente pour le serveur sur le port port
 * @param port : le port utilisÃ©
 * @return la socket d'attente ou -1 en cas d'erreur
 *
 * en cas d'erreur, un message explicatif est affiché sur la sortie d'erreur 
 * standart
 */
int CreeSocketServeur(const char* port);
int CreeSocketClient(const char *serveur, const char* port);

///retourne la socket acceptée
int AcceptConnexion(int s);

/**
 * Lire les données sur une socket et les écrit automatiquement dans un descripteur de fichier
 * Cette fonction stoppe la lecture lorsque la soket est fermée.
 * @param sock : la socket d'où proviennent les données, cela fonctionne aussi si sock est un file descriptor quelconque
 * @param fd : le descripteur de fichier sur lesquel enregistrer les données
 * @return : si cela a fonctionné le nombre d'octet lu, sinon -1
 */
int RecoieEtSauveDonnees(int fd, int sock);

/**
 * Lire les données sur une socket (uniquement j'utilise recv) jusqu'à arrivé à un retour chariot '\n' la donnée est stokée dans un tableau dont la taille est adapté pour cela).
 * @param sock : la socket de lecture
 * @return la chaine lue à libérée par free
 */
char *RecoieLigne(int sock);

/**
 * envoie le message formaté sur la socket s (comme un printf)
 * @param s : la socket sur laquel ecrire le message
 * @param format : le format du message (comme pour printf)
 * @param ...: les parametres optionnels
 * @return le nombre d'octet écrit ou -1 s'il y a eu un problème
 */
int EnvoieMessage(int s, char* format, ...);
