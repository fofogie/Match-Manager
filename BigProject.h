#ifndef BIGPROJECT_H_INCLUDED
#define BIGPROJECT_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct MatchNode {
    string teamA = "";
    string teamB = "";
    int scoreA = 0;
    int scoreB = 0;
    string winner = "";
    bool isCompleted = false;
    MatchNode* nextInRound = nullptr;
    MatchNode* targetMatch = nullptr;
};

struct RoundNode {
    string roundName;
    RoundNode* nextRound = nullptr;
    MatchNode* headMatch = nullptr;
};

void displayBracket(RoundNode* headRound, string title);
void resolveAndTeleport(MatchNode* match);
void insertLastRound(RoundNode* currentHead, string name);
void displayTeamHistory(RoundNode* head, string teamName);
MatchNode* findNextUncompletedMatch(RoundNode* head);

bool isRoundEmpty(RoundNode* r);
RoundNode* deleteFirstRound(RoundNode* currentHead);
void deleteLastRound(RoundNode* currentHead);

bool isPowerOfTwo(int n);
void addMatchToRound(RoundNode* round, string teamA, string teamB);
void displayMenu();
RoundNode* setupCustomTournament(string &title);
void cleanupTournament(RoundNode* head);

#endif
