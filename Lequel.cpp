/**
 * @brief Lequel? Identificaci�n de idioma basada en trigramas
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
 * @param text Vector de l�neas (Texto)
 * @return TrigramProfile El perfil de trigramas
 */
TrigramProfile buildTrigramProfile(const Text& text)
{
    // Conversor de cadenas UTF-8 a UTF-16.
    wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

    // Creaci�n de un perfil de trigramas vac�o.
    TrigramProfile perfil;

    // Iteraci�n sobre cada l�nea del texto.
    for (auto line : text)
    {
        // Eliminar el car�cter de retorno de carro '\r' si est� presente al final de la l�nea.
        if ((line.length() > 0) &&
            (line[line.length() - 1] == '\r'))
            line = line.substr(0, line.length() - 1);

        // Si la l�nea es menor a 3 caracteres, saltarla (no se pueden formar trigramas).
        if (line.length() < 3) {
            continue;
        }

        // Convertir la l�nea a una cadena Unicode (UTF-16).
        std::wstring unicodeLine = converter.from_bytes(line);

        // Iterar a trav�s de la l�nea para extraer trigramas.
        for (int i = 0; i < (unicodeLine.length() - 2); i++) {
            // Extraer un trigrama (tres caracteres) de la l�nea.
            std::wstring unicodeTrigram = unicodeLine.substr(i, 3);

            // Convertir el trigrama de wstring (UTF-16) a string (UTF-8).
            std::string trigram = converter.to_bytes(unicodeTrigram);

            // Incrementar el contador del trigrama en el perfil.
            perfil[trigram]++;
        }
    }

    // Retornar el perfil de trigramas generado.
    return perfil;
}

/**
 * @brief Normaliza un perfil de trigramas.
 *
 * @param trigramProfile El perfil de trigramas.
 */
void normalizeTrigramProfile(TrigramProfile& trigramProfile)
{
    // Inicializar la norma.
    double norma = 0.0;

    // Calcular la suma de los cuadrados de las frecuencias de trigramas.
    for (auto& element : trigramProfile) {
        norma += element.second * element.second;
    }

    // Si la norma es cero (no hay trigramas en el perfil), salir de la funci�n.
    if (norma == 0.0) {
        return;
    }

    // Tomar la ra�z cuadrada de la suma para obtener la norma.
    norma = sqrtf(norma);

    // Normalizar las frecuencias dividi�ndolas por la norma.
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
    // Inicializar la similitud en cero.
    float sim = 0.0;

    // Iterar a trav�s del perfil de trigramas del texto.
    for (auto& element : textProfile) {
        // Buscar el trigrama en el perfil del idioma.
        auto it = languageProfile.find(element.first);

        // Si el trigrama est� presente en el perfil del idioma, calcular la contribuci�n a la similitud.
        if (it != languageProfile.end()) {
            sim += element.second * it->second;
        }
    }

    // Retornar la similitud calculada.
    return sim;
}

/**
 * @brief Identifica el idioma de un texto.
 *
 * @param text Un texto (vector de l�neas).
 * @param languages Una lista de objetos Language que contienen perfiles de trigramas para distintos idiomas.
 * @return string El c�digo del idioma m�s probable.
 */
string identifyLanguage(const Text& text, LanguageProfiles& languages)
{
    // Construir el perfil de trigramas del texto.
    TrigramProfile trig = buildTrigramProfile(text);

    // Normalizar el perfil de trigramas del texto.
    normalizeTrigramProfile(trig);

    // Inicializar las variables para la similitud m�s alta y el idioma correspondiente.
    float sim = 0.0f;
    float aux = 0.0f;
    std::string idioma;

    // Iterar a trav�s de los perfiles de idiomas.
    for (auto& element : languages) {
        // Calcular la similitud de coseno entre el perfil del texto y el perfil del idioma actual.
        aux = getCosineSimilarity(trig, element.trigramProfile);

        // Si la similitud calculada es mayor que la anterior, actualizar la similitud m�s alta y el idioma.
        if (aux > sim) {
            sim = aux;
            idioma = element.languageCode;
        }
    }

    // Retornar el c�digo del idioma que obtuvo la mayor similitud.
    return idioma;
}
