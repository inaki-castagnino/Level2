#include "Agregar-lenguaje.h"

using namespace std;

const string TRIGRAMS_PATH = "C:/Users/Iñaki Castagnino/Documents/EDA/2. Lequel - Starter Code/resources/trigrams/";

void buildLanguageProfile(string path) {
	Text newLang;
	TrigramProfile newProf;
	CSVData newData;
	vector<string> aux; 

	getTextFromFile(path, newLang);
	newProf = buildTrigramProfile(newLang);

	for (auto& element : newProf) {

		aux.clear();
		
		aux.push_back(element.first);
		aux.push_back(to_string(element.second));

		newData.push_back(aux);

	}

	writeCSV(TRIGRAMS_PATH,newData);

	return;
}