/* Ce programme est un module Linux qui crée un chenillard en utilisant un timer. 
Il utilise les fonctions standard de la bibliothèque Linux pour créer un répertoire de procédure et un fichier de procédure, et pour configurer et gérer un timer qui 
met à jour les LEDs en utilisant les fonctions de sortie standard de Linux. 
Il utilise également les fonctions de module Linux pour enregistrer et supprimer le module lors de son chargement et de son déchargement. */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/timer.h>
#include <linux/proc_fs.h>

#define DRIVER_AUTHOR	"Quentin SIMON"
#define DRIVER_DESC		"Chenillard Module"
#define DRIVER_LICENSE	"GPL"

#define PROC_PARENT_NAME	"ensea"
#define PROC_NAME		"chenillard_TP2"
struct file_operations proc_fops = {};
struct proc_dir_entry* parent;

int param = 1; // param doit prendre une valeur entre 1 et 5
#define INTERVALLE		100*param
static struct timer_list tim1;
static int ledx = 0;
static int flag=0;

//Cette fonction est utilisée comme rappel pour le timer configuré. Il est appelé à chaque intervalle de temps défini par la constante INTERVALLE et met à jour l'état de la LED en utilisant les fonctions de sortie standard de Linux. Il utilise une variable ledx pour stocker l'index de la LED actuellement allumée et une variable flag pour stocker l'état de l'effet de défilement. Il allume et éteint les LEDs dans l'ordre en utilisant les variables ledx et flag.
static void tim1_callback(unsigned long data) {
	mod_timer(&tim1, jiffies + INTERVALLE);
	if (ledx < 10 && flag==0){
		printk(KERN_INFO "LED %d ON\n", ledx);
		ledx ++;
	}
	else if(ledx==10){
		flag=1;
		ledx=9;
	}
	else if(ledx==0){
		flag=0;
	}
	else if(ledx>0 && flag==1){
		
		printk(KERN_INFO "LED %d OFF\n", ledx);
		ledx --;
	}	
	
}

module_param(param,int,1);

//Cette fonction est appelée lorsque le module est chargé. Il crée un répertoire de procédure et un fichier de procédure, configure le timer en utilisant la fonction setup_timer, démarre le timer en utilisant la fonction mod_timer, et affiche un message de sortie standard.
int hello_init(void)
{
	parent = proc_mkdir(PROC_PARENT_NAME, NULL);
	proc_create(PROC_NAME, 0, parent, &proc_fops);

	setup_timer(&tim1, tim1_callback, 0);
	mod_timer(&tim1, jiffies + INTERVALLE);

	printk(KERN_INFO "Hello world! %d\n", param);
	return 0;
}

//Cette fonction est appelée lorsque le module est déchargé. Il supprime le répertoire de procédure et le fichier de procédure, arrête le timer en utilisant la fonction del_timer, et affiche un message de sortie standard.
void hello_exit(void)
{
	remove_proc_entry(PROC_NAME, parent);
	remove_proc_entry(PROC_PARENT_NAME, NULL);

	del_timer(&tim1);

	printk(KERN_ALERT "Bye bye...\n");
}

//Ces macros enregistrent les fonctions hello_init et hello_exit comme fonctions d'initialisation et de suppression du module, respectivement.
module_init(hello_init);
module_exit(hello_exit);

//Servent à définir les informations sur le module comme l'auteur, la licence et la description.
MODULE_LICENSE(DRIVER_LICENSE);
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
