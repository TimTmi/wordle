#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <random>
#include <chrono>



using namespace std;



// Text colors
const string BLACK = "\033[30m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";
const string MAGENTA = "\033[35m";
const string CYAN = "\033[36m";
const string WHITE = "\033[37m";
const string GRAY = "\033[90m";
const string BRIGHT_RED = "\033[91m";
const string BRIGHT_GREEN = "\033[92m";
const string BRIGHT_YELLOW = "\033[93m";
const string BRIGHT_BLUE = "\033[94m";
const string BRIGHT_MAGENTA = "\033[95m";
const string BRIGHT_CYAN = "\033[96m";
const string BRIGHT_WHITE = "\033[97m";

// Background colors
const string BG_BLACK = "\033[40m";
const string BG_RED = "\033[41m";
const string BG_GREEN = "\033[42m";
const string BG_YELLOW = "\033[43m";
const string BG_BLUE = "\033[44m";
const string BG_MAGENTA = "\033[45m";
const string BG_CYAN = "\033[46m";
const string BG_WHITE = "\033[47m";
const string BG_BRIGHT_BLACK = "\033[100m";
const string BG_BRIGHT_RED = "\033[101m";
const string BG_BRIGHT_GREEN = "\033[102m";
const string BG_BRIGHT_YELLOW = "\033[103m";
const string BG_BRIGHT_BLUE = "\033[104m";
const string BG_BRIGHT_MAGENTA = "\033[105m";
const string BG_BRIGHT_CYAN = "\033[106m";
const string BG_BRIGHT_WHITE = "\033[107m";

// Text effects
const string BOLD = "\033[1m";
const string DIM = "\033[2m";
const string UNDERLINE = "\033[4m";
const string REVERSE = "\033[7m";
const string HIDDEN = "\033[8m";

// Reset
const string RESET = "\033[0m";
const string RESET_BOLD = "\033[22m";
const string RESET_UNDERLINE = "\033[24m";
const string RESET_REVERSE = "\033[27m";
const string RESET_HIDDEN = "\033[28m";
const string RESET_COLOR = "\033[39m";
const string RESET_BG_COLOR = "\033[49m";

// Line & cursor manipulation
const string CLEAR_LINE = "\033[2K\033[F";



int random_int(int min, int max) {
    static mt19937 gen(static_cast<unsigned int>(chrono::system_clock::now().time_since_epoch().count()));
    uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

int max(int a, int b) {
    return a > b ? a : b;
}

struct AVLTreeNode {
    int score;
    string name;
    AVLTreeNode *left, *right;
    int height;

    AVLTreeNode(int k) : score(k), name("") , left(nullptr), right(nullptr), height(1) {}
};

class AVLTree {
private:
    AVLTreeNode *root;

    void rotate_left(AVLTreeNode *&node) {
        AVLTreeNode *right_node = node->right, *min_node = right_node->left;

        right_node->left = node;
        node->right = min_node;

        node->height = max(get_height(node->left), get_height(node->right)) + 1;
        right_node->height = max(get_height(right_node->left), get_height(right_node->right)) + 1;

        node = right_node;
    }

    void rotate_right(AVLTreeNode *&node) {
        AVLTreeNode *left_node = node->left, *max_node = left_node->right;

        left_node->right = node;
        node->left = max_node;

        node->height = max(get_height(node->left), get_height(node->right)) + 1;
        left_node->height = max(get_height(left_node->left), get_height(left_node->right)) + 1;

        node = left_node;
    }

    void rotate_left_right(AVLTreeNode *&node) {
        rotate_left(node->left);
        rotate_right(node);
    }

    void rotate_right_left(AVLTreeNode *&node) {
        rotate_right(node->right);
        rotate_left(node);
    }
    
    int get_height(AVLTreeNode *node) {
        if (!node) return 0;
        return node->height;
    }

    int get_balance(AVLTreeNode *node) {
        if (!node) return 0;
        return get_height(node->left) - get_height(node->right);
    }

    void insert_internal(AVLTreeNode *&node, int score, const string& name) {
        if (!node) {
            node = new AVLTreeNode(score);
            node->name = name;
            return;
        }

        if (score < node->score) insert_internal(node->left, score, name);
        else if (score > node->score) insert_internal(node->right, score, name);
        else return;

        node->height = max(get_height(node->left), get_height(node->right)) + 1;

        int balance = get_balance(node);

        if (balance > 1) {
            if (score < node->left->score) rotate_right(node);
            else rotate_left_right(node);
        } else if (balance < -1) {
            if (score > node->right->score) rotate_left(node);
            else rotate_right_left(node);
        }
    }

    void print_RNL_internal(AVLTreeNode *node) {
        if (!node) return;

        print_RNL_internal(node->right);
        cout << BOLD << RED;
        cout << "Name: " << node->name << "\t Score: " << node->score << endl;
        cout << RESET;
        print_RNL_internal(node->left);
    }

public:
    AVLTree(): root(nullptr) {}

    void insert(int score, const string& name) {
        insert_internal(root, score, name);
    }
    
    int height() {
        return get_height(root);
    }
    
    
    void print_postorder() {
        print_RNL_internal(root);
        cout << endl;
    }
};



void clear_screen() {
    cout << "\033[2J\033[H";
}

void move_cursor_to(int row, int column = 1) {
    cout << "\033[" << row << ";" << column << "H";
}

void move_cursor_up(int lines) {
    cout << "\033[" << lines << "A";
}

void move_cursor_down(int lines) {
    cout << "\033[" << lines << "B";
}

void clear_line() {
    cout << "\033[2K";
}

void save_cursor_position() {
    cout << "\033[s";
}

void restore_cursor_position() {
    cout << "\033[u";
}

string effect(const string &str, const string &effect, bool reset = true) {
    return effect + str + (reset ? RESET : string());
}



void pause();
void show_main_menu();
void submit_score();
void show_rules();
void show_credits();
void start_game();
void show_leaderboard();
void exit();



AVLTree leaderboard;

class WordList {
private:
    vector<string> list;
    int index;
    mt19937 gen;

public:
    WordList(): index(0), gen(static_cast<uint64_t>(std::chrono::system_clock::now().time_since_epoch().count())) {}

    void add_word(const string &word) {
        list.emplace_back(move(word));
    }

    const string& get_word() {
        index %= list.size();
        if (index == 0) shuffle(list.begin(), list.end(), gen);
        return list[index++];
    }

    bool word_exists(const string &word) {
        return find(list.begin(), list.end(), word) != list.end();
    }
};

WordList valid_guesses;
WordList secret_words;



int main() {
    string word;

    ifstream ifs_valid_guesses("valid-guesses.txt");
    if (!ifs_valid_guesses.is_open()) {
        cout << RED << "Missing word list: \"valid-guesses.txt\"\n\n";
        pause();
        return -1;
    }

    ifstream ifs_secret_words("secret-words.txt");
    if (!ifs_secret_words.is_open()) {
        cout << RED << "Missing word list: \"secret-words.txt\"\n\n";
        pause();
        return -1;
    }

    while (getline(ifs_valid_guesses, word)) valid_guesses.add_word(word);
    while (getline(ifs_secret_words, word)) secret_words.add_word(word);

    show_main_menu();

    return 0;
}



void pause() {
    string idk;
    cout << "Enter anything to continue...";
    getline(cin, idk);
}

void show_main_menu() {
    clear_screen();

    cout << YELLOW;
    cout << "                                 '||  '||          " << '\n';
    cout << "... ... ...   ...   ... ..     .. ||   ||    ....  " << '\n';
    cout << " ||  ||  |  .|  '|.  ||' ''  .'  '||   ||  .|...|| " << '\n';
    cout << "  ||| |||   ||   ||  ||      |.   ||   ||  ||      " << '\n';
    cout << "   |   |     '|..|' .||.     '|..'||. .||.  '|...' " << '\n';
    cout << RESET;
    cout << GREEN << BOLD;
    cout << '\n';
    cout << "Play \n";
    cout << "Rules \n";
    cout << "Credits \n";
    cout << "Leaderboard \n";
    cout << "Exit \n\n";
    cout << RESET;

    string command;
    cout << "Enter command: ";
    getline(cin, command);
    for (char &c: command) {
        c = tolower(c);
    }

    if (command == "play")
        start_game();

    else if (command == "rules")
        show_rules();

    else if (command == "credits")
        show_credits();

    else if (command == "leaderboard")
        show_leaderboard();

    else if (command == "exit")
        exit();

    else {
        cout << "Unknown command \n";
        pause();
    }

    show_main_menu();
}

void dismiss_word(const string &message) {
    move_cursor_down(1);
    clear_line();
    cout << message;
    restore_cursor_position();
    clear_line();
}

void update_alphabet(string &alphabet, char letter, char state) {
    int position = int(letter) - int('A');
    if (state <= alphabet[position]) return;

    alphabet[position] = state;
    move_cursor_to(1, position + 1);

    switch (state) {
        case '1':
            cout << GRAY;
            break;

        case '2':
            cout << YELLOW;
            break;

        case '3':
            cout << GREEN;
            break;
    }

    cout << letter << RESET_COLOR;
    restore_cursor_position();
}

void start_game() {
    string difficulty;
    bool show_alphabet = true;
    int guesses;

    while (true) {
        clear_screen();

        cout << "Select difficulty \n";
        cout << GREEN << "\tEasy \n";
        cout << YELLOW << "\tMedium \n";
        cout << RED << "\tHard \n" << RESET;
        cout << "\tCustom \n\n";
        getline(cin, difficulty);

        for (char &c: difficulty) {
            c = tolower(c);
        }

        if (difficulty == "easy") {
            guesses = 8;
            break;
        }
        
        else if (difficulty == "medium") {
            guesses = 6;
            break;
        }

        else if (difficulty == "hard") {
            guesses = 4;
            break;
        }
        
        else if (difficulty == "custom") {
            cin.ignore();
            cout << "custom";
            cout << "Number of guesses: ";
            cin >> guesses;
            cout << "Show alphabet (0 / 1): ";
            cin >> show_alphabet;
            cin.ignore();
            break;
        }
    }

    const string &secret_word = secret_words.get_word();

    string alphabet = "00000000000000000000000000";

    clear_screen();

    cout << "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    move_cursor_to(4);

    cout << '\t';
    for (int i = 0; i < 5; i++) {
        cout << '_';
    }
    cout << '\n';

    while (guesses > 0) {
        save_cursor_position();
        // move_cursor_to(1);
        // cout << alphabet << '\n';
        move_cursor_to(2);
        cout << "Guesses left: " << guesses << "\n\n\n";
        // cout << BG_WHITE << "                          " << RESET;
        restore_cursor_position();
        move_cursor_down(1);
        clear_line();
        restore_cursor_position();

        string guess;
        cout << '\t';
        getline(cin, guess);

        if (guess.length() != 5) {
            dismiss_word("Invalid guess: word needs to be 5 letters long");
            continue;
        }

        for (char &c: guess) {
            c = toupper(c);
        }

        if (!valid_guesses.word_exists(guess) && !secret_words.word_exists(guess)) {
            dismiss_word("Invalid guess: word is not in word list");
            continue;
        }

        guesses--;

        string secret_word_temp = secret_word;
        string colored_guess;

        for (size_t i = 0, length = guess.length(); i < length; ++i) {
            char c = guess[i];
            if (c == toupper(secret_word_temp[i])) {
                colored_guess += GREEN + string(1, c) + RESET;
                secret_word_temp[i] = '-';
                update_alphabet(alphabet, c, '3');
            }

            else {
                int correct_position = secret_word_temp.find(c);

                if (correct_position != string::npos) {
                    colored_guess += YELLOW + string(1, c) + RESET;
                    secret_word_temp[correct_position] = tolower(secret_word_temp[correct_position]);
                    update_alphabet(alphabet, c, '2');
                }

                else {
                    colored_guess += GRAY + string(1, c) + RESET;
                    update_alphabet(alphabet, c, '1');
                }
            }
        }

        // move_cursor_up(1);
        cout << '\t' << colored_guess << '\n';

        if (guess == secret_word) {
            cout << '\n';
            cout << "Correct! \n";
            break;
        }
    }

    pause();

    submit_score();
}

void submit_score() {
    cout << GREEN;
    cout << "Enter your score: \n";
    cout << RESET;
    cout << RED;
    int score;
    cin.ignore();
    cin >> score;
    cin.ignore();
    cout << RESET;

    cout << GREEN;
    cout << "Enter your name: \n";
    cout << RESET;

    cout <<RED;
    string name;
    getline(cin, name);
    cout << RESET;
    leaderboard.insert(score, name);
}

void show_rules() {
    clear_screen();

    cout << '\a';
    cout << CYAN << BOLD << UNDERLINE;
    cout << "\tRULES\n\n";
    cout << RESET;

    cout << CYAN;
    cout << "The rules of Wordle are elegantly simple. \n";
    cout << "Your objective is to guess a secret five-letter word before you run out of guesses. \n";
    cout << "To submit a guess, type any five-letter word and press Enter. \n";
    cout << "All of your guesses must be real words, according to a dictionary of five-letter words that Wordle allows as guesses. \n";
    cout << "You cannot make up a non-existent word, like AEIOU, just to guess those letters. \n\n";

    cout << "As soon as you have submitted your guess, the game will color-code each letter in your guess to tell you how close it \n";
    cout << "was to the letters in the hidden word. \n";
    cout << "A" << GRAY << BOLD << " GRAY " << RESET << CYAN << "letter means that this letter does not appear in the secret word at all. \n";
    cout << "A" << YELLOW << BOLD << " YELLOW " << RESET << CYAN << "letter means that this letter appears in the secret word, but it is in the wrong spot within the word. \n";
    cout << "A" << GREEN << BOLD << " GREEN " << RESET << CYAN << "letter means that this letter appears in the secret word, and it is in exactly the right place. \n";
    cout << "Getting a green square or yellow square will get you closer to guessing the real secret word, since it means you have \n";
    cout << "guessed a correct letter. \n\n";

    cout << RESET;

    pause();
}

void show_leaderboard() {
    clear_screen();

    cout << GREEN << BOLD << "LEADERBOARD" << RESET << endl;
    leaderboard.print_postorder();

    pause();
}

void show_credits() {
    clear_screen();

    cout << RED << BOLD;
    cout << "Name: Tran Anh Khoa \t MSSV: 23127395 \n";
    cout << "Name: Nguyen Kien Hao \t MSSV: 23127290 \n";
    cout << "Name: Nguyen Cong Toan \t MSSV: 23127500 \n";
    cout << "Name: Tran Huu Luong \t MSSV: 23127415 \n\n";
    cout << RESET;

    pause();
}

void exit() {
    cout << "Exiting the program..." << endl;
    exit(0); 
}