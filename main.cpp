#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>

using namespace std;

class Task {
private:
    int id;
    string title;
    string description;
    string priority;
    bool completed;

public:
    Task(int taskId, string taskTitle,
         string taskDescription, string taskPriority,
         bool isCompleted = false) {
        id = taskId;
        title = taskTitle;
        description = taskDescription;
        priority = taskPriority;
        completed = isCompleted;
    }

    int getId() const { return id; }
    string getTitle() const { return title; }
    string getDescription() const { return description; }
    string getPriority() const { return priority; }
    bool isCompleted() const { return completed; }

    void markCompleted() {
        completed = true;
    }

    void display() const {
        cout << "\nTask ID: " << id;
        cout << "\nTitle: " << title;
        cout << "\nDescription: " << description;
        cout << "\nPriority: " << priority;
        cout << "\nStatus: " << (completed ? "Completed" : "Pending");
        cout << "\n-----------------------------\n";
    }
};

class ToDoList {
private:
    vector<Task> tasks;
    int nextId;
    const string filename = "tasks.txt";

    void saveTasks() const {
        ofstream file(filename);
        if (!file) {
            cout << "Error: Unable to save tasks.\n";
            return;
        }

        for (const Task& task : tasks) {
            file << task.getId() << '|'
                 << task.getTitle() << '|'
                 << task.getDescription() << '|'
                 << task.getPriority() << '|'
                 << task.isCompleted() << '\n';
        }
    }

    void loadTasks() {
        ifstream file(filename);
        if (!file) return;

        string line;
        int maxId = 0;

        while (getline(file, line)) {
            stringstream ss(line);
            string idText, title, description, priority, completedText;

            if (!getline(ss, idText, '|') ||
                !getline(ss, title, '|') ||
                !getline(ss, description, '|') ||
                !getline(ss, priority, '|') ||
                !getline(ss, completedText)) {
                continue;
            }

            try {
                int id = stoi(idText);
                if (id <= 0 ||
                    (completedText != "0" && completedText != "1")) {
                    continue;
                }

                bool completed = (completedText == "1");
                tasks.emplace_back(id, title, description, priority, completed);
                if (id > maxId) maxId = id;
            } catch (const exception&) {
                continue;
            }
        }

        nextId = maxId + 1;
    }

    static bool validPriority(const string& priority) {
        return priority == "High" ||
               priority == "Medium" ||
               priority == "Low";
    }

public:
    ToDoList() : nextId(1) {
        loadTasks();
    }

    void addTask() {
        string title, description, priority;

        cout << "\nEnter task title: ";
        getline(cin, title);
        if (title.empty() || title.find('|') != string::npos) {
            cout << "Invalid title.\n";
            return;
        }

        cout << "Enter task description: ";
        getline(cin, description);
        if (description.find('|') != string::npos) {
            cout << "The '|' character is not allowed.\n";
            return;
        }

        cout << "Enter priority (High/Medium/Low): ";
        getline(cin, priority);
        if (!validPriority(priority)) {
            cout << "Invalid priority.\n";
            return;
        }

        tasks.emplace_back(nextId, title, description, priority);
        cout << "\nTask added successfully!\nAssigned Task ID: "
             << nextId << '\n';
        ++nextId;
        saveTasks();
    }

    void viewTasks() const {
        if (tasks.empty()) {
            cout << "\nNo tasks available.\n";
            return;
        }

        cout << "\n========== YOUR TASKS ==========\n";
        for (const Task& task : tasks) task.display();
    }

    void completeTask() {
        int id;
        cout << "\nEnter Task ID to complete: ";

        if (!(cin >> id)) {
            cout << "Invalid input.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        for (Task& task : tasks) {
            if (task.getId() == id) {
                if (task.isCompleted()) {
                    cout << "Task is already completed.\n";
                } else {
                    task.markCompleted();
                    saveTasks();
                    cout << "Task marked as completed!\n";
                }
                return;
            }
        }
        cout << "Task ID not found.\n";
    }

    void deleteTask() {
        int id;
        cout << "\nEnter Task ID to delete: ";

        if (!(cin >> id)) {
            cout << "Invalid input.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        for (auto it = tasks.begin(); it != tasks.end(); ++it) {
            if (it->getId() == id) {
                tasks.erase(it);
                saveTasks();
                cout << "Task deleted successfully!\n";
                return;
            }
        }
        cout << "Task ID not found.\n";
    }

    void showMenu() const {
        cout << "\n================================\n";
        cout << " TO-DO LIST MANAGEMENT SYSTEM\n";
        cout << "================================\n";
        cout << "1. Add Task\n";
        cout << "2. View Tasks\n";
        cout << "3. Mark Task as Completed\n";
        cout << "4. Delete Task\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
    }

    void run() {
        int choice = 0;
        do {
            showMenu();

            if (!(cin >> choice)) {
                cout << "Please enter a valid number.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (choice) {
                case 1: addTask(); break;
                case 2: viewTasks(); break;
                case 3: completeTask(); break;
                case 4: deleteTask(); break;
                case 5:
                    saveTasks();
                    cout << "Thank you for using the system!\n";
                    break;
                default:
                    cout << "Invalid choice. Try again.\n";
            }
        } while (choice != 5);
    }
};

int main() {
    ToDoList manager;
    manager.run();
    return 0;
}
