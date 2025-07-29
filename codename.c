#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PLAYERS 100
#define MAX_CURRENT_PLAYERS 50
#define MAX_TEAM_SIZE 25
#define NUMBER_OF_CODENAMES 25
#define WORD_LENGTH 36
#define ROWS 5
#define COLUMNS 5

typedef char Str36[WORD_LENGTH];

typedef struct 
{
    Str36 username;
    int gamesPlayed;
    int wins;
    int spyMasterWins;
    int agentWins;
} Player;

void debugCode (int blueCards, int redCards, int bluePoints, int redPoints, Str36 codenames[])
{
    printf("Debugging code...\n");
    printf("Blue Cards: %d\n", blueCards);
    printf("Red Cards: %d\n", redCards);
    printf("Blue Points: %d\n", bluePoints);
    printf("Red Points: %d\n", redPoints);
    // printf("Codenames:\n");
    // for(int i = 0; i < NUMBER_OF_CODENAMES; i++)
    // {
    //     printf("\t- %s\n", codenames[i]);
    // }
}

void updatePlayers(Player playerList[])
{
    FILE *file = fopen("players.txt", "w");
    if(file == NULL)
    {
        printf("Error opening players file.\n");
    }
    else
    {
        for(int i = 0; i < MAX_PLAYERS; i++)
        {
            if(playerList[i].username[0] != '\0')
            {
                fprintf(file, "%s %d %d %d %d\n", playerList[i].username, playerList[i].gamesPlayed, playerList[i].wins, playerList[i].spyMasterWins, playerList[i].agentWins);
            }
        }
        
        fclose(file);
    }
}

void updateStats(Player playerList[], Player team[], Str36 spymaster, int playerCount, int gameWon)
{
    for(int i = 0; i < playerCount; i++)
    {
        team[i].gamesPlayed++;
        if(gameWon)
        {
            team[i].wins++;
            if(!strcmp(team[i].username, spymaster))
            {
                team[i].spyMasterWins++;
            }
            else
            {
                team[i].agentWins++;
            }
        }
        for(int j = 0; j < MAX_PLAYERS; j++)
        {
            if(!strcmp(playerList[j].username, team[i].username))
            {
                playerList[j].gamesPlayed = team[i].gamesPlayed;
                playerList[j].wins = team[i].wins;
                playerList[j].spyMasterWins = team[i].spyMasterWins;
                playerList[j].agentWins = team[i].agentWins;
            }
        }
    }
}

void loadGame(Player currentPlayers[], Player blueTeam[], Player redTeam[], int *playerCountBlue, int *playerCountRed, Str36 blueSpymaster, Str36 redSpymaster, Str36 codenames[], char keycard[], char firstMove, Str36 agentCards[])
{
    FILE *file = fopen("savegame.txt", "r");
    if(file == NULL)
    {
        printf("Error opening save game file.\n");
    }
    else
    {
        fscanf(file, "%d %d %s %s", playerCountBlue, playerCountRed, blueSpymaster, redSpymaster);
        for(int i = 0; i < *playerCountBlue; i++)
        {
            fscanf(file, "%s %d %d %d %d", blueTeam[i].username, &blueTeam[i].gamesPlayed, &blueTeam[i].wins, &blueTeam[i].spyMasterWins, &blueTeam[i].agentWins);
        }
        for(int i = 0; i < *playerCountRed; i++)
        {
            fscanf(file, "%s %d %d %d %d", redTeam[i].username, &redTeam[i].gamesPlayed, &redTeam[i].wins, &redTeam[i].spyMasterWins, &redTeam[i].agentWins);
        }
        fscanf(file, "%c", &firstMove);
        for(int i = 0; i < NUMBER_OF_CODENAMES; i++)
        {
            fscanf(file, "%s %c", codenames[i], &keycard[i]);
        }
        
        fclose(file);
    }
}

void saveGame(Player currentPlayers[], Player blueTeam[], Player redTeam[], int playerCountBlue, int playerCountRed, Str36 blueSpymaster, Str36 redSpymaster, Str36 codenames[], char keycard[], char firstMove)
{
    FILE *file = fopen("savegame.txt", "w");
    if(file == NULL)
    {
        printf("Error opening save game file.\n");
    }
    else
    {
        fprintf(file, "%d %d %s %s\n", playerCountBlue, playerCountRed, blueSpymaster, redSpymaster);
        for(int i = 0; i < playerCountBlue; i++)
        {
            fprintf(file, "%s %d %d %d %d\n", blueTeam[i].username, blueTeam[i].gamesPlayed, blueTeam[i].wins, blueTeam[i].spyMasterWins, blueTeam[i].agentWins);
        }
        for(int i = 0; i < playerCountRed; i++)
        {
            fprintf(file, "%s %d %d %d %d\n", redTeam[i].username, redTeam[i].gamesPlayed, redTeam[i].wins, redTeam[i].spyMasterWins, redTeam[i].agentWins);
        }
        fprintf(file, "%s\n", firstMove);
        for(int i = 0; i < NUMBER_OF_CODENAMES; i++)
        {
            fprintf(file, "%s %c\n", codenames[i], keycard[i]);
        }
        
        fclose(file);
    }
}

void displayMenu()
{
    printf("Welcome to Codenames!\n");
    printf("\t[1] New Game\n");
    printf("\t[2] Continue Game\n");
    printf("\t[3] Leaderboard\n");
    printf("\t[0] Exit\n");
}

void displayStats(Player playerList[])
{
    printf("Leaderboard:\n");
    for(int i = 0; i < MAX_PLAYERS; i++)
    {
        if(playerList[i].username[0] != '\0')
        {
            printf("\t- %s\n", playerList[i].username);
            printf("\t\tGames Played: %d\n", playerList[i].gamesPlayed);
            printf("\t\tTotal Wins: %d\n", playerList[i].wins);
            printf("\t\tWins as Spymaster: %d\n", playerList[i].spyMasterWins);
            printf("\t\tWins as Agent: %d\n", playerList[i].agentWins);
        }
    }
}

void displayBoard(Str36 codenames[])
{
    int gridCount = 0;
    size_t length;

    printf("Codenames Board:\n\n");
    for(int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            length = strlen(codenames[gridCount]);
            printf("[%d] ", gridCount + 1);
            if(gridCount < 9)
            {
                printf(" ");
            }
            printf("%s ", codenames[gridCount]);
            for(int k = 0; k < 16 - length; k++)
            {
                printf(" ");
            }
            gridCount++;
        }
        printf("\n");
    }
}

void displayBoardWithKeycard(Str36 codenames[], char keycard[])
{
    int gridCount = 0;
    size_t length;

    printf("Codenames Board with Keycard:\n\n");
    for(int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            length = strlen(codenames[gridCount]);
            printf("[%d] ", gridCount + 1);
            if(gridCount < 9)
            {
                printf(" ");
            }
            printf("%s ", codenames[gridCount]);
            printf("(%c) ", keycard[gridCount]);
            for(int k = 0; k < 12 - length; k++)
            {
                printf(" ");
            }
            gridCount++;
        }
        printf("\n");
    }
}

void loadPlayers(Player playerList[])
{
    FILE *file = fopen("players.txt", "r");
    if(file == NULL)
    {
        printf("Error opening keycard file.\n");
    }
    else
    {
        for(int i = 0; i < MAX_PLAYERS; i++)
        {
            if(fscanf(file, "%s %d %d %d %d", playerList[i].username, &playerList[i].gamesPlayed, &playerList[i].wins, &playerList[i].spyMasterWins, &playerList[i].agentWins) != 5)
            {
                playerList[i].username[0] = '\0';
                playerList[i].gamesPlayed = 0;
                playerList[i].wins = 0;
                playerList[i].spyMasterWins = 0;
                playerList[i].agentWins = 0;
            }
        }
        fclose(file);
    }
}

void loadCodenames(Str36 codenames[])
{
    FILE *file = fopen("codenames.txt", "r");
    if(file == NULL)
    {
        printf("Error opening codenames file.\n");
    }
    else
    {
        for(int i = 0; i < 400; i++)
        {
            fgets(codenames[i], WORD_LENGTH, file);
            if (codenames[i][strlen(codenames[i]) - 1] == '\n')
            {
                codenames[i][strlen(codenames[i]) - 1] = '\0'; 
            }
        }
        
        fclose(file);
    }
}

void selectKeycard(char keycard[], char *firstMove)
{
    int keycardN, gridCount = 0;
    FILE *file;

    keycardN = rand() % 4;

    switch(keycardN)
    {
        case 0:
            file = fopen("keycards/01.txt", "r");
            break;
        case 1:
            file = fopen("keycards/02.txt", "r");
            break;
        case 2:
            file = fopen("keycards/03.txt", "r");
            break;
        case 3:
            file = fopen("keycards/04.txt", "r");
            break;
    }

    if(file == NULL)
    {
        printf("Error opening players file.\n");
    }
    else
    {
        fscanf(file, "%c", firstMove);
        
        for(int i = 0; i < 5; i++)
        {
            for(int j = 0; j < 5; j++)
            {
                fscanf(file, " %c", &keycard[gridCount]);
                gridCount++;
            }
        }
        
        fclose(file);
    }
}

void randomizeCodenames(Str36 codenames[], Str36 codenamesList[])
{
    int usedWords[400] = {0};
    int index;
    srand(time(NULL));

    for(int i = 0; i < NUMBER_OF_CODENAMES; i++)
    {
        do
        {
            index = rand() % 400; 
        } while(usedWords[index]); 

        usedWords[index] = 1; 
        strcpy(codenames[i], codenamesList[index]);
    }
}

int countCards(char keycard[], char color)
{
    int count = 0;
    for(int i = 0; i < NUMBER_OF_CODENAMES; i++)
    {
        if(keycard[i] == color)
        {
            count++;
        }
    }
    return count;
}

int addPoints(Str36 agentCard, Str36 codenames[])
{
    int points = 0;
    for(int i = 0; i < NUMBER_OF_CODENAMES; i++)
    {
        if(!strcmp(codenames[i], agentCard))
        {
            points++;
        }
    }
    return points;
}

void selectSpymaster(Player team[], Str36 spyMaster)
{
    int done = 0, teamCount = 0, selectOption;
    while(!done)
    {
        printf("Pick a spymaster: \n");
        for(int i = 0; i < MAX_TEAM_SIZE; i++)
        {
            if(team[i].username[0] != '\0')
            {
                printf("\t[%d] %s\n", i + 1, team[i].username);
                teamCount++;
            }
        }
        printf(">> ");
        scanf("%d", &selectOption);
        
        if(selectOption > 0 && selectOption <= teamCount)
        {
            strcpy(spyMaster, team[selectOption - 1].username);
            done = 1;
        }
        else
        {
            printf("Invalid option. Please try again.\n\n");
        }
    }
}

void addExisting(Player playerList[], Player currentPlayers[], Player team[], int *playerCount, int total)
{
    Str36 username;
    int returnFlag = 0, playerNum, validUser = 0, notPlaying = 0;

    while(!validUser || !notPlaying)
    {
        validUser = 0;
        notPlaying = 0;
        printf("Player List:\n");
        for(int i = 0; i < MAX_PLAYERS; i++)
        {
            if(playerList[i].username[0] != '\0')
            {
                printf("\t- %s ", playerList[i].username);
                for(int j = 0; j < MAX_CURRENT_PLAYERS; j++)
                {
                    if(!strcmp(playerList[i].username, currentPlayers[j].username))
                    {
                        printf("(Already Playing)");
                    }
                }
                printf("\n");
            }
        }
        printf("\nType 'return' to go back\n");
        printf("Enter your Username: ");
        scanf("%s", username);
        returnFlag = strcmp(username, "return");
        notPlaying = 1;
        for(int i = 0; i < MAX_PLAYERS; i++)
        {
            if(!strcmp(playerList[i].username, username))
            {
                playerNum = i;
                validUser = 1;
            }
        }
        if (!returnFlag)
        {
            validUser = 1;
        }
        for(int i = 0; i < MAX_CURRENT_PLAYERS; i++)
        {
            if (!strcmp(currentPlayers[i].username, username))
            {
                printf("This player is currently in the game. Enter again.\n");
                notPlaying = 0;
            }
        }
        if(!validUser && notPlaying)
        {
            printf("This player does not exist. Enter again.\n");
        }
    }

    if(returnFlag)
    {
        team[*playerCount] = playerList[playerNum];
        currentPlayers[total] = playerList[playerNum];
        (*playerCount)++;
    }
}

void addNew(Player playerList[], Player currentPlayers[], Player team[], int *playerCount, int total)
{
    Str36 username;
    int returnFlag = 0, validUser;

    do
    {
        printf("Type 'return' to go back\n");
        printf("Enter your Username: ");
        scanf("%s", username);
        returnFlag = strcmp(username, "return");
        validUser = 1;
        for(int i = 0; i < MAX_PLAYERS; i++)
        {
            if(!strcmp(playerList[i].username, username))
            {
                validUser = 0;
            }
        }
        if(!validUser)
        {
            printf("This player already exists. Enter Again.\n");
        }
    } 
    while(!validUser);

    if(returnFlag)
    {
        strcpy(team[*playerCount].username, username);
        strcpy(currentPlayers[total].username, username);
        (*playerCount)++;
        validUser = 0;
        for(int i = 0; i < MAX_PLAYERS; i++)
        {
            if(!validUser && playerList[i].username[0] == '\0')
            {
                strcpy(playerList[i].username, username);
                validUser = 1;
            }
        }
    }
}

void removePlayer(Player currentPlayers[], Player team[], int *playerCount, char teamColor)
{
    Str36 username;
    int returnFlag = 0, playerNum, validUser = 0;

    while(!validUser)
    {
        validUser = 0;
        switch(teamColor)
        {
            case 'B':
                printf("Current Blue Team:\n");
                break;
            case 'R':
                printf("Current Red Team:\n");
                break;
        }
        for(int i = 0; i < MAX_TEAM_SIZE; i++)
        {
            if(team[i].username[0] != '\0')
            {
                printf("\t- %s\n", team[i].username);
            }
        }
        printf("\nType 'return' to go back\n");
        printf("Enter which player to remove: ");
        scanf("%s", username);
        returnFlag = strcmp(username, "return");
        for(int i = 0; i < MAX_TEAM_SIZE; i++)
        {
            if(!strcmp(team[i].username, username))
            {
                validUser = 1;
            }
        }
        if (!returnFlag)
        {
            validUser = 1;
        }
        if(!validUser)
        {
            printf("This player is not in the game or does not exist. Enter again.\n");
        }
    }

    if(returnFlag)
    {
        for(int i = 0; i < MAX_TEAM_SIZE; i++)
        {
            if(!strcmp(team[i].username, username))
            {
                playerNum = i;
            }
        }
        memset(&team[playerNum], 0, sizeof(Player));
        for(int i = 0; i < MAX_TEAM_SIZE; i++)
        {
            if(!strcmp(currentPlayers[i].username, username))
            {
                playerNum = i;
            }
        }
        memset(&currentPlayers[playerNum], 0, sizeof(Player));
        (*playerCount)--;
    }
}

void greetPlayers(Str36 name)
{
    printf("Welcome, %s\n\n", name);
}

int teamTurn(Str36 codenames[], char keycard[], char firstMove, int *gameLost, Str36 agentCards[])
{
    Str36 clue;
    Str36 selectOption;
    int done = 0, cardN, guess, guessCount = 0, turnOver = 0, blueCount = 0, redCount = 0, hintFlag = 0 ,quitGame = 0;
    int cardCount[] = {0, 0};

    printf("\n[%c] Spymaster Phase...\n", firstMove);
    while(!done)
    {
        printf("\nType 'READY to show keycard\n");
        printf(">> ");
        scanf("%s", selectOption);
        if(strcmp(selectOption, "READY") == 0)
        {
            done = 1;
        }
        else
        {
            printf("Type 'READY' to show the keycard.\n");
        }
    }
    
    displayBoardWithKeycard(codenames, keycard);
    printf("\nSpymaster, please give a clue to your team.\n");
    do
    {
        printf("Clue Format: <word> <number>\n");
        printf(">> ");
        scanf("%s %d", clue, &cardN);
        for(int i = 0; i < NUMBER_OF_CODENAMES; i++)
        {
            hintFlag = strcmp(clue, codenames[i]);
        }
        if (!hintFlag)
        {
            printf("You entered a clue that is already a codename. Please enter a different clue.\n");
        }
        if(cardN < 0 || cardN > 9)
        {
            printf("Invalid number of codenames. Please enter a number between 0 and 9.\n");
        }
    } while (!hintFlag || (cardN < 0 || cardN > 9));
    
    done = 0;
    while(!done)
    {
        printf("\nType 'DONE' to end Spymaster phase or 'QUIT' to save and quit the game\n");
        printf(">> ");
        scanf("%s", selectOption);
        if(!strcmp(selectOption, "DONE"))
        {
            done = 1;
        }
        else if(!strcmp(selectOption, "QUIT"))
        {
            done = 1;
            printf("Saving game and quitting...\n");
            quitGame = 1;
        }
        else
        {
            printf("\nType 'DONE' to end Spymaster phase or 'QUIT' to save and quit the game\n");
        }
    }
    if(!quitGame)
    {
        for(int i = 0; i < 10; i++)
        {
            printf("\n");
        }
        printf("Agent phase...\n\n");
        displayBoard(codenames);
        printf("\nAgents, please guess the codenames based on the clue given by your Spymaster.\n");
        printf("Enter '0' to end Agent phase\n\n");
        printf("Clue: %s, Number: %d\n", clue, cardN);
        printf("Enter you guesses (0-25):\n");
        if(cardN == 0)
        {
            cardN = 999;
        }
        cardN += 1;
        while(guessCount < cardN && turnOver == 0)
        {
            for(int i = 0; i < NUMBER_OF_CODENAMES; i++)
            {
                if(keycard[i] == 'B')
                {
                    blueCount++;
                }
                else if(keycard[i] == 'R')
                {
                    redCount++;
                }
            }
            for(int i = 0; i < NUMBER_OF_CODENAMES; i++)
            {
                if(!strcmp(codenames[i], agentCards[0]))
                {
                    cardCount[0]++;
                }
                else if(!strcmp(codenames[i], agentCards[1]))
                {
                    cardCount[1]++;
                }
            }
            printf("Guess %d: ", guessCount + 1);
            scanf("%d", &guess);
            if(guess < 1 || guess > NUMBER_OF_CODENAMES)
            {
                printf("Invalid guess. Please enter a number between 0 and 25.\n");
            }
            else if (!strcmp(codenames[guess - 1], agentCards[0]) || !strcmp(codenames[guess - 1], agentCards[1]) || !strcmp(codenames[guess - 1], agentCards[2]))
            {
                printf("You have already guessed this codename. Please guess another one.\n");
            }
            else
            {
                printf("You guessed: %s\n", codenames[guess - 1]);
                switch(keycard[guess - 1])
                {
                    case 'B':
                        strcpy(codenames[guess - 1], agentCards[0]);
                        break;
                    case 'R':
                        strcpy(codenames[guess - 1], agentCards[1]);
                        break;
                    case 'I':
                        strcpy(codenames[guess - 1], agentCards[2]);
                        break;
                    case 'A':
                        strcpy(codenames[guess - 1], agentCards[3]);
                        break;
                }
                if(keycard[guess - 1] == firstMove)
                {
                    printf("Correct guess!\n");
                    switch(firstMove)
                    {
                        case 'B':
                            printf("Blue Team gets a point!\n");
                            break;
                        case 'R':
                            printf("Red Team gets a point!\n");
                            break;
                    }
                    if(blueCount == cardCount[0] || redCount == cardCount[1])
                    {
                        printf("A team has won the game!\n");
                        turnOver = 1; 
                    }
                }
                else if(keycard[guess - 1] == 'I')
                {
                    printf("Innocent guess. No points.\n");
                    turnOver = 1; 
                }
                else if(keycard[guess - 1] == 'A')
                {
                    printf("Assassin! Game over for your team.\n");
                    turnOver = 1;
                    *gameLost = 1;
                }
                else
                {
                    printf("Wrong guess. Other team gets point\n");
                    turnOver = 1;
                }
                guessCount++;
            }
        }
        if(keycard[guess - 1] == 'A')
        {
            printf("Your team has lost the game due to an assassin guess.\n");
        }
        else if(guessCount >= cardN)
        {
            printf("You guessed %d codenames correctly!\n", cardN);
        }
        else
        {
            printf("You guessed %d codenames, but the clue was for Team %c.\n", guessCount - 1, keycard[guess - 1]);
        }

        printf("\nCurrent Game State:\n");
        displayBoard(codenames);
    }
    return quitGame;
}

void continueGame(Player playerList[], Str36 agentCards[])
{
    Player currentPlayers[MAX_CURRENT_PLAYERS] = {0};
    Player blueTeam[MAX_TEAM_SIZE] = {0};
    Player redTeam[MAX_TEAM_SIZE] = {0};
    Str36 codenames[NUMBER_OF_CODENAMES] = {0};
    Str36 codenamesList[400] = {0};
    Str36 blueSpymaster, redSpymaster, selectOption;
    char keycard[NUMBER_OF_CODENAMES] = {0};
    char firstMove;
    int done = 0, endGame = 0, playerCountBlue = 0, playerCountRed = 0, blueCards = 0, redCards = 0, bluePoints = 0, redPoints = 0, gameLost = 0;

    loadGame(currentPlayers, blueTeam, redTeam, &playerCountBlue, &playerCountRed, blueSpymaster, redSpymaster, codenames, keycard, firstMove, agentCards);

    do
    {
        endGame = teamTurn(codenames, keycard, firstMove, &gameLost, agentCards);
        if (endGame)
        {
            saveGame(currentPlayers, blueTeam, redTeam, playerCountBlue, playerCountRed, blueSpymaster, redSpymaster, codenames, keycard, firstMove);
            printf("Game saved successfully!\n");
            done = 1;
        }
        else
        {
            bluePoints = addPoints(agentCards[0], codenames);
            redPoints = addPoints(agentCards[1], codenames);
            if(gameLost)
            {
                switch (firstMove)
                {
                    case 'R':
                        bluePoints = blueCards;
                        break;
                    case 'B':
                        redPoints = redCards;
                        break;
                }
            }
            switch (firstMove)
            {
                case 'B':
                    firstMove = 'R'; 
                    break;
                case 'R':
                    firstMove = 'B'; 
                    break;
            }
            
            if(bluePoints >= blueCards)
            {
                printf("\nBlue Team wins!\n");
                updateStats(playerList, blueTeam, blueSpymaster, playerCountBlue, 1);
                updateStats(playerList, redTeam, redSpymaster, playerCountRed, 0);
                done = 1;
            }
            else if(redPoints >= redCards)
            {
                printf("\nRed Team wins!\n");
                updateStats(playerList, redTeam, redSpymaster, playerCountRed, 1);
                updateStats(playerList, blueTeam, blueSpymaster, playerCountBlue, 0);
                done = 1;
            }
            
            if(!done)
            {
                printf("\nCurrent # of Blue cards: %d\n", blueCards - bluePoints);
                printf("Current # of Red cards: %d\n", redCards - redPoints);
                printf("\nSwitching turn...\n", firstMove);
            }
        }
        // debugCode(blueCards, redCards, bluePoints, redPoints, codenames);
        
    } while(!done);
    displayBoardWithKeycard(codenames, keycard);
    done = 0;
    printf("\nGame Over!\n");
    do
    {
        printf("Type 'QUIT' to exit the game\n");
        printf(">> ");
        scanf("%s", selectOption);
        if(strcmp(selectOption, "QUIT") == 0)
        {
            done = 1;
        }
    } while(!done);
}


void gameStart(Player playerList[], Str36 codenamesList[], Str36 agentCards[])
{
    Player currentPlayers[MAX_CURRENT_PLAYERS] = {0};
    Player blueTeam[MAX_TEAM_SIZE] = {0};
    Player redTeam[MAX_TEAM_SIZE] = {0};
    Str36 codenames[NUMBER_OF_CODENAMES];
    Str36 blueSpymaster, redSpymaster, selectOption;
    char keycard[NUMBER_OF_CODENAMES] = {0};
    char firstMove;
    int done = 0, endGame = 0, playerCountBlue = 0, playerCountRed = 0, blueCards = 0, redCards = 0, bluePoints = 0, redPoints = 0, gameLost = 0;

    do
    {
        done = 0;
        while(!done)
        {
            printf("Current Blue Team Members: \n");
            for(int i = 0; i < MAX_TEAM_SIZE; i++)
            {
                if(blueTeam[i].username[0] != '\0')
                {
                    printf("\t+ %s\n", blueTeam[i].username);
                }
            }
            printf("\nWhat would you like to do?\n");
            printf("\t[0] Done Adding Team Members\n");
            printf("\t[1] Add Existing Player\n");
            printf("\t[2] Add New Player\n");
            printf("\t[3] Remove Player\n");
            printf(">> ");
            scanf("%s", selectOption);
            
            if(selectOption[1] == '\0')
            {
                switch(selectOption[0])
                {
                    case '1':
                        addExisting(playerList, currentPlayers, blueTeam, &playerCountBlue, playerCountBlue + playerCountRed);
                        break;
                    case '2':
                        addNew(playerList, currentPlayers, blueTeam, &playerCountBlue, playerCountBlue + playerCountRed);
                        break;
                    case '3':
                        removePlayer(currentPlayers, blueTeam, &playerCountBlue, 'B');
                        break;
                    case '0':
                        if(playerCountBlue < 2)
                        {
                            printf("You need at least 2 players on the Blue Team to start the game.\n");
                        }
                        else
                        {
                            done = 1;
                        }
                        break;
                    default:
                        printf("Invalid option. Please try again.\n\n");
                }
            }
            else
            {
                printf("Invalid option. Please try again.\n\n");
            }
        }
        selectSpymaster(blueTeam, blueSpymaster);

        done = 0;
        while(!done)
        {
            printf("Current Red Team Members: \n");
            for(int i = 0; i < MAX_TEAM_SIZE; i++)
            {
                if(redTeam[i].username[0] != '\0')
                {
                    printf("\t+ %s\n", redTeam[i].username);
                }
            }
            printf("\nWhat would you like to do?\n");
            printf("\t[0] Done Adding Team Members\n");
            printf("\t[1] Add Existing Player\n");
            printf("\t[2] Add New Player\n");
            printf("\t[3] Remove Player\n");
            printf(">> ");
            scanf("%s", selectOption);
            
            if(selectOption[1] == '\0')
            {
                switch(selectOption[0])
                {
                    case '1':
                        addExisting(playerList, currentPlayers, redTeam, &playerCountRed, playerCountBlue + playerCountRed);
                        break;
                    case '2':
                        addNew(playerList, currentPlayers, redTeam, &playerCountRed, playerCountBlue + playerCountRed);
                        break;
                    case '3':
                        removePlayer(currentPlayers, redTeam, &playerCountRed, 'R');
                        break;
                    case '0':
                        if(playerCountRed < 2)
                        {
                            printf("You need at least 2 players on the Red Team to start the game.\n");
                        }

                        else
                        {
                            done = 1;
                        }
                        break;
                    default:
                        printf("Invalid option. Please try again.\n\n");
                }
            }
            else
            {
                printf("Invalid option. Please try again.\n\n");
            }
        }
        selectSpymaster(redTeam, redSpymaster);
        endGame = 1;
    } while(!endGame);

    printf("\nCurrent Players:\n");
    for(int i = 0; i < MAX_CURRENT_PLAYERS; i++)
    {
        if(currentPlayers[i].username[0] != '\0')
        {
            printf("\t- %s\n", currentPlayers[i].username);
        }
    }
    printf("\nBlue Players:\n");
    for(int i = 0; i < MAX_TEAM_SIZE; i++)
    {
        if(blueTeam[i].username[0] != '\0')
        {
            printf("\t- %s\n", blueTeam[i].username);
        }
    }
    printf("Blue Spymaster: %s\n\n", blueSpymaster);
    printf("Red Players:\n");
    for(int i = 0; i < MAX_TEAM_SIZE; i++)
    {
        if(redTeam[i].username[0] != '\0')
        {
            printf("\t- %s\n", redTeam[i].username);
        }
    }
    printf("Red Spymaster: %s\n\n", redSpymaster);
    printf("Type 'READY' to start the game\n");
    done = 0;
    do
    {
        printf(">> ");
        scanf("%s", selectOption);
        if(strcmp(selectOption, "READY") == 0)
        {
            done = 1;
        }
        else
        {
            printf("Type 'READY' to start the game.\n");
        }
    } while(!done);
    randomizeCodenames(codenames, codenamesList);
    selectKeycard(keycard, &firstMove);
    printf("\nCodenames have been randomized!\n");
    blueCards = countCards(keycard, 'B');
    redCards = countCards(keycard, 'R');
    printf("Game starting with %d players on Blue Team and %d players on Red Team.\n", playerCountBlue, playerCountRed);
    printf("Spymasters are ready to give clues!\n\n");
    displayBoard(codenames);
    printf("\nFirst move is by: %c\n", firstMove);

    done = 0;
    do
    {
        endGame = teamTurn(codenames, keycard, firstMove, &gameLost, agentCards);
        if (endGame)
        {
            saveGame(currentPlayers, blueTeam, redTeam, playerCountBlue, playerCountRed, blueSpymaster, redSpymaster, codenames, keycard, firstMove);
            printf("Game saved successfully!\n");
            done = 1;
        }
        else
        {
            bluePoints = addPoints(agentCards[0], codenames);
            redPoints = addPoints(agentCards[1], codenames);
            if(gameLost)
            {
                switch (firstMove)
                {
                    case 'R':
                        bluePoints = blueCards;
                        break;
                    case 'B':
                        redPoints = redCards;
                        break;
                }
            }
            switch (firstMove)
            {
                case 'B':
                    firstMove = 'R'; 
                    break;
                case 'R':
                    firstMove = 'B'; 
                    break;
            }
            
            if(bluePoints >= blueCards)
            {
                printf("\nBlue Team wins!\n");
                updateStats(playerList, blueTeam, blueSpymaster, playerCountBlue, 1);
                updateStats(playerList, redTeam, redSpymaster, playerCountRed, 0);
                done = 1;
            }
            else if(redPoints >= redCards)
            {
                printf("\nRed Team wins!\n");
                updateStats(playerList, redTeam, redSpymaster, playerCountRed, 1);
                updateStats(playerList, blueTeam, blueSpymaster, playerCountBlue, 0);
                done = 1;
            }
            
            if(!done)
            {
                printf("\nCurrent # of Blue cards: %d\n", blueCards - bluePoints);
                printf("Current # of Red cards: %d\n", redCards - redPoints);
                printf("\nSwitching turn...\n", firstMove);
            }
        }
        // debugCode(blueCards, redCards, bluePoints, redPoints, codenames);
        
    } while(!done);
    // displayEndGameStats(bluePoints, redPoints, blueTeam, redTeam, playerList);
    displayBoardWithKeycard(codenames, keycard);
    done = 0;
    printf("\nGame Over!\n");
    do
    {
        printf("Type 'QUIT' to exit the game\n");
        printf(">> ");
        scanf("%s", selectOption);
        if(strcmp(selectOption, "QUIT") == 0)
        {
            done = 1;
        }
    } while(!done);

    // greetPlayers(name);
}

int main()
{
    Player playerList[MAX_PLAYERS] = {0};
    Str36 codenamesList[400] = {0};
    Str36 agentCards[] = {"| BLUE AGENT |", "| RED AGENT |", "| INNOCENT |", "| ASSASSIN |"};
    Str36 selectOption;
    int quitGame = 0;

    loadPlayers(playerList);
    loadCodenames(codenamesList);

    do
    {
        displayMenu();
        printf(">> ");
        scanf("%s", selectOption);
        
        if(selectOption[1] == '\0')
        {
            switch(selectOption[0])
            {
                case '1':
                    gameStart(playerList, codenamesList, agentCards);
                    break;
                case '2':
                    continueGame(playerList, agentCards);
                    break;
                case '3': 
                    displayStats(playerList);
                    break;
                case '0':
                    quitGame = 1;
                    break;
                default:
                    printf("Invalid option. Please try again.\n\n");
            }
        }
        else
        {
            printf("Invalid option. Please try again.\n\n");
        }
        
    } while(!quitGame);
    
    updatePlayers(playerList);
    printf("Thank you for playing Codenames!\n");
    return 0;
}