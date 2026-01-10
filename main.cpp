#include "BigProject.h"
#include <iostream>
#include <string>

using namespace std;

int main() {
    string tournamentTitle = "";
    RoundNode* tournament = setupCustomTournament(tournamentTitle);

    int choice;
    bool running = true;

    while (running) {
        displayMenu();
        if (!(cin >> choice)) {
            cin.clear(); cin.ignore(1000, '\n'); continue;
        }
        cin.ignore();

        switch (choice) {
            case 1: displayBracket(tournament, tournamentTitle); break;
            case 2: {
                MatchNode* m = findNextUncompletedMatch(tournament);
                if (m) {
                    cout << "\n--- Current Match: " << m->teamA << " vs " << m->teamB << " ---" << endl;
                    cout << "Score " << m->teamA << ": "; cin >> m->scoreA;
                    cout << "Score " << m->teamB << ": "; cin >> m->scoreB;
                    m->isCompleted = true;
                    resolveAndTeleport(m);
                } else cout << "\nNo ready matches found." << endl;
                break;
            }
            case 3: {
                string name; cout << "Enter team name: "; getline(cin, name);
                displayTeamHistory(tournament, name);
                break;
            }
            case 4: {
                string rName; cout << "New Round Name: "; getline(cin, rName);
                insertLastRound(tournament, rName);
                break;
            }
            case 5: tournament = deleteFirstRound(tournament); break;
            case 6: deleteLastRound(tournament); break;
            case 7: running = false; break;
        }
    }
    cleanupTournament(tournament);
    return 0;
}
