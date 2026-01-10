#include "BigProject.h"
#include <iostream>
#include <limits>

using namespace std;

bool isPowerOfTwo(int n) {
    if (n <= 1) return false;
    return (n & (n - 1)) == 0;
}

bool isRoundEmpty(RoundNode* r) {
    if (r == nullptr) return true;
    MatchNode* curr = r->headMatch;
    while (curr != nullptr) {
        if (curr->teamA != "" && curr->teamA != "TBD") return false;
        if (curr->teamB != "" && curr->teamB != "TBD") return false;
        if (curr->isCompleted) return false;
        curr = curr->nextInRound;
    }
    return true;
}

void resolveAndTeleport(MatchNode* match) {
    if (!match) return;

    if (match->scoreA == match->scoreB) {
        cout << "No ties.." << endl;
        match->isCompleted = false;
        return;
    }

    match->winner = (match->scoreA > match->scoreB) ? match->teamA : match->teamB;

    if (match->targetMatch != nullptr) {
        if (match->targetMatch->teamA == "" || match->targetMatch->teamA == "TBD") {
            match->targetMatch->teamA = match->winner;
        } else {
            match->targetMatch->teamB = match->winner;
        }
        cout << "\n>>> NOTE: " << match->winner << " Moves to the next round!" << endl;
    } else {
        cout << "\n----------------------------------------------" << endl;
        cout << "   IT IS FINISHED " << endl;
        cout << "   CHAMPION: " << match->winner << endl;
        cout << "----------------------------------------------" << endl;
    }
}

RoundNode* setupCustomTournament(string &title) {
    int numTeams = 0;
    while (true) {
        cout << "\n--- Tournament Initialization ---" << endl;
        cout << "Enter Tournament Name: ";
        getline(cin, title);

        cout << "Enter number of teams (Note : Must be the power of 2): ";
        if (!(cin >> numTeams)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (isPowerOfTwo(numTeams)) break;
        else cout << "Error: Not a power of 2. Try again." << endl;
    }

    vector<string> teams;
    for (int i = 0; i < numTeams; i++) {
        string name;
        cout << "Team " << i + 1 << ": ";
        getline(cin, name);
        teams.push_back(name.empty() ? "Team " + to_string(i + 1) : name);
    }

    RoundNode* head = new RoundNode();
    head->roundName = "Round 1";
    for (int i = 0; i < numTeams; i += 2) addMatchToRound(head, teams[i], teams[i + 1]);

    RoundNode* currR = head;
    int matchesNeeded = numTeams / 4;
    int roundCounter = 2;
    while (matchesNeeded >= 1) {
        RoundNode* nextR = new RoundNode();
        nextR->roundName = "Round " + to_string(roundCounter++);
        for (int i = 0; i < matchesNeeded; i++) addMatchToRound(nextR, "TBD", "TBD");

        MatchNode* pM = currR->headMatch;
        MatchNode* nM = nextR->headMatch;
        while (pM && nM) {
            pM->targetMatch = nM;
            if (pM->nextInRound) {
                pM->nextInRound->targetMatch = nM;
                pM = pM->nextInRound;
            }
            pM = pM->nextInRound;
            nM = nM->nextInRound;
        }
        currR->nextRound = nextR;
        currR = nextR;
        matchesNeeded /= 2;
    }
    return head;
}

RoundNode* deleteFirstRound(RoundNode* head) {
    if (!head) return nullptr;

    bool allFinished = true;
    MatchNode* tempM = head->headMatch;
    while (tempM) {
        if (!tempM->isCompleted) {
            allFinished = false;
            break;
        }
        tempM = tempM->nextInRound;
    }

    if (!allFinished) {
        cout << "\n Cannot delete Round 1 because matches are not yet finished!" << endl;
        return head;
    }

    MatchNode* m = head->headMatch;
    while(m) { MatchNode* t = m; m = m->nextInRound; delete t; }
    RoundNode* next = head->nextRound;
    delete head;
    cout << "First round deleted successfully." << endl;
    return next;
}

void deleteLastRound(RoundNode* head) {
    if (!head || !head->nextRound) {
        cout << "\n Cannot delete the only existing round" << endl;
        return;
    }
    RoundNode* prev = head;
    while (prev->nextRound->nextRound) prev = prev->nextRound;

    if (!isRoundEmpty(prev->nextRound)) {
        cout << "\n Cannot delete: Round is not empty!" << endl;
        return;
    }

    MatchNode* m = prev->headMatch;
    while (m) { m->targetMatch = nullptr; m = m->nextInRound; }

    MatchNode* lm = prev->nextRound->headMatch;
    while (lm) { MatchNode* t = lm; lm = lm->nextInRound; delete t; }
    delete prev->nextRound;
    prev->nextRound = nullptr;
    cout << "Last round deleted." << endl;
}

void displayBracket(RoundNode* headRound, string title) {
    cout << "\n-------------------------\n" << title << "\n-------------------------" << endl;
    RoundNode* currR = headRound;
    while (currR) {
        cout << "[" << currR->roundName << "]: ";
        MatchNode* currM = currR->headMatch;
        while (currM) {
            cout << "(" << (currM->teamA == "" ? "TBD" : currM->teamA) << " vs "
                 << (currM->teamB == "" ? "TBD" : currM->teamB) << ") ";
            currM = currM->nextInRound;
        }
        cout << endl;
        currR = currR->nextRound;
    }
}

void insertLastRound(RoundNode* head, string name) {
    if (!head) return;
    RoundNode* curr = head;
    while (curr->nextRound) {
        curr = curr->nextRound;
    }

    int matchCount = 0;
    MatchNode* mCount = curr->headMatch;
    while (mCount) {
        matchCount++;
        mCount = mCount->nextInRound;
    }

    if (matchCount <= 1) {
        cout << "\n Cannot add a new round! The current last round ("
             << curr->roundName << ") is already the Finals" << endl;
        return;
    }

    RoundNode* nr = new RoundNode();
    nr->roundName = name;
    addMatchToRound(nr, "TBD", "TBD");
    MatchNode* m = curr->headMatch;
    while (m) {
        m->targetMatch = nr->headMatch;
        m = m->nextInRound;
    }

    curr->nextRound = nr;
    cout << "\nSuccessfully added new round: " << name << endl;
}

void displayTeamHistory(RoundNode* head, string name) {
    cout << "\nHistory for team: " << name << endl;
    bool found = false;
    while (head) {
        MatchNode* m = head->headMatch;
        while (m) {
            if (m->teamA == name || m->teamB == name) {
                found = true;
                cout << head->roundName << ": vs " << (m->teamA == name ? m->teamB : m->teamA);
                cout << " | Status: " << (m->isCompleted ? (m->winner == name ? "WINNER" : "LOSER") : "Waiting") << endl;
            }
            m = m->nextInRound;
        }
        head = head->nextRound;
    }
    if (!found) cout << "No matches found for this team..." << endl;
}

void addMatchToRound(RoundNode* r, string a, string b) {
    MatchNode* nm = new MatchNode(); nm->teamA = a; nm->teamB = b;
    if (!r->headMatch) r->headMatch = nm;
    else { MatchNode* c = r->headMatch; while(c->nextInRound) c = c->nextInRound; c->nextInRound = nm; }
}

MatchNode* findNextUncompletedMatch(RoundNode* h) {
    while(h) {
        MatchNode* m = h->headMatch;
        while(m) {
            if(!m->isCompleted && m->teamA != "TBD" && m->teamB != "TBD" && m->teamA != "" && m->teamB != "")
                return m;
            m = m->nextInRound;
        }
        h = h->nextRound;
    }
    return nullptr;
}

void cleanupTournament(RoundNode* h) {
    while(h) {
        MatchNode* m = h->headMatch;
        while(m) { MatchNode* t = m; m = m->nextInRound; delete t; }
        RoundNode* t = h; h = h->nextRound; delete t;
    }
}

void displayMenu() {
    cout << "\n1. Display Bracket\n2. Input Score\n3. Team History Search\n4. Add New Final Round\n5. Delete First Round \n6. Delete Last Round \n7. Exit\nChoice: ";
}
