# Modifications apportées pour la conformité à la Norme 42

## Nouveaux fichiers créés

### `srcs/parsing/parse_line.c`
Extrait de `init_texture.c` qui en avait 13 fonctions (max 5).
Contient : `ft_strjoin_char` (static), `read_line`, `has_suffix`.

**Modification logique :** `ft_strjoin_char` ajoute désormais un guard `if (str)` avant
d'appeler `ft_strlen`, pour éviter un comportement indéfini quand `str` est NULL
(cas du premier appel dans `read_line` où `line` vaut NULL).

### `srcs/parsing/parse_texture.c`
Extrait de `init_texture.c`.
Contient : `skip_spaces`, `is_map_line`, `parse_texture_line`, `process_texture_line`,
`init_textures`.

**Modification logique — `process_texture_line` (nouvelle fonction) :**
Le corps de la boucle `while (i < 6)` dans `init_textures` a été extrait dans cette
fonction helper pour ramener `init_textures` sous 25 lignes. La logique est identique :
lire une ligne, la trimmer, l'ignorer si vide, l'identifier comme texture/couleur ou
erreur. `*i` est passé par pointeur pour permettre l'incrémentation depuis l'intérieur.

### `srcs/init/parse_color.c`
Extrait de `data.c` qui en avait 8 fonctions (max 5).
Contient : `parse_color_component` (static), `parse_rgb_color` (static),
`copy_parsing_to_data`.
Logique inchangée.

### `srcs/render/dda.c`
Extrait de `raycasting.c` qui en avait 7 fonctions (max 5).
Contient : `get_map_cell` (static), `init_ray`, `init_step_and_side_dist`,
`perform_dda`.
Logique inchangée. Ces trois fonctions sont désormais non-statiques et déclarées dans
`cube3d.h` pour être accessibles depuis `raycasting.c`.

---

## Fichiers modifiés

### `srcs/parsing/init_texture.c`
**Avant :** 13 fonctions, indentation espaces, `exit_error` (4 params),
`set_texture`/`set_color` (6 params chacun).

**Après :** 5 fonctions, indentation tabulations.

**Modification logique — `set_texture` et `set_color` (6 params → 4) :**
Les paramètres `int fd` et `char *trimmed` ont été supprimés. Ces paramètres
servaient uniquement à être passés à `exit_error` pour fermer le fd et libérer
`trimmed` avant de quitter. Comme on appelle `exit()` dans tous les cas d'erreur,
l'OS se charge de fermer les fd et libérer la mémoire. La logique d'erreur est
désormais inline : `printf(...) + free_parsing(parsing) + exit(ERRORS)`.

**Modification logique — `exit_error` supprimée :**
Cette fonction helper de 4 params n'est plus nécessaire après la refactorisation
de `set_texture` et `set_color`. Sa logique a été inlinée aux quelques endroits
où elle était appelée.

### `srcs/parsing/check_parsing.c`

**Modification logique — `check_wrong_char` (29 lignes → 23) :**
Les deux conditions `if` imbriquées qui checkaient chaque caractère valide
(`map[i][j] != '1' && map[i][j] != '0' && ...`) ont été remplacées par des appels
à `ft_strchr("01 NSEW", map[i][j])` et `ft_strchr("NSEW", map[i][j])`.
Le comportement est identique : même liste de caractères valides et mêmes positions
de spawn détectées.

**Modification logique — `check_walls` (> 25 lignes → 21) :**
Les blocs `if { printf; free_parsing; exit; }` ont été remplacés par le pattern
`(printf(...), free_parsing(parsing), exit(ERRORS))` (opérateur virgule).
Même comportement, moins de lignes.

### `srcs/render/raycasting.c`

**Modification logique — `get_tex_color` (nouvelle fonction) :**
Le calcul de couleur de texture à l'intérieur de la boucle `while` de `render_column`
a été extrait dans cette fonction pour ramener `render_column` sous 25 lignes.
La variable locale `d` a été déplacée dans `get_tex_color`. `tex_y` remplace
`ray->tex_y` (lecture seule dans le loop, la valeur n'était pas réutilisée après).
Comportement identique.

### `srcs/init/data.c`
`parse_color_component`, `parse_rgb_color`, `copy_parsing_to_data` déplacées vers
`parse_color.c`. Logique inchangée.

### `includes/cube3d.h`
- Includes système : `"stdio.h"` → `<stdio.h>` etc.
- `"string.h"` supprimé (non utilisé dans le projet).
- Nouveaux prototypes ajoutés : `set_texture`, `set_color`, `init_ray`,
  `init_step_and_side_dist`, `perform_dda`.

### `includes/handler.h`
- Guard `KEYS_H` → `HANDLER_H` (la norme impose que le guard corresponde au nom
  du fichier : `handler.h` → `HANDLER_H`).

### `srcs/parsing/check_map.c`
Indentation espaces → tabulations. Logique inchangée.

### `srcs/parsing/utils_pars.c`
Ajout de la tabulation manquante entre type de retour et nom de fonction pour
`ft_arraylen` et `ft_arraydup`.

### `srcs/handling/input_handling.c`
Suppression du trailing tab après `data->player.move_y = 1;` (ligne 21).

### `Makefile`
Remplacement de `$(wildcard srcs/**/*.c)` par une liste explicite de tous les
fichiers sources. La norme exige que les sources soient citées explicitement.
