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
    int id = 9;
};

void initialiserGrille(int grille[10][20], Map config) {
    for (int ligne = 0; ligne < 10; ++ligne) {
        for (int colonne = 0; colonne < 20; ++colonne) {
            grille[ligne][colonne] = config.ground;
        }
    }

    for (int ligne = 1; ligne <= 3; ++ligne) {
        for (int colonne = 4; colonne <= 7; ++colonne) {
            grille[ligne][colonne] = config.water;
        }
    }

    grille[3][12] = config.mountain;
    grille[3][13] = config.mountain;
    grille[3][14] = config.mountain;

    grille[7][2] = config.mountain;
    grille[7][3] = config.mountain;
    grille[7][4] = config.mountain;

    grille[6][16] = config.water;
    grille[6][17] = config.water;
}

void afficherGrille(int grille[10][20], hero player, int heroLigne, int heroColonne, Map config) {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif

    for (int ligne = 0; ligne < 10; ++ligne) {
        for (int colonne = 0; colonne < 20; ++colonne) {

            if (ligne == heroLigne && colonne == heroColonne) {
                std::cout << 'H';
            }
            else {
                int caseValeur = grille[ligne][colonne];

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

int main() {
    Map config;
    hero player;

    int grille[10][20];
    initialiserGrille(grille, config);

    int heroLigne = 4;
    int heroColonne = 0;

    bool jeuEnCours = true;

    while (jeuEnCours) {
        afficherGrille(grille, player, heroLigne, heroColonne, config);

        std::cout << "\nDeplacement (Z=Haut, S=Bas, Q=Gauche, D=Droite, X=Quitter) : ";
        char action;
        std::cin >> action;
        action = std::toupper(action);

        int nouvelleLigne = heroLigne;
        int nouvelleColonne = heroColonne;

        if (action == 'Z') nouvelleLigne--;
        else if (action == 'S') nouvelleLigne++;
        else if (action == 'Q') nouvelleColonne--;
        else if (action == 'D') nouvelleColonne++;
        else if (action == 'X') {
            jeuEnCours = false;
            continue;
        }

        if (nouvelleLigne >= 0 && nouvelleLigne < 10 && nouvelleColonne >= 0 && nouvelleColonne < 20) {
            heroLigne = nouvelleLigne;
            heroColonne = nouvelleColonne;
        }
    }

    return 0;
}