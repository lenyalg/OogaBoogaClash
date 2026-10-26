# 🦕 Ooga Booga Clash

> **Smash Bros meets the Stone Age.**
> *Projet étudiant réalisé en C avec la bibliothèque Raylib.*

## 📝 Description

**Ooga Booga Clash** est un jeu de combat et de survie en 2D développé dans le cadre de notre projet C.
Le jeu propose des affrontements dynamiques dans un univers préhistorique. L'architecture du projet a été conçue de manière modulaire pour séparer strictement la logique de jeu (Gameplay) du rendu (Graphics) et faciliter le développement multi-plateforme.

## ✨ Fonctionnalités

* **⚔️ Mode Combat :** Affrontements type "Smash Bros" en local.
* **☄️ Mode Solo (Survie) :** Évitez les météorites avec une difficulté progressive.
* **💾 Système de Sauvegarde :** Gestion des meilleurs scores via `save.txt`.
* **🎨 Moteur Raylib :** Graphismes 2D, animations fluides, HUD et gestion de l'audio.
* **🎮 Contrôles :** Support Clavier et Manette.
* **⚙️ Multi-plateforme :** Système de build adapté pour macOS et Windows.

## 🏗️ Architecture du Projet


```text
├── assets/             # Sprites, Musiques, Fontes
├── include/            # En-têtes (.h) et interfaces
├── src/
│   ├── gameplay/       # Logique pure (Règles, Physique, Collisions)
│   ├── graphics/       # Rendu Raylib, Menus, UI
│   ├── utils/          # Outils génériques
│   └── main.c          # Point d'entrée
├── save.txt            # Fichier de persistance des scores
├── Makefile.mac        # Script de compilation pour macOS
├── Makefile.win        # Script de compilation pour Windows
└── README.md           # Documentation
```

## 🛠️ Installation et Compilation

### Prérequis
* **Compilateur :** `gcc` 
* **Bibliothèque :** [Raylib](https://www.raylib.com/) installée sur votre machine.
* **Outil :** `Make`

### 1. Clonage du dépôt

```bash
it clone https://gitlab.polytech.umontpellier.fr/leny.soulard/projetc.git
cd projetc
```

### 2. Compilation et Exécution

Le projet dispose de Makefiles spécifiques selon votre système d'exploitation.

#### 🍎 Pour macOS
Utilisez le fichier `Makefile.mac`. La commande `run` compile et lance le jeu directement.

```bash
make -f Makefile.mac run
```

*Pour nettoyer les fichiers objets :*
```bash
make -f Makefile.mac clean
```

#### Pour Windows
Utilisez le fichier dédié

```bash
make -f Makefile.windows run
```

## 🕹️ Contrôles

Les contrôles sont disponibles dans le menu **Options**.


## 👥 Auteurs

Projet réalisé par :
* **Lény ALGUAZIL** 
* **Lénie SOULARD**
* **Kevin**

---
*Projet C - Polytech Montpellier - DaMS3*