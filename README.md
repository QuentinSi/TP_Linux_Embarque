
<!-- PROJECT LOGO -->

<div align="center">

 <h1 align="center">TP Linux embarqués :</h3>
</div>

<div>
  
## Membre du groupe

- SIMON Quentin
- SILVESTRE Ricardo

Encadrant: Mr PAPAZOUGLOU  
  
## But des TP:  

Le but de ce projet était de découvrir le fonctionnement de plusieurs fonctions disponible en linux embarqué, comme la compilation croisée par exemple.  
Cela se découpe en 3 parties. Nous utilisons une machine virtuelle ainsi qu'une carte DE-10 VEEK.  
  
# 📖 [Documentation](https://github.com/QuentinSi/TP_Linux_Embarque/)

1. [TP1](#TP1)
2. [TP2](#TP2)
3. [TP3](#TP3)
 </center>

## TP1 

## 1.  Prise en main :

### 1.1 Préparation de la carte SD

Avant de commencer l'utilisation de notre carte VEEK-MT25, nous avons du
préparer notre carte SD en la flashant. Pour cela nous avons utilisé le
logiciel Win32DiskImager. Une fois la carte SD flashé, nous avons allumé
la carte VEEK pour voir si elle s'allumait correctement et que nous
pouvions commencer à l'utiliser.

## 1.2.  Connexion au système

### 1.2.1 Liaison série

Pour pouvoir utiliser notre carte, nous avons du nous connecter pour
pouvoir la manipuler car il n'y a pas de clavier relié à celle-ci. Pour
commencer nous avons donc connecter notre carte et notre ordinateur sur
le même réseau wifi. Une fois ceci fait, il nous fallait son adresse IP
pour pouvoir nous connecter dessus en SSH. Pour cela nous nous sommes
connecter en liaisons série, pour cela nous avons utilisé le logiciel
putty qui nous le permettait. Une fois connecté via la liaison série,
nous avons effectué un reboot pour voir si nous avons bien le contrôle
sur la carte VEEK. La carte a donc bien redémarré. Nous avons regardé
par la suite la taille de l'image que l'on a mis sur notre carte SD.

La taille de l'image est : 1.3Go (dev/root). On a une taille de 3Go, on
utilise 1.3Go et 1.5Go sont disponibles.\
![Une image contenant texte Description générée
automatiquement](./media/image1.png)  
Grace à la commande df -h nous voyons l'espace utilisé sur la carte SD.

Avec cette commande, nous pouvons augmenter la taille du système de
fichiers pour pouvoir profiter de la taille de la carte SD en totalité :
./resize2fs_once lorsque l'on veut faire cela il faut avertir la carte
que l'on va faire ceci en lui faisant un root : ./expand_rootfs.sh. Dès
que cette commande a été effectué nous devons faire un reboot. Une fois
le reboot fait nous vérifions que notre taille a bien augmenter .

![Une image contenant texte Description générée
automatiquement](./media/image2.png)  

Nous pouvons donc voir que la taille est bien de 32Go.

## 1.3 Configuration réseau :

Ensuite, nous devons vérifier si la carte veek dispose bien d'une
adresse IP, pour cela nous devons brancher la carte VEEK sur le switch
via un câble réseau, puis nous faisons ifconfig :

![Une image contenant texte Description générée
automatiquement](./media/image3.png)  

Nous pouvons voir que celle-ci dispose bien d'une adresse IP (inet6 pour
eth0).

Ensuite, nous écrivons un code à l'aide de vim dans le fichier
/etc/network/interfaces pour pouvoir avoir l'adresse IP :

![Une image contenant texte Description générée
automatiquement](./media/image4.png)  

Après avoir reboot, nous refaisons un ifconfig pour voir notre adresse
ip :

![Une image contenant texte Description générée
automatiquement](./media/image5.png)  

Nous testons par la suite le ping via une invite de commande sur notre
machine windows :

![Une image contenant texte Description générée
automatiquement](./media/image6.png)  

Nous pouvons voir que le premier ping c'est exécuté correctement alors
que le deuxième non mais cela est normal car nous avons débrancher le
port ethernet pour voir si cela fonctionnait correctement.

Nous avons également fait un ping depuis notre terminale de machine
virtuelle vers google pour voir si celui-ci fonctionnait correctement :

![](./media/image7.png)  

Nous pouvons voir que celui-ci fonctionne correctement.

Nous avons vérifié que la ligne : « PermitEmptyPasswords yes » celle-ci
était bien présente.

![Une image contenant texte Description générée
automatiquement](./media/image8.png)  

Nous pouvons voir que le terminale nous dit que nous sommes bien
connectés.

### 1.4.1 Exploration des dossiers /sys/class et /proc :

Nous avons explorer les fichiers en allant dans le repertoire
/sys/class :

![Une image contenant texte Description générée
automatiquement](./media/image9.png)  

On peut comparer avec notre machine virtuelle :

![Une image contenant texte Description générée
automatiquement](./media/image10.png)  

Nous pouvons voir qu'il y a des différences entre les deux ce qui
signifie qu'il y a bien des entrées nouvelles.

## 1.4.2 Compilation croisée

Il existe deux méthodes pour compiler un programme sur la carte VEEK :

\- directement sur la carte à l'aide du gcc

\- sur le pc en utilisant une chaine de compilation croisée sous linux
(apt install gcc-arm-gnueabihf)

En ce qui nous concerne nous utilisons une machine virtuelle avec
laquelle nous allons avoir un dossier partagé entre notre ordinateur et
la machine virtuelle.

### 1.4.3 : Hello world !

Nous avons donc créé un code pour afficher « hello world ! », une fois
ceci fait nous avons essayé de le compiler.

Nous avons utilisé le cross-compilateur :
```
arm-linux-gnueabihf-gcc hello.c -o hello.o
```
![Une image contenant texte Description générée
automatiquement](./media/image11.png)  

Après avoir compiler, nous avons essayé de l'exécuter sauf que celui-ci
ne fonctionne pas car ce n'est pas le bon format d'exécutable.

Nous avons donc envoyé le fichier exécutable sur notre carte, pour cela
nous avons utilisé la commande :

![](./media/image12.png)  

Nous regardons donc sur le terminale de notre carte pour regarder si nos
documents sont bien arrivés :

![Une image contenant texte Description générée
automatiquement](./media/image13.png)  

Nous avons donc exécuter le fichier .o, nous pouvons voir que celui-ci
s'exécute bien.

Nous venons donc d'utiliser la compilation croisée, en ayant compiler le
programme sur notre VM et exécuter le fichier sur notre carte.

### 1.4.4 Accès au matériel :

Un certain nombre de drivers sont fournis. Comme tous les drivers sous
Linux, ils sont accessibles sous forme de fichiers. Par exemple pour
allumer l'une des LED rouge de la carte, il suffit d'écrire un '1' dans
le bon fichier.

Nous avons donc fait le test d'allumer les leds à la main :

![Une image contenant texte, clavier, équipement électronique, machine à
écrire Description générée
automatiquement](./media/image14.png)  

Lorsque l'on regarde sur la carte les leds demandées s'allument
correctement.

#### Chenillard :

Nous avons réalisé un chenillard qui nous permet d'allumer les leds les unes après les autres de droite à gauuche. Le code est disponible dans le dossier TP1, cela correspond au fichier chenillard.c    

## TP2 :

Nous avons dû créer un programme qui accède directement aux registres
depuis l'espace utilisateur. A cause de la virtualisation de la mémoire,
il n'est pas possible d'écrire facilement dans un registre comme on en a
l'habitude. Il faut donc remapper la mémoire pour demander à l'os de
nous fournir une adresse virtuelle.

On utilise donc la fonction mmap() (fichier register.c), nous savons que le registre du GPIO
connecté aux LED est disponible à l'adresse : 0xFF203000 :

![Une image contenant texte Description générée
automatiquement](./media/image17.png)  

Nous affichons par la suite le résultat de cette fonction:  
![Une image contenant texte Description générée
automatiquement](./media/image18.png)  

Nous pouvons voir que l'adresse renvoyer n'est pas la meme, cela est dû
à la mémoire virtuelle qui peut causer ce changement de valeur.

### 2.2 Compilation de module noyau sur la VM :

Pour pouvoir compiler des modules sur notre VM, nous avions besoin de
différents paquets :
```
Sudo apt install linux-headers-amd64

Sudo apt install bc
```
Nous avons donc fait un make sur le fichier trouver dans les ressources
du TP2, on peut voir que notre .ko a bien été créé :

![](./media/image19.png)  

Nous testons donc par la suite les commandes :  

- Commande insmod et modinfo:  
![Une image contenant texte Description générée
automatiquement](./media/image20.png)  
- Commande dmesg:  
![Une image contenant texte Description générée
automatiquement](./media/image21.png)  
- Commande rmmod :   
![Une image contenant texte Description générée
automatiquement](./media/image22.png)  
- Commande dmesg pour observer si le rmmod a bien fonctionné:  
![Une image contenant texte Description générée
automatiquement](./media/image23.png)  

Nous avons également ajouté au code la possibilité d'utiliser un timer.

Ensuite nous avons testé avec la création d'une entrée :

![Une image contenant texte Description générée
automatiquement](./media/image24.png)  

Nous pouvons voir que notre entrée est bien créée.

Nous avons testé l'utilisation d'un paramètre dans un module, voici la
fonction codée :

![Une image contenant texte Description générée
automatiquement](./media/image25.png)  

Voici les résultats :

![Une image contenant texte Description générée
automatiquement](./media/image26.png)  

Nous remarquons que le paramètre a bien été créer car il nous est
retourné.

## 2.3 CrossCompilation de module noyau

À cause de la puissance limitée du processeur de la carte cible, la
compilation, en particulier la compilation de modules noyau, est
relativement longue. Nous allons donc, une fois encore, cross-compiler
les modules noyau pour la carte SoC à l'aide de la VM.

### 2.3.1 Préparation de la compilation

Nous avons du installer des fichiers en plus pour que la compilation
puisse se faire correctement :
```
sudo apt install bc

sudo apt install crossbuild-essential-armhf

sudo apt install binutils-multiarch
```
Nous avons également exécuté la commande suivante :
```
whereis arm-linux-gnueabihf-gcc
```
![](./media/image27.png)  

Cela nous renvoie donc son chemin que nous utiliserons par la suite.

### 2.3.2  Récupéreation de la configuration actuelle du noyau :

Nous avons récupérer un fichier sur la carte VEEK ; /proc/config.gz dans
le dossier source du noyau.

Nous avons donc récupérer le fichier que l'on souhaitait pour pouvoir le
dezipper par la suite :

![Une image contenant texte Description générée
automatiquement](./media/image28.png)  

Une fois récupérer nous devons le dezipper :
```
gunzip config.gz

mv config .config
```
Lorsque l'on renomme le fichier en .config celui-ci n'apparait plus car
les fichiers commençant par un point ce sont des dossiers cachés. Pour
pouvoir le voir nous devons faire ls -a.

Ensuite, nous devons exécuter quelques lignes de code afin de tout bien
paramétré :
```
export CROSS_COMPILE=/usr/bin/arm-linux-gnueabihg-

export ARCH=arm

make prepare

make scripts
```
La commande export est utilisée pour garantir que les variables
d\'environnement et les fonctions seront transmises aux processus
enfants. Cela n\'affecte pas la variable d\'environnement existante.

Nous pouvons voir que la ligne export compile s'arrête avec un tiret,
cela est normal car le gcc n'est pas a ajouter il est mis
automatiquement, si nous l'ajoutons après le tiret, nous aurons des
erreurs lors de l'exécution des 2 lignes make.

### 2.3.3 Hello word

Nous avons donc copier dans un dossier pour faire le module hello.c. Ce module est disponible dans le dossier TP2 sous le nom hello.c  

Une fois le code récupéré et placé dans un dossier, nous avons modifier
le makefile (exemple de makefile dans le dossier TP3) pour que celui-ci fonctionne dans notre situation :

![Une image contenant texte Description générée
automatiquement](./media/image31.png)  

Nous pouvons voir le makefile sur la photo ci-dessus, avec la mise à
jour du chemin de notre noyau et l'ajout d'un flags_module. Nous pouvons
également voir la commande scp quoi nous permet de transferer un fichier
de notre machine virtuellle à la carte VEEK.

Voici la capture d'écran qui montre que notre hello word fonctionne bien sur
notre SOC.

![Une image contenant texte Description générée
automatiquement](./media/hello.PNG) 

### Chenillard Yes :

On veut créer un chenillard dont on peut modifier :

--- Le pattern depuis le fichier : /proc/ensea/chenille

--- La vitesse au moment du chargement du module.

Nous avons donc créé un module qui nous permettait de pouvoir respecter
les conditions. Dans cette partie, on ne voulait pas faire clignoter les
leds de la carte VEEK mais juste faire l'affichage pour voir si cette
étape fonctionnait.  

![](./media/image33.png)  

Les leds s'allument bien comme il faut et nous pouvons régler la vitesse
d'affichage des leds avec un paramètre.  

## TP3 

L'objectif de ce TP est de définir son propre périphérique, et de
programmer un module qui identifie la présence du périphérique et se
configure automatiquement en sa présence. Cet automatisme s'appuye sur
le Device Tree.

Le fichier DTS/DTB (.dts) est un fichier qui est installé à coté du
noyau sur la partition de boot, et qui lui indique quels sont les
périphériques à sa disposition. Nous voulons accéder aux LED en direct.
Or le device-tree étant déjà configuré, ces leds sont déjà utilisées par
les drivers fournis par Altera, ce qui nous empêche d'y accéder. Pour
éviter cela, 2 possibilités :  

--- Modifier le fichier dts existant ;  
--- Recréer son propre design avec QSYS, puis ce qui va générer un nouveau fichier dts.

Pour notre part, nous utilisons la première méthode.

Nous avons chercher le fichier soc_system.dts qui se situait dans un
dossier que nous avions télécharger précédemment. Une fois celui-ci
trouver, nous avons du le modifier :

Avant :
```
ledr: gpio@0x100003000 {

> compatible = \"altr,pio-16.1\", \"altr,pio-1.0\";
>
> reg =\<0x00000001 0x00003000 0x00000010\> ;
>
> clocks = \<&clk_50\>;
```
Après :
```
ledr: ensea {

> compatible = \"dev,ensea";
>
> reg =\<0x00000001 0x00003000 0x00000010\> ;
>
> clocks = \<&clk_50\>;
```
Nous installons par la suite le compilateru device tree sur notre VM :
```
sudo apt install device-tree-compiler
```
Puis compiler le fichier .dts en fichier .dtb qui est un fichier lisible
par le noyau :
```
dtc -O dtb -o soc_system.dtb soc_system.dts
```
Pour pouvoir acceder à la partition boot sur la carte VEEK, nous avons
monté la partition :
```
mkdir /root/mntboot

mount /dev/mmcblk0p1 mntboot
```
Nous avons remplacé le fichier existant par celui provenant de notre
machine virtuelle, puis rebooter la carte VEEK.

Nous vérifions que nous avons bien eu ce que l'on souhaitait.

Ensea est bien présent dans un bridge :

![Une image contenant texte Description générée
automatiquement](./media/image34.png)  

Vérification dans le bridge :  

![Une image contenant texte Description générée
automatiquement](./media/image35.png)  

### 3.1 module accedznt au LED via /dev :

Nous disposons d'un fichier gpio-leds .c La compilation de celui-ci a
bien réussi, nous avons bien tous les fichiers créer par le makefile.

![Une image contenant texte Description générée
automatiquement](./media/image36.png)  
Voici les différentes fonctions présentent dans le programme :  

- La fonction probe : elle est appelée chaque fois que le noyau trouve un nouveau
périphérique que notre pilote peut gérer. La fonction probe sert à allumer la led mais également à récupérer les données.  

- La fonction remove : Est appelé chaque fois qu\'un périphérique géré par
ce pilote est supprimé. Elle sert à éteindre la led mais également à supprimer un fichier dans /dev  

- La fonction read : Cette fonction est appelée chaque fois qu\'une
opération de lecture se produit sur l\'un des fichiers de caractères.
Elle sert à récupérer la valeur de la led

- La fonction write : // Cette fonction est appelée chaque fois qu\'une
opération d\'écriture se produit sur l\'un des fichiers de caractères.
Elle sert à écrire une valeur de leds

### 3.2 Module final

Le cahier des charges est le suivant :

Réaliser un chenillard qui rempli les conditions suivantes :

--- Choix de la vitesse de balayage par une option au moment du
chargement du module

--- Récupération de la vitesse courante par lecture du fichier
/proc/ensea/speed

--- Modification de la patern par écriture dans le fichier
/dev/ensea-led

--- Récupération du patern courant par lecture du fichier /dev/ensea-led

--- Modification du sens de balayage par écriture du fichier
/proc/ensea/dir

--- Récupération du sens de balayage par lecture du fichier
/proc/ensea/dir

Nous avons commencé par mettre en place le timer puis par la suite le
paramètre :

![](./media/image37.png)  

Nous avons augmenté la valeur de la fréquence et celui-ci à donc bien
clignoter plus vite

On a implémenté par la suite la fonction speed read qui nous permet de
lire la valeur de speed stocker dans le fichier qui se situe dans
/proc/ensea/speed.

![Une image contenant texte Description générée
automatiquement](./media/image38.png)  

Nous pouvons voir que dans les 2 exemples fait nous avons bien la valeur
de notre vitesse qui est écrite dans le fichier speed.

Ensuite, nous avons fait le pattern avec le sens de defilement des
leds :

![Une image contenant texte Description générée
automatiquement](./media/image39.png)  

Nous pouvons voir que le sens est de 1 donc les leds vont de gauche à
droite et si il s'agit d'une autre valeur cela veut dire que les leds
vont de droite à gauche.

Nous avons fait pour le changement de sens du clignotement des leds,
nous pouvons voir que nous lisons la valeur pour la changer par la
suite :

![Une image contenant texte Description générée
automatiquement](./media/image40.png)  

Nous avons lu la valeur du pattern pour savoir si les leds défilent les
unes apres les autres ou si on clignote toutes les leds.

![Une image contenant texte Description générée
automatiquement](./media/image41.png)  

Nous pouvons voir que la valeur de notre register est de 0 ce qui
signifie que notre fichier est bien ouvert.

Nous avons testé pour modifier le pattern (si le pattern vaut 0 alors
les leds vont s'allumer les unes après les autres alors que s'il vaut 1
nous faisons clignoter les leds).

![Une image contenant texte Description générée
automatiquement](./media/image42.png)  

## Conclusion 

Toutes les séances de TP nous ont permis de pratiquer tout ce que l'on avait vu en cours comme la compilation croisée ou encore la création de module avec modifications ou lecture dans des registres. Nous avons eu le temps de finir le module final et de le tester pour vérifier que celui-ci fonctionnait correctement.
