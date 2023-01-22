/* La fonction principale du programme. Il utilise une boucle infinie pour mettre à jour les LEDs dans un ordre de défilement en boucle. Il utilise une variable ledX pour stocker le chemin d'accès au fichier de contrôle de chaque LED, et un pointeur de fichier fid pour accéder à ces fichiers. Il utilise les fonctions sprintf, fopen, fputs et fclose pour mettre à jour l'état des LEDs en utilisant les fichiers de contrôle dans le système de fichiers. Il utilise également la fonction sleep pour temporiser entre les mises à jour des LEDs. */

#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "time.h"
#include "unistd.h"

int main(void){
	char ledX[sizeof("/sys/class/leds/fpga_led/i/brightness")];
	FILE* fid=NULL;

while(1){
	for(int i=0;i<10;i++){
		sprintf(ledX,"/sys/class/leds/fpga_led%i/brightness",i);
		fid=fopen(ledX,"r+");
		fputs("1",fid);
		fclose(fid);

		if(i>0){
			sprintf(ledX,"/sys/class/leds/fpga_led%i/brightness",i-1);
			fid=fopen(ledX,"r+");
			fputs("0",fid);
			fclose(fid);
		}
		sleep(1);
	}
	for(int i=0;i<10;i++){
		sprintf(ledX,"/sys/class/leds/fpga_led%i/brightness",9-i);
		fid=fopen(ledX,"r+");
		fputs("1",fid);
		fclose(fid);

		if(i>0){
			sprintf(ledX,"/sys/class/leds/fpga_led%i/brightness",9-i+1);
			fid=fopen(ledX,"r+");
			fputs("0",fid);
			fclose(fid);
		}
		sleep(1);
	}
}
	return 0;
}
