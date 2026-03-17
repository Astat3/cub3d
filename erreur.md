# Rapport d'erreurs Cub3D

## 1. Compilation
- [x] `make re` : **SUCCÈS**
- [ ] Warnings : Aucun warning dans le code source du projet (warnings présents uniquement dans la minilibx, ce qui est attendu).

## 2. Tests d'arguments
- [x] `./cub3D` (sans arguments) : **SUCCÈS** (Affiche Usage, exit code 1)
- [x] `./cub3D file_not_exist.cub` : **SUCCÈS** (Affiche Error, exit code 1)
- [x] `./cub3D map.txt` (mauvaise extension) : **SUCCÈS** (Affiche Error, exit code 1)

## 3. Parsing & Validité de la map
- [x] Textures manquantes : **SUCCÈS** (Affiche "Error: Map encountered before all textures", exit code 1)
- [x] Couleurs invalides : **SUCCÈS** (Affiche "Error: Invalid F color format", exit code 1)
- [x] Map ouverte (Flood Fill) : **SUCCÈS** (Affiche "Error: Right wall is not closed", exit code 1)
- [x] Personnages multiples : **ÉCHEC PARTIEL** (Exit code 1 correct, mais **AUCUN MESSAGE D'ERREUR** affiché)
- [x] Personnage manquant : **ÉCHEC PARTIEL** (Exit code 1 correct, mais **AUCUN MESSAGE D'ERREUR** affiché)
- [x] Caractères invalides : **ÉCHEC PARTIEL** (Exit code 1 correct, mais **AUCUN MESSAGE D'ERREUR** affiché)

## 4. Fuites de mémoire (Leaks)
- [x] Sur erreur de parsing : **SUCCÈS** (0 leaks détectés)
- [x] En exécution normale (arrêt propre) : **SUCCÈS** (0 leaks détectés)

# Résumé
Le projet compile parfaitement et gère correctement la mémoire. La gestion des erreurs est en place et empêche les crashs (seganultz), cependant **certaines erreurs critiques (manque de joueur, trop de joueurs, caractères invalides) n'affichent pas de message expliquant le problème**, même si le programme quitte bien avec un code d'erreur. Il serait bon d'ajouter des `printf` ou `write` pour ces cas spécifiques.
