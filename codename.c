/**
 *  Description : Recreation of the board game Codenames
 *  Author/s : Nañawa, Alfred Brant
 *  Section : S24
 *  Last Modified : 7/30/2025
 *  Acknowledgments : <list of references used in the making of this project> 
 */
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

/**
 * Structure to hold player information
 */
typedef struct 
{
    Str36 username;
    int gamesPlayed;
    int wins;
    int spyMasterWins;
    int agentWins;
} Player;

/**
 * Debugging function to print game state
 * @param blueCards Number of blue cards
 * @param redCards Number of red cards
 * @param bluePoints Points for blue team
 * @param redPoints Points for red team
 * @param codenames Array of codenames
 * @return this function does not return anything
 */
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

/**
 * Function update players into the playerlist when starting the game
 * @param playerList Array of Player structures
 * @return this function does not return anything
 */
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

/**
 * Function to update player statistics after a game
 * @param playerList Array of Player structures
 * @param team Array of Player structures for the team
 * @param spymaster The username of the spymaster
 * @param playerCount Number of players in the team
 * @param gameWon Indicates if the game was won (1) or lost (0)
 * @param i index for i
 * @param j index for j
 * @return this function does not return anything
 */
void updateStats(Player playerList[], Player team[], Str36 spymaster, int playerCount, int gameWon)
{
    int i, j;

    for(i = 0; i < playerCount; i++)
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
        for(j = 0; j < MAX_PLAYERS; j++)
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

/**
 * Function to load save data from a file
 * @param saveData Array to store save data
 * @param saveSlot The save slot number to load
 * @param slotEmpty Flag to indicate if the slot is empty
 * @return Returns the save slot number if successful, otherwise returns 0
 */
int loadSaveData()
{
    int saveData[3];
    int saveSlot, slotEmpty = 1;

    FILE *file = fopen("savedata.txt", "r");
    if(file == NULL)
    {
        printf("Error opening save data file.\n");
        return 0;
    }
    else
    {
        for(int i = 0; i < 3; i++)
        {
            fscanf(file, "%d", &saveData[i]);
        }
    }
    fclose(file);

    do
    {
        printf("Select save slot (1-3): ");
        scanf("%d", &saveSlot);
        if (saveSlot < 1 || saveSlot > 3)
        {
            printf("Invalid save slot. Please choose between 1 and 3.\n");
        }
        else if (saveData[saveSlot - 1] == 0)
        {
            printf("Save slot %d is empty. Please pick another slot.", saveSlot);
        }
        else if (saveData[saveSlot - 1] == 1)
        {
            printf("Game is loading...");
        }
        
    } while ((saveSlot < 1 || saveSlot > 3) || slotEmpty);
    return saveSlot; 
}

/**
 * Function to load a saved game
 * @param blueTeam Array of Player structures for the blue team
 * @param redTeam Array of Player structures for the red team
 * @param playerCountBlue Number of players in the blue team
 * @param playerCountRed Number of players in the red team
 * @param blueSpymaster The spymaster for the blue team
 * @param redSpymaster The spymaster for the red team
 * @param codenames Array of codenames
 * @param keycard Array representing the keycard
 * @param guessed Array indicating which codenames have been guessed
 * @param firstMove Pointer to store which team's turn is first
 * @return this function does not return anything
 */
void loadGame(Player blueTeam[], Player redTeam[], int *playerCountBlue, int *playerCountRed, Str36 blueSpymaster, Str36 redSpymaster, Str36 codenames[], char keycard[], int guessed[], char *firstMove)
{
    FILE *file;
    Str36 savefileName;
    int saveData[3] = {0, 0, 0};
    int saveSlot;

    file = fopen("savedata.txt", "r");
    if(file == NULL)
    {
        printf("Error opening save data file.\n");
    }
    for(int i = 0; i < 3; i++)
    {
        fscanf(file, "%d", &saveData[i]);
    }
    fclose(file);

    do
    {
        printf("Select save slot (1-3): ");
        scanf("%d", &saveSlot);
        if (saveSlot < 1 || saveSlot > 3)
        {
            printf("Invalid save slot. Please choose between 1 and 3.\n");
        }
        else if (saveData[saveSlot - 1] == 0)
        {
            printf("Save slot %d is empty. Please pick another slot.\n", saveSlot);
            saveSlot = 0;
        }
    } while (saveSlot < 1 || saveSlot > 3);

    sprintf(savefileName, "savegame%d.txt", saveSlot);
    file = fopen(savefileName, "r");
    if(file == NULL)
    {
        printf("Error opening save game file.\n");
    }
    fscanf(file, "%d %d %s %s", playerCountBlue, playerCountRed, blueSpymaster, redSpymaster);
    for(int i = 0; i < *playerCountBlue; i++)
    {
        fscanf(file, "%s %d %d %d %d", blueTeam[i].username, &blueTeam[i].gamesPlayed, &blueTeam[i].wins, &blueTeam[i].spyMasterWins, &blueTeam[i].agentWins);
    }
    for(int i = 0; i < *playerCountRed; i++)
    {
        fscanf(file, "%s %d %d %d %d", redTeam[i].username, &redTeam[i].gamesPlayed, &redTeam[i].wins, &redTeam[i].spyMasterWins, &redTeam[i].agentWins);
    }
    fscanf(file, " %c", firstMove);
    for(int i = 0; i < NUMBER_OF_CODENAMES; i++)
    {
        fscanf(file, "%s %c %d", codenames[i], &keycard[i], &guessed[i]);
    }
    fclose(file);
}

/**
 * Function to save the current game state to a file
 * @param blueTeam Array of Player structures for the blue team
 * @param redTeam Array of Player structures for the red team
 * @param playerCountBlue Number of players in the blue team
 * @param playerCountRed Number of players in the red team
 * @param blueSpymaster The spymaster for the blue team
 * @param redSpymaster The spymaster for the red team
 * @param codenames Array of codenames
 * @param keycard Array representing the keycard
 * @param guessed Array indicating which codenames have been guessed
 * @param firstMove Indicates which team's turn is first
 * @return this function does not return anything
 */
void saveGame(Player blueTeam[], Player redTeam[], int playerCountBlue, int playerCountRed, Str36 blueSpymaster, Str36 redSpymaster, Str36 codenames[], char keycard[], int guessed[], char firstMove)
{
    FILE *file;
    Str36 savefileName;
    int saveData[3] = {0, 0, 0};
    int saveSlot, i;

    file = fopen("savedata.txt", "r");
    if (file != NULL) 
    {
        for (i = 0; i < 3; i++) 
        {
            fscanf(file, "%d", &saveData[i]);
        }
        fclose(file);
    }

    do
    {
        printf("Select save slot (1-3): ");
        scanf("%d", &saveSlot);
        if (saveSlot < 1 || saveSlot > 3)
        {
            printf("Invalid save slot. Please choose between 1 and 3.\n");
        }
        else if (saveData[saveSlot - 1] == 1)
        {
            int overwrite;
            printf("Save slot %d is already occupied. Overwrite? (1 for Yes, 0 for No): ", saveSlot);
            scanf("%d", &overwrite);
            if (!overwrite) saveSlot = 0;
        }
    } while (saveSlot < 1 || saveSlot > 3);

    saveData[saveSlot - 1] = 1;
    file = fopen("savedata.txt", "w");
    for (i = 0; i < 3; i++) 
    {
        fprintf(file, "%d\n", saveData[i]);
    }
    fclose(file);

    sprintf(savefileName, "savegame%d.txt", saveSlot);
    file = fopen(savefileName, "w");
    if(file == NULL)
    {
        printf("Error opening save game file.\n");
    }
    fprintf(file, "%d %d %s %s\n", playerCountBlue, playerCountRed, blueSpymaster, redSpymaster);
    for(int i = 0; i < playerCountBlue; i++)
    {
        fprintf(file, "%s %d %d %d %d\n", blueTeam[i].username, blueTeam[i].gamesPlayed, blueTeam[i].wins, blueTeam[i].spyMasterWins, blueTeam[i].agentWins);
    }
    for(int i = 0; i < playerCountRed; i++)
    {
        fprintf(file, "%s %d %d %d %d\n", redTeam[i].username, redTeam[i].gamesPlayed, redTeam[i].wins, redTeam[i].spyMasterWins, redTeam[i].agentWins);
    }
    fprintf(file, "%c\n", firstMove);
    for(int i = 0; i < NUMBER_OF_CODENAMES; i++)
    {
        fprintf(file, "%s %c %d\n", codenames[i], keycard[i], guessed[i]);
    }
    fclose(file);
}

/**
 * Function to display the main menu of the game
 * @return this function does not return anything
 */
void displayMenu()
{
    printf("Welcome to Codenames!\n");
    printf("\t[1] New Game\n");
    printf("\t[2] Continue Game\n");
    printf("\t[3] Leaderboard\n");
    printf("\t[0] Exit\n");
}

/**
 * Function to display the leaderboard of players
 * @param playerList Array of Player structures
 * @return this function does not return anything
 */
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

/**
 * Function to display the game board
 * @param codenames Array of codenames
 * @param keycard Array representing the keycard
 * @param guessed Array indicating which codenames have been guessed
 * @param agentCards Array of agent cards for display
 * @param gridCount The current grid count
 * @param length The length of the string of codenames
 * @return this function does not return anything
 */
void displayBoard(Str36 codenames[], char keycard[], int guessed[], Str36 agentCards[])
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
            if(guessed[gridCount])
            {
                switch (keycard[gridCount])
                {
                    case 'B':
                        printf("%s ", agentCards[0]);
                        length = strlen(agentCards[0]);
                        break;
                    case 'R':
                        printf("%s ", agentCards[1]);
                        length = strlen(agentCards[1]);
                        break;
                    case 'I':
                        printf("%s ", agentCards[2]);
                        length = strlen(agentCards[2]);
                        break;
                    case 'A':
                        printf("%s ", agentCards[3]);
                        length = strlen(agentCards[3]);
                        break;
                }
            }
            else
            {
                printf("%s ", codenames[gridCount]);
            }
            for(int k = 0; k < 16 - length; k++)
            {
                printf(" ");
            }
            gridCount++;
        }
        printf("\n");
    }
}

/**
 * Function to display the game board with assigned keycard
 * @param codenames Array of codenames
 * @param keycard Array representing the keycard
 * @param guessed Array indicating which codenames have been guessed
 * @param agentCards Array of agent cards for display
 * @param gridCount The current grid count
 * @param length The length of the stiring of codenames
 * @return this function does not return anything
 */
void displayBoardWithKeycard(Str36 codenames[], char keycard[], int guessed[], Str36 agentCards[])
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
            if(guessed[gridCount])
            {
                switch (keycard[gridCount])
                {
                    case 'B':
                        printf("%s     ", agentCards[0]);
                        length = strlen(agentCards[0]);
                        break;
                    case 'R':
                        printf("%s     ", agentCards[1]);
                        length = strlen(agentCards[1]);
                        break;
                    case 'I':
                        printf("%s     ", agentCards[2]);
                        length = strlen(agentCards[2]);
                        break;
                    case 'A':
                        printf("%s     ", agentCards[3]);
                        length = strlen(agentCards[3]);
                        break;
                }
            }
            else
            {
                printf("%s ", codenames[gridCount]);
                printf("(%c) ", keycard[gridCount]);
            }
            for(int k = 0; k < 16 - length; k++)
            {
                printf(" ");
            }
            gridCount++;
        }
        printf("\n");
    }
}

/**
 * Function to load player data from a file
 * @param playerList Array of Player structures
 * @return this function does not return anything
 */
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

/**
 * Function to load codenames from a file
 * @param codenames Array of codenames
 * @return this function does not return anything
 */
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

/**
 * Function to select a keycard for the game
 * @param keycard Array representing the keycard
 * @param firstMove Pointer to store which team's turn is first
 * @return this function does not return anything
 */
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

/**
 * Function to randomize codenames from a list
 * @param codenames Array to store the selected codenames
 * @param codenamesList Array containing all available codenames
 * @param usedWords Array to track which words have been used
 * @param index Index for random selection
 * @return this function does not return anything
 */
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

/**
 * Function to count the number of cards of a specific color in the keycard
 * @param keycard Array representing the keycard
 * @param color The color to count ('B', 'R', 'I', or 'A')
 * @param count The count of cards of the specified color
 * @return Returns the count of cards of the specified color
 */
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

/**
 * Function to add points based on the agent card
 * @param agentCard The agent card to check against
 * @param codenames Array of codenames
 * @param points The points to add
 * @return Returns the number of points added
 */
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

/**
 * Function to select a spymaster from the team
 * @param team Array of Player structures representing the team
 * @param spyMaster The username of the selected spymaster
 * @param done Flag to indicate if the selection is done
 * @param teamCount The number of players in the team
 * @param selectOption The option selected by the user
 * @return this function does not return anything
 */
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

/**
 * Function to add an existing player to the team
 * @param playerList Array of Player structures containing all players
 * @param currentPlayers Array of Player structures containing currently playing players
 * @param team Array of Player structures representing the team
 * @param playerCount Pointer to the number of players in the team
 * @param total Total number of players currently playing
 * @param username The username of the player to add
 * @param returnFlag Flag to indicate if the user wants to return
 * @param validUser Flag to indicate if the user is valid
 * @param notPlaying Flag to indicate if the player is not currently playing
 * @param playerNum The index of the player in the player list
 * @return this function does not return anything
 */
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

/**
 * Function to add a new player to the game
 * @param playerList Array of Player structures containing all players
 * @param currentPlayers Array of Player structures containing currently playing players
 * @param team Array of Player structures representing the team
 * @param playerCount Pointer to the number of players in the team
 * @param total Total number of players currently playing
 * @param username The username of the player to add
 * @param returnFlag Flag to indicate if the user wants to return
 * @param validUser Flag to indicate if the user is valid
 * @return this function does not return anything
 */
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

/**
 * Function to remove a player from the game
 * @param currentPlayers Array of Player structures containing currently playing players
 * @param team Array of Player structures representing the team
 * @param playerCount Pointer to the number of players in the team
 * @param teamColor The color of the team ('B' for Blue, 'R' for Red)
 * @param username The username of the player to remove
 * @param returnFlag Flag to indicate if the user wants to return
 * @param validUser Flag to indicate if the user is valid
 * @param playerNum The index of the player in the team
 * @return this function does not return anything
 */
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

/**
 * Function to greet players at the start of the game
 * @param name The name of the player
 * @return this function does not return anything
 */
void greetPlayers(Str36 name)
{
    printf("Welcome, %s\n\n", name);
}

/**
 * Function to handle the team turn phase of the game
 * @param codenames Array of codenames
 * @param keycard Array representing the keycard
 * @param firstMove Indicates which team's turn is first
 * @param gameLost Flag to indicate if the game is lost
 * @param agentCards Array of agent cards for display
 * @param guessed Array indicating which codenames have been guessed
 * @param clue The clue given by the spymaster
 * @param selectOption The option selected by the user
 * @param done Flag to indicate if the phase is done
 * @param cardN The number of codenames to guess
 * @param guess The current guess made by the agents
 * @param guessCount The count of guesses made by the agents
 * @param turnOver Flag to indicate if the turn is over
 * @param blueCount The count of blue cards guessed
 * @param redCount The count of red cards guessed
 * @param hintFlag Flag to indicate if the clue is valid
 * @param quitGame Flag to indicate if the game is quit
 * @param cardCount Array to count the number of cards guessed
 * @return Returns 1 if the game is lost, otherwise returns 0
 */
int teamTurn(Str36 codenames[], char keycard[], char firstMove, int *gameLost, Str36 agentCards[], int guessed[])
{
    Str36 clue;
    Str36 selectOption;
    int done = 0, cardN, guess, guessCount = 0, turnOver = 0, blueCount = 0, redCount = 0, hintFlag = 0, quitGame = 0;
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
    
    displayBoardWithKeycard(codenames, keycard, guessed, agentCards);
    printf("\nSpymaster, please give a clue to your team.\n");
    do
    {
        printf("Clue Format: <word> <number>\n");
        printf(">> ");
        scanf("%s %d", clue, &cardN);
        hintFlag = 1; 
        for(int i = 0; i < NUMBER_OF_CODENAMES; i++)
        {
            if(strcmp(clue, codenames[i]) == 0)
            {
                hintFlag = 0;
            }
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
            quitGame = 1;
            printf("Saving game and quitting...\n");
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
        displayBoard(codenames, keycard, guessed, agentCards);
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
            if(guess == 0)
            {
                done = 1;
                turnOver = 1;
                printf("Agent phase ended.\n");
            }
            else if(guess < 1 || guess > NUMBER_OF_CODENAMES)
            {
                printf("Invalid guess. Please enter a number between 0 and 25.\n");
            }
            else if (guessed[guess - 1])
            {
                printf("You have already guessed this codename. Please guess another one.\n");
            }
            else
            {
                printf("You guessed: %s\n", codenames[guess - 1]);
                guessed[guess - 1] = 1;
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
        displayBoard(codenames, keycard, guessed, agentCards);
    }
    return quitGame;
}

/**
 * Function to continue a saved game
 * @param playerList Array of Player structures containing all players
 * @param agentCards Array of agent cards for display
 * @param currentPlayers Array of Player structures containing currently playing players
 * @param blueTeam Array of Player structures representing the Blue team
 * @param redTeam Array of Player structures representing the Red team
 * @param codenames Array of codenames
 * @param codenamesList Array containing all available codenames
 * @param blueSpymaster The username of the Blue team's spymaster
 * @param redSpymaster The username of the Red team's spymaster
 * @param selectOption The option selected by the user
 * @param keycard Array representing the keycard
 * @param firstMove Indicates which team's turn is first
 * @param guessed Array indicating which codenames have been guessed
 * @param done Flag to indicate if the game is done
 * @param endGame Flag to indicate if the game has ended
 * @param playerCountBlue The number of players in the Blue team
 * @param playerCountRed The number of players in the Red team
 * @param blueCards The number of Blue cards
 * @param redCards The number of Red cards
 * @param bluePoints The points scored by the Blue team
 * @param redPoints The points scored by the Red team
 * @param gameLost Flag to indicate if the game is lost
 * @return this function does not return anything
 */
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
    int guessed[NUMBER_OF_CODENAMES] = {0};
    int done = 0, endGame = 0, playerCountBlue = 0, playerCountRed = 0, blueCards = 0, redCards = 0, bluePoints = 0, redPoints = 0, gameLost = 0;

    loadGame(blueTeam, redTeam, &playerCountBlue, &playerCountRed, blueSpymaster, redSpymaster, codenames, keycard, guessed, &firstMove);

    do
    {
        endGame = teamTurn(codenames, keycard, firstMove, &gameLost, agentCards, guessed);
        if (endGame)
        {
            saveGame(blueTeam, redTeam, playerCountBlue, playerCountRed, blueSpymaster, redSpymaster, codenames, keycard, guessed, firstMove);
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

            blueCards = countCards(keycard, 'B');
            redCards = countCards(keycard, 'R');
            
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
}

/**
 * Function to start the game by setting up teams and players
 * @param playerList Array of Player structures containing all players
 * @param codenamesList Array containing all available codenames
 * @param agentCards Array of agent cards for display
 * @param currentPlayers Array of Player structures containing currently playing players
 * @param blueTeam Array of Player structures representing the Blue team
 * @param redTeam Array of Player structures representing the Red team
 * @param codenames Array of codenames
 * @param blueSpymaster The username of the Blue team's spymaster
 * @param redSpymaster The username of the Red team's spymaster
 * @param selectOption The option selected by the user
 * @param keycard Array representing the keycard
 * @param firstMove Indicates which team's turn is first
 * @param guessed Array indicating which codenames have been guessed
 * @param done Flag to indicate if the game is done
 * @param endGame Flag to indicate if the game has ended
 * @param playerCountBlue The number of players in the Blue team
 * @param playerCountRed The number of players in the Red team
 * @param blueCards The number of Blue cards
 * @param redCards The number of Red cards
 * @param bluePoints The points scored by the Blue team
 * @param redPoints The points scored by the Red team
 * @param gameLost Flag to indicate if the game is lost
 * @return this function does not return anything
 */
void gameStart(Player playerList[], Str36 codenamesList[], Str36 agentCards[])
{
    Player currentPlayers[MAX_CURRENT_PLAYERS] = {0};
    Player blueTeam[MAX_TEAM_SIZE] = {0};
    Player redTeam[MAX_TEAM_SIZE] = {0};
    Str36 codenames[NUMBER_OF_CODENAMES];
    Str36 blueSpymaster, redSpymaster, selectOption;
    char keycard[NUMBER_OF_CODENAMES] = {0};
    char firstMove;
    int guessed[NUMBER_OF_CODENAMES] = {0};
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
    displayBoard(codenames, keycard, guessed, agentCards);
    printf("\nFirst move is by: %c\n", firstMove);

    done = 0;
    do
    {
        endGame = teamTurn(codenames, keycard, firstMove, &gameLost, agentCards, guessed);
        if (endGame)
        {
            saveGame(blueTeam, redTeam, playerCountBlue, playerCountRed, blueSpymaster, redSpymaster, codenames, keycard, guessed, firstMove);
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

            blueCards = countCards(keycard, 'B');
            redCards = countCards(keycard, 'R');
            
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
    displayBoardWithKeycard(codenames, keycard, guessed, agentCards);
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

/**
 * This is to certify that this project is my/our own work, based on my/our personal  
 * efforts in studying and applying the concepts learned. I/We have constructed the  
 * functions and their respective algorithms and corresponding code by myself/ourselves.  
 * The program was run, tested, and debugged by my/our own efforts. I/We further certify  
 * that I/we have not copied in part or whole or otherwise plagiarized the work of 
 * other students and/or persons.
 * 
 * Nañawa, Alfred Brant A. (DLSU ID# 12486825)
 * */