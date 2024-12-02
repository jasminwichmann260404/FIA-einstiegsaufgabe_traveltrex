#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
using namespace std;

struct Question
{
    string text;
    vector<string> options;
    int correctOption;
    int timesAsked = 0; // Wie oft die Frage gestellt wurde
};

// Funktion zur Auswahl von selten gestellten Fragen
vector<Question> selectRareQuestions(vector<Question> &questions, int count)
{
    sort(questions.begin(), questions.end(), [](const Question &a, const Question &b)
         { return a.timesAsked < b.timesAsked; });

    vector<Question> selectedQuestions;
    for (int i = 0; i < count && i < (int)questions.size(); ++i)
    {
        selectedQuestions.push_back(questions[i]);
    }

    return selectedQuestions;
}

// Funktion, um eine einzelne Frage zu stellen
bool askQuestion(Question &q, int questionNumber, const string &playerName, bool isMultiplayer)
{
    // Ausgabe der Frage mit oder ohne Spielername, abhängig vom Modus
    if (isMultiplayer)
    {
        cout << "\n"
             << playerName << ", deine Frage " << questionNumber << ": " << q.text << endl;
    }
    else
    {
        cout << "\nFrage " << questionNumber << ": " << q.text << endl;
    }

    for (size_t i = 0; i < q.options.size(); ++i)
        cout << i + 1 << ". " << q.options[i] << endl;

    int userAnswer;
    cout << "Deine Antwort (1-" << q.options.size() << "): ";
    cin >> userAnswer;

    q.timesAsked++;
    return (userAnswer - 1 == q.correctOption);
}

// Funktion zur Anzeige des Ergebnisses
void displayResults(const vector<int> &scores, const vector<string> &playerNames, bool isMultiplayer)
{
    cout << "\nErgebnisse:\n";

    if (isMultiplayer)
    {
        int highestScore = 0;
        vector<string> winners;

        // Ergebnisse anzeigen und den/die Spieler mit der höchsten Punktzahl ermitteln
        for (size_t i = 0; i < playerNames.size(); ++i)
        {
            cout << playerNames[i] << ": " << scores[i] << " Punkte" << endl;
            if (scores[i] > highestScore)
            {
                highestScore = scores[i];
                winners.clear(); // Aktuelle Gewinnerliste leeren
                winners.push_back(playerNames[i]);
            }
            else if (scores[i] == highestScore)
            {
                winners.push_back(playerNames[i]); // Spieler mit gleicher Punktzahl hinzufügen
            }
        }

        // Ausgabe: Gewinner oder Unentschieden
        if (winners.size() == 1)
        {
            cout << "\nGewonnen hat Spieler: " << winners[0] << " mit " << highestScore << " Punkten! Glückwunsch!\n";
        }
        else
        {
            cout << "\nEs ist ein Unentschieden zwischen: ";
            for (size_t i = 0; i < winners.size(); ++i)
            {
                if (i > 0)
                    cout << " und ";
                cout << winners[i];
            }
            cout << " mit jeweils " << highestScore << " Punkten! Gut gemacht!\n";
        }
    }
    else
    {
        cout << playerNames[0] << ", du hast " << scores[0] << " Punkte erreicht.\n";
        if (scores[0] == 10)
        {
            cout << "Du scheinst ein richtiger Experte zu sein! Schaffst du das auch ein zweites Mal?";
        }
        else if (scores[0] == 9)
        {
            cout << "Das war knapp! Schaffst du beim nächsten Mal die volle Punktzahl? ";
        }
        else if (scores[0] == 8)
        {
            cout << "Gut gemacht! Aber da ist noch Luft nach oben!";
        }
        else if (scores[0] == 7)
        {
            cout << "Sehr gut! Du hast viele richtige Antworten gegeben!";
        }
        else if (scores[0] == 6)
        {
            cout << "Sechs Punkte – eine starke Leistung!";
        }
        else if (scores[0] == 5)
        {
            cout << "Gutes Ergebnis – genau die Hälfte der Punkte erzielt!";
        }
        else if (scores[0] == 4)
        {
            cout << "Vier Punkte – du bist auf dem richtigen Weg!";
        }
        else if (scores[0] == 3)
        {
            cout << "Drei von zehn Punkten – eine solide Basis für mehr!";
        }
        else if (scores[0] == 2)
        {
            cout << "Zwei Punkte erreicht – gut gemacht, aber da ist noch Luft nach oben!";
        }
        else if (scores[0] == 1)
        {
            cout << "Ein Punkt! Ein erster Schritt in die richtige Richtung!";
        }
        else
        {
            cout << "Du kannst das besser! Versuch es noch einmal!";
        }
    }
}

// Mehrspielermodus
void playQuiz(vector<Question> &allQuestions, int playerCount, vector<string> &playerNames)
{
    const int questionCount = 10;
    vector<Question> selectedQuestions = selectRareQuestions(allQuestions, questionCount);
    vector<int> scores(playerCount, 0); // Punktestände der Spieler

    bool isMultiplayer = (playerCount > 1);

    // Spieler beantworten abwechselnd die Fragen
    for (size_t i = 0; i < selectedQuestions.size(); ++i)
    {
        int currentPlayer = i % playerCount; // Abwechselnd Spieler
        if (isMultiplayer)                   // Nur anzeigen, wenn es mehr als einen Spieler gibt
        {
            cout << "\nSpieler " << playerNames[currentPlayer] << " ist am Zug!\n";
        }

        if (askQuestion(selectedQuestions[i], i + 1, playerNames[currentPlayer], isMultiplayer))
        {
            cout << "Richtig!\n";
            ++scores[currentPlayer];
        }
        else
        {
            cout << "Falsch! Die richtige Antwort war: "
                 << selectedQuestions[i].options[selectedQuestions[i].correctOption] << ".\n";
        }
    }

    displayResults(scores, playerNames, isMultiplayer);
}

int main()
{
    vector<Question> allQuestions =
        {
            {"Welches Skigebiet hat die meisten Pistenkilometer?", {"Les 3 Vallees", "Meribel", "Kloesterle", "Feldthurns"}, 0},
            {"In welchem Skigebiet gibt es die hoechste Bergstation Europas?", {"Zermatt cervinia", "Spittal an der Dau", "Zermatt Matterhorn", "Stilfts"}, 2},
            {"Welches ist das groesste zusammenhaengende Skigebiet der Welt?", {"Sils Maria", "Golling", "Les 3 Vallees ", "Titisee-Neustadt"}, 2},
            {"Welche Region gilt als schneesicherste in Oesterreich?", {"Arlberg", "Heiligenblut", "Kaprun", "Soell"}, 0},
            {"Wo liegt der Snowpark Absolut Park?", {"livigno", "Saint sorlin", "St.Johann im Pongau", "Flachauwinkl"}, 3},
            {"Wo befindet sich der Snowboarder funpark crazy hole?", {"Bad Gastein", "Pas de la Casa", "Dorfgastein", "Olympiaregion Seefeld"}, 3},
            {"In welchem Skigebiet ist das jaehrliche Hahnenkamm-Rennen?", {"Doellach", "Courchevel", "Kitzbuehel", "Westendorf"}, 2},
            {"Wieviele Skigebiete verbindet les sybelles?", {"5", "8", "4", "6"}, 3},
            {"Welches Skigebiet bietet den hoechsten Drop fuer Freerider?", {"Chamonix-Mont-Blanc", "Nauders", "Vigo di Fassa", "Grindelwald"}, 0},
            {"In welchem Skigebiet steht der Sessellift La Turra?", {"Le Cobier", "Val Thorens", "Val Cenis Vanoise", "Les Arcs"}, 2},
            {"Wie viele Skiroutenkilomenter hat das Skigebiet Gastein", {"30km ", "18km", "25km", "21km "}, 3},
            {"Welches Skigebiet ist fuer den Weissen Ring bekannt?", {"Oberstdorf", "Lech Zuers", "Pinzolo", "Predazzo"}, 1},
            {"Wo befindet sich das Skigebiet mit dem groessten Hoehenunterschied?", {"St.Martin", "Chamonix-Mont-Blanc", "Soldeu", "Brixen im Thale"}, 1},
            {"Welches franzoesische Skigebiet ist Teil von Les Trois Vallees?", {"Valloire", "Courchevel", "Valfrejus", "Le Corbier"}, 1},
            {"Wo liegt das Skigebiet Zermatt-Matterhorn?", {"Schweiz", "Italien", "Deutschland", "Frankreich"}, 0},
            {"Wie lang ist die Abfahrt der Gran Pista?", {"3,5km ", "4km", "3km", "2,5km"}, 0},
            {"Welches italienische Skigebiet hat die laengste beleuchtete Nachtpiste?", {"Andalo", "Monte Bondone", "Obereggen", "Folgaria"}, 2},
            {"Welches Skigebiet hat das groesste Anfaenger-Areal?", {"Rauris", "Innsbruck", "Alpbach", "Alpbach-Wildschoenau"}, 3},
            {"In welchem Skigebiet liegt der Gipfel Roche de mio?", {"Les 2 Alpes", "La Plagne", "Val Thorens", "Val Cenis"}, 1},
            {"Wann wurde die 3S Jandri-Gondelbahn neu eroeffnet?", {"Dezember 2024", "November 2024", "Dezember 2023", "November 2023"}, 0},
        };

    cout << "Willkommen zum Quiz!\n";
    int playerCount;
    cout << "Wie viele Spieler nehmen teil? ";
    cin >> playerCount;
    cin.ignore(); // Eingabe bereinigen

    vector<string> playerNames(playerCount);
    for (int i = 0; i < playerCount; ++i)
    {
        // Unterscheidung zwischen Einzel- und Mehrspielermodus
        if (playerCount > 1)
        {
            cout << "Spieler " << i + 1 << ", bitte gib deinen Namen ein: ";
        }
        else
        {
            cout << "Gib deinen Namen ein: ";
        }
        getline(cin, playerNames[i]);
    }

    char playAgain;
    do
    {
        playQuiz(allQuestions, playerCount, playerNames);

        cout << "\nMoechtet ihr nochmal spielen? (j/n): ";
        cin >> playAgain;
        cin.ignore(); // Für saubere Eingabe nach cin
    } while (tolower(playAgain) == 'j');

    cout << "\nDanke fürs Spielen! Bis zum naechsten Mal!\n";

    return 0;
}