#include "catalog.h"

using namespace std;


// FUNCTION DEFINITIONS


/*********************************************************************
** Description: Returns true if wizardsFileName and spellbooksFileName could be openede
*********************************************************************/

bool CanOpenFiles(char* wizardsFileName, char* spellbooksFileName) {
	ifstream wizardsFile;	ifstream spellbooksFile;

	wizardsFile.open(wizardsFileName);	spellbooksFile.open(spellbooksFileName);	if (!wizardsFile || !spellbooksFile) {		cout << "Couldnt find inputted database(s)." << endl;		return false;	}	wizardsFile.close();	spellbooksFile.close();

	return true;
}




/*********************************************************************
** Description: Returns true if string is integer
*********************************************************************/
 
bool IsInt(string inputString) {
	for (int i = 0; i < inputString.length(); i++)
	{
		if (inputString[i] < '0' || inputString[i] > '9') {
			return false;
		}
	}
	return true;
}




/*********************************************************************
** Description: Returns an integer inputted by the user
*********************************************************************/

void GetIntInput(string prompt, int& referenceInt) {
	string inputString; cout << prompt; getline(cin, inputString);		if (inputString.length() < 1)		GetIntInput(prompt, referenceInt);	else {		if (!IsInt(inputString)) {			cout << "Invalid input. Please enter integers only." << endl;
			GetIntInput(prompt, referenceInt);		}		else {			referenceInt = atoi(inputString.c_str());		}
	}
}




/*********************************************************************
** Description: Returns a string inputted by the user
*********************************************************************/

void GetStringInput(string prompt, string& referenceString) {
	string inputString; 
	cout << prompt; getline(cin, inputString);
	
	if (inputString.length() < 1)
		GetStringInput(prompt, referenceString);
	else
		referenceString = inputString;
}




/*********************************************************************
** Description: Prints all info associated with a wizard
*********************************************************************/

void PrintWizardInfo(wizard w) {
	cout << "" << endl;
	cout << "Name: " << w.name << endl;
	cout << "ID: " << w.id << endl;	cout << "Position/Title: " << w.position_title << endl;	cout << "Beard Length: " << w.beard_length << endl;
}




/*********************************************************************
** Description: Returns true if wizard is a student
*********************************************************************/

bool IsStudent(wizard w) {
	if (w.position_title == "Student")
		return true;
	return false;
}




/*********************************************************************
** Description: Prompts for and assigns ID & Password
*********************************************************************/

void PromptLoginInfo(int& idInput, string& passwordInput) {	GetIntInput("Please enter ID: ", idInput);	GetStringInput("Please enter password: ", passwordInput);
}




/*********************************************************************
** Description: Returns true and assigns wizard if matching login info could be found in the database
*********************************************************************/

bool CanFindMatchingInfo(int idInput, string passwordInput, string wizardsFileName, wizard& myWizard) {
	ifstream wizardsFile;
	wizardsFile.open(wizardsFileName.c_str());

	int numberOfWizards; wizardsFile >> numberOfWizards;

	for (int i = 0; i < numberOfWizards; i++)
	{
		wizard currentWizard;

		wizardsFile >> currentWizard.name >> currentWizard.id >> currentWizard.password >> currentWizard.position_title >> currentWizard.beard_length;
		if (currentWizard.id == idInput && currentWizard.password == passwordInput) {
			myWizard = currentWizard;
			wizardsFile.close();
			return true;
		}
	}
	wizardsFile.close();
	return false;
}




/*********************************************************************
** Description: Returns true if the user could successfully login before exceeding failed attempts
*********************************************************************/

bool TryLogin(int attempts, int maxAttempts, string wizardsFileName, wizard& myWizard) {
	cout << "" << endl;
	if (attempts > 0)
		cout << "You have made " << attempts << " failed attempts to login." << endl;
	if (attempts > maxAttempts) {
		cout << "The maximum allowed number of failed attempts to login has been exceeded." << endl;
		return false;
	}

	int idInput;
	string passwordInput;

	PromptLoginInfo(idInput, passwordInput);

	if (CanFindMatchingInfo(idInput, passwordInput, wizardsFileName, myWizard))
		return true;
	else
		return TryLogin(++attempts, maxAttempts, wizardsFileName, myWizard);
}




/*********************************************************************
** Description: Prints a prompt to console of the available program functions
*********************************************************************/

void PrintAvailableSelections() {
	cout << " " << endl;
	cout << "Available selections:" << endl;
	cout << "1. Sort spellbooks by # pages (1)," << endl;
	cout << "2. Sort spells by effect (2)," << endl;
	cout << "3. Sort spellbooks by average success rate of contained spells (3)," << endl;
	cout << "4. Quit (4)," << endl;
	cout << " " << endl;
}




/*********************************************************************
** Description: Prompts user for desired program function, and assigns that selection
*********************************************************************/

void PromptProgSelection(string& progSelection) {

	PrintAvailableSelections();

	switch (IntInput("Enter a selection: "))
	{
	case 1:
		progSelection = "SortBooksByPagesCount";
		break;
	case 2:
		progSelection = "SortSpellsByEffect";
		break;
	case 3:
		progSelection = "SortBooksBySuccess";
		break;
	case 4:
		progSelection = "Quit";
		break;
	default:
		cout << "Invalid Selection, try again." << endl;
		PromptProgSelection(progSelection);
		break;
	}
}




/*********************************************************************
** Description: Creates an array of spellbooks
*********************************************************************/

spellbook* create_spellbooks(int spellbookCount) {
	spellbook* spellbookArray = new spellbook[spellbookCount];
	return spellbookArray;
}




/*********************************************************************
** Description: Populates an array of spellbooks
*********************************************************************/

void populate_spellbook_data(spellbook* spellbookArray, int spellbookCount, ifstream& spellbooksFile) {
	// Populate each book
	for (int i = 0; i < spellbookCount; i++)
	{
		spellbooksFile >> spellbookArray[i].title >> spellbookArray[i].author >> spellbookArray[i].num_pages >> spellbookArray[i].edition >> spellbookArray[i].num_spells;

		spellbookArray[i].s = new spell[spellbookArray[i].num_spells];

		// Populate each spell
		for (int j = 0; j < spellbookArray[i].num_spells; j++)
		{
			spellbooksFile >> spellbookArray[i].s[j].name >> spellbookArray[i].s[j].success_rate >> spellbookArray[i].s[j].effect;
		}
	}
}




/*********************************************************************
** Description: Deletes an array of spellbooks
*********************************************************************/

void delete_info(spellbook** spellbookArray, int spellbookCount) {
	// For each spell book, delete all of its spells, then delete itsself.
	for (int j = 0; j < spellbookCount; j++)
	{
		delete[] (*spellbookArray)[j].s;
	}
	delete[] (*spellbookArray);
	*spellbookArray = NULL;
}




/*********************************************************************
** Description: Creates and populates an array of spellbooks based on input file
*********************************************************************/

spellbook* InitializeSpellbooks(int spellbookCount, ifstream& spellbooksFile) {
	spellbook* spellbookArray = create_spellbooks(spellbookCount);
	populate_spellbook_data(spellbookArray, spellbookCount, spellbooksFile);
	return spellbookArray;
}




/*********************************************************************
** Description: Prints non-prohibited spellbooks' title and page counts to console
*********************************************************************/

void PrintTitleAndPageCount(spellbook* spellbookArray, int spellbookCount, bool isStudent) {
	cout << " " << endl;
	cout << "Spellbooks Sorted by Amount of Pages:" << endl;

	for (int i = 0; i < spellbookCount; i++)
	{
		bool prohibited = false;

		for (int j = 0; j < spellbookArray[i].num_spells; j++)
		{
			if (spellbookArray[i].s[j].effect == "death" || spellbookArray[i].s[j].effect == "poison")
				prohibited = true;
		}

		if (!isStudent || !prohibited)
			cout << spellbookArray[i].title << " | " << spellbookArray[i].num_pages << " pages" << endl;
	}
}




/*********************************************************************
** Description: Writes non-prohibited spellbooks' title and page counts to provided destination
*********************************************************************/

void WriteTitleAndPageCount(spellbook* spellbookArray, int spellbookCount, bool isStudent) {
	string fileName;
	GetStringInput("Provide desired file to output information: ", fileName);

	ofstream fileOutput;
	fileOutput.open(fileName.c_str(), ios::app);

	if (fileOutput) {

		fileOutput << "\nSPELLBOOKS BY NUMBER OF PAGES:\n";

		for (int i = 0; i < spellbookCount; i++)
		{
			bool prohibited = false;

			for (int j = 0; j < spellbookArray[i].num_spells; j++)
			{
				if (spellbookArray[i].s[j].effect == "death" || spellbookArray[i].s[j].effect == "poison")
					prohibited = true;
			}

			if (!isStudent || !prohibited)
				fileOutput << spellbookArray[i].title << " | " << spellbookArray[i].num_pages << " pages\n";
		}

		cout << "Wrote to file" << endl;
	}
	
	fileOutput.close();
}




/*********************************************************************
** Description: Prints non-prohibited spells and their effects to console
*********************************************************************/

void PrintSpellAndEffect(spell* spellArray, int spellCount, bool isStudent) {
	cout << " " << endl;
	cout << "Spells Sorted by Effect:" << endl;

	for (int i = 0; i < spellCount; i++)
	{
		bool prohibited = false;

		if (spellArray[i].effect == "death" || spellArray[i].effect == "poison")
			prohibited = true;

		if (!isStudent || !prohibited)
			cout << spellArray[i].effect << " | " << spellArray[i].name << endl;
	}
}




/*********************************************************************
** Description: Writes non-prohibited spells and their effects to provided destination
*********************************************************************/

void WriteSpellAndEffect(spell* spellArray, int spellCount, bool isStudent) {
	string fileName;
	GetStringInput("Provide desired file to output information: ", fileName);

	ofstream fileOutput;
	fileOutput.open(fileName.c_str(), ios::app);

	if (fileOutput) {

		fileOutput << "\nSPELLS BY EFFECT:\n";

		for (int i = 0; i < spellCount; i++)
		{
			bool prohibited = false;

			if (spellArray[i].effect == "death" || spellArray[i].effect == "poison")
				prohibited = true;

			if (!isStudent || !prohibited)
				fileOutput << spellArray[i].effect << " | " << spellArray[i].name << "\n";
		}
		cout << "Wrote to file" << endl;
	}
	fileOutput.close();
}




/*********************************************************************
** Description: Prints non-prohibited spellbooks' title and success rate to console
*********************************************************************/

void PrintTitleAndSuccessRate(spellbook book, bool isStudent, float avSuccess) {
	bool prohibited = false;

	for (int j = 0; j < book.num_spells; j++)
	{
		if (book.s[j].effect == "death" || book.s[j].effect == "poison")
			prohibited = true;
	}

	if (!isStudent || !prohibited)
		cout << book.title << " | " << avSuccess << "%" << endl;
}




/*********************************************************************
** Description: Writes non-prohibited spellbooks' title and success rate to provided destination
*********************************************************************/

void WriteTitleAndSuccessRate(spellbook book, bool isStudent, float avSuccess, ofstream& fileOutput) {
	if (fileOutput) {
		bool prohibited = false;

		for (int j = 0; j < book.num_spells; j++)
		{
			if (book.s[j].effect == "death" || book.s[j].effect == "poison")
				prohibited = true;
		}

		if (!isStudent || !prohibited)
			fileOutput << book.title << " | " << avSuccess << "%\n";
	}
}




/*********************************************************************
** Description: Outputs books by pages based on inputted options
*********************************************************************/

void OutputBooksByPages(int displayOption, spellbook* spellbookArray, int spellbookCount, bool isStudent, string spellbooksFileName) {
	switch (displayOption)
	{
	case 1: // Print to console
		PrintTitleAndPageCount(spellbookArray, spellbookCount, isStudent);
		break;

	case 2: // Write to file
		WriteTitleAndPageCount(spellbookArray, spellbookCount, isStudent);
		break;

	default:
		cout << "Invalid display option." << endl;
		SortBooksByPagesCount(spellbooksFileName, isStudent, spellbookArray, spellbookCount);
		break;
	}
}




/*********************************************************************
** Description: Outputs spells by effect based on inputted options
*********************************************************************/

void OutputSpellsByEffect(int displayOption, spellbook* spellbookArray, int spellbookCount, bool isStudent, string spellbooksFileName, spell* spellArray, int spellCount) {
	switch (displayOption)
	{
	case 1: // Print to console
		PrintSpellAndEffect(spellArray, spellCount, isStudent);
		break;

	case 2: // Write to file
		WriteSpellAndEffect(spellArray, spellCount, isStudent);
		break;

	default:
		cout << "Invalid display option." << endl;
		SortSpellsByEffect(spellbooksFileName, isStudent, spellbookArray, spellbookCount);
		break;
	}
}




/*********************************************************************
** Description: Outputs spellbooks by success based on inputted options
*********************************************************************/

void OutputBooksBySuccess(spellbook* spellbookArray, float* correspondingAvSuccessRates, int spellbookCount, int displayOption, bool isStudent, string spellbooksFileName) {

	string fileName;
	ofstream fileOutput;

	switch (displayOption)
	{
	case 1: // Print to console
		cout << " " << endl;
		cout << "Spellbooks Sorted by Average Success rate:" << endl;

		for (int i = 0; i < spellbookCount; i++)
		{
			PrintTitleAndSuccessRate(spellbookArray[i], isStudent, correspondingAvSuccessRates[i]);
		}
		break;

	case 2: // Write to file
		GetStringInput("Provide desired file to output information: ", fileName);
		fileOutput.open(fileName.c_str(), ios::app);
		fileOutput << "\nSPELLBOOKS BY AVERAGE SUCCESS RATE:\n";

		for (int i = 0; i < spellbookCount; i++)
		{
			WriteTitleAndSuccessRate(spellbookArray[i], isStudent, correspondingAvSuccessRates[i], fileOutput);
		}
		cout << "Wrote to file" << endl;
		fileOutput.close();
		break;

	default:
		cout << "Invalid display option." << endl;
		SortBooksBySuccess(spellbooksFileName, isStudent, spellbookArray, spellbookCount);
		break;
	}

}




/*********************************************************************
** Description: Outputs arrays of wizard information depending on sort type, display option, and other factors
*********************************************************************/

void OutputSortedInformation(spellbook* spellbookArray, spell* spellArray, float* correspondingAvSuccessRates, int spellbookCount, int spellCount, int sortType, int displayOption, bool isStudent, string spellbooksFileName) {
	switch (sortType)
	{
	// CASE 1: SORT BOOKS BY PAGES COUNT
	case 1:
		OutputBooksByPages(displayOption, spellbookArray, spellbookCount, isStudent, spellbooksFileName);
		break;
	// CASE 2: SORT SPELLS BY EFFECT
	case 2: 
		OutputSpellsByEffect(displayOption, spellbookArray, spellbookCount, isStudent, spellbooksFileName, spellArray, spellCount);
		break;
	// CASE 3: SORT BOOKS BY AVERAGE SUCCESS RATE OF SPELLS
	case 3:
		OutputBooksBySuccess(spellbookArray, correspondingAvSuccessRates, spellbookCount, displayOption, isStudent, spellbooksFileName);
		break;
	default:
		break;
	}
}




/*********************************************************************
** Description: Specifically sorts the spell array by effect alphabetically
*********************************************************************/

void SortSpellArrayByEffect(spell* spellArray, int totalSpellsCount) {
	// SORT SPELLS ARRAY BY EFFECT	for (int i = 0; i < totalSpellsCount - 1; i++)
	{		for (int j = 0; j < totalSpellsCount - 1; j++)
		{
			if (spellArray[j].effect > spellArray[j + 1].effect) {
				spell tempSpell;

				// SWAP SPELLS
				tempSpell = spellArray[j];
				spellArray[j] = spellArray[j + 1];
				spellArray[j + 1] = tempSpell;
			}
		}	}
}




/*********************************************************************
** Description: Returns integer input
*********************************************************************/

int IntInput(string prompt) {
	int input;
	GetIntInput(prompt, input);
	return input;
}




/*********************************************************************
** Description: Returns the total number of spells in a spellbook array
*********************************************************************/

int TotalSpellsCount(spellbook* spellbookArray, int spellbookCount) {
	int totalSpellsCount = 0;	// GET NUMBER OF TOTAL SPELLS	for (int i = 0; i < spellbookCount; i++)
	{
		totalSpellsCount += spellbookArray[i].num_spells;
	}
	return totalSpellsCount;
}




/*********************************************************************
** Description: Sorts and outputs spellbooks by their number of pages
*********************************************************************/

void SortBooksByPagesCount(string spellbooksFileName, bool isStudent, spellbook* spellbookArray, int spellbookCount) {
	// SORT SPELLBOOKS BY NUMBER OF PAGES (ASCENDING ORDER)	for (int i = 0; i < spellbookCount - 1; i++)
	{
		for (int j = 0; j < spellbookCount - 1; j++)
		{
			if (spellbookArray[j].num_pages > spellbookArray[j + 1].num_pages) {

				spellbook tempSpellbook;

				// SWAP SPELLBOOKS
				tempSpellbook = spellbookArray[j];
				spellbookArray[j] = spellbookArray[j + 1];
				spellbookArray[j + 1] = tempSpellbook;

			}
		}
	}
	// OUTPUT
	OutputSortedInformation(spellbookArray, NULL, NULL, spellbookCount, 0, 1, IntInput("Print to console (1), or print to file (2): "), isStudent, spellbooksFileName);
}




/*********************************************************************
** Description: Sorts and outputs spells alphabetically by the names of their effects
*********************************************************************/

void SortSpellsByEffect(string spellbooksFileName, bool isStudent, spellbook* spellbookArray, int spellbookCount) {

	int totalSpellsCount = TotalSpellsCount(spellbookArray, spellbookCount);

	spell* spellArray = new spell[totalSpellsCount];

	// POPULATE SPELLS ARRAY
	int currentSpellIteration = 0;
	for (int i = 0; i < spellbookCount; i++)
	{
		for (int j = 0; j < spellbookArray[i].num_spells; j++)
		{
			spellArray[currentSpellIteration++] = spellbookArray[i].s[j];
		}
	}

	SortSpellArrayByEffect(spellArray, totalSpellsCount);
	// OUTPUT
	OutputSortedInformation(spellbookArray, spellArray, NULL, spellbookCount, totalSpellsCount, 2, IntInput("Print to console (1), or print to file (2): "), isStudent, spellbooksFileName);

	delete[] spellArray;
	spellArray = NULL;
}




/*********************************************************************
** Description: Sorts and outputs spellbooks based on their average success rate of all of their spells combined
*********************************************************************/

void SortBooksBySuccess(string spellbooksFileName, bool isStudent, spellbook* spellbookArray, int spellbookCount) {

	float* correspondingAvSuccessRates = new float[spellbookCount];	// CALCULATE AVERAGE SUCCESS RATES FOR EACH BOOK	for (int i = 0; i < spellbookCount; i++)
	{
		float sum = 0;
		for (int j = 0; j < spellbookArray[i].num_spells; j++)
		{
			sum += spellbookArray[i].s[j].success_rate;
		}
		correspondingAvSuccessRates[i] = sum / spellbookArray[i].num_spells;
	}	// SORT SPELLBOOKS BY AVERAGE SUCCESS RATES (ASCENDING)	for (int i = 0; i < spellbookCount - 1; i++)
	{
		for (int j = 0; j < spellbookCount - 1; j++)
		{
			if (correspondingAvSuccessRates[j] > correspondingAvSuccessRates[j + 1]) {

				spellbook tempSpellbook;

				// SWAP SPELLBOOKS
				tempSpellbook = spellbookArray[j];
				spellbookArray[j] = spellbookArray[j + 1];
				spellbookArray[j + 1] = tempSpellbook;

				float tempSuccessRate;

				// SWAP THEIR CORRESPONDING AVERAGE SUCCESS RATES
				tempSuccessRate = correspondingAvSuccessRates[j];
				correspondingAvSuccessRates[j] = correspondingAvSuccessRates[j + 1];
				correspondingAvSuccessRates[j + 1] = tempSuccessRate;
			}
		}
	}
	// OUTPUT
	OutputSortedInformation(spellbookArray, NULL, correspondingAvSuccessRates, spellbookCount, 0, 3, IntInput("Print to console (1), or print to file (2): "), isStudent, spellbooksFileName);

	delete[] correspondingAvSuccessRates;
	correspondingAvSuccessRates = NULL;
}




/*********************************************************************
** Description: Restarts the main loop so user can continue to use program / make new choices / choose different outputs of data
*********************************************************************/

void ResetMainLoop(ifstream& spellbooksFile, spellbook* spellbookArray, int spellbookCount, wizard& myWizard, bool isStudent, string spellbooksFileName) {
	spellbooksFile.close();
	if (spellbookArray != NULL)
		delete_info(&spellbookArray, spellbookCount);	MainLoop(myWizard, isStudent, spellbooksFileName);
}




/*********************************************************************
** Description: This is the primary looping of the program. The user is prompted for a program selection, and then that selection runs, until they select to quit.
*********************************************************************/

void MainLoop(wizard& myWizard, bool isStudent, string spellbooksFileName) {
	string progSelection;	PromptProgSelection(progSelection);	ifstream spellbooksFile;
	spellbooksFile.open(spellbooksFileName.c_str());

	int spellbookCount;
	spellbooksFile >> spellbookCount;

	spellbook* spellbookArray = InitializeSpellbooks(spellbookCount, spellbooksFile);
	if (progSelection == "Quit") {
		cout << "Terminating program." << endl;		spellbooksFile.close();
		if (spellbookArray != NULL)
			delete_info(&spellbookArray, spellbookCount);
	}
	else {
		if (progSelection == "SortBooksByPagesCount") {			SortBooksByPagesCount(spellbooksFileName, isStudent, spellbookArray, spellbookCount);		}		else if (progSelection == "SortSpellsByEffect") {			SortSpellsByEffect(spellbooksFileName, isStudent, spellbookArray, spellbookCount);		}		else if (progSelection == "SortBooksBySuccess") {			SortBooksBySuccess(spellbooksFileName, isStudent, spellbookArray, spellbookCount);		}

		ResetMainLoop(spellbooksFile, spellbookArray, spellbookCount, myWizard, isStudent, spellbooksFileName);
	}
}

