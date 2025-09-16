#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <random>

using namespace std;

// Function to load words from a file into a vector
vector<string> load_word_list(const string& filename) {
    vector<string> word_list;
    ifstream file(filename);
    string word;
    if (file.is_open()) {
        while (file >> word) {
            if (word.length() == 5) {
                transform(word.begin(), word.end(), word.begin(), ::tolower);
                word_list.push_back(word);
            }
        }
        file.close();
    }
    return word_list;
}

// Function to filter the word list based on user feedback
void filter_words(vector<string>& words, const string& guess, const string& feedback) {
    words.erase(remove_if(words.begin(), words.end(),
        [&](const string& word) {
            for (int i = 0; i < 5; ++i) {
                if (feedback[i] == 'g') { // Green
                    if (word[i] != guess[i]) {
                        return true;
                    }
                } else if (feedback[i] == 'y') { // Yellow
                    if (word[i] == guess[i] || word.find(guess[i]) == string::npos) {
                        return true;
                    }
                } else { // Black/Gray
                    if (word.find(guess[i]) != string::npos) {
                        // Handle duplicate letters correctly
                        int guess_count = count(guess.begin(), guess.end(), guess[i]);
                        int feedback_count = 0;
                        for(int j=0; j<5; ++j) {
                            if (guess[j] == guess[i] && (feedback[j] == 'g' || feedback[j] == 'y')) {
                                feedback_count++;
                            }
                        }
                        if (count(word.begin(), word.end(), guess[i]) > feedback_count) {
                             return true;
                        }
                    }
                }
            }
            return false;
        }), words.end());
}


// Function to suggest the next guess
string suggest_guess(const vector<string>& words) {
    if (words.empty()) {
        return "";
    }
    // For simplicity, we'll just return a random word from the list.
    // A more advanced solver would calculate the best word to guess.
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, words.size() - 1);
    return words[distrib(gen)];
}

int main() {
    vector<string> words = load_word_list("wordlist.txt");
    if (words.empty()) {
        cerr << "Word list is empty or could not be loaded. Make sure 'wordlist.txt' exists and contains 5-letter words." << endl;
        return 1;
    }

    cout << "Welcome to the Wordle Solver!" << endl;
    cout << "Instructions: Enter a 5-letter feedback string." << endl;
    cout << "Use 'g' for green (correct letter, correct position)." << endl;
    cout << "Use 'y' for yellow (correct letter, wrong position)." << endl;
    cout << "Use 'b' for black/gray (wrong letter)." << endl;
    cout << "Example feedback: gybbg" << endl;


    for (int attempt = 1; attempt <= 6; ++attempt) {
        string guess = suggest_guess(words);
        if (guess.empty()) {
            cout << "No possible words left. Something went wrong." << endl;
            break;
        }

        cout << "\nAttempt " << attempt << ": I suggest you guess: " << guess << endl;
        
        string feedback;
        cout << "Enter the feedback for your guess: ";
        cin >> feedback;

        if (feedback == "ggggg") {
            cout << "Congratulations! We solved it." << endl;
            break;
        }

        if (feedback.length() != 5) {
            cout << "Invalid feedback length. Please enter a 5-character string." << endl;
            --attempt; // Retry this attempt
            continue;
        }

        filter_words(words, guess, feedback);

        if (words.empty()) {
            cout << "No words match the feedback provided." << endl;
            break;
        }
    }

    return 0;
}
