#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PLAYERS 100
#define MAX_CURRENT_PLAYERS 50
#define MAX_TEAM_SIZE 25

typedef char Str36[36];

typedef struct 
{
    Str36 username;
    int gamesPlayed;
    int wins;
    int spyMasterWins;
    int agentWins;
} Player;

void displayMenu()
{
    printf("Welcome to Codenames!\n");
    printf("\t[1] New Game\n");
    printf("\t[2] Continue Game\n");
    printf("\t[3] Leaderboard\n");
    printf("\t[0] Exit\n");
}

void continueGame()
{
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
        }
    }
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
            case 'b':
                printf("Current Blue Team:\n");
                break;
            case 'r':
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

void gameStart(Player playerList[])
{
    Player currentPlayers[MAX_CURRENT_PLAYERS] = {0};
    Player blueTeam[MAX_TEAM_SIZE] = {0};
    Player redTeam[MAX_TEAM_SIZE] = {0};
    char selectOption[2];
    int done = 0, endGame = 0, playerCountBlue = 0, playerCountRed = 0;
    Str36 blueSpymaster, redSpymaster;
    Str36 codenames[25];

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
                        removePlayer(currentPlayers, blueTeam, &playerCountBlue, 'b');
                        break;
                    case '0':
                        done = 1;
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
                        removePlayer(currentPlayers, redTeam, &playerCountRed, 'r');
                        break;
                    case '0':
                        done = 1;
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

    // greetPlayers(name);
}

int main()
{
    Player playerList[MAX_PLAYERS] = {0};
    char selectOption[2];
    int quitGame = 0;

    strcpy(playerList[0].username, "Alfred");
    playerList[0].gamesPlayed = 15;
    strcpy(playerList[1].username, "rokishi");
    playerList[1].gamesPlayed = 10;
    strcpy(playerList[2].username, "NotSocialite"); 
    playerList[2].gamesPlayed = 5;
    
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
                    gameStart(playerList);
                    break;
                case '2':
                    continueGame();
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
    
    return 0;
}