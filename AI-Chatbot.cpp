#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <stack>
#include <queue>
#include <ctime>
#include <cctype>

using namespace std;

struct User {
    string name;
    string email;
    string password;
    string dob;
    string language;
    int age;
    User() {}
    User(string n, string e, string p, string d, string l, int a)
        : name(n), email(e), password(p), dob(d), language(l), age(a) {}
};

class AuthenticationSystem {
private:
    unordered_map<string, User> users;
    const int MIN_AGE = 15;
    bool isValidEmail(const string& email) {
        size_t at = email.find('@');
        size_t dot = email.find('.', at);
        return at != string::npos && dot != string::npos;
    }
    bool isStrongPassword(const string& pwd) {
        return pwd.length() >= 8;
    }
    bool checkAge(const string& dob) {
        if (dob.size() < 10) return false;
        int year = stoi(dob.substr(6, 4));
        int age = 2025 - year;
        return age >= MIN_AGE;
    }
public:
    bool signUp() {
        cout << "\n--- SIGN UP ---\n";
        string name, email, pwd, confirm, dob, lang;
        cout << "Name: ";
        getline(cin, name);
        if (name.empty()) {
            cout << "Name cannot be empty.\n";
            return false;
        }
        cout << "Email: ";
        getline(cin, email);
        if (!isValidEmail(email)) {
            cout << "Invalid email.\n";
            return false;
        }
        if (users.count(email)) {
            cout << "Email already registered.\n";
            return false;
        }
        cout << "Password (min 8 chars): ";
        getline(cin, pwd);
        if (!isStrongPassword(pwd)) {
            cout << "Password too weak.\n";
            return false;
        }
        cout << "Confirm Password: ";
        getline(cin, confirm);
        if (pwd != confirm) {
            cout << "Passwords do not match.\n";
            return false;
        }
        cout << "DOB (DD-MM-YYYY): ";
        getline(cin, dob);
        if (!checkAge(dob)) {
            cout << "You must be at least " << MIN_AGE << " years old.\n";
            return false;
        }
        cout << "Preferred Language: ";
        getline(cin, lang);
        if (lang.empty()) lang = "English";
        int year = stoi(dob.substr(6, 4));
        int age = 2025 - year;
        users[email] = User(name, email, pwd, dob, lang, age);
        cout << "Registration successful! You can now log in.\n";
        return true;
    }
    bool login(string& userEmail) {
        cout << "\n--- LOGIN ---\n";
        string email, pwd;
        cout << "Email: "; getline(cin, email);
        if (!users.count(email)) {
            cout << "No account with that email.\n";
            return false;
        }
        cout << "Password: "; getline(cin, pwd);
        if (users[email].password != pwd) {
            cout << "Incorrect password.\n";
            return false;
        }
        userEmail = email;
        cout << "Login successful! Welcome, " << users[email].name << ".\n";
        return true;
    }
};

class Chatbot {
private:
    unordered_map<string, vector<string>> qa;
    queue<pair<string,string>> history;
    stack<string> undoStk;
    const size_t MAX_HISTORY = 10;
    string normalize(const string& s) {
        string r;
        for (char c : s) if (isalnum(c) || isspace(c)) r += tolower(c);
        size_t b = r.find_first_not_of(' ');
        size_t e = r.find_last_not_of(' ');
        return b == string::npos ? "" : r.substr(b, e - b + 1);
    }
    void record(const string& q, const string& a) {
        if (history.size() == MAX_HISTORY) history.pop();
        history.push({q,a});
        undoStk.push(a);
    }
    void showHistory() {
        if (history.empty()) { cout << "No history.\n"; return; }
        auto tmp = history;
        cout << "--- Conversation History ---\n";
        while (!tmp.empty()) {
            cout << "You: " << tmp.front().first << "\n";
            cout << "Bot: " << tmp.front().second << "\n";
            tmp.pop();
        }
        cout << "----------------------------\n";
    }
    void undo() {
        if (undoStk.empty()) {
            cout << "Nothing to undo.\n";
            return;
        }
        cout << "Removed: " << undoStk.top() << "\n";
        undoStk.pop();
        if (!history.empty()) history.pop();
    }
public:
    Chatbot() {
        vector<pair<string,string>> pairs = {
            {"hi", "Good! What about you?"},
            {"hello", "Hello there! How can I help?"},
            {"hey", "Hey! What's up?"},
            {"how are you", "I'm doing great, thanks!"},
            {"what is your name", "I'm your friendly chatbot."},
            {"who are you", "I'm an AI assistant."},
            {"help", "Sure—ask away!"},
            {"thank you", "You're welcome!"},
            {"thanks", "Anytime!"},
            {"bye", "Goodbye!"},
            {"good morning", "Good morning to you!"},
            {"good night", "Good night, sleep tight."},
            {"joke", "Why don't scientists trust atoms? Because they make up everything!"},
            {"weather", "I can't fetch live weather yet."},
            {"time", "I don't have a clock."},
            {"date", "I have no calendar."},
            {"programming", "I love coding talk!"},
            {"sports", "Which sport do you like?"},
            {"music", "Music soothes the soul."},
            {"movie", "I enjoy sci-fi films."},
            {"what can you do", "I answer questions, tell jokes, and help with info!"},
            {"tell me a fact", "Honey never spoils—archaeologists found edible 3000-year-old honey!"},
            {"define AI", "AI stands for Artificial Intelligence—machines that mimic human thinking."},
            {"who made you", "I was created by developers using machine learning."},
            {"are you human", "Nope, I’m a program, but I try my best!"},
            {"do you sleep", "I don’t need sleep, but servers need breaks!"},
            {"favorite book", "I can’t read, but users love *The Hitchhiker’s Guide to the Galaxy*!"},
            {"meaning of life", "42 (according to Douglas Adams) or your own purpose!"},
            {"how old are you", "I’m ageless—just code running on servers!"},
            {"capital of Japan", "Tokyo!"},
            {"best programming language", "Depends! Python for beginners, C++ for speed, JavaScript for web."},
            {"how to learn coding", "Try free platforms like Codecademy or freeCodeCamp!"},
            {"what is ChatGPT", "An AI chatbot by OpenAI, similar to me!"},
            {"do you dream", "Only of electric sheep (kidding—I don’t dream)."},
            {"favorite movie", "I can’t watch films, but *The Matrix* is a classic!"},
            {"tell me a riddle", "What has keys but no locks? A piano!"},
            {"how big is the universe", "Observable universe: ~93 billion light-years across!"},
            {"why is the sky blue", "Rayleigh scattering—sunlight disperses blue light more!"},
            {"best pizza topping", "Controversial! Pepperoni or mushrooms?"},
            {"how to meditate", "Sit quietly, focus on breath, let thoughts pass."},
            {"what is love", "A complex emotion—or a Haddaway song!"},
            {"how to tie a tie", "YouTube has great tutorials for the Windsor knot!"},
            {"who won the World Cup", "Argentina in 2022 (as of now)."},
            {"best phone brand", "Subjective! Apple, Samsung, or Google Pixel?"},
            {"how to lose weight", "Eat balanced meals, exercise, and stay consistent."},
            {"what is Bitcoin", "A decentralized digital cryptocurrency."},
            {"how to invest", "Start with index funds or consult a financial advisor."},
            {"best travel destination", "Bali for beaches, Japan for culture!"},
            {"how to cook pasta", "Boil water, add salt, cook al dente (~8 mins)!"},
            {"what is blockchain", "A secure, decentralized digital ledger system."},
            {"how to write a resume", "Keep it concise, highlight skills, use action verbs."},
            {"funny meme", "Why did the scarecrow win an award? Because he was outstanding in his field!"},
            {"how to study better", "Use the Pomodoro Technique (25-min focused sessions)."},
            {"what is Python", "A versatile, easy-to-learn programming language."},
            {"how to make coffee", "Brew ground beans with hot water—or use a machine!"},
            {"who is Elon Musk", "CEO of Tesla, SpaceX, and X (formerly Twitter)."},
            {"how to fix a leak", "Use plumber’s tape or call a professional!"},
            {"what is COVID", "A disease caused by the SARS-CoV-2 virus."},
            {"how to save money", "Budget, cut unnecessary expenses, automate savings."},
            {"best gaming console", "PS5, Xbox Series X, or Nintendo Switch—depends on games!"},
            {"how to train a dog", "Positive reinforcement (treats/praise) works best!"},
            {"what is Netflix", "A streaming service for movies/TV shows."},
            {"how to take a screenshot", "On Windows: PrtScn. On Mac: Cmd+Shift+4."},
            {"who is Shakespeare", "A famous English playwright (*Romeo and Juliet*)."},
            {"how to be happy", "Practice gratitude, connect with others, pursue passions."},
            {"what is Instagram", "A photo/video-sharing social media app."},
            {"how to start a blog", "Pick a niche, use WordPress, write consistently!"},
            {"who is the richest person", "Elon Musk or Jeff Bezos (varies by stock prices)."},
            {"how to change a tire", "Loosen lug nuts, jack up car, replace tire."},
            {"what is a NFT", "A non-fungible token—unique digital asset on blockchain."},
            {"how to get a job", "Network, tailor your resume, practice interviews."},
            {"best car brand", "Toyota for reliability, Tesla for tech!"},
            {"how to draw", "Start with basic shapes, practice daily!"},
            {"what is Twitter", "A social media platform for short posts (now X)."},
            {"how to grow plants", "Water, sunlight, and good soil are key!"},
            {"who is Einstein", "A physicist famous for E=mc² and relativity."},
            {"how to make friends", "Be genuine, join clubs, listen actively."},
            {"what is YouTube", "A video-sharing platform for creators."},
            {"how to clean a laptop", "Use microfiber cloth + isopropyl alcohol."},
            {"who is Messi", "A legendary Argentine soccer player."},
            {"how to speak French", "Try Duolingo or immersion!"},
            {"what is Amazon", "An e-commerce and cloud computing giant."},
            {"how to reduce anxiety", "Deep breathing, exercise, limit caffeine."},
            {"best laptop brand", "Apple, Dell, or HP for reliability."},
            {"how to bake a cake", "Mix flour, sugar, eggs, and bake at 350°F!"},
            {"who is Beethoven", "A German composer (*Moonlight Sonata*)."},
            {"how to use Excel", "Learn formulas (=SUM), pivot tables, shortcuts!"},
            {"what is Tiktok", "A short-form video-sharing app."},
            {"how to swim", "Take lessons, practice floating/kicking."},
            {"who is Ronaldo", "A Portuguese soccer superstar."},
            {"how to write a novel", "Outline your plot, write daily, edit later!"},
            {"what is Facebook", "A social media platform for connecting."},
            {"how to delete files", "Right-click → Delete or use Shift+Del (permanent)."},
            {"who is Tesla", "Nikola Tesla—inventor of AC electricity."},
            {"how to paint", "Start with basic brushes, mix colors, practice!"},
            {"what is LinkedIn", "A professional networking platform."},
            {"how to train for a marathon", "Start with 5Ks, increase mileage slowly."},
            {"who is Hawking", "Stephen Hawking—cosmologist (*A Brief History of Time*)."},
            {"how to use Photoshop", "Learn layers, masks, and shortcuts!"},
            {"what is Spotify", "A music/podcast streaming service."},
            {"how to fix Wi-Fi", "Restart router, check cables, update firmware."},
            {"who is Gandhi", "Indian leader of nonviolent independence movement."},
            {"how to make tea", "Steep tea leaves in hot water for 3-5 mins."},
            {"what is Zoom", "A video conferencing app for meetings."},
            {"how to train your brain", "Solve puzzles, learn new skills, meditate."},
            {"who is Napoleon", "French military leader/emperor in the 1800s."},
            {"how to use GitHub", "Commit code, push repos, collaborate!"},
            {"what is Reddit", "A forum-based social media platform."},
            {"how to build muscle", "Lift weights, eat protein, rest well."},
            {"who is Da Vinci", "Renaissance artist (*Mona Lisa*) and inventor."},
            {"how to make sushi", "Prepare rice, slice fish, roll with seaweed!"},
            {"what is WhatsApp", "A messaging app for texts/calls."},
            {"how to stop procrastinating", "Break tasks into chunks, set deadlines."},
            {"who is Cleopatra", "Last active ruler of ancient Egypt."},
            {"how to use Canva", "Drag-and-drop design tool for graphics."},
            {"what is Discord", "A chat app for gamers/communities."},
            {"how to improve memory", "Use mnemonics, sleep well, stay organized."},
            {"who is Mozart", "Classical composer (*The Magic Flute*)."},
            {"how to make pancakes", "Mix flour, milk, eggs; cook on a griddle!"},
            {"what is a meme", "A humorous image/video spread online."},
            {"how to be confident", "Practice, positive self-talk, fake it till you make it!"},
            {"who is Van Gogh", "Dutch painter (*Starry Night*)."},
            {"how to use Python", "Install Python, write scripts, run them!"},
            {"what is a VPN", "Virtual Private Network—encrypts internet traffic."},
            {"how to stop overthinking", "Distract yourself, set time limits for decisions."}
        };
        for (auto& p : pairs) qa[p.first].push_back(p.second);
        srand((unsigned)time(nullptr));
    }
    void start() {
        cout << "\nChatbot: Hello! (type 'history','undo','bye')\n";
        string line;
        while (true) {
            cout << "You: "; getline(cin, line);
            string cmd = normalize(line);
            if (cmd == "bye") {
                cout << "Chatbot: Goodbye!\n";
                break;
            }
            if (cmd == "history") { showHistory(); continue; }
            if (cmd == "undo") { undo(); continue; }
            string reply;
            auto it = qa.find(cmd);
            if (it != qa.end()) reply = it->second[rand() % it->second.size()];
            else reply = "I'm sorry, I don't know how to answer that.";
            cout << "Chatbot: " << reply << "\n";
            record(cmd, reply);
        }
    }
};

int main() {
    AuthenticationSystem auth;
    while (true) {
        cout << "\n1) Login\n2) Sign Up\n3) Exit\nChoose: ";
        string opt; getline(cin, opt);
        if (opt == "1") {
            string user;
            if (auth.login(user)) {
                Chatbot bot;
                bot.start();
                break;
            }
        } else if (opt == "2") {
            auth.signUp();
        } else if (opt == "3") {
            cout << "Exiting...\n";
            break;
        } else {
            cout << "Invalid option.\n";
        }
    }
    return 0;
}
