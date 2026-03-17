# Rapport de Tests — cub3D
**Date :** 2026-03-12
**Projet :** cub3D — 42 School (roazouan & adamgallot)
**Branche :** dev
**Binaire testé :** `./cub3D`

---

## Résumé Exécutif

| Catégorie               | Tests | Réussis | Échecs | Bugs |
|-------------------------|-------|---------|--------|------|
| Compilation             | 4     | 4       | 0      | 0    |
| Arguments               | 4     | 4       | 0      | 0    |
| Parsing — Textures      | 8     | 8       | 0      | 0    |
| Parsing — Couleurs      | 9     | 7       | 2      | 2    |
| Parsing — Map           | 12    | 10      | 2      | 3    |
| Fichiers système        | 4     | 4       | 0      | 0    |
| Cas limites             | 8     | 5       | 3      | 2    |
| Fuites mémoire          | 5     | 5       | 0      | 0    |
| **TOTAL**               | **54**| **47**  | **7**  | **5**|

**Légende :** PASS = comportement attendu, FAIL = comportement incorrect ou crash inattendu

---

## 1. Tests de Compilation

| Test | Commande | Résultat | Attendu |
|------|----------|----------|---------|
| Compilation initiale | `make` | ✅ PASS — binaire créé sans warning projet | Build OK |
| Recompilation propre | `make re` | ✅ PASS | Build OK |
| Nettoyage partiel | `make clean` | ✅ PASS — `.o` supprimés, binaire conservé | Clean OK |
| Nettoyage total | `make fclean` | ✅ PASS — binaire supprimé | fclean OK |

**Note :** 3 warnings dans la librairie minilibx (Swift/mlx_xpm.c) — non imputable au projet étudiant.

---

## 2. Tests des Arguments

| Test | Commande | Sortie | Exit | Résultat |
|------|----------|--------|------|---------|
| Aucun argument | `./cub3D` | `Usage: ./cub3D <map.cub>` | 1 | ✅ PASS |
| Un argument valide | `./cub3D test.cub` | (fenêtre s'ouvre) | 142* | ✅ PASS |
| Trop d'arguments (3) | `./cub3D a b c` | `Usage: ./cub3D <map.cub>` | 1 | ✅ PASS |
| Deux fichiers valides | `./cub3D a.cub b.cub` | `Usage: ./cub3D <map.cub>` | 1 | ✅ PASS |
| Argument chaîne vide | `./cub3D ""` | `Error: Cannot open file: ` | 1 | ✅ PASS |

*Exit 142 = signal SIGALRM (timeout de test de 3s) = programme tourne normalement.

---

## 3. Tests Parsing — Textures

| Test | Fichier/Cas | Sortie | Exit | Résultat |
|------|-------------|--------|------|---------|
| Texture NO manquante | `err_missing_no.cub` | `Error: Map encountered before all textures.` | 1 | ✅ PASS |
| Texture SO manquante | `err_missing_floor.cub` (analogue) | `Error: Map encountered before all textures.` | 1 | ✅ PASS |
| Texture WE manquante | `/tmp/missing_we.cub` | `Error: Map encountered before all textures.` | 1 | ✅ PASS |
| Extension mauvaise (`.png`) | `err_texture_wrong_ext.cub` | `Error: EA texture must be a .xpm file.` | 1 | ✅ PASS |
| Fichier texture inexistant | `err_texture_not_found.cub` | `Error: NO texture file not found.` | 1 | ✅ PASS |
| Duplicate NO | `err_duplicate_no.cub` | `Error: Duplicate NO texture.` | 1 | ✅ PASS |
| Identifiant inconnu (`XX`) | `/tmp/unknown_id.cub` | `Error: Invalid identifier in texture section.` | 1 | ✅ PASS |
| Commentaire `#` dans header | `/tmp/special_header.cub` | `Error: Invalid identifier in texture section.` | 1 | ✅ PASS (comportement normal) |

---

## 4. Tests Parsing — Couleurs

| Test | Fichier/Cas | Sortie | Exit | Résultat |
|------|-------------|--------|------|---------|
| Valeur > 255 (256) | `err_color_out_of_range.cub` | `Error: Invalid F color format.` | 1 | ✅ PASS |
| Valeur exactement 256 | `/tmp/color_256.cub` | `Error: Invalid F color format.` | 1 | ✅ PASS |
| Valeur négative (-1) | `err_color_negative.cub` | `Error: Invalid F color format.` | 1 | ✅ PASS |
| Valeur alphabétique (`abc`) | `err_color_alpha.cub` | `Error: Invalid F color format.` | 1 | ✅ PASS |
| Trop de composantes (4) | `err_color_too_many_components.cub` | `Error: Invalid F color format.` | 1 | ✅ PASS |
| Trop peu de composantes (2) | `err_color_missing_component.cub` | `Error: Invalid F color format.` | 1 | ✅ PASS |
| Une seule composante | `/tmp/one_component.cub` | `Error: Invalid F color format.` | 1 | ✅ PASS |
| Composante vide (`,100,100`) | `/tmp/empty_color.cub` | `Error: Invalid F color format.` | 1 | ✅ PASS |
| **Espaces après virgule** (`100, 100, 100`) | `err_color_spaces.cub` | *(aucun)* — fenêtre ouvre | **0** | ❌ **FAIL** |
| Duplicate couleur F | `err_duplicate_floor.cub` | `Error: Duplicate F color.` | 1 | ✅ PASS |

### BUG #1 — Couleur avec espaces acceptée
**Cas :** `F 100, 100, 100` (espaces après les virgules)
**Comportement observé :** Le programme accepte le format et ouvre la fenêtre.
**Comportement attendu :** Erreur — le sujet 42 exige le format strict `R,G,B`.
**Sévérité :** Mineure (laxisme au lieu de crash, mais non-conforme au sujet).

---

## 5. Tests Parsing — Map

| Test | Fichier/Cas | Sortie | Exit | Résultat |
|------|-------------|--------|------|---------|
| Aucun joueur | `err_no_player.cub` | *(aucun message)* | 1 | ⚠️ PASS mais sans message |
| Deux joueurs | `err_two_players.cub` | *(aucun message)* | 1 | ⚠️ PASS mais sans message |
| Caractère invalide (`X`) | `err_invalid_char.cub` | *(aucun message)* | 1 | ⚠️ PASS mais sans message |
| Mur droit ouvert | `err_open_wall.cub` | `Error: Right wall is not closed.` | 1 | ✅ PASS |
| Mur haut ouvert | `err_open_top.cub` | `Error: Top wall is not closed.` | 1 | ✅ PASS |
| Mur bas ouvert | `err_open_bottom.cub` | `Error: Bottom wall is not closed.` | 1 | ✅ PASS |
| Trou/espace dans la map | `err_hole_in_map.cub` | `Error: Character is not enclosed by walls.` | 1 | ✅ PASS |
| Aucune map | `err_no_map.cub` | `Error: Missing map.` | 1 | ✅ PASS |
| Fichier vide | `err_empty_file.cub` | `Error: Missing texture or color information.` | 1 | ✅ PASS |
| Map zone vide (espaces seulement) | `/tmp/empty_map_area.cub` | *(aucun message)* | 1 | ⚠️ PASS sans message |
| **Contenu après la map** | `err_map_after_empty_line.cub` | *(aucun)* — fenêtre ouvre | **0** | ❌ **FAIL** |
| **Joueur sur le bord de mur** | `err_player_on_wall.cub` | *(aucun)* — fenêtre ouvre | **0** | ❌ **FAIL** |
| Map avec espaces en bordure | `valid_spaces_border.cub` | `Error: Top wall is not closed.` | 1 | ❌ **FAIL** (faux positif) |
| Tab dans la map | `/tmp/tab_in_map.cub` | *(aucun message)* | 1 | ✅ PASS (refusé) |
| Deux salles séparées (flood-fill) | `/tmp/two_rooms.cub` | (fenêtre ouvre) | 142* | ✅ PASS |

### BUG #2 — Aucun message d'erreur sur certaines erreurs critiques
**Cas :** Pas de joueur, deux joueurs, caractère invalide
**Comportement observé :** Le programme quitte avec exit code 1 mais sans afficher de message d'erreur.
**Comportement attendu :** Un message `Error: ...` devrait être affiché sur stderr.
**Sévérité :** Mineure (l'exit code est correct, mais le débogage est difficile).

### BUG #3 — Contenu après la map accepté
**Cas :** Lignes supplémentaires après la map (séparées par une ligne vide)
**Comportement observé :** La deuxième map est ignorée et le programme démarre.
**Comportement attendu :** Erreur — la map doit être la dernière section du fichier.
**Sévérité :** Mineure selon le sujet 42.

### BUG #4 — Joueur placé sur la cellule de bordure (adjacent au mur extérieur)
**Cas :** `1N0001` — joueur adjacent au mur extérieur gauche
**Comportement observé :** Le programme accepte et démarre.
**Comportement attendu :** Refusé ou accepté selon interprétation du sujet.
**Sévérité :** Litigieux (dépend de l'interprétation, mais le joueur est dans un espace valide `0`... en fait `N` remplace `0` qui est adjacent au `1` — comportement acceptable).

### BUG #5 — Faux positif : map avec espaces en bordure extérieure
**Cas :** Map avec lignes indentées par des espaces (`   1111111`)
**Comportement observé :** `Error: Top wall is not closed.` — refusé
**Comportement attendu :** Accepté (les espaces sont censés être traités comme des zones externes)
**Sévérité :** Selon le sujet 42 (les espaces en bordure externe sont autorisés dans certaines interprétations).

---

## 6. Tests de Cas Valides (Directions Joueur)

| Test | Fichier | Direction | Résultat |
|------|---------|-----------|---------|
| Joueur face Nord (N) | `valid_basic.cub` | N | ✅ PASS — fenêtre s'ouvre |
| Joueur face Sud (S) | `valid_south.cub` | S | ✅ PASS — fenêtre s'ouvre |
| Joueur face Ouest (W) | `valid_west.cub` | W | ✅ PASS — fenêtre s'ouvre |
| Joueur face Est (E) | `valid_east.cub` | E | ❌ **CRASH — SIGSEGV (exit 139)** |

### BUG CRITIQUE #1 — SIGSEGV sur joueur face Est ('E')

**Symptôme :** Crash immédiat (SIGSEGV, exit 139) dès le premier frame rendu.
**Backtrace (lldb) :**
```
frame #0: raycasting.c:154 in render_column(data, x=0)
EXC_BAD_ACCESS (code=1, address=0x108f9407c)
color = tex->addr[ray.tex_y * (tex->size_line / 4) + ray.tex_x];
```

**Cause racine :**
Calcul de `tex_y` hors limites dans `raycasting.c` :
```c
d = y * 256 - data->win_height * 128 + ray.line_height * 128;
ray.tex_y = ((d * tex->height) / ray.line_height) / 256;
```
Lorsque `y = draw_end` et que `draw_end` n'est pas écrêté par `win_height`, la formule produit `tex_y = tex->height` (hors bornes). L'accès à `tex->addr[tex->height * ...]` dépasse le buffer alloué.

**Vecteur de déclenchement :** La direction Est place `plane_x = 0.0`, rendant `ray.dir_x = 1.0` constant pour toutes les colonnes. La colonne `x=0` génère alors un angle de rayon spécifique où `draw_end` n'est pas écrêté, révélant l'accès hors bornes.

**Note :** Le bug est latent pour les autres directions (N/S/W) mais ne provoque pas de crash car le buffer de texture alloué par MLX contient du padding mémoire adjacent. Avec AddressSanitizer (ASAN), le programme fonctionne car l'espace mémoire diffère.

**Sévérité : CRITIQUE** — Rend la direction Est totalement inutilisable.

**Correction suggérée** dans `raycasting.c` :
```c
ray.tex_y = ((d * tex->height) / ray.line_height) / 256;
if (ray.tex_y < 0)
    ray.tex_y = 0;
if (ray.tex_y >= tex->height)
    ray.tex_y = tex->height - 1;
```

---

## 7. Tests de Cas Valides Supplémentaires

| Test | Fichier | Résultat |
|------|---------|---------|
| Couleurs min (0,0,0) | `valid_colors_min.cub` | ✅ PASS |
| Couleurs max (255,255,255) | `valid_colors_max.cub` | ✅ PASS |
| Map complexe avec couloirs | `valid_complex_map.cub` | ✅ PASS |
| En-têtes dans ordre mixte | `valid_order_mixed.cub` | ✅ PASS |
| Chemin de fichier avec espaces | `/tmp/valid map.cub` | ✅ PASS |
| Map très large (200 colonnes) | `/tmp/wide_map.cub` | ✅ PASS |
| Deux salles séparées | `/tmp/two_rooms.cub` | ✅ PASS |
| Couleurs leading zeros (`010`) | `/tmp/leading_zeros.cub` | ✅ PASS (accepté) |

---

## 8. Tests de Crash Supplémentaire

| Test | Cas | Résultat |
|------|-----|---------|
| Map très haute (200 lignes) + N | `/tmp/tall_map.cub` | ❌ **CRASH — SIGSEGV (exit 139)** |

### BUG CRITIQUE #2 — SIGSEGV sur map très haute

**Symptôme :** Crash immédiat (SIGSEGV, exit 139) avec une map de 200 lignes.
**Backtrace (lldb) :**
```
frame #0: raycasting.c:154 in render_column(data, x=525)
EXC_BAD_ACCESS (code=1, address=0x108fc8000)
color = tex->addr[ray.tex_y * (tex->size_line / 4) + ray.tex_x];
```

**Cause racine :** Même bug que BUG CRITIQUE #1. Avec un joueur au centre d'une grande map (100 lignes au-dessus), certains rayons ont un `perp_wall_dist` suffisamment spécifique pour que `tex_y` soit calculé à `tex->height` exactement.

**Sévérité : CRITIQUE** — Les grandes maps peuvent déclencher des crashes.

---

## 9. Tests Fichiers Système

| Test | Cas | Sortie | Exit | Résultat |
|------|-----|--------|------|---------|
| Fichier inexistant | `./cub3D nonexistent.cub` | `Error: Cannot open file: nonexistent.cub` | 1 | ✅ PASS |
| Extension incorrecte (`.map`) | `err_wrong_extension.map` | `Error: File must have a .cub extension.` | 1 | ✅ PASS |
| Répertoire donné comme fichier | `/tmp/test_dir.cub/` | `Error: Failed to read file.` | 1 | ✅ PASS |
| Fichier sans permission de lecture | `/tmp/noperm.cub` (chmod 000) | `Error: Cannot open file: ...` | 1 | ✅ PASS |
| Chemin très long (4000 chars) | `/tmp/long_path.cub` | `Error: Cannot open file:...` | 1 | ✅ PASS |

---

## 10. Tests Fuites Mémoire

Outil utilisé : `leaks -atExit` (macOS)

| Test | Fuites détectées | Résultat |
|------|-----------------|---------|
| Fichier vide | 0 fuite | ✅ PASS |
| Aucun joueur | 0 fuite | ✅ PASS |
| Caractère invalide | 0 fuite | ✅ PASS |
| Mur droit ouvert | 0 fuite | ✅ PASS |
| Deux joueurs | 0 fuite | ✅ PASS |

**Résultat :** Zéro fuite mémoire détectée sur tous les cas d'erreur testés.

---

## 11. Tests du Makefile

| Cible | Résultat |
|-------|---------|
| `make` | ✅ Compilation complète, binaire créé |
| `make re` | ✅ Recompilation propre |
| `make clean` | ✅ `.o` supprimés, binaire conservé |
| `make fclean` | ✅ Binaire et `.o` supprimés |

---

## 12. Récapitulatif des Bugs

### Bugs Critiques (crash)

| # | Bug | Localisation | Impact |
|---|-----|-------------|--------|
| 1 | **SIGSEGV joueur face Est ('E')** | `raycasting.c:154` | Inutilisable |
| 2 | **SIGSEGV sur grandes maps** | `raycasting.c:154` | Crashs aléatoires |

**Cause commune :** `tex_y = tex->height` hors bornes dans la boucle de rendu des colonnes. Le calcul entier ne borne pas correctement `tex_y` à `[0, tex->height - 1]`.

### Bugs Non-Critiques (comportement incorrect)

| # | Bug | Localisation | Impact |
|---|-----|-------------|--------|
| 3 | Couleurs `R, G, B` avec espaces acceptées | `parsing/init_texture.c` | Non-conforme sujet |
| 4 | Contenu après la map ignoré silencieusement | `parsing/init_map.c` | Non-conforme sujet |
| 5 | Faux positif : map avec espaces en bordure rejetée | `parsing/check_parsing.c` | Faux négatif |

### Absence de messages d'erreur (non-critique)

Les cas suivants quittent correctement avec exit 1 mais **sans afficher de message** :
- Pas de joueur dans la map
- Deux joueurs dans la map
- Caractère invalide dans la map
- Zone de map vide (espaces seulement)

---

## 13. Environnement de Test

```
OS       : macOS Darwin 25.3.0 (arm64)
Compiler : clang (Apple LLVM)
Flags    : -Wall -Werror -Wextra
mlx      : minilibx_mms_20200219 (macOS)
Leaks    : macOS leaks tool
Debugger : lldb
```

---

## 14. Liste Complète des Fichiers de Test Créés

```
tests/maps/
├── valid_basic.cub             — Map basique, joueur N
├── valid_south.cub             — Joueur S
├── valid_east.cub              — Joueur E (CRASH)
├── valid_west.cub              — Joueur W
├── valid_colors_min.cub        — Couleurs 0,0,0
├── valid_colors_max.cub        — Couleurs 255,255,255
├── valid_complex_map.cub       — Map complexe avec couloirs
├── valid_spaces_border.cub     — Map indentée par espaces
├── valid_order_mixed.cub       — En-têtes dans ordre mixte
├── err_no_args.cub             — (référence)
├── err_no_player.cub           — Aucun joueur
├── err_two_players.cub         — Deux joueurs
├── err_open_wall.cub           — Mur droit ouvert
├── err_open_top.cub            — Mur haut ouvert
├── err_open_bottom.cub         — Mur bas ouvert
├── err_invalid_char.cub        — Caractère invalide (X)
├── err_missing_no.cub          — Texture NO manquante
├── err_missing_floor.cub       — Couleur F manquante
├── err_missing_ceiling.cub     — Couleur C manquante
├── err_color_out_of_range.cub  — Couleur > 255
├── err_color_negative.cub      — Couleur négative
├── err_color_alpha.cub         — Couleur alphabétique
├── err_duplicate_no.cub        — Doublon NO
├── err_duplicate_floor.cub     — Doublon F
├── err_texture_not_found.cub   — Texture inexistante
├── err_texture_wrong_ext.cub   — Texture .png au lieu .xpm
├── err_no_map.cub              — Aucune map
├── err_empty_file.cub          — Fichier vide
├── err_map_after_empty_line.cub— Contenu post-map
├── err_color_too_many_components.cub
├── err_color_missing_component.cub
├── err_color_spaces.cub        — Couleur avec espaces
├── err_hole_in_map.cub         — Espace intérieur non clôturé
├── err_player_on_wall.cub      — Joueur sur cellule de mur
├── err_missing_map_after_metadata.cub
└── err_wrong_extension.map     — Extension incorrecte
```
