#include <stdio.h>
#include <string.h>

#define ROW 8
#define COL 8

main()
{
	printf("Minesweeper\n");

	//declaring variables
	int save = 32;

	FILE* mineFile;

	int gameOver = 0;//game ends if not 0

	//matrix arrays
	int gameBoard[ROW][9];/*the one extra column is to stop the array from jumping
						  to the next row when near the edge of the column*/
	int gameCover[ROW][COL];//User Interface

	int remaining = 0;//displays remaining unchecked tiles

	//scores
	int player[] = {0, 0};
	int turn = 0;//tracks current player turn

	while (save < 0 || save > 1)//ask users about loading previous save file
	{
		printf("Load Previous Save? (0=NO/1=YES): ");
		scanf("%d", &save);
		if (save < 0 || save > 1)
			printf("ERROR\n");
	}

	for (int r = 0; r < ROW; r++)//creating a blank matrix array
	{
		for (int c = 0; c < 9; c++)
		{
			gameBoard[r][c] = 0;
		}
	}

	for (int r = 0; r < ROW; r++)//creating the UI matrix array the user will see
	{
		for (int c = 0; c < COL; c++)
		{
			gameCover[r][c] = 512;
		}
	}

	srand(time(NULL));//resets random
	int guess = 0;//prevent user from inputing extreme values

	for (int i = 0; i < 10; i++)//adding mines to gameBoard matrix
	{
		int mineX = rand();
		mineX = mineX % 8;
		int mineY = rand();
		mineY = mineY % 8;

			for (int r = 0; r < ROW; r++)//prevent duplicates
			{
				for (int c = 0; c < COL; c++)
				{
					if (gameBoard[mineX][mineY] == -64)
					{
						gameBoard[mineX][mineY] = 0;
						i--;
					}
				}
			}
		gameBoard[mineX][mineY] = -64;
	}

	for (int r = 0; r < ROW; r++)//increase the values around a mine
	{
		for (int c = 0; c < 9; c++)
		{
			if (gameBoard[r][c] < -1)
				for (int r2 = r - 1; r2 <= r + 1; r2++)
					for (int c2 = c - 1; c2 <= c + 1; c2++)
						gameBoard[r2][c2]++;
		}
	}

	int players = 0;

	int firstRound = 1;//if very first tile in round 1 is a mine, the current player gets a second chance

	if (save == 0)//creates new file
	{
		while (players < 1 || players > 2)//user enters amount of players
		{
			printf("One or Two Players?: ");
			scanf("%d", &players);
			if (players < 1 || players > 2)
				printf("ERROR\n");
		}

		mineFile = fopen("minefile.txt", "w");
		fclose(mineFile);
	}
	else//loads previous file
	{
		mineFile = fopen("minefile.txt", "r");
		{
			mineFile = fopen("minefile.txt", "w");
			fscanf(mineFile, "%d", turn);
			for (int r = 0; r < ROW; r++)
			{
				for (int c = 0; c < 9; c++)
				{
					fscanf(mineFile, "%d", gameBoard[r][c]);
				}
			}
			for (int r = 0; r < ROW; r++)
			{
				for (int c = 0; c < COL; c++)
				{
					fscanf(mineFile, "%d", gameCover[r][c]);
				}
			}
			fscanf(mineFile, "%d%d", player[0], player[1]);
			fscanf(mineFile, "%d", firstRound);
			fclose(mineFile);
		}
	}


	while (gameOver == 0)//game start
	{
		printf("\t\t\t\t    X\n\tX0\tX1\tX2\tX3\tX4\tX5\tX6\tX7\n");
		for (int r = 0; r < ROW; r++)//display gameCover UI
		{
			if(r == 4)
			printf("Y   ---------------------------------------------------------------------\n  Y%d|\t", r);
			else
			printf("    ---------------------------------------------------------------------\n  Y%d|\t", r);
			for (int c = 0; c < COL; c++)
			{
				printf("%d\t",gameCover[r][c]);
				if (gameCover[r][c] > 8)//tracks remaining free tiles
					remaining++;
			}
			printf("|\n");
		}
		printf("    ---------------------------------------------------------------------\n");
		printf("\n%d REMAINING TILES\n", remaining);

		if (remaining == 10)//if 10 tile remain - only mines
			gameOver++;
		else
		{
			remaining = 0;

			if(players == 1)//if 1 player
				printf("\nPlayer One - %d\n", player[turn]);
			if(players == 2)//if 2 player
				printf("\nPlayer One - %d\tPlayer Two - %d\n", player[0], player[1]);

			printf("\n\t\t\t    -CHOOSE A TILE-\n\n\n\n");

			int X = 64;
			int Y = 64;

			while (guess == 0)//user inputs coordinates of a tile
			{
				guess++;
				printf("Cordinate X: ");
				scanf("%d", &X);
				printf("Cordinate Y: ");
				scanf("%d", &Y);
				//error checking
				if (X < 0 || X > 7)
					guess = 0;
				if (Y < 0 || Y > 7)
					guess = 0;
				if (guess == 0)
					printf("INVALID CORDINATES\n\n");
				if (gameCover[Y][X] >= 0 && gameCover[Y][X] <= 8)
				{
					guess = 0;
					printf("USED TILE\n\n");
				}
				if (firstRound == 1 && gameBoard[Y][X] < 0)//if mine found on first round
				{
					firstRound = 0;
					gameOver--;
					printf("VERY FIRST TILE WAS MINE - CHOOSE ANOTHER SPACE\n");
				}
			}
			guess = 0;

			if (gameBoard[Y][X] == 0)//if tile was a 0
			{
				gameCover[Y][X] = gameBoard[Y][X];//swap gameCover(UI) value with gameBoard value
				
				player[turn]++;

				//variables that return to current position
				int setX = X;
				int setY = Y;
				int around = 0;

				//check other values surround current 0 value
				while (around == 0)//left
				{
					X--;
					if (X == -1)//if checker goes out of array range
						around++;
					if (around == 0)
					{
						gameCover[Y][X] = gameBoard[Y][X];

						player[turn]++;//current player score goes up

						if (gameBoard[Y][X] >= 1)//if the value is 1 or over
						{
							gameCover[Y][X] = gameBoard[Y][X];
							around++;
						}
					}
				}
				around = 0;
				X = setX;
				while (around == 0)//right
				{
					X++;
					if (X == 8)
						around++;
					if (around == 0)
					{
						gameCover[Y][X] = gameBoard[Y][X];

						player[turn]++;

						if (gameBoard[Y][X] >= 1)
						{
							gameCover[Y][X] = gameBoard[Y][X];
							around++;
						}
					}
				}
				around = 0;
				X = setX;
				while (around == 0)//up
				{
					Y--;
					if (Y == -1)
						around++;
					if (around == 0)
					{
						gameCover[Y][X] = gameBoard[Y][X];

						player[turn]++;

						if (gameBoard[Y][X] >= 1)
						{
							gameCover[Y][X] = gameBoard[Y][X];
							around++;
						}
					}
				}
				around = 0;
				Y = setY;
				while (around == 0)//down
				{
					Y++;
					if (Y == 8)
						around++;
					if (around == 0)
					{
						gameCover[Y][X] = gameBoard[Y][X];

						player[turn]++;

						if (gameBoard[Y][X] >= 1)
						{
							gameCover[Y][X] = gameBoard[Y][X];
							around++;
						}
					}
				}
			}//end of if statement
			else if (gameBoard[Y][X] >= 1)//if the value was 1 or over
			{
				player[turn]++;
				gameCover[Y][X] = gameBoard[Y][X];
			}
			else//if the value was negative (mine hit)
				gameOver++;

			printf("\n\n\n\n\n\n\n\n");

			firstRound = 0;//end of first round

			if (players == 2)//move to next player turn
			{
				if (turn == 0)
					turn++;
				else
					turn--;
			}

			save = 32;
			if (gameOver == 0)
			{
				while (save < 0 || save > 1)//ask user about saving
				{
					printf("Would You Like To Save? (0=NO/1=YES): ");
					scanf("%d", &save);
					if (save < 0 || save > 1)
						printf("ERROR\n");
				}
				if (save == 1)//save important values
				{
					mineFile = fopen("minefile.txt", "w");
					fprintf(mineFile, "%d", turn);
					for (int r = 0; r < ROW; r++)
					{
						for (int c = 0; c < 9; c++)
						{
							fprintf(mineFile, "%d", gameBoard[r][c]);
						}
					}
					for (int r = 0; r < ROW; r++)
					{
						for (int c = 0; c < COL; c++)
						{
							fprintf(mineFile, "%d", gameCover[r][c]);
						}
					}
					fprintf(mineFile, "%d%d", player[0], player[1]);
					fprintf(mineFile, "%d", firstRound);
					fclose(mineFile);
				}
			}
		}
	}//end of while loop

	printf("\t\t\t    -GAME OVER-");

	//display the scores
	printf("\n\nPlayer One - %d", player[0]);
	if (players == 2)
	{
		printf("\nPlayer Two - %d", player[1]);
		if(player[0] > player[1])//checks who won
			printf("\nPlayer One Wins");
		else if (player[0] < player[1])
			printf("\nPlayer Two Wins");
		else
			printf("\nPlayers Tied");
	}
}
