/**
 * @brief Lequel? Identificación de idioma basada en trigramas
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
 * @brief Construye un perfil de trigramas a partir de un texto dado.
 *
 * @param text Vector de líneas (Texto)
 * @return TrigramProfile El perfil de trigramas
 */
TrigramProfile buildTrigramProfile(const Text& text)
{

    wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

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

        for (int i = 0; i < (unicodeLine.length() - 2); i++) {
            std::wstring unicodeTrigram = unicodeLine.substr(i, 3);
            std::string trigram = converter.to_bytes(unicodeTrigram);
            perfil[trigram]++;
        }
    }
    return perfil;
}

/**
 * @brief Normaliza un perfil de trigramas.
 *
 * @param trigramProfile El perfil de trigramas.
 */
void normalizeTrigramProfile(TrigramProfile& trigramProfile)
{
    double norma = 0.0;

    for (auto& element : trigramProfile) {
        norma += element.second * element.second;
    }

    if (norma == 0.0) {
        return;
    }

    norma = sqrtf(norma);

    for (auto& element : trigramProfile) {
        element.second /= norma;
    }
}

/**
 * @brief Calcula la similitud de coseno entre dos perfiles de trigramas.
 *
 * @param textProfile El perfil de trigramas del texto.
 * @param languageProfile El perfil de trigramas del idioma.
 * @return float El puntaje de similitud de coseno.
 */
float getCosineSimilarity(TrigramProfile& textProfile, TrigramProfile& languageProfile)
{
    float highestSimilarity = 0.0;

    for (const auto& element : textProfile) {
        auto it = languageProfile.find(element.first);
        if (it != languageProfile.end()) {
            highestSimilarity += element.second * it->second;
        }
    }
    return highestSimilarity;
}

/**
 * @brief Identifica el idioma de un texto.
 *
 * @param text Un texto (vector de líneas).
 * @param languages Una lista de objetos Language que contienen perfiles de trigramas para distintos idiomas.
 * @return string El código del idioma más probable.
 */
string identifyLanguage(const Text& text, LanguageProfiles& languages)
{
    TrigramProfile trig = buildTrigramProfile(text);

    normalizeTrigramProfile(trig);

    float highestSimilarity = 0.0f;
    float similarity = 0.0f;
    std::string idioma;

    for (auto& element : languages) {
        similarity = getCosineSimilarity(trig, element.trigramProfile);

        if (similarity > highestSimilarity) {
            highestSimilarity = similarity;
            idioma = element.languageCode;
        }
    }

    return idioma;
}
