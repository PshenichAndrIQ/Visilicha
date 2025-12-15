#include "hangman.h"
#include <iostream>

using namespace std;

int main() {
    char choice;

    do {
        playGame();

        cout << "\nХотите сыграть еще раз? (д/н): ";
        cin >> choice;
        cin.ignore();

    } while (choice == 'д' || choice == 'Д' || choice == 'y' || choice == 'Y');

    cout << "\nДо свидания!\n";

    return 0;
}
