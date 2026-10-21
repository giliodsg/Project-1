#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <random>
#include <cctype>
#include <conio.h> 

struct Map {
    int ground = 0;
    int water = 1;
    int mountain = 2;
};

struct hero {
    int hp = 1;
    int id = 0;
};

struct Position {
    int ligne;
    int colonne;

    bool operator==(const Position& other) const {
        return ligne == other.ligne && colonne == other.colonne;
    }
};

struct Ennemi {
    Position pos;
};

void InitialiserGrilleEtEnnemis(int grille[10][20], Map config, std::vector<Ennemi>& ennemis) {
    for (int Ligne = 0; Ligne < 10; ++Ligne) {
        for (int Colonne = 0; Colonne < 20; ++Colonne) {
            grille[Ligne][Colonne] = config.ground;
        }
    }

    // Eau
    for (int Ligne = 1; Ligne <= 3; ++Ligne) {
        for (int Colonne = 5; Colonne <= 8; ++Colonne) {
            grille[Ligne][Colonne] = config.water;
        }
    }

    // Montagnes
    grille[2][12] = config.mountain;
    grille[2][13] = config.mountain;
    grille[2][14] = config.mountain;

    grille[6][2] = config.mountain;
    grille[6][3] = config.mountain;
    grille[6][4] = config.mountain;

    grille[6][16] = config.water;
    grille[6][17] = config.water;

    // Emplacements initiaux des ennemis
    ennemis = {
        {{2, 2}},
        {{4, 11}},
        {{6, 11}},
        {{8, 8}},
        {{8, 16}}
    };
}

bool EstTraversable(int grille[10][20], int ligne, int colonne, const Map& config) {
    // 1. Sortie de carte
    if (ligne < 0 || ligne >= 10 || colonne < 0 || colonne >= 20) {
        return false;
    }

    // 2. Terrain non franchissable
    int caseValeur = grille[ligne][colonne];
    if (caseValeur == config.water || caseValeur == config.mountain) {
        return false;
    }
    return true;
}

void DeplacerEnnemis(int grille[10][20], std::vector<Ennemi>& ennemis, const Map& config) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> dist(0, 3); 

    for (auto& ennemi : ennemis) {
        int dir = dist(gen);
        int nLigne = ennemi.pos.ligne;
        int nColonne = ennemi.pos.colonne;

        if (dir == 0) nLigne--;
        else if (dir == 1) nLigne++; 
        else if (dir == 2) nColonne--; 
        else if (dir == 3) nColonne++;

        if (EstTraversable(grille, nLigne, nColonne, config)) {
            ennemi.pos.ligne = nLigne;
            ennemi.pos.colonne = nColonne;
        }
    }
}

void AfficherGrille(int grille[10][20], int heroLigne, int heroColonne, const std::vector<Ennemi>& ennemis, Map config) {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif

    for (int Ligne = 0; Ligne < 10; ++Ligne) {
        for (int Colonne = 0; Colonne < 20; ++Colonne) {

            // Priorité de rendu : Héros > Ennemis > Terrain
            if (Ligne == heroLigne && Colonne == heroColonne) {
                std::cout << 'H';
            }
            else {
                bool estEnnemi = false;
                for (const auto& e : ennemis) {
                    if (e.pos.ligne == Ligne && e.pos.colonne == Colonne) {
                        estEnnemi = true;
                        break;
                    }
                }

                if (estEnnemi) {
                    std::cout << 'X';
                }
                else {
                    int caseValeur = grille[Ligne][Colonne];
                    if (caseValeur == config.ground) {
                        std::cout << '.';
                    }
                    else if (caseValeur == config.water) {
                        std::cout << '~';
                    }
                    else if (caseValeur == config.mountain) {
                        std::cout << 'A';
                    }
                }
            }
        }
        std::cout << '\n';
    }
}

bool VerificationCollision(int heroLigne, int heroColonne, const std::vector<Ennemi>& ennemis, hero& player) {
    for (const auto& e : ennemis) {
        if (e.pos.ligne == heroLigne && e.pos.colonne == heroColonne) {
            player.hp--;
            return true;
        }
    }
    return false;
}

int main() {
    Map config;
    hero player;
    std::vector<Ennemi> ennemis;

    char commande = ';';



    int grille[10][20]; 
    InitialiserGrilleEtEnnemis(grille, config, ennemis);

    int heroLigne = 4;
    int heroColonne = 0;

    bool jeuEnCours = true;

    while (jeuEnCours) {
        // Render
        AfficherGrille(grille, heroLigne, heroColonne, ennemis, config);

        // 1. Player Input
        std::cout << "\nDeplacement (Z-Haut, S-Bas, Q-Gauche, D-Droite, E-Quitter) : " << std::flush;

        // On lit directement la touche sans passer par std::cin
        char action = _getch();
        action = std::toupper(action);

        int nouvelleLigne = heroLigne;
        int nouvelleColonne = heroColonne;

        if (action == 'Z') nouvelleLigne--;
        else if (action == 'S') nouvelleLigne++;
        else if (action == 'Q') nouvelleColonne--;
        else if (action == 'D') nouvelleColonne++;
        else if (action == 'E') {
            jeuEnCours = false;
            continue;
        }




        // 2. Hero Update
        if (EstTraversable(grille, nouvelleLigne, nouvelleColonne, config)) {
            heroLigne = nouvelleLigne;
            heroColonne = nouvelleColonne;
        }

        // 3. Collision Check (Après le coup du joueur)
        if (VerificationCollision(heroLigne, heroColonne, ennemis, player)) {
            if (player.hp <= 0) {
                AfficherGrille(grille, heroLigne, heroColonne, ennemis, config);
                std::cout << "\n           GAME OVER           \n";
                jeuEnCours = false;
                continue;
            }
        }

        // 4. Enemy Update (Déplacement aléatoire)
        DeplacerEnnemis(grille, ennemis, config);

        // 5. Collision Check (Après le coup des ennemis)
        if (VerificationCollision(heroLigne, heroColonne, ennemis, player)) {
            if (player.hp <= 0) {
                AfficherGrille(grille, heroLigne, heroColonne, ennemis, config);
                std::cout << "\n        GAME OVER                          \n";
                std::cout << "\n        Va dormir sah c'est mieux pour toi \n";
                jeuEnCours = false;
            }
        }
    }

    return 0;
}