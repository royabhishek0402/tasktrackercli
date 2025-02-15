#include "nlohmann/json.hpp" 
#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;
using json = nlohmann::json;

//Add task 
void addTask(const string& taskDescription){
    time_t now = time(0);
    string dt = ctime(&now);
    dt.pop_back();

    ifstream inputFile("task.json");
    json tasks;
    if(!inputFile.is_open()){
        std::cout << "JSON file not found. Creating a new JSON file: task.json" << endl;
    }
    else{
        inputFile >> tasks;
    }

    if (!tasks.is_array()) {
        tasks = json::array();
    }

    json jsonData = {
        {"description", taskDescription},
        {"status", "todo"},
        {"createdAt", dt},
        {"updatedAt", dt},
        {"id", tasks.size() + 1}
    };

    tasks.push_back(jsonData);

    ofstream outFile("task.json");
    if (outFile.is_open()) {
        outFile << tasks.dump(4); 
        outFile.close();
        std::cout << "Task added successfully. ID: "<<tasks.size()<< endl;
    }
}

//lists all task 
void displayTask(){
    ifstream inputFile("task.json");
    json tasks;
    if(!inputFile.is_open()){
        std::cerr << "JSON file not found. Nothing to display. \n";
        return;
    }
    else{
        try {
        inputFile >> tasks;
        } catch (...) {
            std::cerr << "Error: Failed to parse JSON file.\n";
            return;
        }

        if(tasks.size() < 1) 
            std::cout << "Nothing to display" << endl;
    }

    if (!tasks.is_array()) {
        std::cerr << "Error: Tasks data is not an array.\n";
    }

    for(int i = 0; i < tasks.size(); ++i){
        cout<<tasks[i]<<endl;
    }
}

//list status wise task
void displayTask(const string& taskDescription){
    ifstream inputFile("task.json");
    json tasks;
    if(!inputFile.is_open()){
        std::cout << "JSON file not found. Nothing to display" << endl;
    }
    else{
        try {
        inputFile >> tasks;
        } catch (...) {
            std::cerr << "Error: Failed to parse JSON file.\n";
            return;
        }

    }

    if (!tasks.is_array()) {
        std::cerr << "Error: Tasks data is not an array.\n";
    }

    for(int i = 0; i < tasks.size(); ++i){
        if(tasks[i]["status"] == taskDescription)
            cout<<tasks[i]<<endl;
    }
}

//mark task progress
void markTask(int taskIndex,const string& taskStatus){
    ifstream inputFile("task.json");
    json tasks;
    if(!inputFile.is_open()){
        std::cout << "JSON file not found. Nothing to display" << endl;
    }
    else{
        try {
        inputFile >> tasks;
        } catch (...) {
            std::cerr << "Error: Failed to parse JSON file.\n";
            return;
        }
    }

    if (!tasks.is_array()) {
        std::cerr << "Error: Tasks data is not an array.\n";
    }

    if (taskIndex < 1 || taskIndex > tasks.size()) {
        std::cerr << "Error: Task index out of range. Use a valid index (1-" << tasks.size() << ").";
        return;
    }

    time_t now = time(0);
    string dt = ctime(&now);
    dt.pop_back();

    tasks[taskIndex - 1]["status"] = taskStatus;
    tasks[taskIndex - 1]["updatedAt"] = dt;
    std::cout << "Task " << taskIndex << " updated successfully.\n";

    ofstream outFile("task.json");
    if (outFile.is_open()) {
        outFile << tasks.dump(4); 
        outFile.close();
        std::cout << "data updated successfully." << endl;
    }
}

//update task status
void updateTask(int taskIndex,const string& taskDescription){
    ifstream inputFile("task.json");
    json tasks;
    if(!inputFile.is_open()){
        std::cout << "JSON file not found. Nothing to display" << endl;
    }
    else{
        try {
        inputFile >> tasks;
        } catch (...) {
            std::cerr << "Error: Failed to parse JSON file.\n";
            return;
        }
    }

    if (!tasks.is_array()) {
        std::cerr << "Error: Tasks data is not an array.\n";
    }

    if (taskIndex < 1 || taskIndex > tasks.size()) {
        std::cerr << "Error: Task index out of range. Use a valid index (1-" << tasks.size() << ").";
        return;
    }

    time_t now = time(0);
    string dt = ctime(&now);
    dt.pop_back();

    tasks[taskIndex - 1]["description"] = taskDescription;
    tasks[taskIndex - 1]["updatedAt"] = dt;
    std::cout << "Task " << taskIndex << " updated successfully.\n";

    ofstream outFile("task.json");
    if (outFile.is_open()) {
        outFile << tasks.dump(4); 
        outFile.close();
        std::cout << "data updated successfully." << endl;
    }
}

//delete task
void deleteTask(int taskIndex){
    ifstream inputFile("task.json");
    json tasks;
    if(!inputFile.is_open()){
        std::cout << "JSON file not found. Nothing to display" << endl;
    }
    else{
        try {
        inputFile >> tasks;
        } catch (...) {
            std::cerr << "Error: Failed to parse JSON file.\n";
            return;
        }
    }

    if (!tasks.is_array()) {
        std::cerr << "Error: Tasks data is not an array.\n";
    }

    if (taskIndex < 1 || taskIndex > tasks.size()) {
        std::cerr << "Error: Task index out of range. Use a valid index (1-" << tasks.size() << ").";
        return;
    }

    tasks.erase(tasks.begin() + (taskIndex - 1));
    std::cout << "Task " << taskIndex << " deleted successfully."<<endl;

    for(int i = 0; i < tasks.size(); ++i){
       tasks[i]["id"] = i + 1;
    }
    std::cout << "Task id reassigned .\n"; 

    std::ofstream outputFile("task.json");
    if (outputFile.is_open()) {
        outputFile << tasks.dump(4); 
        outputFile.close();
    } else {
        std::cerr << "Error: Unable to write to file.\n";
    }
}

int main(int argc, char* argv[])
{

    if(argc < 2){
        std::cout<<"Invalid command"<<endl;
        return 0;
    }

    string command = argv[1];

    if(command == "add"){
        if(argc < 3){
            std::cout<<"missing description. return failure"<<endl;
            return 0;
        }

        string taskdec = argv[2];
        addTask(taskdec);
    }
    else if(command == "list"){
        if(argc < 3){
            displayTask();
        }
        else{
            string taskdec = argv[2];
            displayTask(taskdec);
        }
    }
    else if(command == "delete"){
        if(argc < 3){
            std::cout<<"missing index. return failure"<<endl;
            return 0;
        }

        int idx = stoi(argv[2]);
        deleteTask(idx);
    }
    else if(command == "update"){
        if(argc < 4){
            std::cout<<"missing index/description. return failure"<<endl;
            return 0;
        }

        int idx = stoi(argv[2]);
        string taskdec = argv[3];
        updateTask(idx,taskdec);
    }
    else if((command == "mark-in-progress")||(command == "mark-done")){
        if(argc < 3){
            std::cout<<"missing index. return failure"<<endl;
            return 0;
        }

        string taskstatus = argv[1];
        taskstatus.erase(taskstatus.begin(),taskstatus.begin() + 5);
        int idx = stoi(argv[2]);

        markTask(idx,taskstatus);
    }
    else{
        cout<<"Invalid argument"<<endl;
        return 0;
    }      
    return 1;
}