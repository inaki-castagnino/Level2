/**
 * @brief Lequel? language identification based on trigrams
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 *
 * @cite https://towardsdatascience.com/understanding-cosine-similarity-and-its-application-fd42f585296a
 */

#include <cmath>
#include <codecvt>
#include <locale>
#include <iostream>

#include "Lequel.h"

using namespace std;

/**
 * @brief Builds a trigram profile from a given text.
 *
 * @param text Vector of lines (Text)
 * @return TrigramProfile The trigram profile
 */
TrigramProfile buildTrigramProfile(const Text &text)
{
    wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

    // Your code goes here...
    TrigramProfile perfil;

    for (auto line : text)
    {
        if ((line.length() > 0) &&
            (line[line.length() - 1] == '\r'))
            line = line.substr(0, line.length() - 1);
       
        if (line.length() < 3) {
            continue;
        }

        std::wstring unicodeLine = converter.from_bytes(line);

        // Iterar a través de la línea y extraer trigramas
        for (size_t i = 0; i < (unicodeLine.length() - 2); i++) {
            // Extraer el trigrama como wstring
            std::wstring unicodeTrigram = unicodeLine.substr(i, 3);

            // Convertir el trigrama de wstring a UTF-8 string
            std::string trigram = converter.to_bytes(unicodeTrigram);

            // Incrementar el contador del trigrama en el perfil
            perfil[trigram]++;
        }
    }
    

    // Tip: converts UTF-8 string to wstring
    // wstring unicodeString = converter.from_bytes(textLine);

    // Tip: convert wstring to UTF-8 string
    // string trigram = converter.to_bytes(unicodeTrigram);

    return perfil; // Fill-in result here
}

/**
 * @brief Normalizes a trigram profile.
 *
 * @param trigramProfile The trigram profile.
 */
void normalizeTrigramProfile(TrigramProfile &trigramProfile)
{
    // Your code goes here...
    double norma = 0.0;

    for (auto& element : trigramProfile) {
        norma += element.second * element.second;
    }

    if (norma == 0.0) {
        return;
    }

    norma = sqrtf(norma);
       
    // Normalizar las frecuencias dividiéndolas por la frecuencia total
    for (auto& element : trigramProfile) {
        element.second /= norma;
    }


    return;
}

/**
 * @brief Calculates the cosine similarity between two trigram profiles
 *
 * @param textProfile The text trigram profile
 * @param languageProfile The language trigram profile
 * @return float The cosine similarity score
 */
float getCosineSimilarity(TrigramProfile &textProfile, TrigramProfile &languageProfile)
{
    // Your code goes here...
    float sim = 0.0;

    for (auto& element : textProfile) {
        
        auto it = languageProfile.find(element.first);

        if (it != languageProfile.end()) {
            sim += element.second * it->second;
        }
    }

    return sim; // Fill-in result here
}

/**
 * @brief Identifies the language of a text.
 *
 * @param text A Text (vector of lines)
 * @param languages A list of Language objects
 * @return string The language code of the most likely language
 */
string identifyLanguage(const Text &text, LanguageProfiles &languages)
{
    // Your code goes here...
    TrigramProfile trig;
    trig = buildTrigramProfile(text);
    normalizeTrigramProfile(trig);

    float sim = 0.0f;
    float aux = 0.0f;
    std::string idioma;

    for (auto& element : languages) {
        aux = getCosineSimilarity(trig, element.trigramProfile);

        if (aux > sim) {
            sim = aux;
            idioma = element.languageCode;
        }
    }

    return idioma; // Fill-in result here
}
