#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <random>
#include <chrono>



using namespace std;


class ANSICodes {
public:
    // Text colors
    static constexpr const char* BLACK = "\033[30m";
    static constexpr const char* RED = "\033[31m";
    static constexpr const char* GREEN = "\033[32m";
    static constexpr const char* YELLOW = "\033[33m";
    static constexpr const char* BLUE = "\033[34m";
    static constexpr const char* MAGENTA = "\033[35m";
    static constexpr const char* CYAN = "\033[36m";
    static constexpr const char* WHITE = "\033[37m";
    static constexpr const char* GRAY = "\033[90m";
    static constexpr const char* BRIGHT_RED = "\033[91m";
    static constexpr const char* BRIGHT_GREEN = "\033[92m";
    static constexpr const char* BRIGHT_YELLOW = "\033[93m";
    static constexpr const char* BRIGHT_BLUE = "\033[94m";
    static constexpr const char* BRIGHT_MAGENTA = "\033[95m";
    static constexpr const char* BRIGHT_CYAN = "\033[96m";
    static constexpr const char* BRIGHT_WHITE = "\033[97m";

    // Background colors
    static constexpr const char* BG_BLACK = "\033[40m";
    static constexpr const char* BG_RED = "\033[41m";
    static constexpr const char* BG_GREEN = "\033[42m";
    static constexpr const char* BG_YELLOW = "\033[43m";
    static constexpr const char* BG_BLUE = "\033[44m";
    static constexpr const char* BG_MAGENTA = "\033[45m";
    static constexpr const char* BG_CYAN = "\033[46m";
    static constexpr const char* BG_WHITE = "\033[47m";
    static constexpr const char* BG_BRIGHT_BLACK = "\033[100m";
    static constexpr const char* BG_BRIGHT_RED = "\033[101m";
    static constexpr const char* BG_BRIGHT_GREEN = "\033[102m";
    static constexpr const char* BG_BRIGHT_YELLOW = "\033[103m";
    static constexpr const char* BG_BRIGHT_BLUE = "\033[104m";
    static constexpr const char* BG_BRIGHT_MAGENTA = "\033[105m";
    static constexpr const char* BG_BRIGHT_CYAN = "\033[106m";
    static constexpr const char* BG_BRIGHT_WHITE = "\033[107m";

    // Text effects
    static constexpr const char* BOLD = "\033[1m";
    static constexpr const char* DIM = "\033[2m";
    static constexpr const char* UNDERLINE = "\033[4m";
    static constexpr const char* REVERSE = "\033[7m";
    static constexpr const char* HIDDEN = "\033[8m";

    // Reset
    static constexpr const char* RESET = "\033[0m";
    static constexpr const char* RESET_BOLD = "\033[22m";
    static constexpr const char* RESET_UNDERLINE = "\033[24m";
    static constexpr const char* RESET_REVERSE = "\033[27m";
    static constexpr const char* RESET_HIDDEN = "\033[28m";
    static constexpr const char* RESET_COLOR = "\033[39m";
    static constexpr const char* RESET_BG_COLOR = "\033[49m";

    // Line & cursor manipulation
    static constexpr const char* CLEAR_LINE = "\033[2K\033[F";
};



// int random_int(int min, int max) {
//     static mt19937 gen(static_cast<unsigned int>(chrono::system_clock::now().time_since_epoch().count()));
//     uniform_int_distribution<> dis(min, max);
//     return dis(gen);
// }

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

    void clear(AVLTreeNode *node) {
        if (!node) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

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
        cout << ANSICodes::BOLD << ANSICodes::RED;
        cout << "Name: " << node->name << "\t Score: " << node->score << endl;
        cout << ANSICodes::RESET;
        print_RNL_internal(node->left);
    }

public:
    AVLTree(): root(nullptr) {}

    ~AVLTree() {
        clear(root);
    }

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


class ConsoleDisplay {
public:
    static void clear_screen() {
        cout << "\033[2J\033[H";
    }

    static void move_cursor_to(int row, int column = 1) {
        cout << "\033[" << row << ";" << column << "H";
    }

    static void move_cursor_up(int lines) {
        cout << "\033[" << lines << "A";
    }

    static void move_cursor_down(int lines) {
        cout << "\033[" << lines << "B";
    }

    static void clear_line() {
        cout << "\033[2K";
    }

    static void save_cursor_position() {
        cout << "\033[s";
    }

    static void restore_cursor_position() {
        cout << "\033[u";
    }
};



class WordList {
private:
    vector<string> list;
    int index;
    mt19937 gen;

public:
    WordList(): index(0), gen(static_cast<uint64_t>(std::chrono::system_clock::now().time_since_epoch().count())) {}

    void add_word(const string &word) {
        list.emplace_back(word);
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

AVLTree leaderboard;
WordList valid_guesses;
WordList secret_words;

void pause() {
    cout << "Enter anything to continue...";
    cin.get();
}

class Game {
private:
    // void pause();
    // void show_main_menu();
    // void submit_score();
    // void show_rules();
    // void show_credits();
    // void start_game();
    // void show_leaderboard();
    // void exit_game();

    void show_main_menu() {
        ConsoleDisplay::clear_screen();

        cout << ANSICodes::YELLOW;
        cout << "                                 '||  '||          " << '\n';
        cout << "... ... ...   ...   ... ..     .. ||   ||    ....  " << '\n';
        cout << " ||  ||  |  .|  '|.  ||' ''  .'  '||   ||  .|...|| " << '\n';
        cout << "  ||| |||   ||   ||  ||      |.   ||   ||  ||      " << '\n';
        cout << "   |   |     '|..|' .||.     '|..'||. .||.  '|...' " << '\n';
        cout << ANSICodes::RESET;
        cout << ANSICodes::GREEN << ANSICodes::BOLD;
        cout << '\n';
        cout << "Play \n";
        cout << "Rules \n";
        cout << "Credits \n";
        cout << "Leaderboard \n";
        cout << "Exit \n\n";
        cout << ANSICodes::RESET;

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
            exit_game();

        else {
            cout << "Unknown command \n";
            pause();
        }

        show_main_menu();
    }

    void dismiss_word(const string &message) {
        ConsoleDisplay::move_cursor_down(1);
        // ConsoleDisplay::clear_line();
        cout << message;
        ConsoleDisplay::restore_cursor_position();
        ConsoleDisplay::clear_line();
    }

    void update_alphabet(string &alphabet, char letter, char state) {
        int position = int(letter) - int('A');
        if (state <= alphabet[position]) return;

        alphabet[position] = state;
        ConsoleDisplay::move_cursor_to(1, position * 2 + 1);

        switch (state) {
            case '1':
                cout << ANSICodes::GRAY;
                break;

            case '2':
                cout << ANSICodes::YELLOW;
                break;

            case '3':
                cout << ANSICodes::GREEN;
                break;
        }

        cout << letter << ANSICodes::RESET_COLOR;
        // ConsoleDisplay::restore_cursor_position();
    }

    void start_game() {
        string difficulty;
        bool show_alphabet = true;
        int guesses;

        while (true) {
            ConsoleDisplay::clear_screen();

            cout << "Select difficulty \n";
            cout << ANSICodes::GREEN << "\tEasy \n";
            cout << ANSICodes::YELLOW << "\tMedium \n";
            cout << ANSICodes::RED << "\tHard \n" << ANSICodes::RESET;
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
        bool correct = false;
        string alphabet = "00000000000000000000000000";

        ConsoleDisplay::clear_screen();

        cout << "A B C D E F G H I J K L M N O P Q R S T U V W X Y Z";

        ConsoleDisplay::move_cursor_to(5);

        cout << '\t';
        for (int i = 0; i < 5; i++) {
            cout << '_';
        }
        cout << '\n';

        while (guesses > 0) {
            ConsoleDisplay::save_cursor_position();
            ConsoleDisplay::move_cursor_to(3);
            ConsoleDisplay::clear_line();
            cout << "Guesses left: " << guesses << "\n\n\n";
            ConsoleDisplay::restore_cursor_position();

            string guess;
            cout << '\t';
            getline(cin, guess);

            ConsoleDisplay::move_cursor_down(1);
            ConsoleDisplay::clear_line();
            ConsoleDisplay::move_cursor_up(1);

            if (guess.length() != 5) {
                dismiss_word("Invalid guess: word needs to be 5 letters long");
                continue;
            }

            for (char &c: guess) {
                c = toupper(c);
            }

            if (!valid_guesses.word_exists(guess) && !secret_words.word_exists(guess)) {
                dismiss_word("Invalid guess: \'" + guess + "\' is not in word list");
                continue;
            }

            guesses--;

            string secret_word_temp = secret_word;
            string colored_guess;

            for (size_t i = 0, length = guess.length(); i < length; ++i) {
                char c = guess[i];
                if (c == toupper(secret_word_temp[i])) {
                    colored_guess += ANSICodes::GREEN + string(1, c) + ANSICodes::RESET;
                    secret_word_temp[i] = '-';
                    update_alphabet(alphabet, c, '3');
                }

                else {
                    bool handled = false;

                    while (true) {
                        int correct_position = secret_word_temp.find(c);

                        if (correct_position == string::npos) {
                            break;
                        }

                        if (guess[correct_position] == secret_word_temp[correct_position]) {
                            secret_word_temp[correct_position] = tolower(secret_word_temp[correct_position]);
                            continue;
                        }

                        colored_guess += ANSICodes::YELLOW + string(1, c) + ANSICodes::RESET;
                        secret_word_temp[correct_position] = tolower(secret_word_temp[correct_position]);
                        update_alphabet(alphabet, c, '2');
                        handled = true;
                        break;
                    }

                    if (!handled) {
                        colored_guess += ANSICodes::GRAY + string(1, c) + ANSICodes::RESET;
                        update_alphabet(alphabet, c, '1');
                    }
                }
            }

            ConsoleDisplay::restore_cursor_position();
            cout << '\t' << colored_guess << '\n';

            if (guess == secret_word) {
                correct = true;
                break;
            }
        }

        cout << '\n';

        if (correct) {
            cout << "You won! \n";
        }

        else {
            cout << "You lost! \n";
            cout << "The secret word is \'" << secret_word << "\' \n";
        }

        submit_score(guesses);

        string play_again;

        while (true) {
            ConsoleDisplay::clear_screen();
            cout << "Play again? \n";
            cout << ANSICodes::GRAY << "(Yes / No) \n" << ANSICodes::RESET_COLOR;
            getline(cin, play_again);

            for (char &c: play_again) {
                c = tolower(c);
            }

            if (play_again == "yes") {
                start_game();
                break;
            }

            else if (play_again == "no") break;
        }
    }

    void submit_score(int score) {
        cout << ANSICodes::GREEN;
        cout << "Enter your name: \n";
        cout << ANSICodes::RESET;

        cout << ANSICodes::RED;
        string name;
        getline(cin, name);
        cout << ANSICodes::RESET;
        leaderboard.insert(score, name);
    }

    void show_rules() {
        ConsoleDisplay::clear_screen();

        cout << '\a';
        cout << ANSICodes::CYAN << ANSICodes::BOLD << ANSICodes::UNDERLINE;
        cout << "\tRULES\n\n";
        cout << ANSICodes::RESET;

        cout << ANSICodes::CYAN;
        cout << "The rules of Wordle are elegantly simple. \n";
        cout << "Your objective is to guess a secret five-letter word before you run out of guesses. \n";
        cout << "To submit a guess, type any five-letter word and press Enter. \n";
        cout << "All of your guesses must be real words, according to a dictionary of five-letter words that Wordle allows as guesses. \n";
        cout << "You cannot make up a non-existent word, like AEIOU, just to guess those letters. \n\n";

        cout << "As soon as you have submitted your guess, the game will color-code each letter in your guess to tell you how close it \n";
        cout << "was to the letters in the hidden word. \n";
        cout << "A" << ANSICodes::GRAY << ANSICodes::BOLD << " GRAY " << ANSICodes::RESET << ANSICodes::CYAN << "letter means that this letter does not appear in the secret word at all. \n";
        cout << "A" << ANSICodes::YELLOW << ANSICodes::BOLD << " YELLOW " << ANSICodes::RESET << ANSICodes::CYAN << "letter means that this letter appears in the secret word, but it is in the wrong spot within the word. \n";
        cout << "A" << ANSICodes::GREEN << ANSICodes::BOLD << " GREEN " << ANSICodes::RESET << ANSICodes::CYAN << "letter means that this letter appears in the secret word, and it is in exactly the right place. \n";
        cout << "Getting a green square or yellow square will get you closer to guessing the real secret word, since it means you have \n";
        cout << "guessed a correct letter. \n\n";

        cout << ANSICodes::RESET;

        pause();
    }

    void show_leaderboard() {
        ConsoleDisplay::clear_screen();

        cout << ANSICodes::GREEN << ANSICodes::BOLD << "LEADERBOARD" << ANSICodes::RESET << '\n';
        leaderboard.print_postorder();

        pause();
    }

    void show_credits() {
        ConsoleDisplay::clear_screen();

        cout << ANSICodes::RED << ANSICodes::BOLD;
        cout << "Name: Tran Anh Khoa \t MSSV: 23127395 \n";
        cout << "Name: Nguyen Kien Hao \t MSSV: 23127290 \n";
        cout << "Name: Nguyen Cong Toan \t MSSV: 23127500 \n";
        cout << "Name: Tran Huu Luong \t MSSV: 23127415 \n\n";
        cout << ANSICodes::RESET;

        pause();
    }

    void exit_game() {
        cout << "Exiting the program..." << endl;
        std::exit(0); 
    }

public:
    void start() {
        leaderboard.insert(999, "khoa");
        show_main_menu();
    }
};



int main() {
    string word;

    ifstream ifs_valid_guesses("valid-guesses.txt");
    if (!ifs_valid_guesses.is_open()) {
        cout << ANSICodes::RED << "Missing word list: \"valid-guesses.txt\"\n\n";
        pause();
        return -1;
    }

    ifstream ifs_secret_words("secret-words.txt");
    if (!ifs_secret_words.is_open()) {
        cout << ANSICodes::RED << "Missing word list: \"secret-words.txt\"\n\n";
        pause();
        return -1;
    }

    while (getline(ifs_valid_guesses, word)) valid_guesses.add_word(word);
    while (getline(ifs_secret_words, word)) secret_words.add_word(word);

    Game game;

    game.start();

    return 0;
}