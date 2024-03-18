#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cctype>

using namespace std;

// TASK CLASS
class Task{
private:
    string title;
    string description;
    bool completed;
    string dueDate;

public:
    // parameterised constructor
    Task(const string& _title, const string& _description, const string& _dueDate, bool _completed = false):
        title(_title), description(_description), dueDate(_dueDate), completed(_completed){}

    // virtual destructor
    virtual ~Task(){}

    // getter and setter for title
    string getTitle() const { return title; }
    void setTitle(const string& newTitle){ title = newTitle; }

    // getter and setter for description
    string getDescription() const { return description; }
    void setDescription(const string& newDescription) { description = newDescription; }

    // getter and setter for dueDate
    string getDueDate() const { return dueDate; }
    void setDueDate(const string& newDueDate) { dueDate = newDueDate; }

    // getter and setter for completed
    bool isCompleted() const { return completed; }
    void markAsCompleted(){ completed = true; }

    // display details about a task
    virtual void displayDetails() const {
        cout << "Title: " << title <<
                "\nDescription: " << description <<
                "\nDue date: " << dueDate <<
                "\nStatus: " << (completed ? "Completed" : "Not completed");
    }

};

// PERSONAL TASK CLASS
class PersonalTask : public Task{
private:
    int urgencyLevel;

public:
    // parameterised constructor
    PersonalTask(const string& _title, const string& _description, const string& _dueDate, int _urgencyLevel, bool _completed = false) :
        Task(_title, _description, _dueDate, _completed), urgencyLevel(_urgencyLevel) {}

    // getter and setter for urgency level
    int getUrgencyLevel() const { return urgencyLevel; }
    void setUrgencyLevel(int& newUrgencyLevel) { urgencyLevel = newUrgencyLevel; }

    // display details about a task (It overrides displayDetails() function on Task Class)
    void displayDetails() const override{
        Task::displayDetails();
        cout << "\nUrgency Level: " << urgencyLevel << endl;
    }

};

// SCHOOL TASK CLASS
class SchoolTask : public Task{
private:
    string subject;
    int studyTime;

public:
    // parameterised constructor
    SchoolTask(const string& _title, const string& _description, const string& _dueDate, const string& _subject, int& _studyTime,
               bool _completed = false) : Task(_title, _description, _dueDate, _completed),
               subject(_subject), studyTime(_studyTime) {}

    // getter and setter for subject
    string getSubject() const { return subject; }
    void setSubject (const string& newSubject) { subject = newSubject; }

    // getter and setter for study time
    int getStudyTime() const { return studyTime; }
    void setStudyTime(int& newStudyTime) { studyTime = newStudyTime; }

    // display details about a task (It overrides displayDetails() function on Task Class)
    void displayDetails() const override {
        Task::displayDetails();
        cout << "\nSubject: " << subject <<
                "\nEstimated study time: " << studyTime << " minutes" << endl;
    }

};

// USER CLASS
class User {
private:
    string username;
    vector<Task*> tasks;

public:
    // parameterised constructor
    User(const string& _username) : username(_username) {}

    // add task for this user
    void addTask(Task* task){ tasks.push_back(task); }

    // remove task for this user
    void removeTask(int& taskID) {
        if (taskID >= 0 && taskID < tasks.size()) {
            delete tasks[taskID];
            tasks.erase(tasks.begin() + taskID);
            cout << "Task has been removed successfully." << endl;
        }
        else {
            cout << "Task ID is not valid. Failed to remove the task." << endl;
        }
    }

    // mark a specific task as completed
    void markTaskAsCompleted(int& taskID){
        if (taskID >= 0 && taskID < tasks.size()){
            tasks[taskID]->markAsCompleted();
            cout << "Task has been marked as completed." << endl;
        }
        else {
            cout << "Task ID is not valid. Failed to mark the task as completed." << endl;
        }
    }

    // display all the tasks available
    void displayAllTasks() const {
        for (int i = 0; i < tasks.size(); i++){
            cout << "Task " << i << endl;
            tasks[i]->displayDetails();
            cout << "\n\n";
        }
    }

    // display tasks by status (completed or not completed)
    void displayTasksByStatus(bool status) const {
        cout << (status ? "Completed" : "Not completed") << endl;
        for (int i = 0; i < tasks.size(); i++){
            if (tasks[i]->isCompleted() == status){
                cout << "Task " << i << endl;
                tasks[i]->displayDetails();
                cout << "\n";
            }
        }
    }

    // getter and setter for username
    string getUsername() const { return username; }
    void setUsername(const string& newUsername) { username = newUsername; }

    // getter and setter for tasks
    vector<Task*> getTasks() const { return tasks; }
    void setTasks(const vector<Task*>& newTasks) {
        for(Task* task: tasks)
            delete task;

        tasks.clear();
        tasks = newTasks;
    }

};

// FILE MANAGER CLASS
class FileManager {
public:
    // save tasks to the chosen file
    static void saveTasks(const vector<Task*>& tasks, const string& fileName) {
        ofstream file(fileName);
        if(!file.is_open()){
            cerr << "Failed to open the file called " << fileName << endl;
        }
        else {
            for (const auto& task : tasks){
                file << task->getTitle() << "," << task->getDescription() << "," << task->getDueDate() << "," << task->isCompleted() << endl;
            }
            file.close();
        }
    }

    // load tasks from the file

    static vector<Task*> loadTasks(const string& fileName) {
        ifstream  file(fileName);
        vector<Task*> tasks;
        string line;

        if(!file.is_open()){
            cerr << "Failed to open the file called " << fileName << endl;
        }
        else {
            while (getline(file, line)){
                stringstream ss(line);
                string title, description, dueDate, strCompleted;
                bool completed;

                getline(ss, title, ',');
                getline(ss, description ,',');
                getline(ss, dueDate, ',');
                getline(ss, strCompleted, ',');

                completed = (strCompleted == "1");

                tasks.push_back(new Task(title, description, dueDate, completed));
            }
            file.close();
            return tasks;
        }
    }

};

int main(){
    User user("ertan"); // change the username to your username
    string fileName = ""; // put the path for tasks.txt file for your computer

    // Load the tasks
    vector<Task*> tasksLoaded = FileManager::loadTasks(fileName);
    for (Task* task : tasksLoaded){
        user.addTask(task);
    }

    int choice = 0;

    do {
        cout << "===== TASK MANAGER =====" << endl << endl;
        cout << "1. Add a task" << endl;
        cout << "2. Remove a task" << endl;
        cout << "3. View all tasks" << endl;
        cout << "4. Mark a task as completed" << endl;
        cout << "5. Display tasks by status" << endl;
        cout << "6. Exit" << endl << endl;
        cout << "Enter your choice: " << endl;

        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: {
                // add a new task
                string title, description, dueDate;
                int type;
                cout << "=== Add a task ===" << endl << endl;
                cout << "Enter task details" << endl;

                cout << "Task title: " << endl;
                getline(cin, title);

                cout << "Task description: " << endl;
                getline(cin, description);

                cout << "Task deadline (DD/MM/YYYY): " << endl;
                getline(cin, dueDate);

                cout << "Personal(1) or School related task(2): " << endl;
                cin >> type;
                cin.ignore();


                if (type == 1){
                    int urgencyLevel;
                    cout << "Urgency level 1 (not important) - 10 (very important): " << endl;
                    cin >> urgencyLevel;
                    cin.ignore();
                    Task* newTask = new PersonalTask(title, description, dueDate, urgencyLevel);
                    user.addTask(newTask);
                }

                else if (type == 2) {
                    string subject;
                    int studyTime;
                    cout << "Subject: " << endl;
                    getline(cin, subject);
                    cout << "Estimated study time in minutes: " << endl;
                    cin >> studyTime;
                    Task* newTask = new SchoolTask(title, description, dueDate, subject, studyTime);
                    user.addTask(newTask);
                }


                else {
                    cout << "Failed to add the task. Invalid type" << endl;
                }

                FileManager::saveTasks(user.getTasks(), fileName);
                cout << "Task added" << endl;
                break;
            }
            case 2: {
                // remove a task
                cout << "=== Remove a task ===" << endl << endl;
                user.displayAllTasks();
                int taskID;
                cout << "Task ID: " << endl;
                cin >> taskID;
                cin.ignore();

                if (isdigit(taskID) || taskID >= 0 || taskID >= user.getTasks().size()){
                    user.removeTask(taskID);
                    FileManager::saveTasks(user.getTasks(), fileName);
                }
                else {
                    cout << "Failed to remove the task. Invalid task ID" << endl;
                }
                break;
            }
            case 3: {
                // view all tasks
                cout << "=== View all tasks ===" << endl << endl;
                user.displayAllTasks();
                break;
            }
            case 4: {
                // mark a task as completed
                cout << "=== Mark a task as completed ===" << endl << endl;
                user.displayAllTasks();
                int taskID;
                cout << "Task ID: " << endl;
                cin >> taskID;
                user.markTaskAsCompleted(taskID);
                FileManager::saveTasks(user.getTasks(), fileName);
                break;
            }
            case 5: {
                // display tasks by status
                bool status;
                cout << "=== Mark a task as completed ===" << endl << endl;
                cout << "Display completed tasks (1) or incomplete tasks(0): " << endl;
                cin >> status;
                cin.ignore();
                user.displayTasksByStatus(status);
                break;
            }
            case 6: {
                // exit
                cout << "=== EXIT ===" << endl << endl;
                cout << "You just exited Task Manager" << endl;
                break;
            }
        }

    } while (choice != 6);

    return 0;
}















