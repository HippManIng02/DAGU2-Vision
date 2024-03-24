## La conduite automatique basée sur la vision par ordinateur

**Ce projet consiste, dans le cadre du projet interfilaire, à mettre en place sur le Robot DAGU 2 un système de conduite automatique basé sur la vision en C++ en utilisant la bibliothèque OpenCV.»** 

### Compilation
>Commande pour la compilation :
`make`
`make clean` : Permet de supprimer l'executable et les fichiers intermédiaires pour la compilation (*.o)

>À la suite de cette commande, un exécutable du nom de **vision** est généré.

### Exécution

##### Option 1 : 

`./vision <imagepath>` : On n'oublie de mettre le chemin vers l'image qu'on veut tester.  

##### Option 2 : 

 `./vision` : faire l'execution avec un flux vidéo. (webcam de la machine prise par défaut).  


>NB : La webcam est utilisée par défaut. Pour effectuer un véritable test, il faudra modifier le code pour mettre le flux d'entrée sur 2, ce qui correspond à la caméra externe.

>NB : La version 4 de opencv utilisée.

>NB : Ce projet est a exécuté dans un environnement Ros ou mise en commentaire de la partie qui publié des msg sur le topic ROS

### Gestion du code source

##### Quelques commandes utiles
 `git clone url`

 `git fetch --all`

 `git checkout develop`

 `git branch ma_branche_de_dev`

 `git checkout ma_branche_de_dev`

 `git add .`

 >Méthode personnel pour pusher

 `git stash`

 `git pull origin develop`

 `git stash apply`

 >Ensuite résolution de conflit en local s'il y en a

 `git add .`

 `git commit -m "Message"`

 `git push origin ma_branche_de_dev`

 >Et je fais le merge en ligne avec la branche develop
