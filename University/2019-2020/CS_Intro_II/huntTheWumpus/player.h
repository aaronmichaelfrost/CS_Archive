#ifndef PLAYER_H
#define PLAYER_H


struct Player {
	bool has_collected_gold;
	bool has_killed_wumpus;

	bool escaped;
	bool killed;
	bool bat_swarmed;

	int row;
	int col;

	int arrows;
};


#endif // !PLAYER_H