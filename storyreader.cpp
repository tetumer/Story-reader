//Titumir’s contribution
#include<iostream>
#include<limits>
#include<windows.h>
#include<fstream>
#include<string>
#include<vector>
#include<cstdio>
#include<thread>
#include<chrono>
#include<iomanip>
#include<algorithm>
class History {
private:
    std::vector<std::string> storyHistory;
    size_t historyLimit = 10;
public:
    void addStory(const std::string& storyTitle) {
        if (storyHistory.size() >= historyLimit) {
            storyHistory.erase(storyHistory.begin()); // Remove the oldest story
        }
        storyHistory.push_back(storyTitle);
    }
    std::vector<std::string> getHistory() const {
        return storyHistory;
    }
    void displayHistory() const {
        if (storyHistory.empty()) {
            std::cout << "No history available.\n";
            return;
        }
        std::cout << "Story History:\n";
        for (const auto& story : storyHistory) {
            std::cout << story << '\n';
        }
    }
};
class User : public History {
private:
    std::string userName;
public:
    User(const std::string& name) : userName(name) {}
    std::string getUserName() const {
        return userName;
    }
    void setUserName(const std::string& name) {
        userName = name;
    }
    void saveToFile() const {
        std::ofstream outFile(userName + ".txt");
        if (!outFile.is_open()) {
            std::cerr << "Error opening file for saving user data.\n";
            return;
        }
        outFile << "User Name: " << userName << '\n';
        outFile << "Story History:\n";
        for (const auto& story : getHistory()) {
            outFile << story << '\n';
        }
        outFile.close();
    }
    void loadFromFile() {
        std::ifstream inFile(userName + ".txt");
        if (!inFile.is_open()) {
            std::cerr << "Error opening file for loading user data.\n";
            return;
        }
        std::string line;
        while (std::getline(inFile, line)) {
            if (line.find("User Name: ") != std::string::npos) {
                userName = line.substr(11);
            } else if (line != "Story History:") {
                addStory(line);
            }
        }
        inFile.close();
    }
};
std::vector<std::string> loadUsers() {
    std::vector<std::string> users;
    std::ifstream inFile("totaluser.txt");
    if (!inFile.is_open()) {
        std::ofstream outFile("totaluser.txt");
        if (outFile.is_open()) {
            std::cout << "User file not found. Creating a new one...\n";
            outFile.close();
        } else {
            std::cerr << "Error creating user file.\n";
         return users;
        }
    } else {
        std::string line;
        while (std::getline(inFile, line)) {
            users.push_back(line);
        }
        inFile.close();
    }
    return users;
}
void saveUsers(const std::vector<std::string>& users) {
    std::ofstream outFile("totaluser.txt");
    if (outFile.is_open()) {
        for (const auto& user : users) {
            outFile << user << '\n';
        }
        outFile.close();
    } else {
        std::cerr << "Error opening user file for saving.\n";
    }
}
int getWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int width = 80;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    return width;
}
void stdcout(const std::string& text) {
    int width = getWidth();
    int padding = (width - text.length()) / 2;
    if (padding > 0) {
        std::cout << std::setw(padding) << " " << text << std::setw(padding) << " " << std::endl;
    } else {
        std::cout << text << std::endl;
    }
}
void setColor(int textColor, int bgColor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bgColor << 4) | textColor);
}
void setFontSize(int size) {
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetCurrentConsoleFontEx(hConsole, FALSE, &cfi);
    cfi.dwFontSize.Y = size;
    SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);
}
void applySettings(int textColor, int bgColor, int fontSize) {
    setColor(textColor, bgColor);
    setFontSize(fontSize);
}
void showColors() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int i = 0; i < 16; ++i) {
        SetConsoleTextAttribute(hConsole, i);
        std::cout << std::setw(4) << i << " ";
    }
    std::cout << std::endl;
    SetConsoleTextAttribute(hConsole, 7);
}
void changeAppearance(int& textColor, int& bgColor, int& fontSize) {
    int choice;
    while (true) {
        stdcout("Change Appearance Settings");
        stdcout("1. Change Text Color");
        stdcout("2. Change Background Color");
        stdcout("3. Change Font Size");
        stdcout("4. Return to Main Menu");
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            stdcout("Invalid input. Please try again.");
            continue;
        }
        switch (choice) {
            case 1: {
                showColors();
                int newTextColor;
                stdcout("Enter text color (0-15): ");
                std::cin >> newTextColor;
                if (newTextColor >= 0 && newTextColor <= 15) {
                    textColor = newTextColor;
                    applySettings(textColor, bgColor, fontSize);
                } else {
                    stdcout("Invalid color. Please try again.");
                }
                break;
            }
            case 2: {
                showColors();
                int newBgColor;
                stdcout("Enter background color (0-15): ");
                std::cin >> newBgColor;
                if (newBgColor >= 0 && newBgColor <= 15) {
                    bgColor = newBgColor;
                    applySettings(textColor, bgColor, fontSize);
                } else {
                    stdcout("Invalid color. Please try again.");
                }
                break;
            }
            case 3: {
                int newFontSize;
                stdcout("Enter font size: ");
                std::cin >> newFontSize;
                if (newFontSize > 0) {
                    fontSize = newFontSize;
                    applySettings(textColor, bgColor, fontSize);
                } else {
                    stdcout("Invalid size. Please try again.");
                }
                break;
            }
            case 4:
                return;
            default:
                stdcout("Invalid choice. Please try again.");
        }
        system("cls");
    }
}
class Story{
    public:
    static std::string menufile;
    int number;
    std::string Title;
    std::string author;
    std::string genre;
    std::string filelocation;
    int bookmark;
      Story(const std::string& Title, const std::string& author, const std::string& genre) {
        this->number = 0;
        this->Title = Title;
        this->author = author;
        this->genre = genre;
        this->bookmark = 0;
        saveMetadata();
    }
    void saveMetadata() const {
        std::string filename = Title + "data.txt";
        std::ofstream outFile(filename, std::ios::app);
        if (outFile.is_open()) {
            outFile << Title << std::endl;
            outFile << author << std::endl;
            outFile << genre << std::endl;
            outFile << bookmark << std::endl;
            outFile.close();
            std::cout << "Metadata has been written to " << filename << std::endl;
        } else {
            std::cout << "Unable to open file: " << filename << std::endl;
        }
    }
    static void removeStory(const std::string& title) {
        std::string metadataFilename = title + "data.txt";
        std::string storyFilename = title + ".txt";

        if (std::remove(metadataFilename.c_str()) == 0) {
            std::cout << "Deleted file: " << metadataFilename << std::endl;
        } else {
            std::cout << "File not found: " << metadataFilename << std::endl;
        }
        if (std::remove(storyFilename.c_str()) == 0) {
            std::cout << "Deleted file: " << storyFilename << std::endl;
        } else {
            std::cout << "File not found: " << storyFilename << std::endl;
        }
    }
    public:
    static void saveBookmark(const std::string& title, int bookmark) {
        std::ofstream outFile(title + "data.txt", std::ios::trunc);
        if (outFile.is_open()) {
            outFile << bookmark << std::endl;
            outFile.close();
        }
    }
    static int loadBookmark(const std::string& title) {
        std::ifstream inFile(title + "data.txt");
        int bookmark = 0;
        if (inFile.is_open()) {
            inFile >> bookmark;
            inFile.close();
        }
        return bookmark;
    }
};
void addTitleToMenu(const std::string& title) {
    std::ofstream outFile(Story::menufile, std::ios::app);
    if (outFile.is_open()) {
        outFile << title << '\n';
        outFile.close();
        std::cout << "Title added to menu: " << title << '\n';
    } else {
        std::cout << "Error: Unable to open menu file for writing.\n";
    }
}
void removeTitleFromMenu(const std::string& title) {
    std::ifstream inFile(Story::menufile);
    if (!inFile.is_open()) {
        std::cout << "Error: Unable to open menu file for reading.\n";
        return;
    }

    std::vector<std::string> titles;
    std::string line;
    while (std::getline(inFile, line)) {
        if (line != title) {
            titles.push_back(line);
        }
    }
    inFile.close();
    std::ofstream outFile(Story::menufile, std::ios::trunc);
    if (!outFile.is_open()) {
        std::cout << "Error: Unable to open menu file for writing.\n";
        return;
    }
    for (const auto& t : titles) {
        outFile << t << '\n';
    }
    outFile.close();
    std::cout << "Title removed from menu: " << title << '\n';
}
std::string Story::menufile = "menuFile.txt";
void addstory() {
    std::string title;
    std::cout << "What should be the Title: ";
    std::getline(std::cin >> std::ws, title);
    std::string author;
    std::cout << "What should be the Author: ";
    std::getline(std::cin >> std::ws, author);
    std::string genre;
    std::cout << "What should be the Genre: ";
    std::getline(std::cin >> std::ws, genre);
    Story story1(title, author, genre);
    addTitleToMenu(title);
    std::string option;
    std::cout << "Would you like to copy-paste the story content or use an existing text file? (Enter 'paste' or 'file'): ";
    std::cin >> option;
    std::string storyFilename = title + ".txt";
    if (option == "paste") {
        std::ofstream storyFile(storyFilename);
        if (!storyFile) {
            std::cout << "Unable to create file: " << storyFilename << std::endl;
            return;
        }
        std::cout << "Please start entering the story content. When you are done, type 'END' on a new line:" << std::endl;
        std::string line;
        std::cin.ignore();
        while (std::getline(std::cin, line)) {
            if (line == "END") {
                break;
            }
            storyFile << line << std::endl;
        }
        storyFile.close();
        std::cout << "Story content has been written to " << storyFilename << std::endl;
    } else if (option == "file") {
        std::cout << "Please make sure the file is named \"" << storyFilename << "\" and is placed in the same folder." << std::endl;
    } else {
        std::cout << "Invalid option. Please enter 'paste' or 'file'." << std::endl;
    }
}
std::vector<std::string> loadMenu() {
    std::vector<std::string> menuItems;
    std::ifstream inFile(Story::menufile);
    if (!inFile.is_open()) {
        std::ofstream outFile(Story::menufile);
        if (outFile.is_open()) {
            std::cout << "Menu file not found. Creating a new one...\n";
            outFile.close();
        } else {
            std::cout << "Menu file not found.\nError creating a menu file.\n";
        }
    } else {
        std::string line;
        while (std::getline(inFile, line)) {
            menuItems.push_back(line);
        }
        inFile.close();
    }
    return menuItems;
}
void displayTitles(const std::vector<std::string>& titles) {
    if (titles.empty()) {
        std::cout << "No stories available in the menu.\n";
     return;
    }
    std::cout << "Available Stories:\n";
    for (size_t i = 0; i < titles.size(); ++i) {
        stdcout(std::to_string(i + 1) + ". " + titles[i]);
    }
}
void deleteStory() {
    std::string title;
    std::cout << "Enter the title of the story you want to delete: ";
    std::getline(std::cin >> std::ws, title);
    Story::removeStory(title);
    removeTitleFromMenu(title);
}
void readStory(const std::string& title) {
    std::ifstream storyFile(title + ".txt");
    if (!storyFile.is_open()) {
        std::cout << "Please try again :( .\n";
        return;
    }
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(storyFile, line)) {
        lines.push_back(line);
    }
    storyFile.close();
    int bookmark = Story::loadBookmark(title);
    if (bookmark < 0 || bookmark >= lines.size()) {
        bookmark = 0;
    }
    int currentLine = bookmark;
    bool isBookmarkSaved = false;
    char userInput;
    while (currentLine < lines.size() && currentLine >= 0) {
        for (int i = 0; i < 10 && currentLine < lines.size(); ++i, ++currentLine) {
            stdcout(lines[currentLine]);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        userInput = std::cin.get();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (userInput == 'x'|| userInput == 'X') {
            Story::saveBookmark(title, currentLine);
            isBookmarkSaved = true;
            std::cout << "Bookmark saved at line: " << currentLine + 1 << '\n';
            return ;
        } else if (userInput == 'b') {
            if (currentLine > 10) {
                currentLine -= 20;
            } else {
                currentLine = -1;
            }
        }
    }
    if (!isBookmarkSaved) {
        std::cout << "Bookmark not saved. Use 'x' to save next time.\n";
    }
    if (currentLine >= lines.size()) {
        std::cout << "End of the story.\n";
    }
}
//Rafi’s contribution
void readInfo(const std::string& title) {
    std::ifstream storyFile(title + "data.txt");
    if (!storyFile.is_open()) {
        stdcout("Please try again.");
        return;
    }
    std::string author, genre;
    int bookmark;
    std::getline(storyFile, author);
    std::getline(storyFile, genre);
    storyFile >> bookmark;
    storyFile.close();
    stdcout("Story Information:");
    stdcout("Title: " + title);
    stdcout("Author: " + author);
    stdcout("Genre: " + genre);
    stdcout("Bookmark Position: " + std::to_string(bookmark));
    stdcout("Press enter any key to return to the main menu...");
    std::cin.ignore();
    std::cin.get();
}
void showInfo() {
    std::vector<std::string> titles = loadMenu();
    displayTitles(titles);
    if (!titles.empty()) {
        int choice;
        stdcout("Enter the number of the story for information (or 0 to go back): ");
        std::cin >> choice;
        if (std::cin.fail() || choice < 0 || choice > static_cast<int>(titles.size())) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            stdcout("Invalid input. Returning to main menu.");
        } else if (choice > 0) {
            std::string selectedTitle = titles[choice - 1];
            stdcout("You selected: " + selectedTitle);
            readInfo(selectedTitle);
        }
    }
}
//Toukir’s contribution:
    int main() {
    int textColor = 7;
    int bgColor = 0;
    int fontSize = 16;
    applySettings(textColor, bgColor, fontSize);
    stdcout("Welcome to the multiverse");
    int userChoice = 0;
      User user("");
    while (userChoice != 1 && userChoice != 2) {
        stdcout("Are you an old watcher or one?\n");
        stdcout("Choose which one you are.\n");
        stdcout("1. Old User\n");
        stdcout("2. New User\n");
        std::cin >> userChoice;
        if (std::cin.fail() || (userChoice != 1 && userChoice != 2)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please try again.\n";
            userChoice = 0;
    }
    std::vector<std::string> users = loadUsers();
    if (userChoice == 1) {
        std::string userName;
        bool found = false;
        while (!found) {
            stdcout("Enter your name: ");
            std::cin >> userName;
            auto it = std::find(users.begin(), users.end(), userName);
            if (it != users.end()) {
                user.setUserName(userName);
                user.loadFromFile();
                found = true;
            } else {
                stdcout("Theres no watcher named by this\n");
            }
        }
    } else {
        stdcout("Enter your new name: ");
        std::string userName;
        std::cin >> userName;
        user.setUserName(userName);
        users.push_back(userName);
        saveUsers(users);
    }
}
    int choice;
    while(true){
        stdcout("1.Watch a universe");
        stdcout("2.Create a universe");
        stdcout("3.Destroy a universe");
        stdcout("4.Information about a universe");
        stdcout("5.Change appearance");
        stdcout("6.Wake up?");
    std::cin  >> choice;
    if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
    }
    system("cls");
    switch(choice){
    case 1: {
    std::vector<std::string> titles = loadMenu();
    displayTitles(titles);
    if (!titles.empty()) {
        int choice;
        std::cout << "Enter the number of the story you want to read (or 0 to go back): ";
        std::cin >> choice;
        if (std::cin.fail() || choice < 0 || choice > static_cast<int>(titles.size())) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Returning to main menu.\n";
        } else if (choice == 0) {
            std::cout << "Returning to the main menu.\n";
        } else {
            std::string selectedTitle = titles[choice - 1];
            std::cout << "You selected: " << selectedTitle << '\n';
            std::cout << "enter b to read previous page, enter x to exit , and press enter to read next page \n" ;
            user.saveToFile();
            readStory(selectedTitle);
            user.addStory(selectedTitle);
        }
    }
    break;
}
    case 2:
        addstory();
        break;
    case 3:
        deleteStory();
        break;
    case 4:
        showInfo();
        break;
    case 5:
            changeAppearance(textColor, bgColor, fontSize);
            applySettings(textColor, bgColor, fontSize);
            break;
    case 6:
        user.saveToFile();
        std::cout << "byebye\n";
        return 0;
    default:
        std::cout << "invalid input \n";
    }
    }
}






