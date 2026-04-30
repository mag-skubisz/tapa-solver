#!/bin/bash

make

# Déduit les flags à activer depuis le nom du dossier de règle
flags_depuis_dossier() {
    local name=$1
    local flags=""
    [[ "$name" == *"2x2"*        ]] && flags+="2"
    [[ "$name" == *"indices"*    ]] && flags+="i"
    [[ "$name" == *"continuite"* ]] && flags+="c"
    echo "$flags"
}

# Teste tous les fichiers d'un dossier et compare le résultat attendu
tester_dossier() {
    local dir=${1%/} flags=$2 attendu=$3
    for f in "$dir"/*; do
        # Lance le solveur et capture la sortie
        result=$(./TapaSolver "$flags" "$f" 2>/dev/null)
        # Détecte SAT ou UNSAT dans la sortie
        if echo "$result" | grep -q "^SAT"; then got="SAT"; else got="UNSAT"; fi
        # Compare avec le résultat attendu
        if [ "$got" = "$attendu" ]; then
            echo -e "\033[32mPASS\033[0m [$attendu] $f"
        else
            echo -e "\033[31mFAIL\033[0m [attendu $attendu, obtenu $got] $f"
        fi
    done
}

# Parcourt OK (attendu SAT) et ERREUR (attendu UNSAT)
for category in OK ERREUR; do
    [ "$category" = "OK" ] && attendu="SAT" || attendu="UNSAT"
    for rule_dir in JeuTests/"$category"/*/; do
        rule=$(basename "$rule_dir")
        flags=$(flags_depuis_dossier "$rule")
        echo "--- $category/$rule [flags: '$flags'] ---"
        tester_dossier "$rule_dir" "$flags" "$attendu"
    done
done