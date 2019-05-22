#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <fstream>
#include <random>
#include <chrono>
#include <cstdlib>
#include <ctime>
using namespace std;


const string TITLE = 
R"(| |                                          
| |__   __ _ _ __   __ _ _ __ ___   __ _ _ __  
| '_ \ / _` | '_ \ / _` | '_ ` _ \ / _` | '_ \
| | | | (_| | | | | (_| | | | | | | (_| | | | |
|_| |_|\__,_|_| |_|\__, |_| |_| |_|\__,_|_| |_|
                    __/ |                      
                   |___/                       )";

const string ALPHABET = "abcdefghijklmnopqrstuvwxyz";

struct next_draw {
    int char_position;  // position in hangman string to draw next char
    char char_to_draw;  // next char to draw
};


vector<string> get_words_from_file(const char *filename)
{
    // Reads words in from file filename, return vector of words
    vector<std::string> words;
    ifstream file(filename);

    if (!file) {
        cout << "Could not open file" << filename << endl;
        exit(1);
    }

    string word;
    while (getline(file, word)) {
        words.push_back(word);
    }

    return words;
}


void draw_guesses(set<char> guesses)
{
    // Draw guessed letters, leaving out letters that have not been guessed
    cout << "Guessed letters: ";
    for (auto ch : ALPHABET) {
        if (guesses.find(ch) != guesses.end()) {
            cout << ch;
        } else {
            cout << '_';
        }
    }
    cout << endl;
}


void draw_word(string word, set<char> guesses)
{
    // Draw word, leaving out unguessed letters
    cout << "Word to guess: ";
    for (auto& ch : word) {
        if (guesses.find(ch) != guesses.end()) {
            cout << ch;
        } else {
            cout << '_';
        }
    }
}


bool word_guessed(string word, set<char> guesses)
{
    // Check if word has been guessed correctly
    for (auto& ch : word) {
        if (guesses.find(ch) == guesses.end()) {
            return false;
        }
    }
    return true;
}


int main()
{
    string hangman = R"(
  +---+
      |
      |
      |
      |
      |
=========)";

    vector<next_draw> draws {
        {11, '|'},
        {19, 'O'},
        {27, '|'},
        {26, '/'},
        {28, '\\'},
        {34, '/'},
        {36, '\\'},
    };  // define

    const int max_num_incorrect_guesses = draws.size() - 1;

    cout << TITLE << endl;

    srand(time(NULL));

    // Choose random word from file
    vector<string> words = get_words_from_file("words.txt");
    string rand_word = words[rand() % words.size()];

    set<char> guesses;
    int num_incorrect_guesses {0};
    bool player_wins = false;
    char letter;

    while (num_incorrect_guesses < max_num_incorrect_guesses) {
        cout << hangman << endl;
        draw_word(rand_word, guesses);
        draw_guesses(guesses);
        cout << "\nGuess a letter: ";

        cin >> letter;

        // Check if already guessed letter
        if (guesses.find(letter) != guesses.end()) {
            cout << "You already guessed that!" << endl;
            continue;
        }
        guesses.insert(letter);

        // Check if guess in word; if not, draw another limb
        if (rand_word.find(letter) == string::npos) {
            cout << "Incorrect guess" << endl;

            // draw next body part on hangman
            next_draw draw = draws[num_incorrect_guesses];
            hangman[draw.char_position] = draw.char_to_draw;

            num_incorrect_guesses++;
        } else {
            cout << "Correct!" << endl;
        }

        if (word_guessed(rand_word, guesses)) {
            player_wins = true;
            break;
        }
    }

    // Check if player has lost (i.e. if loop exited normally)
    if (num_incorrect_guesses == max_num_incorrect_guesses) {
        next_draw draw = draws[num_incorrect_guesses];
        hangman[draw.char_position] = draw.char_to_draw;
        cout << hangman << endl;
        cout << "You lose!" << endl;
    }

    if (player_wins) {
        cout << rand_word << endl;
        cout << "You win!" << endl;
    }

    return 0;
}