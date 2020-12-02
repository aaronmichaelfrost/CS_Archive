#ifndef CATALOG_H
#define CATALOG_H
#include <iostream>#include <fstream>#include <string>#include <cstring>#include <cstdlib>using namespace std;// FUNCTION & DATA TYPE DECLARATIONSstruct wizard {
	string name;
	int id;
	string password;
	string position_title;
	float beard_length;
};struct spellbook {
	string title;
	string author;
	int num_pages;
	int edition;
	int num_spells;
	float avg_success_rate;
	struct spell* s;
};struct spell {
	string name;
	float success_rate;
	string effect;
};bool CanOpenFiles(char* wizardsFileName, char* spellbooksFileName);bool IsInt(string inputString);void GetIntInput(string prompt, int& referenceInt);int IntInput(string prompt);void GetStringInput(string prompt, string& referenceString);void PrintWizardInfo(wizard w);bool IsStudent(wizard w);bool TryLogin(int attempts, int maxAttempts, string wizardsFileName, wizard& myWizard);void PromptLoginInfo(int& idInput, string& passwordInput);bool CanFindMatchingInfo(int idInput, string passwordInput, string wizardsFileName, wizard& myWizard);void PromptProgSelection(string& progSelection);void PrintAvailableSelections();spellbook* create_spellbooks(int spellbookCount);void populate_spellbook_data(spellbook* spellbookArray, int spellbookCount, ifstream& spellbooksFile);void delete_info(spellbook** spellBookArray, int spellbookCount);int TotalSpellsCount(spellbook* spellbookArray, int spellbookCount);void SortBooksByPagesCount(string spellbooksFileName, bool isStudent, spellbook* spellbookArray, int spellbookCount);void PrintTitleAndPageCount(spellbook* spellbookArray, int spellbookCount, bool isStudent);void WriteTitleAndPageCount(spellbook* spellbookArray, int spellbookCount, bool isStudent);void SortSpellsByEffect(string spellbooksFileName, bool isStudent, spellbook* spellbookArray, int spellbookCount);void PrintSpellAndEffect(spell* spellArray, int spellCount, bool isStudent);void WriteSpellAndEffect(spell* spellArray, int spellCount, bool isStudent);void SortSpellArrayByEffect(spell* spellArray, int totalSpellsCount);void SortBooksBySuccess(string spellbooksFileName, bool isStudent, spellbook* spellbookArray, int spellbookCount);void PrintTitleAndSuccessRate(spellbook book, bool isStudent, float avSucess);void WriteTitleAndSuccessRate(spellbook book, bool isStudent, float avSucess, ofstream& fileOutput);spellbook* InitializeSpellbooks(int spellbookCount, ifstream& spellbooksFile);void OutputBooksByPages(int displayOption, spellbook* spellbookArray, int spellbookCount, bool isStudent, string spellbooksFileName);void OutputSpellsByEffect(int displayOption, spellbook* spellbookArray, int spellbookCount, bool isStudent, string spellbooksFileName, spell* spellArray);void OutputBooksBySuccess(spellbook* spellbookArray, float* correspondingAvSuccessRates, int spellbookCount, int displayOption, bool isStudent, string spellbooksFileName);void OutputSortedInformation(spellbook* spellbookArray, spell* spellArray, float* correspondingAvSuccessRates, int spellbookCount, int spellCount, int sortType, int displayType, bool isStudent, string spellbooksFileName);void MainLoop(wizard& myWizard, bool isStudent, string spellbooksFileName);void ResetMainLoop(ifstream& spellbooksfile, spellbook* spellbookArray, int spellbookCount, wizard& myWizard, bool isStudent, string spellbooksFileName);#endif