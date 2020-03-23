# SPACE INVADERS

## Présentation du projet

Dans le cadre de ma formation (3ème année de bachelor) nous avons eu à réaliser le jeu **Space Invaders** en **Language C** sur une carte **STM32F4_Discovery**

### Présentation du jeu
Le principe est de détruire des vagues d'aliens au moyen d'un canon laser en se déplaçant horizontalement sur l'écran

le joueur dispose de 3 vies, dès qu'il est touché par une bombe ennemi celui-ci perd une vie

pour gagner il suffit de détruire tous les vaisseaux ennemis.

### Présentation de la carte

La carte **STM32F4_Discovery** envoie une suite de caracteres via sa liaison série série le jeu étant affiché sur un terminal série d'un ordinateur par exemple

![branchement](img/space.jpg)

Comme le montre le cablage nous avons un cable USB permettant d'alimenter la carte et de téléverser le programme et un autre cable USB permettant de communiquer en série avec un terminal

# Le code

Pour développer le jeu j'ai utilisé le logiciel Atolic True Studio, il contient :
- un éditeur de texte
- un compilateur
- un debuggeur

qui m'ont été utile durant toute la période du projet.

## La communication série

Il faut tout d'abord établir une liaison UART entre la carte et notre ordinateur
comme il s'agit d'une communication asyncrone il faut préciser un débit de transmission, dans mon cas j'ai choisi un baudrate de 115 200 baud

J'ai donc utilisé les extension **serial.h** et **serial.c** qui comporte 3 fonctions utile dans le cadre de mon projet, la premiere est :
```c
void serial_init(const uint32_t baudrate);
```
Elle permet de définir la vitesse de transmission de ma carte

la seconde est :
```c
signed char serial_get_last_char(void);
```
Elle permet de récuperer la dernière touche appuyée, dans mon cas j'utilise cette fonction de la manière suivante : 
```c
input = serial_get_last_char();
```
Cela permet de lire la valeur du **serial_get_last_char()** et de stocker cette valeur dans la variable **input**, par exemple si la derniere touche appuyée est la barre espace, j'aurais son numéro dans la table ascii qui est 32.

les deux prochaines sont:
```c
void serial_putchar(const volatile char c);
void serial_puts(const volatile char *s);
```

Celles-ci permettent d'envoyer pour **serial_putchar** d'envoyer un seul caractère et pour **serial_puts** d'envoyer plusieurs caractères sur la liason série

Pour recevoir ces transmission sur notre ordinateur nous utilisons un terminal connecté sur la liaison série du port **ttyUSB0** dans notre cas
```
screen /dev/ttyUSB0 115200
```
J'utilise l'application screen pour ouvrir sur un terminal connecté à ma port série

## VT100

Pour notre jeu nous utilisons le format d'affichage du VT100 qui est du 80x24

les VT100 dispose de séquence d'échappement permettant par exemple de déplacer le curseur de notre terminal aux coordonées souhaitées ou encore d'éffacer l'intégralité de l'écran du terminal

Nous avons donc utilisé les extensions **vt100.h** et **vt100.c**

ces extensions comporte deux fonctions :
```c
void vt100_move(uint8_t x, uint8_t y)
{
    serial_putchar(VT100_ESC);
    serial_putchar('[');
    serial_putchar('0' + (y / 100));
    serial_putchar('0' + ((y / 10) % 10));
    serial_putchar('0' + (y % 10));
    serial_putchar(';');
    serial_putchar('0' + (x / 100));
    serial_putchar('0' + ((x / 10) % 10));
    serial_putchar('0' + (x % 10));
    serial_putchar('H');
}
```
**vt100_move** comprend deux paramètres d'entrés **x** et **y** qui permettent de déplacer le curseur du terminal sur les coordonées **x** et **y**

```c
void vt100_clear_screen(void)
{
    serial_putchar(VT100_ESC);
    serial_putchar('[');
    serial_putchar('2');
    serial_putchar('J');
    serial_putchar(VT100_ESC);
    serial_putchar('[');
    serial_putchar('?');
    serial_putchar('2');
    serial_putchar('5');
    serial_putchar('l');
}
```
**vt100_clear_screen** qui permet d'éffacer l'intégralité de l'écran

Ces deux fonctions sont très utilisées dans mon programme.

## Game status

Les extensions **game_status.h** et **game_status.h** sont deux extension crée par moi

le game_status contient les fonctions suivantes :
```c
void game_waiting_screen(void);
void game_loose(void);
void game_win(void);
void game_credit(void);
int refresh_screen(void);
```
Le **game_waiting_screen()** est l'écran d'accueil du jeu, on peut y retouver un logo en ascii art et un titre "Space Invarders", en dessous nous avons un menu contenant 3 choix possibles :
- Play (pour lancer le jeu)
- Rules (un onglet contenant les commandes ainsi que le bareme des points)
- Crédits (Pour afficher les crédits du jeu)

```c
while (input != 32) /*32 ASCII de l'espace*/
{
    if (input == 'z') {
        if (selection > 0) {
            selection--;
            refresh_screen();
            }
    }
    if (input == 's') {
        if (selection < 2) {
            selection++;
            refresh_screen();
        }
    }

    input = serial_get_last_char();
}
```
Nous avons une variable **selection** qui peut contenir 3 valeurs de 0 à 2 nous pouvons modifier cette variable avec les deux touches **Z** pour incrémenter et **S** pour décrémenter, cette boucle est effuctuer tant que l'utilisateur n'appuie pas sur la barre espace.

Une fois la barre espace appuyé suivant la valeur de sélection cela va soit lancer le jeu, menu des règles ou celui des crédits
```c
switch (selection)
{
case 0:

    break;
case 1:
    game_rules();
    break;
case 2:
    game_credit();
    break;
}
```
Le **refresh_screen** quant à lui permet d'actualiser l'écran d'accueil en fonction de la sélection de l'utilisateur

```c
int refresh_screen(void) 
{
    switch (selection)
    {
    case 0:
        vt100_move(37, 17);
        serial_puts("-> Play <-");
        vt100_move(37, 19);
        serial_puts("  ");
        vt100_move(46, 19);
        serial_puts("  ");
        vt100_move(36, 21);
        serial_puts("  ");
        vt100_move(47, 21);
        serial_puts("  ");
        break;
    case 1:
        vt100_move(37, 19);
        serial_puts("-> Rules <-");
        vt100_move(37, 17);
        serial_puts("  ");
        vt100_move(45, 17);
        serial_puts("  ");
        vt100_move(36, 21);
        serial_puts("  ");
        vt100_move(47, 21);
        serial_puts("  ");
        break;
    case 2:
        vt100_move(36, 21);
        serial_puts("-> Credits <-");
        vt100_move(36, 17);
        serial_puts("  ");
        vt100_move(45, 17);
        serial_puts("  ");
        vt100_move(37, 19);
        serial_puts("  ");
        vt100_move(46, 19);
        serial_puts("  ");
        break;
    }
    return 0;
}
```
il permet de mettre des fleches sur la selection de l'utilisateur et d'éffacer celles de la sélection précedente

Voici le rendu sur le terminal :
![menu](img/menu.png)

Suivant la valeur de la variable sélection, un menu différent s'ouvre (**game_rules** ou **game_credits**)

le **game_rules** affiche le menu des commandes et barème de point des monstres
```c
void game_rules(void) {
    input = 0;
    vt100_clear_screen();
    vt100_move(38, 3);
    serial_puts("Controls :");
    vt100_move(30, 5);
    serial_puts("Q : Left   |   Right : D");
    vt100_move(35, 7);
    serial_puts("SPACE : Shoot");
    vt100_move(38, 11);
    serial_puts("Points :");
    vt100_move(35, 13);
    serial_puts("<VV> : 500 Pts");
    vt100_move(35, 15);
    serial_puts("<TT> : 250 Pts");
    vt100_move(35, 17);
    serial_puts("<==> : 100 Pts");
    vt100_move(30, 22);
    serial_puts("Press <SPACE> to play !");

    while (input != 32) /*32 ASCII de l'espace*/
    {
        input = serial_get_last_char();
    }
}
```
On vient afficher toutes les informations nessaires à l'utilisateur et tant qu'il n'appuie pas sur la barre espace (32 en ascii) on ne passe pas à l'instruction suivante

voici le rendu sur un terminal :
![rules](img/rules.png)

le **game_credit** est similaire au **game_rules** sauf que celui-ci affiche les crédits du jeu
```c
void game_credit(void) {
    input = 0;
    vt100_clear_screen();
    vt100_move(37, 3);
    serial_puts("Credits :");
    vt100_move(24, 5);
    serial_puts("This game was created by Lucas Pihery");
    vt100_move(21, 7);
    serial_puts("A project proposed by Mr Vincent Poulailleau.");
    vt100_move(31, 11);
    serial_puts("github.com/lucaspihery");
    vt100_move(29, 22);
    serial_puts("Press <SPACE> to play !");

    while (input != 32) /*32 ASCII de l'espace*/
    {
        input = serial_get_last_char();
    }
}
```

Le **game_loose()** et le **game_win()** sont deux fonctions qui génere un écran avec le score

```c
void game_loose(void) {

    vt100_clear_screen(); /* on éfface l'écran avant d'afficher l'écran de fin */
    vt100_move(29, 10);
    serial_puts("╔═╗╔═╗╔╦╗╔═╗  ╔═╗╦  ╦╔═╗╦═╗");
    vt100_move(29, 11);
    serial_puts("║ ╦╠═╣║║║║╣   ║ ║╚╗╔╝║╣ ╠╦╝");
    vt100_move(29, 12);
    serial_puts("╚═╝╩ ╩╩ ╩╚═╝  ╚═╝ ╚╝ ╚═╝╩╚═");
    vt100_move(34, 14);
    serial_puts("Your score : ");
    serial_puts(c_score);
    while (1) {
    }
}
```
le **game_win()** affiche un "YOU WIN" à la place du "GAME OVER"

Voici le rendu sur un terminal :
![loose](img/game_loose.png)

## Main

### Déclaration des structures et enum

Chaque alien a un sens (gauche ou droite), je crée donc un type d'enum **direction**
```c
typedef enum {
    Gauche, Droite
} direction;
```

Dans mon jeu space invaders il y a plusieurs aliens, j'ai deux crée un type de struct **t_alien**.

Chaque alien contient :
- x (coordonée en x de l'alien)
- y (coordonée en y de l'alien)
- sens (je viens utilisé mon type direction)
- status (si il est en vie ou non)
- type (3 types différents d'alien)

que je déclare de la manière suivante :

```c
typedef struct {
    uint8_t x;
    uint8_t y;
    direction sens;
    uint8_t status;
    uint8_t type;
} t_alien;
```

### Déclaration des variables globales
```c
uint8_t input;
char c_score[4]; /* Chaine de caracteres contenant le score */
```
Ce sont deux variables qui j'utilise dans **game_status.h**

### Déclaration des variables locales

les **coordonées**
```c
uint8_t x_vaisseau = 40;
uint8_t y_vaisseau = 20;
uint8_t x_missile = 40;
uint8_t y_missile = 20;
uint8_t x_bombe;
uint8_t y_bombe;
```
les **skins**
```c
char missile = '|';
char bombe = '*';
char ship[] = "<=^=>";
char alien1[] = "<VV>";
char alien2[] = "<TT>";
char alien3[] = "<==>";
```
les **compteurs** pour ajouter une notion de temps a notre programme 
```c
int i = 0; /* Déplacement alien */
int h = 0; /* Déplacement missile */
int j = 0; /* Temps entre chaque lancement de bombe */
int k = 0; /* Déplacement bombe */
```
**autres**
```c
uint8_t alien_speed = 15; /* Permet de faire varié la vitesse des aliens */
uint8_t live = 51; /* 3 = 51 en ASCII, nombre de vies */
uint16_t score = 0; /* Score du joueur */
uint8_t missile_lance = 0; 
uint8_t bombe_lance = 0;
uint8_t va; /* Compteur pour les boucles aliens */
t_alien aliens[15]= {0}; /* On déclare nos 15 aliens avec notre structure crée précedement */
```

## Initialisation des aliens
```c
/* Initialisation du tableau des aliens */
for (va = 0; va < 5; va++) {
    aliens[va].y = 4;
    aliens[va].x = va * 8;
    aliens[va].status = 1;
    aliens[va].sens = Droite;
    aliens[va].type = 1;
}
for (va = 5; va < 10; va++) {
    aliens[va].y = 6;
    aliens[va].x = (va - 5) * 8;
    aliens[va].status = 1;
    aliens[va].sens = Droite;
    aliens[va].type = 2;
}
for (va = 10; va < 15; va++) {
    aliens[va].y = 8;
    aliens[va].x = (va - 10) * 8;
    aliens[va].status = 1;
    aliens[va].sens = Droite;
    aliens[va].type = 3;
}
```
On effectue 3 bloucles, ou on leurs attribue une **ordonnée** :
- 4 : Pour les aliens de 0 à 5
- 6 : Pour les aliens de 5 à 10
- 8 : Pour les aliens de 10 à 15
  
 une **abcisse** avec un espacement entre chaque alien de 8 caractère (comme chaque alien fait 4 caractères il y a en réalité 4 caractères entre chaque alien),un **status** (en vie = 1 par défaut), un **sens** (Droite par défaut) et on leurs attribue également un **type** :
- 1 : Pour les aliens de 0 à 5
- 2 : Pour les aliens de 5 à 10
- 3 : Pour les aliens de 10 à 15