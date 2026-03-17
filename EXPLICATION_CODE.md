# Explication détaillée du code cub3D

**Auteurs :** roazouan & adamgallot — École 42

---

## Table des matières

1. [Vue d'ensemble](#1-vue-densemble)
2. [Structures de données](#2-structures-de-données)
3. [Fichiers d'en-tête](#3-fichiers-den-tête)
4. [Flux d'exécution complet](#4-flux-dexécution-complet)
5. [srcs/main.c](#5-srcsmainc)
6. [srcs/parsing/init_texture.c](#6-srcsparsinginit_texturec)
7. [srcs/parsing/init_map.c](#7-srcsparsinginit_mapc)
8. [srcs/parsing/check_parsing.c](#8-srcsparsingcheck_parsingc)
9. [srcs/parsing/check_map.c](#9-srcsparsingcheck_mapc)
10. [srcs/parsing/utils_pars.c](#10-srcsparsingutils_parsc)
11. [srcs/init/data.c](#11-srcsinitdatac)
12. [srcs/init/player.c](#12-srcsinitplayerc)
13. [srcs/init/textures.c](#13-srcsiniттexturesc)
14. [srcs/handling/input_handling.c](#14-srcshandlinginput_handlingc)
15. [srcs/handling/game_loop.c](#15-srcshandlinggame_loopc)
16. [srcs/render/render.c](#16-srcsrenderrenderc)
17. [srcs/render/raycasting.c](#17-srcsrenderraycastingc)
18. [srcs/exit/freeeee.c](#18-srcsexitfreeeec)
19. [srcs/exit/exit.c](#19-srcsexitexitc)
20. [Makefile](#20-makefile)
21. [Mathématiques du raycasting](#21-mathématiques-du-raycasting)

---

## 1. Vue d'ensemble

cub3D est un moteur 3D minimaliste inspiré de **Wolfenstein 3D** (1992). Il affiche un labyrinthe en fausse 3D grâce à une technique appelée **raycasting** : pour chaque colonne de pixels de la fenêtre, on envoie un rayon depuis le joueur et on calcule à quelle distance il touche un mur, ce qui donne la hauteur à afficher.

### Architecture en 3 grandes phases

```
┌─────────────────────────────────────────────────────┐
│  PHASE 1 : PARSING                                  │
│  Lire et valider le fichier .cub                    │
│  → init_textures() → init_map() → check_parsing()  │
└──────────────────────────┬──────────────────────────┘
                           │
┌──────────────────────────▼──────────────────────────┐
│  PHASE 2 : INITIALISATION RUNTIME                   │
│  Ouvrir MLX, charger textures, créer framebuffer    │
│  → init_data() → init_mlx() → init_textures_img()  │
└──────────────────────────┬──────────────────────────┘
                           │
┌──────────────────────────▼──────────────────────────┐
│  PHASE 3 : BOUCLE DE JEU (mlx_loop)                 │
│  Lire clavier → bouger joueur → rendre la frame     │
│  → game_loop() → update_player() → render_frame()  │
└─────────────────────────────────────────────────────┘
```

### Pourquoi séparer parsing et runtime ?

Le parsing valide tout **avant** d'ouvrir la fenêtre graphique. Si le fichier `.cub` est invalide, le programme quitte avec un message d'erreur sans jamais toucher MLX. C'est le principe **fail-fast** : échouer tôt, clairement, sans laisser de ressources graphiques ouvertes.

---

## 2. Structures de données

Toutes les structures sont définies dans `includes/cube3d.h`.

### `t_parsing` — données brutes du fichier .cub

```c
typedef struct s_parsing
{
    char  *no_texture;      // Chemin vers texture mur Nord (ex: "textures/wall.xpm")
    char  *so_texture;      // Chemin vers texture mur Sud
    char  *we_texture;      // Chemin vers texture mur Ouest
    char  *ea_texture;      // Chemin vers texture mur Est
    char  *floor_color;     // Couleur sol sous forme texte (ex: "100,200,50")
    char  *ceiling_color;   // Couleur plafond sous forme texte
    char  **map;            // La carte ligne par ligne (tableau de chaînes)
    char  **cp_map;         // Copie de travail pour le flood-fill
    int   player_start;     // Compteur de joueurs trouvés (doit valoir 1)
    int   player_start_x;   // Colonne du joueur dans la carte
    int   player_start_y;   // Ligne du joueur dans la carte
}   t_parsing;
```

Cette structure n'existe que pendant le parsing. Une fois les données transférées dans `t_data`, elle est libérée.

### `t_img` — image MLX générique

```c
typedef struct s_img
{
    void  *img;         // Handle opaque MLX de l'image
    int   *addr;        // Tableau de pixels (chaque int = couleur 0xRRGGBB)
    int   pixel_bits;   // Bits par pixel (généralement 32)
    int   size_line;    // Octets par ligne (peut différer de width * 4 à cause du padding)
    int   endian;       // Ordre des octets (0 = little-endian)
    int   width;        // Largeur en pixels
    int   height;       // Hauteur en pixels
}   t_img;
```

`addr` est l'adresse directe du buffer de pixels. Écrire `addr[y * (size_line/4) + x] = color` place un pixel à la position (x, y). Cette structure est utilisée pour les textures ET pour le framebuffer.

### `t_tex` — conteneur des 4 textures de murs

```c
typedef struct s_tex
{
    t_img north;   // Texture face Nord
    t_img south;   // Texture face Sud
    t_img east;    // Texture face Est
    t_img west;    // Texture face Ouest
    int   size;    // Dimension référence (largeur de north)
}   t_tex;
```

### `t_player` — état du joueur

```c
typedef struct s_player
{
    char    get_dir;    // Direction initiale lue dans la carte ('N','S','E','W')
    double  pos_x;      // Position X dans la grille (ex: 3.5 = centre de la case 3)
    double  pos_y;      // Position Y dans la grille
    double  dir_x;      // Vecteur direction de vue, composante X (longueur ~1)
    double  dir_y;      // Vecteur direction de vue, composante Y
    double  plane_x;    // Vecteur plan caméra, composante X (perpendiculaire à dir)
    double  plane_y;    // Vecteur plan caméra, composante Y
    bool    has_moved;  // Vrai si le joueur a bougé ce frame (optimisation rendu)
    int     move_x;     // Intention de déplacement latéral : -1=gauche, 0=rien, 1=droite
    int     move_y;     // Intention de déplacement avant/arrière : -1=recul, 0=rien, 1=avance
    int     rotation;   // Intention de rotation : -1=gauche, 0=rien, 1=droite
}   t_player;
```

**Point clé :** `dir` et `plane` forment toujours deux vecteurs **orthogonaux**. `dir` est la direction où le joueur regarde. `plane` est perpendiculaire à `dir` et représente la largeur du champ de vision (FOV). La longueur de `plane` ≈ 0.66 donne un FOV d'environ 66 degrés.

### `t_ray` — état temporaire d'un rayon (une colonne écran)

```c
typedef struct s_ray
{
    double  camera_x;       // Position sur le plan caméra : -1.0 (gauche) à +1.0 (droite)
    double  dir_x;          // Direction X de ce rayon précis
    double  dir_y;          // Direction Y de ce rayon précis
    int     map_x;          // Case grille X où se trouve le rayon
    int     map_y;          // Case grille Y où se trouve le rayon
    double  side_dist_x;    // Distance jusqu'à la prochaine bordure verticale (axe X)
    double  side_dist_y;    // Distance jusqu'à la prochaine bordure horizontale (axe Y)
    double  delta_dist_x;   // Distance entre deux bordures verticales consécutives
    double  delta_dist_y;   // Distance entre deux bordures horizontales consécutives
    int     step_x;         // Sens de progression X : +1 ou -1
    int     step_y;         // Sens de progression Y : +1 ou -1
    int     hit;            // 0 = pas encore touché de mur, 1 = mur touché
    int     side;           // 0 = mur vertical (EO) touché, 1 = mur horizontal (NS)
    double  perp_wall_dist; // Distance perpendiculaire au mur (corrigée, sans fish-eye)
    int     line_height;    // Hauteur en pixels de la colonne à dessiner
    int     draw_start;     // Pixel Y de début de la colonne
    int     draw_end;       // Pixel Y de fin de la colonne
    double  wall_x;         // Position exacte d'impact sur le mur (0.0 à 1.0)
    int     tex_x;          // Colonne dans la texture correspondante
    int     tex_y;          // Ligne dans la texture (calculée pixel par pixel)
}   t_ray;
```

Cette structure est locale à `render_column` — elle est recréée pour chaque colonne de l'écran.

### `t_data` — état global runtime

```c
typedef struct s_data
{
    void      *mlx;            // Connexion MLX (contexte graphique)
    void      *win;            // Fenêtre MLX
    int       win_width;       // Largeur fenêtre = 1024
    int       win_height;      // Hauteur fenêtre = 720
    char      *no_texture;     // Chemin texture Nord (owned par data après transfert)
    char      *so_texture;     // Chemin texture Sud
    char      *we_texture;     // Chemin texture Ouest
    char      *ea_texture;     // Chemin texture Est
    char      *floor_color;    // Couleur sol texte ("R,G,B")
    char      *ceiling_color;  // Couleur plafond texte
    int       floor_rgb;       // Couleur sol convertie en entier 0xRRGGBB
    int       ceiling_rgb;     // Couleur plafond convertie en entier 0xRRGGBB
    char      **map;           // Carte normalisée (toutes lignes même longueur)
    t_player  player;          // État complet du joueur
    t_tex     textures;        // Les 4 textures chargées en mémoire
    t_img     frame;           // Framebuffer off-screen (1024×720 pixels)
}   t_data;
```

### Enums utilitaires

```c
typedef enum e_macro { SUCCESS = 0, ERRORS = 1, EXIT = 2 } t_macro;
typedef enum e_texture { NO, SO, WE, EA, F, C } t_texture;
```

`SUCCESS` et `ERRORS` sont utilisés comme valeurs de retour des fonctions d'initialisation, ce qui permet d'écrire `if (func() != SUCCESS) return (ERRORS);` de manière lisible.

---

## 3. Fichiers d'en-tête

### `includes/cube3d.h`

Définit toutes les structures, constantes et prototypes du projet.

**Constantes importantes :**
```c
#define WIDTH      1024      // Largeur de la fenêtre en pixels
#define HEIGHT     720       // Hauteur de la fenêtre en pixels
#define MOVE_SPEED 0.08      // Distance parcourue par frame lors d'un déplacement
#define ROT_SPEED  0.05      // Angle de rotation par frame (en radians)
```

`MOVE_SPEED = 0.08` signifie que le joueur avance de 0.08 case par frame. À ~60 FPS, c'est ~4.8 cases/seconde, ce qui donne un déplacement fluide sans traverser les murs (les murs sont à distance entière).

`ROT_SPEED = 0.05` radians ≈ 2.86° par frame. À 60 FPS, la rotation complète prend environ 21 secondes — suffisamment lent pour être contrôlable.

### `includes/handler.h`

Définit les codes de touches clavier de façon portable entre macOS et Linux.

```c
#ifdef __APPLE__
#  define XK_Escape  53      // Touche Echap macOS
#  define XK_Left    123     // Flèche gauche macOS
#  define XK_w       13      // Touche W sur clavier AZERTY/QWERTY macOS
// ...
#else
#  define XK_Escape  65307   // Touche Echap Linux (X11)
#  define XK_Left    65361   // Flèche gauche Linux
#  define XK_w       119     // Touche W Linux
// ...
#endif
```

**Pourquoi des codes différents ?** Sur macOS, MLX utilise les codes du système Apple (Carbon). Sur Linux, MLX utilise les keysyms X11. Sans cette abstraction, le jeu ne fonctionnerait que sur un seul OS.

---

## 4. Flux d'exécution complet

```
main(argc, argv)
│
├── [Vérification] argc != 2 → erreur "Usage"
│
├── ft_calloc(t_parsing)           ← allocation structure parsing
│
├── parsing_func(av[1], parsing)
│   ├── open(fichier.cub)
│   ├── has_suffix(".cub")         ← vérifie extension
│   ├── init_textures(fd)          ← lit NO/SO/WE/EA/F/C
│   ├── init_map(fd)               ← lit la carte
│   └── check_parsing()            ← valide tout
│
├── init_game(&data, parsing)
│   ├── init_data(&data)           ← initialise t_data à zéro
│   ├── copy_parsing_to_data()     ← transfère les pointeurs
│   ├── init_player_dir()          ← place le joueur sur la carte
│   ├── init_mlx()                 ← ouvre fenêtre MLX
│   ├── init_textures_img()        ← charge les .xpm en mémoire
│   └── init_frame_img()           ← crée le framebuffer off-screen
│
├── detach_from_parsing()          ← NULLifie les pointeurs transférés
├── free_parsing()                 ← libère t_parsing
│
├── listen_input(&data)            ← branche les callbacks clavier
│   ├── mlx_hook(key_press)
│   ├── mlx_hook(key_release)
│   ├── mlx_hook(close_window)
│   └── mlx_loop_hook(game_loop)
│
├── render_frame(&data)            ← premier rendu (avant la boucle)
│
└── mlx_loop()                     ← boucle infinie MLX
    └── [chaque frame] game_loop()
        ├── update_player()
        │   ├── move_player()      ← WASD avec collision
        │   └── rotate_player()    ← flèches avec matrice rotation
        └── render_frame()
            ├── draw_floor_and_ceiling()   ← fond couleur uni
            └── [pour chaque colonne x]
                render_column(x)
                ├── init_ray()
                ├── init_step_and_side_dist()
                ├── perform_dda()          ← trouve le mur
                ├── select_texture()       ← choisit N/S/E/W
                ├── calculate_line_and_texture()
                └── [pour chaque pixel y] dessin avec tex_y
```

---

## 5. srcs/main.c

### `free_parsing` (static)

```c
static void free_parsing(t_parsing *parsing)
```

Libère toute la mémoire allouée dans `t_parsing` :
- Les 6 chaînes (`no_texture`, `so_texture`, `we_texture`, `ea_texture`, `floor_color`, `ceiling_color`)
- Les deux tableaux de la carte (`map` et `cp_map`) via `free_tab()`
- La structure elle-même

Chaque `free` est conditionnel (`if (parsing->no_texture)`) car certains champs peuvent ne pas avoir été alloués si une erreur survient tôt dans le parsing.

---

### `detach_from_parsing` (static)

```c
static void detach_from_parsing(t_parsing *parsing)
```

Problème à résoudre : après `copy_parsing_to_data()`, les pointeurs `no_texture`, `so_texture`, etc. existent **dans les deux structures à la fois** (`t_parsing` et `t_data`). Si on appelle `free_parsing()` maintenant, on libèrerait des pointeurs que `t_data` utilise encore — c'est un **double free**.

Solution : avant d'appeler `free_parsing()`, on met les pointeurs de `t_parsing` à `NULL`. Ainsi `free_parsing()` ne libèrera que ce qui est réellement possédé par `t_parsing`.

```
AVANT detach :          APRÈS detach :
parsing->no_texture → "textures/wall.xpm" ← data->no_texture
                  ↓           ↓
              NULL ←   parsing->no_texture     data->no_texture → "textures/wall.xpm"
```

C'est une gestion explicite de la **propriété des pointeurs** en C.

---

### `parsing_func` (static)

```c
static int parsing_func(char *file, t_parsing *parsing)
```

Pipeline de parsing en 3 étapes :

1. `open(file, O_RDONLY)` — ouvre le fichier en lecture seule. Si échec (fichier inexistant, permissions insuffisantes), retourne une erreur immédiatement.
2. `has_suffix(file, ".cub")` — vérifie que le fichier a l'extension `.cub`. Cette vérification se fait **après** l'ouverture pour donner une erreur cohérente (si le fichier n'existe pas, on parle d'absence de fichier, pas d'extension).
3. `init_textures()` → `init_map()` → `check_parsing()` — les trois étapes de parsing, dans cet ordre précis.

**Pourquoi cet ordre ?** `init_textures` lit les 6 métadonnées en haut du fichier. Le descripteur de fichier `fd` avance automatiquement. `init_map` continue depuis là où `init_textures` s'est arrêté et lit le reste. Ils partagent le même `fd` ouvert.

---

### `init_game` (static)

```c
static int init_game(t_data *data, t_parsing *parsing)
```

Séquence d'initialisation runtime. Chaque étape peut échouer :

1. `init_data(data)` — met tout à zéro/NULL (pas d'échec possible)
2. `copy_parsing_to_data(data, parsing)` — transfère les pointeurs et convertit les couleurs texte en entiers RGB
3. `init_player_dir(data, parsing)` — positionne le joueur d'après la carte
4. `init_mlx(data)` — crée le contexte graphique et la fenêtre
5. `init_textures_img(data)` — charge les fichiers .xpm en mémoire GPU
6. `init_frame_img(data)` — crée l'image off-screen pour le double buffering

Si une étape échoue, on retourne `ERRORS` immédiatement et `main` appellera `free_data()` pour nettoyer ce qui a été initialisé.

---

### `main`

```c
int main(int ac, char **av, char **env)
```

Le `(void)env` signifie qu'on accepte `env` dans la signature (requis par les normes 42) mais on ne l'utilise pas.

Séquence principale :
1. Vérification `ac != 2` — exactement 1 argument requis
2. `ft_calloc(1, sizeof(t_parsing))` — `calloc` initialise à zéro, ce qui signifie que tous les pointeurs de `t_parsing` sont `NULL` dès le départ (sécurité pour les `free` conditionnels)
3. `parsing_func` — si échoue, libère `parsing` et quitte
4. `init_game` — si échoue, libère `data` ET `parsing` avant de quitter
5. `detach_from_parsing` puis `free_parsing` — transfert de propriété
6. `listen_input` — branche les callbacks (ne démarre pas la boucle)
7. `render_frame` — **premier rendu manuel** avant de lancer la boucle, pour afficher quelque chose immédiatement
8. `mlx_loop` — cède le contrôle à MLX

---

## 6. srcs/parsing/init_texture.c

### `exit_error` (static)

```c
static void exit_error(int fd, const char *msg)
```

Fonction d'erreur centralisée utilisée pendant le parsing. Elle :
1. Affiche le message si non NULL
2. Ferme le descripteur de fichier si `fd >= 0` (évite une fuite de fd)
3. Appelle `exit(ERRORS)`

Sans cette fonction, chaque erreur aurait besoin de `close(fd); printf(...); exit(...)` — répétition de code et risque d'oublier le `close`.

---

### `ft_strjoin_char` (static)

```c
static char *ft_strjoin_char(char *str, char c)
```

Ajoute un caractère `c` à la fin de la chaîne `str` allouée dynamiquement. Alloue `strlen + 2` octets (pour le char + le `\0`), copie, ajoute `c`, **libère l'ancienne chaîne** et retourne la nouvelle.

Utilisée dans `read_line` pour construire une ligne caractère par caractère. C'est comme `ft_strjoin` mais pour un seul caractère, ce qui évite de créer une chaîne temporaire de 2 caractères à chaque itération.

---

### `read_line`

```c
char *read_line(int fd)
```

Lit une ligne complète depuis un descripteur de fichier, sans utiliser `get_next_line`. Fonctionnement :
- Lis 1 octet à la fois avec `read(fd, &c, 1)`
- Accumule dans `line` via `ft_strjoin_char`
- Retourne dès que `\n` est lu (le `\n` est **inclus** dans la chaîne retournée)
- Retourne `NULL` si EOF atteint sans avoir lu aucun caractère

**Pourquoi lire octet par octet ?** C'est simple à implémenter et garantit qu'on ne "dépasse" jamais dans le fichier. La performance n'est pas critique ici car le parsing n'est fait qu'une seule fois.

---

### `skip_spaces` (static)

```c
static char *skip_spaces(char *str)
```

Avance le pointeur `str` tant qu'il pointe sur un espace `' '` ou une tabulation `'\t'`. Retourne un pointeur vers le premier caractère non-blanc.

Exemple : `skip_spaces("   textures/wall.xpm")` retourne `"textures/wall.xpm"`.

---

### `is_map_line` (static, dans init_texture.c)

```c
static int is_map_line(const char *line)
```

Retourne 1 si la ligne contient **uniquement** des caractères valides d'une carte : `0`, `1`, espace, `N`, `S`, `E`, `W`.

Utilisée dans `init_textures` pour détecter si une ligne de carte apparaît **trop tôt** (avant que les 6 métadonnées soient lues). Dans ce cas, l'erreur `"Map encountered before all textures"` est déclenchée.

---

### `has_whitespace` (static)

```c
static int has_whitespace(const char *str)
```

Retourne 1 si la chaîne contient au moins un espace ou une tabulation. Utilisée pour valider les chemins de textures : un chemin comme `"textures/wall with spaces.xpm"` serait ambigu à parser.

---

### `has_suffix`

```c
int has_suffix(const char *str, const char *suffix)
```

Vérifie que `str` se termine par `suffix`. Implémentation :
- Calcule les deux longueurs
- Retourne 0 si `str` est plus courte que `suffix`
- Compare les `suffix_len` derniers caractères avec `ft_strncmp`

Utilisée pour vérifier `.cub` (dans `main`) et `.xpm` (dans `set_texture`). C'est une seule fonction pour les deux, ce qui évite la duplication.

---

### `parse_color_value` (static)

```c
static int parse_color_value(const char *str, int *i)
```

Parse un entier représentant une composante RGB [0..255] à la position `*i` dans `str`. Le paramètre `i` est un **pointeur** pour avancer automatiquement dans la chaîne après la lecture (le `i` du caller est modifié).

Validations :
- Saute les espaces/tabulations avant et après
- Refuse si aucun chiffre trouvé (`digits == 0`)
- Refuse si la valeur dépasse 255 (détection **pendant** la lecture, pas après)

Retourne -1 en cas d'erreur, sinon la valeur.

---

### `is_valid_color` (static)

```c
static int is_valid_color(const char *str)
```

Valide le format complet `R,G,B`. Séquence :
1. Parse `R`, vérifie que le caractère suivant est `,`
2. Avance après `,`, parse `G`, vérifie `,`
3. Avance, parse `B`
4. Vérifie que le reste (après espaces) est `\0`

Si l'une de ces étapes échoue, retourne 0. La chaîne `"100, 100, 100"` (avec espaces) **passe** cette validation car `parse_color_value` accepte les espaces avant et après.

---

### `set_texture` (static)

```c
static void set_texture(char **dst, const char *value, const char *id, int fd)
```

Enregistre un chemin de texture dans `*dst` après une série de validations :
1. `*dst != NULL` → doublon → erreur `"Duplicate NO texture"`
2. `value == NULL || value[0] == '\0'` → valeur vide → erreur
3. `has_whitespace(value)` → espace dans le chemin → erreur
4. `!has_suffix(value, ".xpm")` → mauvaise extension → erreur
5. `open(value, O_RDONLY) < 0` → fichier inexistant → erreur

Si tout est valide : `ft_strdup(value)` pour conserver une copie propre.

**Pourquoi vérifier l'existence du fichier si tôt ?** Pour donner une erreur claire `"NO texture file not found"` dès le parsing, avant que MLX soit ouvert. Si on attendait `mlx_xpm_file_to_image`, MLX peut crasher ou donner un message cryptique.

---

### `set_color` (static)

```c
static void set_color(char **dst, const char *value, const char *id, int fd)
```

Identique à `set_texture` mais pour les couleurs F/C. Validations plus simples :
1. Doublon
2. Valeur vide
3. Format invalide via `is_valid_color`

Stocke la couleur **comme chaîne** (pas encore convertie en entier). La conversion en `0xRRGGBB` se fait plus tard dans `copy_parsing_to_data`.

---

### `parse_texture_line` (static)

```c
static int parse_texture_line(t_parsing *parsing, char *line, int fd)
```

Routeur : détermine l'identifiant d'une ligne et appelle `set_texture` ou `set_color` approprié.

Vérification : `ft_strncmp(line, "NO", 2) == 0 && (line[2] == ' ' || '\t' || '\0')` — les 2 premières lettres correspondent ET le troisième caractère est un séparateur (pas `"NORTH"` ou `"NON"`).

Retourne 1 si la ligne est reconnue, 0 sinon. Ce retour permet à `init_textures` de distinguer une ligne valide d'un identifiant inconnu.

---

### `init_textures`

```c
void init_textures(t_parsing *parsing, int fd)
```

Lit les 6 métadonnées obligatoires du fichier `.cub` :

```c
while (i < 6)  // On attend exactement 6 lignes de métadonnées
{
    line = read_line(fd);           // Lire une ligne brute
    trimmed = ft_strtrim(line, " \t\n");  // Enlever espaces/newlines

    if (trimmed[0] == '\0')         // Ligne vide → ignorer
        continue;

    if (parse_texture_line(...))    // Ligne reconnue (NO/SO/...) → i++
        i++;
    else if (is_map_line(trimmed))  // Ressemble à une carte → trop tôt
        exit_error("Map encountered before all textures");
    else                            // Identifiant inconnu
        exit_error("Invalid identifier");
}
```

Les lignes vides (`trimmed[0] == '\0'`) sont ignorées sans incrémenter `i`. Cela permet d'avoir des lignes vides entre les métadonnées.

---

## 7. srcs/parsing/init_map.c

### `normalize_map`

```c
void normalize_map(char **map)
```

Rend toutes les lignes de la carte de **même longueur** en remplissant les lignes courtes avec `'Z'`.

**Pourquoi ?** La carte peut avoir des lignes de longueurs différentes (si les murs de droite sont implicites par des espaces). Pour le raycasting et la collision, on accède à `map[y][x]` directement. Si la ligne est plus courte que `x`, on sort du tableau → segfault. Avec la normalisation, toute position hors de la carte originale retourne `'Z'` (traité comme mur).

Implémentation en 2 passes :
1. Trouver `max_len` (longueur de la ligne la plus longue)
2. Pour chaque ligne plus courte : `ft_calloc` → `ft_memset('Z')` → `ft_memcpy` (copie le contenu original) → libère l'ancienne

---

### `is_map_line` (static, dans init_map.c)

```c
static int is_map_line(char *line)
```

Version plus simple que celle de `init_texture.c` : vérifie uniquement le **premier caractère** de la ligne. Si c'est `1`, `0`, espace, `N`, `S`, `E`, ou `W` → c'est une ligne de carte.

Cette différence est intentionnelle : dans `init_map`, on est moins strict car on veut simplement séparer les lignes de carte des lignes vides ou parasites. La validation complète des caractères se fait plus tard dans `check_wrong_char`.

---

### `append_line` (static)

```c
static char *append_line(char *dst, char *line)
```

Concatène `line` à `dst` avec `ft_strjoin`, libère `dst` et retourne le nouveau pointeur. Permet de construire la map complète comme une seule grande chaîne avant de la découper avec `ft_split`.

---

### `init_map`

```c
void init_map(t_parsing *parsing, int fd)
```

Lit tout ce qui reste du fichier après les métadonnées :

1. Initialise `map_str` comme chaîne vide
2. Lit ligne par ligne avec `read_line`
3. N'ajoute que les lignes qui "ressemblent à une carte" (`is_map_line`)
4. Ferme `fd`
5. Si `map_str` est vide → erreur `"Missing map"`
6. `ft_split(map_str, '\n')` découpe la chaîne en tableau de lignes

**Point subtil :** `init_map` ne valide pas le contenu de la carte, juste sa présence. La validation (caractères valides, murs fermés, joueur unique) est déléguée à `check_parsing`.

---

## 8. srcs/parsing/check_parsing.c

### `check_wrong_char` (static)

```c
static void check_wrong_char(char **map, t_parsing *parsing)
```

Parcourt toute la carte et vérifie chaque caractère :
- Caractères autorisés : `1`, `0`, espace, `N`, `S`, `E`, `W`
- Tout autre caractère → `exit(ERRORS)` immédiat

En parallèle, **compte les joueurs** : chaque `N/S/E/W` incrémente `parsing->player_start` et mémorise sa position (`player_start_x`, `player_start_y`).

Après le scan : `player_start != 1` → erreur (0 joueur ou 2+ joueurs).

**Attention :** le `exit` est appelé avant le `printf` dans le code source. C'est un ordre incorrect (le message peut ne pas s'afficher) — c'est un bug connu documenté dans les tests.

---

### `check_top_bottom` (static)

```c
static void check_top_bottom(char **map, int len)
```

Vérifie que la **première et la dernière ligne** de la carte sont entièrement composées de `'1'`. Si un caractère différent de `'1'` est trouvé → erreur `"Top wall is not closed"` ou `"Bottom wall is not closed"`.

**Limitation :** Cette vérification ne gère pas les espaces en début de ligne. Une carte avec `"   11111"` comme première ligne (espaces + murs) serait rejetée même si c'est techniquement valide selon le sujet.

---

### `check_walls` (static)

```c
static void check_walls(char **map)
```

Vérifications structurelles :
1. La carte n'est pas vide (`!map || !map[0]`)
2. Au moins 3 lignes (une rangée de murs haut, au moins une ligne intérieure, une rangée bas)
3. `check_top_bottom` pour les bords horizontaux
4. Pour chaque ligne : non vide, commence par `'1'` (mur gauche), finit par `'1'` (mur droit)

Ces vérifications détectent les cas évidents avant le flood-fill plus coûteux.

---

### `check_map` (static)

```c
static void check_map(char **map, t_parsing *parsing)
```

Orchestre la validation complète de la carte en 5 étapes :

1. `parsing->player_start = 0` — reset du compteur (peut être appelé plusieurs fois)
2. `check_wrong_char` — validation caractères + localisation joueur
3. `check_walls` — validation des bordures externes
4. `ft_arraydup` — copie profonde de la carte dans `cp_map`
5. `check_map_flood` — flood-fill depuis la position du joueur
6. `chrmap(cp_map)` — vérifie que tous les marqueurs `N/S/E/W` ont été explorés par le flood
7. `normalize_map` — normalise la carte pour le runtime

**Pourquoi dupliquer la carte ?** Le flood-fill est **destructif** : il remplace les cases `'0'` par `'F'`. Si on travaillait directement sur `map`, la carte serait abîmée. `cp_map` est la copie de travail.

---

### `check_parsing`

```c
int check_parsing(t_parsing *parsing)
```

Point d'entrée de la validation globale. Vérifie d'abord que tous les champs obligatoires sont présents (si le fichier n'a pas de carte par exemple), puis appelle `check_map`.

Retourne `ERRORS` si un champ est manquant (sans `exit`), pour laisser `main` gérer la libération propre.

---

## 9. srcs/parsing/check_map.c

Ce fichier implémente l'algorithme de **flood-fill** pour détecter si la carte est hermétiquement fermée.

### Principe du flood-fill

L'idée : depuis la position du joueur, on "inonde" toutes les cases accessibles (cases `'0'`). Si l'inondation atteint le bord de la carte ou un espace `' '`, c'est que la carte a un trou — le joueur pourrait "s'échapper". Si l'inondation reste entièrement à l'intérieur des murs `'1'`, la carte est valide.

```
AVANT flood-fill :    APRÈS flood-fill :    RÉSULTAT :
111111                111111
100001     →          1FFFF1     →  OK (joueur reste dans les murs)
100N01                1FFFФ1
100001                1FFFF1
111111                111111
```

### `is_valid_pos` (static)

```c
static int is_valid_pos(t_parsing *parsing, int y, int x)
```

Vérifie qu'une position `(y, x)` est dans les limites de `cp_map` :
- `y` doit être dans `[0, nombre_de_lignes - 1]`
- La ligne `cp_map[y]` doit exister (non NULL)
- `x` doit être dans `[0, strlen(cp_map[y]) - 1]`

Retourne 1 si valide, 0 sinon. Appelée systématiquement avant tout accès indexé à `cp_map`.

---

### `can_flood` (static)

```c
static int can_flood(t_parsing *parsing, int y, int x, char perso)
```

Classifie ce qui se trouve en `(y, x)` du point de vue du flood-fill :
- `-1` : position invalide (hors carte) **ou** espace `' '` → **fuite détectée**
- `0` : mur `'1'` ou déjà visité `'F'` ou marqueur joueur `perso` → **bloquer ici**
- `1` : case libre `'0'` → **à explorer**

Le retour `-1` signifie "la carte a un trou ici". Le retour `0` signifie "ne pas continuer". Le retour `1` signifie "continuer le flood".

---

### `flood_direction` (static)

```c
static void flood_direction(t_parsing *p, int y, int x, char perso)
```

Depuis la case actuelle `(y, x)`, vérifie les 4 voisins (haut, bas, gauche, droite) :

**Étape 1 — Vérification de fuite :** Si **n'importe quel** voisin retourne `-1`, la carte est ouverte → `exit(ERRORS)` immédiat. On vérifie tous les voisins avant de recurser, ce qui permet d'afficher l'erreur correctement.

**Étape 2 — Récursion :** Pour chaque voisin qui retourne `1` (case libre non visitée), on appelle `check_map_flood` récursivement.

L'ordre est important : vérifier D'ABORD les fuites, PUIS recurser.

---

### `check_map_flood`

```c
void check_map_flood(t_parsing *parsing, int new_y, int new_x, char perso)
```

Corps principal du flood-fill :
1. `is_valid_pos` — si hors carte → erreur (ne devrait pas arriver si `can_flood` est appelé avant, mais défense en profondeur)
2. Espace `' '` → erreur
3. Mur `'1'` ou déjà visité `'F'` → retour immédiat (base de la récursion)
4. `cp_map[new_y][new_x] = 'F'` — **marquer comme visité**
5. `flood_direction` — explorer les 4 voisins

Le marquage `'F'` empêche de revisiter une case (évite la récursion infinie).

---

### `chrmap`

```c
int chrmap(char **map)
```

Après le flood-fill, vérifie qu'**aucun marqueur de joueur** (`N/S/E/W`) ne subsiste dans `cp_map`. Si un marqueur subsiste, cela signifie que le flood-fill ne l'a pas atteint — ce qui indiquerait que le joueur est dans une zone isolée. Retourne 0 (problème) si un marqueur est trouvé, 1 (OK) sinon.

En pratique, le flood-fill commence sur la case du joueur et la remplace par `'F'` dès le début — donc un marqueur ne devrait jamais subsister après un flood réussi. `chrmap` est un garde-fou de cohérence.

---

## 10. srcs/parsing/utils_pars.c

### `ft_arraylen`

```c
int ft_arraylen(char **array)
```

Compte le nombre d'éléments d'un tableau `char **` terminé par `NULL`. Équivalent de `strlen` mais pour les tableaux de chaînes. Utilisé pour connaître le nombre de lignes de la carte.

---

### `ft_arraydup`

```c
char **ft_arraydup(char **src, char **dst)
```

Copie profonde d'un tableau de chaînes : chaque `src[i]` est dupliqué avec `ft_strdup` dans `dst[i]`. Le `dst` doit être pré-alloué avec `ft_calloc(ft_arraylen + 1)`.

Le `NULL` final est posé par `ft_calloc` (qui initialise à 0) ou explicitement à la fin.

Utilisé pour créer `cp_map` — la copie de travail du flood-fill.

---

## 11. srcs/init/data.c

### `init_player` (static)

```c
static void init_player(t_player *player)
```

Initialise tous les champs de `t_player` à leurs valeurs neutres : `0.0` pour les doubles, `false` pour le bool, `0` pour les ints. Assure qu'aucune valeur non initialisée ne cause de comportement indéfini avant que `init_player_dir` place le joueur.

---

### `init_img` (static)

```c
static void init_img(t_img *img)
```

Initialise une structure image à l'état vide : pointeurs `NULL`, dimensions à 0. Cela permet de vérifier plus tard si une image a été chargée (`if (img->addr)`) de façon fiable.

---

### `parse_color_component` (static)

```c
static int parse_color_component(const char *str, int *i)
```

Identique à `parse_color_value` dans `init_texture.c`. Double implémentation justifiée : `init_texture.c` valide la couleur **pendant le parsing** (pour afficher une erreur lisible), tandis que `data.c` re-parse la couleur **pendant l'initialisation** pour la convertir en entier. Ce sont deux contextes distincts avec des besoins différents.

---

### `parse_rgb_color` (static)

```c
static int parse_rgb_color(const char *str, int *rgb)
```

Convertit une chaîne `"R,G,B"` en entier `0xRRGGBB` par **décalage binaire** :

```c
*rgb = (r << 16) | (g << 8) | b;
```

Explication du packing :
- `r << 16` place les 8 bits de rouge aux positions 23-16 : `0x__RR0000` → `0xRR0000`
- `g << 8` place les 8 bits de vert aux positions 15-8 : `0x00GG00`
- `b` reste aux positions 7-0 : `0x0000BB`
- L'opérateur `|` combine les trois : `0xRRGGBB`

C'est le format de couleur natif de MLX pour les pixels. `my_mlx_pixel_put` peut écrire cette valeur directement dans le framebuffer.

---

### `init_data`

```c
void init_data(t_data *data)
```

Initialise `t_data` à un état cohérent "vide" : `mlx` et `win` à `NULL`, dimensions à `WIDTH`/`HEIGHT`, tous les pointeurs à `NULL`, et appelle `init_img` et `init_player` pour les sous-structures.

**Pourquoi initialiser à zéro ?** Si `init_game` échoue à l'étape 3 (par exemple), `free_data` sera appelé sur une structure partiellement initialisée. Si les pointeurs non-initialisés étaient à des valeurs aléatoires, `free_data` pourrait appeler `mlx_destroy_image` sur un pointeur invalide → crash. Avec l'initialisation à `NULL`, chaque `free/destroy` conditionnel est sûr.

---

### `copy_parsing_to_data`

```c
int copy_parsing_to_data(t_data *data, t_parsing *parsing)
```

**Transfert de propriété** des données de parsing vers le runtime :

```c
data->no_texture = parsing->no_texture;   // Pas de strdup ! On transfère le pointeur
data->map = parsing->map;
// ... etc
```

Puis conversion des couleurs texte en entiers :

```c
parse_rgb_color(data->floor_color, &data->floor_rgb);
parse_rgb_color(data->ceiling_color, &data->ceiling_rgb);
```

Après cet appel, `data` **possède** les pointeurs. C'est pourquoi `detach_from_parsing` est appelé ensuite pour éviter le double free.

---

### `init_mlx`

```c
int init_mlx(t_data *data)
```

Initialise le contexte graphique MLX en 2 étapes :
1. `mlx_init()` — crée la connexion au serveur d'affichage (ouvre le display)
2. `mlx_new_window(mlx, width, height, "cub3D")` — crée la fenêtre visible

Les deux peuvent retourner `NULL` en cas d'échec (display indisponible, mémoire insuffisante).

---

### `init_frame_img`

```c
int init_frame_img(t_data *data)
```

Crée le **framebuffer off-screen** : une image en mémoire de la même taille que la fenêtre (1024×720).

```c
data->frame.img = mlx_new_image(data->mlx, width, height);
data->frame.addr = (int *)mlx_get_data_addr(img, &pixel_bits, &size_line, &endian);
```

`mlx_new_image` alloue la mémoire. `mlx_get_data_addr` retourne un pointeur vers le tableau de pixels bruts — c'est sur ce tableau que `my_mlx_pixel_put` écrit. Une fois toute la frame dessinée, `mlx_put_image_to_window` copie ce buffer vers la fenêtre en une seule opération atomique : c'est le **double buffering**, qui élimine le scintillement.

---

## 12. srcs/init/player.c

### Les 4 fonctions de direction (static)

```c
static void set_dir_north(t_player *player)
static void set_dir_south(t_player *player)
static void set_dir_east(t_player *player)
static void set_dir_west(t_player *player)
```

Chaque fonction définit le couple `(dir, plane)` pour une orientation cardinale. Les valeurs :

| Direction | `dir_x` | `dir_y` | `plane_x` | `plane_y` |
|-----------|---------|---------|-----------|-----------|
| Nord      | 0.0     | -1.0    | 0.66      | 0.0       |
| Sud       | 0.0     | +1.0    | -0.66     | 0.0       |
| Est       | +1.0    | 0.0     | 0.0       | +0.66     |
| Ouest     | -1.0    | 0.0     | 0.0       | -0.66     |

**La valeur 0.66 :** C'est le rapport `tan(FOV/2)` pour un FOV de ~66.4°. Plus précisément, si on considère que `|plane| / |dir| = tan(FOV/2)`, et que `|dir| = 1`, alors `|plane| = tan(33.2°) ≈ 0.656 ≈ 0.66`. C'est la valeur "classique" du raycasting de Lode's tutorial.

**L'orthogonalité :** `dir` et `plane` doivent toujours être perpendiculaires. Pour Nord : `dir = (0,-1)` et `plane = (0.66, 0)` → produit scalaire = `0*0.66 + (-1)*0 = 0` ✓. La rotation via matrice de rotation conserve cette propriété.

---

### `init_player_dir`

```c
void init_player_dir(t_data *data, t_parsing *parsing)
```

Positionne le joueur dans le monde :

```c
data->player.pos_x = (double)parsing->player_start_x + 0.5;
data->player.pos_y = (double)parsing->player_start_y + 0.5;
```

Le `+ 0.5` centre le joueur dans sa case. Si le joueur est à la case (3, 2), sa position est `(3.5, 2.5)` — au milieu de cette case. Cela évite que le joueur démarre collé contre un mur.

Ensuite, le caractère `N/S/E/W` est lu depuis la carte, la case est remplacée par `'0'` (espace vide), et le setter de direction approprié est appelé.

---

## 13. srcs/init/textures.c

### `destroy_texture_img` (static)

```c
static void destroy_texture_img(t_data *data, t_img *img)
```

Détruit une image MLX de façon sûre : vérifie que `data->mlx` et `img->img` sont non-NULL avant d'appeler `mlx_destroy_image`. Remet les pointeurs à NULL pour éviter le double free.

---

### `load_single_texture` (static)

```c
static int load_single_texture(t_data *data, t_img *img, char *path)
```

Charge un fichier XPM en mémoire GPU :

```c
img->img = mlx_xpm_file_to_image(data->mlx, path, &img->width, &img->height);
img->addr = (int *)mlx_get_data_addr(img->img, &img->pixel_bits, &img->size_line, &img->endian);
```

`mlx_xpm_file_to_image` décode le fichier XPM et crée une image MLX. `width` et `height` sont remplis automatiquement. Puis `mlx_get_data_addr` donne l'accès direct au tableau de pixels pour la lecture lors du rendu.

---

### `free_loaded_textures` (static)

```c
static void free_loaded_textures(t_data *data)
```

Détruit les 4 textures potentiellement chargées. Utilisé comme **rollback** : si la 3ème texture échoue au chargement, on détruit les 2 premières déjà chargées.

---

### `init_textures_img`

```c
int init_textures_img(t_data *data)
```

Charge les 4 textures dans l'ordre : nord → sud → est → ouest.

**Stratégie tout ou rien :** si n'importe quel chargement échoue, `free_loaded_textures` est appelé pour annuler tous les chargements précédents. On ne laisse jamais l'application dans un état partiellement initialisé.

À la fin : `data->textures.size = data->textures.north.width` — stocke la dimension de référence (toutes les textures sont supposées carrées et de même taille).

---

## 14. srcs/handling/input_handling.c

### `key_press_handling` (static)

```c
static int key_press_handling(int key_input, t_data *data)
```

Callback appelé par MLX à chaque appui de touche. Ne fait **pas** bouger le joueur directement. Met à jour des **flags d'intention** :

| Touche | Action | Flag modifié |
|--------|--------|-------------|
| W | Avancer | `move_y = 1` |
| S | Reculer | `move_y = -1` |
| A | Strafe gauche | `move_x = -1` |
| D | Strafe droit | `move_x = 1` |
| ← | Rotation gauche | `rotation = -1` |
| → | Rotation droite | `rotation = 1` |
| Esc | Quitter | `quit_code()` |

**Pourquoi des flags et non des déplacements directs ?** Si on déplaçait directement dans le callback, le mouvement serait irrégulier (dépend du taux de répétition clavier du système). Avec des flags, `game_loop` applique le déplacement à chaque frame, indépendamment du clavier — mouvement fluide.

---

### `key_release_handler` (static)

```c
static int key_release_handler(int key_input, t_data *data)
```

Callback au relâchement de touche. Remet le flag à 0 **uniquement** si c'est bien la même direction qui était active. Exemple : si W est enfoncé (`move_y = 1`), relâcher W met `move_y = 0`. Mais relâcher S (alors que W est enfoncé) **ne remet pas** `move_y` à 0, ce qui permet le maintien correct avec plusieurs touches simultanées.

---

### `listen_input`

```c
void listen_input(t_data *data)
```

Enregistre les 4 hooks MLX :

```c
mlx_hook(win, 2, 1L<<0, &key_press_handling, data);   // event 2  = KeyPress
mlx_hook(win, 3, 1L<<1, &key_release_handler, data);  // event 3  = KeyRelease
mlx_hook(win, 17, 0,    &quit_code, data);             // event 17 = DestroyNotify (fermeture fenêtre)
mlx_loop_hook(data->mlx, &game_loop, data);            // appelé à chaque frame
```

Les numéros `2`, `3`, `17` sont des codes d'événements X11. `1L<<0` et `1L<<1` sont des masques d'événements.

---

## 15. srcs/handling/game_loop.c

### `is_walkable` (static)

```c
static int is_walkable(t_data *data, double x, double y)
```

Vérifie si la position flottante `(x, y)` est une case traversable (`'0'`). Convertit en indices entiers puis accède à `map[y][x]`.

Vérifications de sécurité : `x < 0`, `y < 0`, `map[y] == NULL`, `x >= strlen(map[y])` — toutes les conditions qui causeraient un accès hors bornes.

Retourne 1 si `map[map_y][map_x] == '0'`, 0 sinon (mur, hors carte, etc.).

---

### `move_player` (static)

```c
static void move_player(t_data *data)
```

Calcule et applique le déplacement du joueur.

**Calcul du déplacement :**
```c
step_x = (dir_x * move_y + plane_x * move_x) * MOVE_SPEED;
step_y = (dir_y * move_y + plane_y * move_x) * MOVE_SPEED;
```

- `move_y = 1` (avancer) → déplacement dans la direction `dir` → `step = dir * MOVE_SPEED`
- `move_x = 1` (strafe droit) → déplacement dans la direction `plane` → `step = plane * MOVE_SPEED`
- Les deux peuvent être combinés (diagonale)

**Collision avec offset :**
```c
offset_x = (step_x > 0) ? 0.20 : -0.20;
if (is_walkable(data, pos_x + step_x + offset_x, pos_y))
    pos_x += step_x;
```

L'offset de 0.20 est ajouté dans le sens du déplacement pour tester la collision non pas au centre du joueur mais légèrement devant. Cela crée une "bulle" de collision qui empêche le joueur de coller exactement aux murs.

**Collision séparée X et Y :** on applique X et Y indépendamment. Si le déplacement en X crée une collision mais pas en Y, on applique quand même le Y. C'est le **wall sliding** : le joueur glisse le long des murs plutôt que de s'y bloquer complètement.

---

### `rotate_player` (static)

```c
static void rotate_player(t_data *data)
```

Applique une rotation via la **matrice de rotation 2D** :

```
rot = rotation * ROT_SPEED   (angle en radians, positif = droite, négatif = gauche)

Nouvelle direction :
dir_x' = dir_x * cos(rot) - dir_y * sin(rot)
dir_y' = dir_x * sin(rot) + dir_y * cos(rot)

Nouveau plan caméra :
plane_x' = plane_x * cos(rot) - plane_y * sin(rot)
plane_y' = plane_x * sin(rot) + plane_y * cos(rot)
```

La matrice de rotation préserve :
- La **longueur** des vecteurs (|dir| reste ≈ 1)
- L'**orthogonalité** entre dir et plane
- Le **sens de rotation** (cos/sin avec le bon angle)

Un `old_dir_x` est sauvegardé avant modification car `dir_x` est utilisé pour calculer à la fois le nouveau `dir_x` et le nouveau `dir_y` — sans sauvegarde, le calcul de `dir_y` utiliserait déjà la valeur modifiée.

---

### `update_player`

```c
void update_player(t_data *data)
```

Applique les intentions de mouvement pour le frame courant :
1. Reset `has_moved = false`
2. Si `move_x != 0 || move_y != 0` → `move_player()`, set `has_moved = true`
3. Si `rotation != 0` → `rotate_player()`, set `has_moved = true`

`has_moved` n'est pas utilisé dans le code actuel pour optimiser le rendu, mais son infrastructure est en place.

---

### `game_loop`

```c
int game_loop(t_data *data)
```

Callback appelé par `mlx_loop_hook` à chaque frame. Séquence minimale :
1. `update_player(data)` — traite les inputs
2. `render_frame(data)` — redessine tout

Retourne `0` (requis par MLX pour continuer la boucle).

---

## 16. srcs/render/render.c

### `my_mlx_pixel_put`

```c
void my_mlx_pixel_put(t_img *img, int x, int y, int color)
```

Écrit un pixel dans une image MLX à la position `(x, y)` :

```c
index = y * (img->size_line / 4) + x;
img->addr[index] = color;
```

`size_line` est le nombre d'**octets** par ligne. En divisant par 4 (chaque pixel = 4 octets = int), on obtient le nombre de **pixels** par ligne. L'index dans le tableau d'entiers est alors `y * pixels_par_ligne + x`.

**Pourquoi `size_line` peut différer de `width * 4` ?** MLX peut aligner les lignes sur des multiples de 4 ou 8 octets pour les performances. `size_line` inclut ce padding.

Vérifications de sécurité : si `img` ou `img->addr` est NULL, ou si `(x, y)` est hors bornes → retour silencieux.

---

### `draw_floor_and_ceiling` (static)

```c
static void draw_floor_and_ceiling(t_data *data)
```

Remplit le framebuffer avec deux couleurs unies :
- Moitié supérieure (`y < win_height / 2`) → `ceiling_rgb` (plafond)
- Moitié inférieure (`y >= win_height / 2`) → `floor_rgb` (sol)

Ces couleurs seront **partiellement recouvertes** par les colonnes de murs dessinées ensuite. L'ordre est important : fond d'abord, murs par-dessus.

---

### `render_frame`

```c
void render_frame(t_data *data)
```

Produit et affiche une frame complète :

1. `draw_floor_and_ceiling` — fond couleur unie
2. Boucle `x` de 0 à `win_width - 1` : `render_column(data, x)` — dessine chaque colonne de mur
3. `mlx_put_image_to_window` — **blit** : copie le framebuffer vers la fenêtre visible

Ce schéma est le **double buffering** classique : on dessine dans un buffer caché, puis on l'affiche d'un coup. L'œil ne voit jamais une frame en cours de dessin.

---

## 17. srcs/render/raycasting.c

C'est le cœur du moteur. Pour chaque colonne de pixels, on trace un rayon et on dessine la tranche de mur correspondante.

### `get_map_cell` (static)

```c
static char get_map_cell(t_data *data, int map_x, int map_y)
```

Accès sécurisé à la carte. Toute position invalide (hors bornes, ligne NULL, colonne dépassant la longueur) retourne `'1'` — traité comme un mur. Cela protège le DDA contre les sorties de grille sans nécessiter de vérifications supplémentaires dans `perform_dda`.

---

### `init_ray` (static)

```c
static void init_ray(t_data *data, t_ray *ray, int x)
```

Initialise le rayon pour la colonne `x` :

```c
ray->camera_x = (2.0 * x) / win_width - 1.0;   // -1.0 (gauche) à +1.0 (droite)
ray->dir_x = player.dir_x + player.plane_x * camera_x;
ray->dir_y = player.dir_y + player.plane_y * camera_x;
```

**Explication géométrique :**
- `camera_x = -1` : rayon le plus à gauche de la fenêtre (direction `dir - plane`)
- `camera_x = 0` : rayon central (direction `dir`)
- `camera_x = +1` : rayon le plus à droite (direction `dir + plane`)

Les rayons des colonnes intermédiaires interpolent linéairement entre ces extrêmes, créant un balayage continu de gauche à droite.

```c
if (ray->dir_x == 0.0)  ray->delta_dist_x = 1e30;
else                     ray->delta_dist_x = fabs(1.0 / ray->dir_x);
```

`delta_dist_x` = distance entre deux intersections verticales consécutives (entre les lignes x=0, x=1, x=2...). Si `dir_x = 0`, le rayon est horizontal et ne croise jamais une ligne verticale → `1e30` (infini pratique). `fabs(1/dir_x)` vient de la géométrie de la grille.

---

### `init_step_and_side_dist` (static)

```c
static void init_step_and_side_dist(t_data *data, t_ray *ray)
```

Prépare le DDA en calculant :
- `step_x/step_y` : direction de progression dans la grille (+1 ou -1)
- `side_dist_x/y` : distance jusqu'à la **première** intersection de grille

Pour `step_x = 1` (rayon va à droite) : `side_dist_x = (map_x + 1.0 - pos_x) * delta_dist_x`
→ distance depuis la position actuelle jusqu'au bord droit de la case courante.

Pour `step_x = -1` (rayon va à gauche) : `side_dist_x = (pos_x - map_x) * delta_dist_x`
→ distance jusqu'au bord gauche de la case courante.

---

### `perform_dda` (static)

```c
static void perform_dda(t_data *data, t_ray *ray)
```

L'algorithme DDA (Digital Differential Analyzer) avance case par case jusqu'à toucher un mur.

```c
while (!ray->hit)
{
    if (side_dist_x < side_dist_y)  // La prochaine intersection est verticale
    {
        side_dist_x += delta_dist_x;  // Avancer vers la prochaine ligne verticale
        map_x += step_x;
        side = 0;  // Mur vertical (face Est ou Ouest)
    }
    else  // La prochaine intersection est horizontale
    {
        side_dist_y += delta_dist_y;
        map_y += step_y;
        side = 1;  // Mur horizontal (face Nord ou Sud)
    }
    if (get_map_cell(data, map_x, map_y) != '0')
        ray->hit = 1;  // Mur touché !
}
```

À chaque itération, on choisit **l'axe le plus proche** (smallest `side_dist`). C'est la clé du DDA : on avance toujours vers l'intersection la plus proche, garantissant qu'on ne saute aucune case.

---

### `select_texture` (static)

```c
static t_img *select_texture(t_data *data, t_ray *ray)
```

Choisit la texture selon la **face du mur touchée** :

| Condition | Signification | Texture |
|-----------|--------------|---------|
| `side == 0 && dir_x > 0` | Mur vertical, rayon va à droite → face Ouest du mur | West |
| `side == 0 && dir_x < 0` | Mur vertical, rayon va à gauche → face Est du mur | East |
| `side == 1 && dir_y > 0` | Mur horizontal, rayon va vers le bas → face Nord du mur | North |
| `side == 1 && dir_y < 0` | Mur horizontal, rayon va vers le haut → face Sud du mur | South |

Le choix est basé sur la **direction du rayon**, pas sur la position absolue du mur. Un mur vertical (`side = 0`) frappé par un rayon allant vers la droite → on voit sa face Ouest.

---

### `calculate_line_and_texture` (static)

```c
static void calculate_line_and_texture(t_data *data, t_ray *ray, t_img *tex)
```

Convertit le hit DDA en informations de dessin :

**Distance perpendiculaire (correction fish-eye) :**
```c
// Pour mur vertical (side == 0) :
perp_wall_dist = (map_x - pos_x + (1 - step_x) / 2.0) / dir_x;
```
On utilise la distance **perpendiculaire au plan caméra**, pas la distance euclidienne au point d'impact. La distance euclidienne créerait un effet fish-eye (les murs semblent courbés aux bords de l'écran). La distance perpendiculaire donne un rendu plat correct.

**Hauteur de la colonne :**
```c
line_height = (int)(win_height / perp_wall_dist);
```
Plus le mur est proche (`perp_wall_dist` petit), plus la colonne est haute. Un mur à distance 1 occupe toute la hauteur, à distance 2 occupe la moitié, etc.

**Bornes de dessin :**
```c
draw_start = -line_height/2 + win_height/2;   // Centre vertical, on remonte la moitié
draw_end   =  line_height/2 + win_height/2;   // Centre vertical, on descend la moitié
// Écrêtage si la colonne dépasse la fenêtre
if (draw_start < 0) draw_start = 0;
if (draw_end >= win_height) draw_end = win_height - 1;
```

**Coordonnée X de texture (`tex_x`) :**
```c
wall_x = pos_y + perp_wall_dist * dir_y;  // Position Y exacte d'impact (pour mur vertical)
wall_x -= floor(wall_x);                  // Partie fractionnaire [0.0, 1.0)
tex_x = (int)(wall_x * tex->width);       // Convertit en colonne de texture [0, width-1]
```

Le mirroring (`tex_x = width - tex_x - 1`) selon la direction du rayon évite l'effet de texture retournée.

---

### `render_column`

```c
void render_column(t_data *data, int x)
```

Dessine une colonne verticale complète de mur pour la colonne `x` de l'écran :

1. `init_ray` → `init_step_and_side_dist` → `perform_dda` → trouver le mur
2. `select_texture` → choisir la texture correcte
3. Vérification `tex->addr` (si NULL, la texture n'est pas chargée → retour)
4. `calculate_line_and_texture` → calculer les paramètres de dessin

```c
while (y <= ray.draw_end)
{
    d = y * 256 - win_height * 128 + line_height * 128;
    tex_y = ((d * tex->height) / line_height) / 256;
    // Bornes : tex_y clampé dans [0, tex->height - 1]
    if (tex_y < 0) tex_y = 0;
    if (tex_y >= tex->height) tex_y = tex->height - 1;
    color = tex->addr[tex_y * (tex->size_line / 4) + tex_x];
    my_mlx_pixel_put(&data->frame, x, y, color);
    y++;
}
```

**Calcul de `tex_y` :** La formule avec `d * 256` et les divisions est une façon entière de calculer `tex_y = (y - draw_start) / line_height * tex->height`. L'utilisation de 256 comme facteur évite les calculs flottants tout en maintenant une précision suffisante.

Le **clampage de `tex_y`** (`if (tex_y >= tex->height) tex_y = tex->height - 1`) est essentiel : sans lui, la formule entière peut produire `tex_y = tex->height` exactement à la dernière ligne, causant un accès hors bornes et un SIGSEGV. Ce bug affectait notamment la direction Est et les grandes cartes.

---

## 18. srcs/exit/freeeee.c

### `free_tab`

```c
void free_tab(char **tab)
```

Libère un tableau de chaînes `char **` terminé par NULL :
1. Itère et `free` chaque chaîne `tab[i]`
2. `free` le tableau lui-même

Le `if (!tab) return;` protège contre les appels sur un pointeur NULL (idempotent).

---

### `free_textures_img` (static)

```c
static void free_textures_img(t_data *data)
```

Détruit toutes les images MLX : le framebuffer (`frame`) et les 4 textures. Chaque destruction est conditionnelle (`if (img->img)`). Remet les pointeurs à NULL après destruction pour éviter le double free.

Note : `mlx_destroy_image` libère la mémoire côté MLX. On n'appelle pas `free` dessus car MLX gère son propre allocateur.

---

### `free_strings` (static)

```c
static void free_strings(t_data *data)
```

Libère les 6 chaînes de `t_data` : les 4 chemins de textures et les 2 couleurs. Chaque `free` est conditionnel et suivi d'un `= NULL` (cleanup idempotent).

---

### `free_data`

```c
void free_data(t_data *data)
```

Point d'entrée unique pour la libération complète de `t_data` :
1. `free_textures_img` — images MLX
2. `free_strings` — chaînes allouées
3. `free_tab(data->map)` — tableau de la carte

Pas de `free(data)` car `data` est une variable locale de `main` (allouée sur la pile).

---

## 19. srcs/exit/exit.c

### `quit_code`

```c
int quit_code(t_data *data)
```

Fermeture propre du programme, appelée depuis :
- La touche Echap (`key_press_handling`)
- La fermeture de fenêtre (event `17`)

Séquence :
1. `free_data(data)` — libère toutes les ressources
2. `mlx_destroy_window` — ferme la fenêtre MLX (si `win` et `mlx` non NULL)
3. `exit(0)` — quitte le programme

Le `return (0)` après `exit(0)` ne sera jamais atteint mais est présent pour satisfaire la signature de fonction requise par MLX.

---

## 20. Makefile

### Variables principales

```makefile
NAME = cub3D
SRCS = $(wildcard srcs/*.c) $(wildcard srcs/init/*.c) \
       $(wildcard srcs/parsing/*.c) $(wildcard srcs/exit/*.c) \
       $(wildcard srcs/handling/*.c) $(wildcard srcs/render/*.c)
CFLAGS = -Wall -Werror -Wextra -I includes/ -g
```

`wildcard` trouve automatiquement tous les `.c` dans chaque sous-dossier. Ajouter un nouveau fichier `.c` dans `srcs/init/` le compile automatiquement sans modifier le Makefile.

`-Wall -Werror -Wextra` : tous les warnings sont activés et traités comme des erreurs (norme 42). `-g` ajoute les symboles de débogage pour lldb/gdb.

### Variables MLX

```makefile
MLX_DIR = lib/minilibx_mms_20200219
MLX_LIB = $(MLX_DIR)/libmlx.dylib
MLX_FLAGS = -L $(MLX_DIR) -lmlx
MLX_LOCAL = libmlx.dylib
```

MLX est compilé en bibliothèque dynamique `.dylib` (macOS). Elle doit être présente dans le répertoire courant à l'exécution, d'où le `cp -f $(MLX_LIB) $(MLX_LOCAL)` qui la copie à côté du binaire.

### Cibles

| Cible | Description |
|-------|-------------|
| `all` | Compile tout (par défaut) |
| `libs` | Compile `libft` et `mlx` (appelé par `all`) |
| `clean` | Supprime les `.o` et `libmlx.dylib` local |
| `fclean` | `clean` + suppression du binaire |
| `re` | `fclean` + `all` (recompilation complète) |
| `debug` | Compilation verbeuse avec output explicite |

### Règle de compilation implicite

```makefile
srcs/%.o: srcs/%.c
    @cc $(CFLAGS) -c $< -o $@
    @printf "  ✓ %s\n" $(notdir $<)
```

`$<` = fichier source, `$@` = fichier objet. Le `@` en début de commande supprime l'affichage de la commande elle-même (seul le `printf` s'affiche).

---

## 21. Mathématiques du raycasting

### Le problème à résoudre

On veut afficher une vue 3D d'une grille 2D. Pour chaque colonne de pixels :
1. On lance un rayon depuis la position du joueur
2. On trouve où ce rayon touche un mur
3. On en déduit la hauteur à afficher (mur proche = grand, mur loin = petit)

### L'algorithme DDA en images

```
Joueur à (3.5, 2.5), regarde à droite :

    0   1   2   3   4   5   6
  +---+---+---+---+---+---+---+
0 | 1 | 1 | 1 | 1 | 1 | 1 | 1 |
  +---+---+---+---+---+---+---+
1 | 1 |   |   |   |   |   | 1 |
  +---+---+---+---+---+---+---+
2 | 1 |   |   | P →→→|   | 1 |   P = joueur à (3.5, 2.5)
  +---+---+---+---+---+---+---+       rayon vers la droite
3 | 1 |   |   |   |   |   | 1 |       touche le mur en (6, 2)
  +---+---+---+---+---+---+---+
4 | 1 | 1 | 1 | 1 | 1 | 1 | 1 |
  +---+---+---+---+---+---+---+

perp_wall_dist = 6 - 3.5 = 2.5
line_height = 720 / 2.5 = 288 pixels
```

### Correction fish-eye

Sans correction, les rayons des bords de l'écran parcourent une **plus grande distance** que le rayon central (pour le même mur), ce qui ferait paraître les murs courbés. La correction consiste à utiliser la **distance perpendiculaire** (projection sur le plan caméra) plutôt que la distance euclidienne :

```
Vue de dessus :

          plan caméra
    ←←← [===========] →→→
              ↑
              | dir
              |
            [P]  ← joueur

    Rayon central → dist perpendiculaire = dist euclidienne
    Rayon de bord → dist perpendiculaire < dist euclidienne
```

### Le plan caméra et le FOV

```
Vue de dessus :

       plane = (-0.66, 0)
       ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
      ←←←←←←←←←←←←←←←
       _______________
      |    fenêtre    |
      |_______________|

       dir = (0, -1)    ← regarder vers le Nord

FOV = 2 * arctan(|plane| / |dir|) = 2 * arctan(0.66) ≈ 66°
```

Un FOV de 66° est un compromis : assez large pour voir confortablement, pas si large que la distorsion de perspective soit trop prononcée.

---

*Documentation rédigée à partir du code source de cub3D — roazouan & adamgallot, École 42, 2026.*
