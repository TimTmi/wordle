#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <random>
#include <chrono>
#include <limits>
#include <iomanip>



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

template<typename T, typename U>
struct AVLTreeNode {
    T key;
    U value;
    AVLTreeNode *left, *right;
    int height;

    AVLTreeNode() {}
    AVLTreeNode(T k, U v) : key(k), value(v), left(nullptr), right(nullptr), height(1) {}
};

template<typename T, typename U>
class AVLTree {
private:
    AVLTreeNode<T, U> *root;

    void clear(AVLTreeNode<T, U> *node) {
        if (!node) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

    void rotate_left(AVLTreeNode<T, U> *&node) {
        AVLTreeNode<T, U> *right_node = node->right, *min_node = right_node->left;

        right_node->left = node;
        node->right = min_node;

        node->height = max(get_height(node->left), get_height(node->right)) + 1;
        right_node->height = max(get_height(right_node->left), get_height(right_node->right)) + 1;

        node = right_node;
    }

    void rotate_right(AVLTreeNode<T, U> *&node) {
        AVLTreeNode<T, U> *left_node = node->left, *max_node = left_node->right;

        left_node->right = node;
        node->left = max_node;

        node->height = max(get_height(node->left), get_height(node->right)) + 1;
        left_node->height = max(get_height(left_node->left), get_height(left_node->right)) + 1;

        node = left_node;
    }

    void rotate_left_right(AVLTreeNode<T, U> *&node) {
        rotate_left(node->left);
        rotate_right(node);
    }

    void rotate_right_left(AVLTreeNode<T, U> *&node) {
        rotate_right(node->right);
        rotate_left(node);
    }

    int get_height(AVLTreeNode<T, U> *node) const {
        return node ? node->height : 0;
    }

    int get_balance(AVLTreeNode<T, U> *node) {
        return node ? get_height(node->left) - get_height(node->right) : 0;
    }

    void insert_internal(AVLTreeNode<T, U> *&node, T key, U value) {
        if (!node) {
            node = new AVLTreeNode<T, U>(key, value);
            return;
        }

        if (key < node->key) insert_internal(node->left, key, value);
        else insert_internal(node->right, key, value);

        node->height = max(get_height(node->left), get_height(node->right)) + 1;

        int balance = get_balance(node);

        if (balance > 1) {
            if (key < node->left->key) rotate_right(node);
            else rotate_left_right(node);
        } else if (balance < -1) {
            if (key > node->right->key) rotate_left(node);
            else rotate_right_left(node);
        }
    }

    void traverse_reverse_inorder_internal(AVLTreeNode<T, U> *node, void (*executor)(const AVLTreeNode<T, U>&), bool (*condition)(const AVLTreeNode<T, U>&)) const {
        if (!node) return;

        traverse_reverse_inorder_internal(node->right, executor, condition);

        if (condition(*node)) executor(*node);
        // cout << "Key: " << node->key << "\t Value: " << node->value << endl;
        traverse_reverse_inorder_internal(node->left, executor, condition);
    }

    void serialize(ofstream& ofs, AVLTreeNode<T, U>* node) {
        bool is_null;

        if (!node) {
            is_null = true;
            ofs.write(reinterpret_cast<const char*>(&is_null), sizeof(bool));
            return;
        }
        
        is_null = false;
        ofs.write(reinterpret_cast<const char*>(&is_null), sizeof(bool));

        ofs.write(reinterpret_cast<const char*>(&node->key), sizeof(node->key));
        ofs.write(reinterpret_cast<const char*>(&node->value), sizeof(node->value));
        
        serialize(ofs, node->left);
        serialize(ofs, node->right);
    }

    AVLTreeNode<T, U>* deserialize(ifstream& ifs) {
        bool is_null;
        ifs.read(reinterpret_cast<char*>(&is_null), sizeof(bool));
        if (is_null) return nullptr;

        auto node = new AVLTreeNode<T, U>;
        
        ifs.read(reinterpret_cast<char*>(&node->key), sizeof(node->key));
        ifs.read(reinterpret_cast<char*>(&node->value), sizeof(node->value));

        node->left = deserialize(ifs);
        node->right = deserialize(ifs);
        
        return node;
    }

public:
    AVLTree() : root(nullptr) {}

    ~AVLTree() {
        clear(root);
    }

    void insert(T key, U value) {
        insert_internal(root, key, value);
    }

    int height() const {
        return get_height(root);
    }

    void traverse_reverse_inorder(void (*executor)(const AVLTreeNode<T, U>&), bool (*condition)(const AVLTreeNode<T, U>&) = [](const AVLTreeNode<T, U>&) { return true; }) const {
        traverse_reverse_inorder_internal(root, executor, condition);
    }

    void save(const string& filename) {
        ofstream ofs(filename, ios::binary);

        if (!ofs) {
            throw runtime_error("Failed to open file for writing.");
        }

        serialize(ofs, root);
        ofs.close();
    }

    void load(const string& filename) {
        ifstream ifs(filename, ios::binary);

        if (!ifs) {
            throw runtime_error("Failed to open file for reading.");
        }

        root = deserialize(ifs);
        ifs.close();
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

    static void draw_separator() {
        cout << "--------------------------\n";
    }
};



class WordList {
private:
    vector<string> list;
    int index;
    mt19937 gen;

public:
    WordList(): index(0), gen(static_cast<uint64_t>(chrono::system_clock::now().time_since_epoch().count())) {}

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

enum class Difficulty {
    EASY,
    MEDIUM,
    HARD,
    CUSTOM
};

struct Submission {
    char name[16];
    Difficulty difficulty;

    Submission() {}
    Submission(const string &name, Difficulty difficulty): difficulty(difficulty) {
        copy(name.begin(), name.end(), this->name);
        this->name[name.size()] = '\0';
    }
};

void pause() {
    cout << "Enter anything to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    // cin.get();
}



class Game {
private:
    AVLTree<int, Submission> leaderboard;
    WordList valid_guesses;
    WordList secret_words;

    void show_main_menu() {
        while (true) {
            ConsoleDisplay::clear_screen();
            cout << ANSICodes::YELLOW;
            cout << "                                 '||  '||          " << '\n';
            cout << "... ... ...   ...   ... ..     .. ||   ||    ....  " << '\n';
            cout << " ||  ||  |  .|  '|.  ||' ''  .'  '||   ||  .|...|| " << '\n';
            cout << "  ||| |||   ||   ||  ||      |.   ||   ||  ||      " << '\n';
            cout << "   |   |     '|..|' .||.     '|..'||. .||.  '|...' " << '\n';
            cout << ANSICodes::RESET;
            cout << ANSICodes::GREEN << ANSICodes::BOLD;
            cout << "\n\n";
            cout << "[Play] \n";
            cout << "[Rules] \n";
            cout << "[Leaderboard] \n";
            cout << "[Credits] \n\n";
            cout << "[Exit] \n\n";
            cout << ANSICodes::RESET;

            string command;
            cout << "> ";
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
                cout << '\n' << "Unknown command \n";
                pause();
            }
        }
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
        string input;
        Difficulty difficulty;
        bool show_alphabet = true;
        int total_guesses, remaining_guesses;
        auto start_seconds = chrono::steady_clock::now();

        while (true) {
            ConsoleDisplay::clear_screen();

            cout << "Select difficulty \n\n";
            cout << ANSICodes::GREEN << "[Easy] \n";
            cout << ANSICodes::YELLOW << "[Medium] \n";
            cout << ANSICodes::RED << "[Hard] \n";
            cout << ANSICodes::RESET_COLOR << "[Custom] \n\n";
            cout << "[Back] \n\n";
            cout << "> ";
            getline(cin, input);

            for (char &c: input) {
                c = tolower(c);
            }

            if (input == "easy") {
                difficulty = Difficulty::EASY;
                total_guesses = 8;
                break;
            }
            
            else if (input == "medium") {
                difficulty = Difficulty::MEDIUM;
                total_guesses = 6;
                break;
            }

            else if (input == "hard") {
                difficulty = Difficulty::HARD;
                total_guesses = 5;
                break;
            }
            
            else if (input == "custom") {
                difficulty = Difficulty::CUSTOM;
                ConsoleDisplay::clear_screen();
                cout << "Number of guesses: ";
                cin >> total_guesses;
                cout << "Show alphabet (0 / 1): ";
                cin >> show_alphabet;
                cin.ignore();
                break;
            }

            else if (input == "back") {
                return;
            }
        }

        remaining_guesses = total_guesses;
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

        while (remaining_guesses > 0) {
            ConsoleDisplay::save_cursor_position();
            ConsoleDisplay::move_cursor_to(3);
            ConsoleDisplay::clear_line();
            cout << "Guesses left: " << remaining_guesses << "\n\n\n";
            ConsoleDisplay::restore_cursor_position();

            string guess;
            cout << '\t';
            getline(cin, guess);

            ConsoleDisplay::move_cursor_down(1);
            ConsoleDisplay::clear_line();
            ConsoleDisplay::move_cursor_up(1);

            for (char &c: guess) {
                c = toupper(c);
            }

            if (guess == "QUIT") {
                return;
            }

            if (guess.length() != 5) {
                dismiss_word("Invalid guess: word needs to be 5 letters long");
                continue;
            }

            if (!valid_guesses.word_exists(guess) && !secret_words.word_exists(guess)) {
                dismiss_word("Invalid guess: \'" + guess + "\' not found");
                continue;
            }

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

            remaining_guesses--;
        }

        cout << '\n';

        if (correct) {
            double elapsed_seconds = chrono::duration<double>(chrono::steady_clock::now() - start_seconds).count();
            int elapsed_seconds_int = static_cast<int>(elapsed_seconds);
            int minutes = elapsed_seconds_int / 60;
            int seconds = elapsed_seconds_int % 60;
            int score = remaining_guesses * 1000 / min(elapsed_seconds, 1000.0);
            cout << "You won! \n";

            pause();
            ConsoleDisplay::clear_screen();

            cout << "Guesses used:\t" << total_guesses - remaining_guesses << '\n';
            cout << "Time taken:\t" << minutes << ':' << setw(2) << setfill('0') << seconds << "s \n" << setfill(' ');
            cout << "Your score is:\t" << score << "\n\n";
            if (difficulty == Difficulty::CUSTOM) pause();
            else submit_score(score, difficulty);
        }

        else {
            cout << "You lost! \n";
            cout << "The secret word is \'" << secret_word << "\' \n";
            pause();
        }

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

    void submit_score(int score, Difficulty difficulty) {
        cout << ANSICodes::GREEN;
        cout << "Enter your name: \n";
        cout << ANSICodes::RESET;

        cout << ANSICodes::RED;
        ConsoleDisplay::save_cursor_position();
        string name;
        getline(cin, name);
        while (name.size() > 15) {
            ConsoleDisplay::move_cursor_down(1);
            cout << "Name no longer than 15 letters...\n";\
            ConsoleDisplay::restore_cursor_position();
            ConsoleDisplay::clear_line();
            getline(cin, name);
        }

        cout << ANSICodes::RESET;
        leaderboard.insert(score, Submission(name, difficulty));
    }

    void show_rules() {
        ConsoleDisplay::clear_screen();

        cout << ANSICodes::CYAN << ANSICodes::BOLD << ANSICodes::UNDERLINE;
        cout << "\tRULES\n\n";
        cout << ANSICodes::RESET;

        cout << ANSICodes::CYAN;
        cout << "The rules of Wordle are simple: \n";
        cout << "- Your objective is to guess a secret five-letter word before you run out of guesses. \n";
        cout << "- To submit a guess, type any five-letter word and press Enter. \n";
        cout << "- All of your guesses must be real words, according to a dictionary of five-letter words that Wordle" << endl; 
        cout << "  allows as guesses. \n";
        cout << "- You cannot make up a non-existent word, like AEIOU, just to guess those letters. \n\n";

        cout << "- Once your guess has been submitted, the game will color-code each letter in your guess to tell you" << endl; 
        cout << "  how close it was to the letters in the hidden word. \n";
        cout << "- A" << ANSICodes::GRAY << ANSICodes::BOLD << " GRAY " << ANSICodes::RESET << ANSICodes::CYAN << "letter means that this letter does not appear in the secret word at all. \n";
        cout << "- A" << ANSICodes::YELLOW << ANSICodes::BOLD << " YELLOW " << ANSICodes::RESET << ANSICodes::CYAN << "letter means that this letter appears in the secret word, but it is in the wrong spot" << endl; 
        cout << "  within the word. \n";
        cout << "- A" << ANSICodes::GREEN << ANSICodes::BOLD << " GREEN " << ANSICodes::RESET << ANSICodes::CYAN << "letter means that this letter appears in the secret word, and it is in exactly the right place. \n";
        cout << "- Getting a green square or yellow square will get you closer to guessing the real secret word, since it" << endl; 
        cout << "  means you have guessed a correct letter. \n\n";

        cout << ANSICodes::RESET;

        pause();
    }

    void show_leaderboard(Difficulty difficulty = Difficulty::MEDIUM) {
        while (true) {
            string input;
            bool (*condition)(const AVLTreeNode<int, Submission>&);

            ConsoleDisplay::clear_screen();
            cout << ANSICodes::BLUE << ANSICodes::BOLD << "LEADERBOARD" << ANSICodes::RESET << "\n\n";

            switch (difficulty) {
                case Difficulty::EASY:
                    cout << ANSICodes::GREEN;
                    condition = [](const AVLTreeNode<int, Submission>& node) { return node.value.difficulty == Difficulty::EASY; };
                    break;
                case Difficulty::MEDIUM:
                    cout << ANSICodes::YELLOW;
                    condition = [](const AVLTreeNode<int, Submission>& node) { return node.value.difficulty == Difficulty::MEDIUM; };
                    break;
                case Difficulty::HARD:
                    cout << ANSICodes::RED ;
                    condition = [](const AVLTreeNode<int, Submission>& node) { return node.value.difficulty == Difficulty::HARD; };
                    break;
            }

            ConsoleDisplay::draw_separator();
            cout << left;
            leaderboard.traverse_reverse_inorder([](const AVLTreeNode<int, Submission>& node) { cout << setw(20) << node.value.name << node.key << '\n'; }, condition);
            ConsoleDisplay::draw_separator();

            cout << ANSICodes::RESET_COLOR << '\n';

            cout << ANSICodes::GREEN << "[Easy] \n";
            cout << ANSICodes::YELLOW << "[Medium] \n";
            cout << ANSICodes::RED << "[Hard] \n\n";
            cout << ANSICodes::RESET_COLOR << "[Back] \n\n";
            cout << "> ";
            getline(cin, input);

            for (char &c: input) c = tolower(c);

            if (input == "easy") difficulty = Difficulty::EASY;
            else if (input == "medium") difficulty = Difficulty::MEDIUM;
            else if (input == "hard") difficulty = Difficulty::HARD;
            else if (input == "back") break;
        }
    }

    void show_credits() {
        ConsoleDisplay::clear_screen();

        int width = 20;

        cout << ANSICodes::RED << ANSICodes::BOLD;
        cout << left;
        cout << setw(width) << "Name" << "MSSV \n\n";
        cout << setw(width) << "Tran Anh Khoa" << "23127395 \n";
        cout << setw(width) << "Nguyen Kien Hao" << "23127290 \n";
        cout << setw(width) << "Nguyen Cong Toan" << "23127500 \n";
        cout << setw(width) << "Tran Huu Luong" << "23127415 \n\n";
        cout << ANSICodes::RESET;

        pause();
    }

    void process_file(const string& input_file, const string& output_file, WordList& word_list) {
        char buffer[6];

        ifstream ifs(output_file, ios::binary);

        if (ifs.is_open()) {
            while (ifs.read(buffer, 6)) {
                string word(buffer, buffer + 6);
                word.erase(word.find_last_not_of('\0') + 1, string::npos);
                word_list.add_word(word);
            }
        }
        
        else {
            ifstream ifs_text(input_file);
            ofstream ofs_binary(output_file, ios::binary);

            if (!ifs_text.is_open()) {
                cout << ANSICodes::RED << "Missing word list: \"" << input_file << "\"\n\n";
                pause();
                return;
            }

            while (ifs_text.getline(buffer, sizeof(buffer))) {
                ofs_binary.write(buffer, sizeof(buffer));
                word_list.add_word(string(buffer));
            }

            ifs_text.close();
            ofs_binary.close();
        }

        ifs.close();
    }

    void exit_game() {
        cout << "Exiting the program..." << endl;
        leaderboard.save("leaderboard.dat");
        exit(0); 
    }

public:
    void start() {
        process_file("valid-guesses.txt", "valid-guesses.dat", valid_guesses);
        process_file("secret-words.txt", "secret-words.dat", secret_words);

        leaderboard.insert(999, Submission("khoa", Difficulty::HARD));
        leaderboard.load("leaderboard.dat");
        show_main_menu();
    }
};



int main() {
    Game game;
    game.start();

    return 0;
}