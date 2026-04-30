# Solveur Tapa
Solveur du jeu de puzzle Tapa

## Compilation
make

## Utilisation
./TapaSolver <flags> <fichier>

### Flags
| Flag | Règle activée |
| `2`  | Carrés 2x2 |
| `i`  | Indices/clues |
| `c`  | Continuité des cases noires |

Les flags peuvent être combinés. Par exemple, pour activer toutes les règles :
./TapaSolver 2ic tests/ma_grille

Note : les indices sont toujours partiellement actifs (cases blanches/noires forcées),
le flag `i` active en plus les contraintes de voisinage.

## Tests

Un script de test automatique est fourni. Il parcourt le dossier `JeuTests/` qui contient :
- `OK/` : grilles qui doivent être **SAT**
- `ERREUR/` : grilles qui doivent être **UNSAT**

Les flags sont déduits automatiquement depuis le nom du sous-dossier (`2x2`, `indices`, `continuite`).
Pour lancer les tests :

```bash
./run_tests.sh
```
(`chmod +x run_tests.sh` aura déjà été lancé en amont lors de l'instalation cf. INSTALL.TXT)

Chaque test affiche `PASS` ou `FAIL` avec le résultat obtenu et attendu.



Vous pourrez trouver le compte rendu en .pdf et en .html le compte rendu ayant été rédigé en markdown.
