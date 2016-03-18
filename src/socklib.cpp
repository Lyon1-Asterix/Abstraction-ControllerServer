#include <netdb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdarg.h>

#include "socklib.h"

#define TAILLE_BUFF 10
#define FALSE 0
#define TRUE 1


int CreeSocketServeur(const char* port) {
  int s=-1;
  // structure pour faire la demande de port
  struct addrinfo hints;
  // structure pour stocker et lire les résultats
  struct addrinfo *result, *rp;
  // variables pour tester si les fonctions donnent un résultats ou une erreur
  int bon, res;
  // Des variable pour contenir de adresse de machine et des numero de port afin de les afficher
  char hname[NI_MAXHOST], sname[NI_MAXSERV];

  // ######################################
  // mise en place de la connexion
  // ######################################
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;    /* IPv4 ou IPv6 */
  hints.ai_socktype = SOCK_STREAM; /* socket flux connectée */
  hints.ai_flags = AI_PASSIVE;    /* Les signifie que toutes les addresse de la machine seront utilisée */
  hints.ai_protocol = 0;          /* Any protocol */

  // on effectue la demande pour le port demandé
  res = getaddrinfo(NULL, port, &hints, &result);
  if (res != 0) { // c'est une erreur
    fprintf(stdout, "getaddrinfo: %s\n", gai_strerror(s));
    return -1;
  }

  // si res = 0 le véritable résultat de la fontion est l'argument result
  // qui contient une liste d'addresses correspondant à la demande on va les
  // tester jusqu'à en trouver une qui convient
  rp = result;
  bon = 0;
  while (rp != NULL) { // on parcourt la liste
    int yes = 1;

    s = socket(rp->ai_family, rp->ai_socktype,rp->ai_protocol);
    // si le résultat est -1 cela n'a pas fonctionné on recommence avec la prochaine
    if (s == -1) {
      fprintf(stdout, "Création de la socket %s\n", strerror(errno));
      continue;
    }

    // partie optionnelle pour éviter d'Ãªtre rejeté par le système si le précédant test a planté
    res = setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &yes,
		     sizeof(int));
    if (res == -1) {
      fprintf(stdout, "setsockopt %s\n", strerror(errno));
            continue;
    }
    // fin de la partie optionnelle

    // si la socket a été obtenue, on essaye de réserver le port
    res = bind(s, rp->ai_addr, rp->ai_addrlen);
    if (res == 0 ) {// cela a fonctionné on affiche l'information
      bon = 1;

      // on récupère des informations affichables
      res = getnameinfo(rp->ai_addr, rp->ai_addrlen,
			hname, NI_MAXHOST,
			sname, NI_MAXSERV,
			NI_NUMERICSERV|NI_NUMERICHOST);
      if (res != 0) {
	fprintf(stdout, "getnameinfo: %s\n", gai_strerror(res));
	return -1;
      }
      printf ("La socket %d est maintenant en attente sur l'adresse %s le port %s\n",
	      s, hname, sname);
      break;
    }
    else { // sinon le bind a été impossible, il faut fermer la socket
      fprintf(stdout, "Imposible de réserver l'adresse %s\n", strerror(errno));
      close (s);
    }

    rp = rp->ai_next;
  }

  if (bon == 0) { // Cela n'a jamais fonctionné
    fprintf(stdout, "Impossible de faire un bind\n");
    return -1;
  }

  // on libère la structure devenue inutile
  freeaddrinfo(result);

  //ajout de la socket à la liste des socket a fermer.
  //addSocketTerminaison(s);

  res = listen (s, 42);
  if (res < 0) {
    fprintf(stdout, "listen %s\n", strerror(errno));
    return -1;
  }

  return s;
}

int CreeSocketClient(const char *host, const char* port) {
  // structure pour faire la demande
  struct addrinfo hints;
  // structure pour stocker et lire les résultats
  struct addrinfo *result, *rp;
  // socket  (s)
  int s=-1;
  // variables pour tester si les fonctions donnent un résultats ou une erreur
  int res;
  int bon;
  // Des variable pour contenir de adresse de machine et des numero de port afin de les afficher
  char hname[NI_MAXHOST], sname[NI_MAXSERV];


  // on rempli la structure hints de demande d'adresse
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;    /* IPv4 ou IPv6 */
  hints.ai_socktype = SOCK_STREAM; /* socket flux connectée */
  hints.ai_flags = 0;
  hints.ai_protocol = 0;          /* Any protocol */
  hints.ai_addrlen = 0;
  hints.ai_addr = NULL;
  hints.ai_canonname = NULL;
  hints.ai_next = NULL;

  res = getaddrinfo(host, port, &hints, &result);
  if (res != 0) { // c'est une erreur
    fprintf(stdout, "getaddrinfo: %s\n", gai_strerror(res));
    return -1;
  }

  // si res = 0 le véritable résultat de la fontion est l'argument result
  // qui contient une liste d'addresse correspondant à la demande on va les
  // rester jusqu'a trouver une qui convient
  rp = result;
  bon = 0;
  while (rp != NULL) {
    // on parcourt la liste pour en trouver une qui convienne

    // on récupère des informations affichables
    res = getnameinfo(rp->ai_addr, rp->ai_addrlen,
		      hname, NI_MAXHOST,
		      sname, NI_MAXSERV,
		      NI_NUMERICSERV|NI_NUMERICHOST);
    if (res != 0) {
      fprintf(stdout, "getnameinfo: %s\n", gai_strerror(res));
      return -1;
    }
    fprintf (stdout, "On tente l'adresse %s sur le port %s .....",
	    hname, sname);

    // on essaye
    s = socket(rp->ai_family, rp->ai_socktype,rp->ai_protocol);
    // si le résultat est -1 cela n'a pas fonctionné on recommence avec la prochaine
    if (s == -1) {
      fprintf(stdout, "Création de la socket %s\n", strerror(errno));
      continue;
    }

    // si la socket a été obtenue, on essaye de se connecter
    res = connect(s, rp->ai_addr, rp->ai_addrlen);
    if (res == 0 ) {// cela a fonctionné on est connecté
      bon = 1;
      fprintf (stdout, "OK\n");
      break;
    }
    else { // sinon le bind a été impossible, il faut fermer la socket
      fprintf(stdout, "Imposible de se connecter %s\n", strerror(errno));
      close (s);
    }

    rp = rp->ai_next;
  }

  if (bon == 0) { // Cela n'a jamais fonctionné
    fprintf(stdout, "Aucune connexion possible\n");
    return -1;
  }

  freeaddrinfo(result);           /* No longer needed */

  return s;
}

int AcceptConnexion(int s){
  int sClient;
  int res;
  char host[NI_MAXHOST], service[NI_MAXSERV];
  struct sockaddr_storage tadr;
  socklen_t tadr_len = sizeof(struct sockaddr_storage);

  sClient = accept(s, (struct sockaddr*) &tadr, &tadr_len);

  //Nouvelle socket a fermer lors de la terminaison.
  //addSocketTerminaison(sClient);

  res = getnameinfo((struct sockaddr*) &tadr, tadr_len, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV);
  if(res!=0){
    fprintf(stdout, "getnameinfo: %s\n", gai_strerror(res));
    return -1;
  }
  printf("log: main: connexion depuis %s,%s\n", host, service);

  return sClient;
}


int RecoieEtSauveDonnees(int fd, int s) {
  char buff[TAILLE_BUFF];
  int res, res2;
  int nb_recu = 0;

  while(1) {
    res = read(s, buff, TAILLE_BUFF);
    if (res < 0) {
      fprintf(stdout, "Probleme lors de la lecture sur s %s\n", strerror(errno));
      return -1;
    }
    if (res == 0) {
      // Le fichier/socket est terminé
      // on stop la fonction
      break;
    }

    nb_recu += res;

    //    fprintf(stdout, "Recu %d oct total %d oct\n", res, nb_recu);

    res2 = write(fd, buff, res);
    if (res != res2) {
      fprintf(stdout, "Probleme ecriture fichier%s\n", strerror(errno));
      return -1;
    }
  }

  return nb_recu;
}


char *RecoieLigne(int sock) {
  char buff[TAILLE_BUFF+1];
  int res;
  int fini = FALSE;
  int taillerecu = 0;
  int tailletotal = 0;
  int taillereserve = TAILLE_BUFF;
  int aretirer;

  // le restultat est une chaine vide au départ
  char *result = (char*)malloc(taillereserve*sizeof(char));
  result[0] = '\0';

  while(!fini) {
    // lecture sans vraiment lire pour savoir si la chaine contient un \n
    res = recv(sock, buff, TAILLE_BUFF, MSG_PEEK);
    if (res < 0) {
      perror("Probleme à la lecture de la socket");
      free(result);
      return NULL;
    }
    if (res == 0) {
      // la socket est coupée sans retour chariot,
      if (taillerecu > 0) {
	// si on a recu des octets,  on suppose que ce n'est pas une erreur
	// et on retourne ce qu'on a.
	break;
      } else {
	// sinon on est en train de lire sur une socket fermée, on retourne NULL
	free(result);
	return NULL;
      }
    }
    // on s'assure que la chaine buff ne sera pas analysée après la fin des octets reçus
    buff[res] = '\0';
    taillerecu += res;

    int nbchar = res;
    // on cherche si c'est fini en utilisant strtok_r (version reentrant de strtok).
    char *pos = strchr(buff, '\n');
    if (pos == NULL) {
      // le carractère \n n'a pas été trouvé, la chaine n'est pas terminée dans ce qu'on a recu
      tailletotal += res;
      // il aut tout retirer
      aretirer = res;
    } else {
      // le caractère \n a été trouvé en posision pos
      nbchar = pos-buff;
      tailletotal += nbchar;
      buff[nbchar] = '\0';
      aretirer = nbchar+1;
      fini = TRUE;
    }

    if (tailletotal+1 > taillereserve) {
      // il manque de place dans le tableau
      taillereserve *= 2;
      result = (char*)realloc(result, taillereserve);
    }
    // strlen(result)+strlen(buff) est forcement < taillereserve car soit tailletotal<taillereserve soit on vient de rajouter au moins TAILLE_BUFF>res
    strcat(result, buff);

    // il faut vider la socket de ce qu'on a lu c'est à dire la chaine + le \n
    res = recv(sock, buff, aretirer, 0);
    if (res < 0) {
      perror("second recv");
      free(result);
      return NULL;
    }
    if (res == 0) {
      perror("second recv renvoie 0");
      free(result);
      return NULL;
    }
  }

  // certaine fois, les fins de lignes comportent \r\n, dans ce cas, ce code pose problème car il laisse le \r
  if (result[tailletotal-1] == '\r') {
    result[tailletotal-1] = '\0';
  }

  return result;
}


int EnvoieMessage(int s, char* format, ...){
  int i;
  int res;

  va_list liste_des_arguments;
  va_start(liste_des_arguments, format);

  // on calcul la taille du message
  int taille = vsnprintf(NULL, 0, format, liste_des_arguments);
  va_end(liste_des_arguments);

  // un tableau un peu plus grand pour le \0
  char chaine[taille+1];

  va_start(liste_des_arguments, format);
  vsnprintf(chaine, taille+1, format, liste_des_arguments);
  va_end(liste_des_arguments);

  //  fprintf(stdout, "Envoie %s\n", chaine);
  i = 0;
  while (i < taille) { // attention, il ne faut pas envoyer le \0
    res = write(s, chaine+i, taille-i);
    if(res==-1){
      fprintf(stdout, "error: write %s car %s\n", chaine, strerror(errno));
      return -1;
    }
    i += res;
  }

  return i;
}
