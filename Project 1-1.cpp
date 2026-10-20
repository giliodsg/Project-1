#include <iostream>
#include <string>
#include <cstdlib>

struct Map {
    int ground = 0;
    int water = 1;
    int mountain = 2;
    int enemies = 3;
};

struct hero {
    int hp = 1;
    int id = 0;
};

void InitialiserGrille(int grille[10][20], Map config) {
    for (int Ligne = 0; Ligne < 10; ++Ligne) {
        for (int Colonne = 0; Colonne < 20; ++Colonne) {
            grille[Ligne][Colonne] = config.ground;
        }
    }


    for (int Ligne = 1; Ligne <= 3; ++Ligne) {
        for (int Colonne = 5; Colonne <= 8; ++Colonne) {
            grille[Ligne][Colonne] = config.water;
        }
    }

    grille[2][12] = config.mountain;
    grille[2][13] = config.mountain;
    grille[2][14] = config.mountain;

    grille[6][2] = config.mountain;
    grille[6][3] = config.mountain;
    grille[6][4] = config.mountain;

    grille[6][16] = config.water;
    grille[6][17] = config.water;

    grille[2][2] = config.enemies;
    grille[4][11] = config.enemies;
    grille[6][11] = config.enemies;
    grille[8][8] = config.enemies;
    grille[8][16] = config.enemies;
}

void AfficherGrille(int grille[10][20], hero player, int heroLigne, int heroColonne, Map config) {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif

    for (int Ligne = 0; Ligne < 10; ++Ligne) {
        for (int Colonne = 0; Colonne < 20; ++Colonne) {

            if (Ligne == heroLigne && Colonne == heroColonne) {
                std::cout << 'H';
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
                else if (caseValeur == config.enemies) {
                    std::cout << 'X';
                }
            }
        }
        std::cout << '\n';
    }
}


bool EstTraversable(int grille[10][20], int ligne, int colonne, const Map& config) {
    if (ligne < 0 || ligne >= 10 || colonne < 0 || colonne >= 20) {
        return false;
    }

    int caseValeur = grille[ligne][colonne];
    if (caseValeur == config.water || caseValeur == config.mountain) {
        return false;
    }
    return true;
}


int main() {
    Map config;
    hero player;

    int grille[10][20];
    InitialiserGrille(grille, config);

    int heroLigne = 4;
    int heroColonne = 0;

    bool jeuEnCours = true;

    while (jeuEnCours) {
        AfficherGrille(grille, player, heroLigne, heroColonne, config);

        std::cout << "\nDeplacement (Z-Haut, S-Bas, Q-Gauche, D-Droite, E-Quitter) : ";
        char action;
        std::cin >> action;
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

        if (EstTraversable(grille, nouvelleLigne, nouvelleColonne, config)) {
            heroLigne = nouvelleLigne;
            heroColonne = nouvelleColonne;

            if (grille[heroLigne][heroColonne] == config.enemies) {
                player.hp--;

                if (player.hp <= 0) {
                    AfficherGrille(grille, player, heroLigne, heroColonne, config);
                    std::cout << "\n          GAME OVER           \n";
                    jeuEnCours = false;
                }
            }
        }
       
    }

    return 0;
}