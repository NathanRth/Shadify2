# Shadify v0.1	{#mainpage}

## Introduction
Première version du moteur de test Shadify.

Shadify est un moteur de rendu *OpenGL Modern* de type différé.

La partie la plus importante du moteur est située dans le fichier `glfwwindow.cpp`. 

La classe `Scene` contient la liste des models et lumières présent dans la scène. On considère une scène comme la collection des objets physiques présents dans le monde 3D.

Un `Model` est un objet 3D composé d'un ou plusieurs `Mesh` et `Texture`.

Une `Light` est un objet abstrait portant les caractéristiques d'une lumière. On peut instancier 2 types de lumière, des `PointLight` et des `DirectionalLigth`.

## Compilation

Shadify2 utilise les librairies Assimp, GLFW, glm, stb

- [GLFW](http://www.glfw.org/) permet la création d'un context OpenGL ainsi que d'une fenêtre de manière cross-plateforme (GLFW est distribué sur windows et sur MacOS). Elle gère aussi les évènement claviers et souris.
- [Assimp](http://www.assimp.org/) est une librairie d'importation de fichier 3D. Elle supporte un très grand nombre de fichiers.
- [stb_image](https://github.com/nothings/stb) est une petite libraire permettant d'importer des images. Elle sert ici à l'importation de textures.
- [glm](https://glm.g-truc.net/0.9.9/index.html) est la librairie mathématique basé sur les spécifications de glsl

GLFW, Assimp et glm ont besoin d'être compilés.
Compiler GLFW : link[http://www.glfw.org/docs/latest/compile_guide.html]
Compiler Assimp : link[http://assimp.sourceforge.net/lib_html/cmake_build.html]

### Instructions:
Pour les 3 librairies :
1. Télécharger l'archive de la librairie
2. Extraire les fichiers
3. `cd <chemin du dossier extrait>`
4. `cmake CmakeList.txt`
5. `make`

Télécharger et installer Qt (Qt 5.11 gcc 64bit + QtCreator)
Lancer Qt puis ouvrir le fichier .pro qui se trouve dans Shadify2.
Si Qt demande de reconfigurer le projet, accepter.
Une fois le projet ouvert, appuyer sur le boutton "play" en bas à gauche pour build+launch, ou sur le marteau pour build uniquement.

## Utiliser le moteur

### Paramétrer une scène

Paramétrer une scène dans le moteur se fait dans la fonction

```cpp
void GlfwWindow::setupScene() {...}
```

Pour ajouter un objet dans la scène: 
Model:
```cpp
m_scene->addModel(Model(...));
```
```cpp
Model(
	std::string path, 	// le chemin d'accès au fichier 3D
	glm::vec3 position,
	glm::vec3 color		// la couleur par défaut de l'objet s'il n'existe pas de texture ou qu'une erreur est survenue
);
```	
Pour ajouter une lumière dans la scène:
PointLight:
```cpp
m_scene->addLight(new PointLight(...));
```
```cpp
PointLight(
	glm::vec3 color,
	glm::vec3 position,
	float intensity
);
```
DirectionalLight:
```cpp
m_scene->addLight(new DirectionaltLight(...));
```
```cpp
DirectionalLight(
	glm::vec3 color,
	glm::vec3 position,
	float intensity,
	glm::vec3 direction
);
```

### Naviguer dans la scène

Shift + LeftClick + MouseY = déplacer la caméra de haut en bas.
Shift + LeftClick + MouseX = déplacer la caméra de gauche à droite.
LeftClick + MouseXY = Faire pivoter la vue

### Parcourir les différents calques

Il est possible de changer de mode de rendu en appuyant sur les touches 1 à 7 (rangée numérique) et de changer le type d'affichage du SSDO en appuyant sur les touches de 8 à 0. Pour activer/desactiver la skybox, appuyer sur B. Pour démarrer un compteur de fps, appuyer sur W, X pour l'arreter.