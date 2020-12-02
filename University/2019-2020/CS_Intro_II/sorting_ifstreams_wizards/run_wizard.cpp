/*********************************************************************
** Program Filename: run_wizard.cpp
** Author: Aaron Frost
** Date: 4/12/2020
** Description: Allows user to login and sort spells and spell books based on several factors
** Input: argc, argv, login info, sorting selection
** Output: My wizard info, sorted spells/spellbooks
*********************************************************************/#include "catalog.h"using namespace std;// MAIN FUNCTIONint main(int argc, char** argv) {
	if (argc != 3) {		cout << "Invalid number of arguments. Please enter ./executableName <wizardsFileName> <spellbooksFileName>" << endl;		return 1;	}	if (!CanOpenFiles(argv[1], argv[2]))		return 1;	wizard myWizard;	if (TryLogin(0, 3, argv[1], myWizard)) {		cout << "Successful Login." << endl;		PrintWizardInfo(myWizard);	}	else 		return 1;	MainLoop(myWizard, IsStudent(myWizard), argv[2]);	return 0;}