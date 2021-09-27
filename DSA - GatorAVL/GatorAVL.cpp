#pragma once
#include <cctype>
#include <algorithm>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <stack>

class Student
{
public:
    std::string name;
    int id;
    Student* left;
    Student* right;
    Student()
    {
        name = "";
        id = NULL;
        left = nullptr;
        right = nullptr;
    }
    Student* createStudent(std::string NAME, int ID)
    {
        Student* newStudent = new Student;
        newStudent->name = NAME;
        newStudent->id = ID;
        newStudent->left = nullptr;
        newStudent->right = nullptr;
        return newStudent;
    }
    
    // --- FUNCTIONALITY COMMANDS ---

    // INSERTION
    Student* insert(Student* root, std::string NAME, std::string ID)
    {
        // Check ID and Name validity
        if (validNAME(NAME) && validID(ID))
        {
            // create new student
            if (root == nullptr) 
            {
                std::cout << "successful\n";
                return createStudent(NAME, stoi(ID));
            }

            // traverse the tree, check uniqueness
            if (stoi(ID) < root->id)
                root->left = insert(root->left, NAME, ID);
            else if (stoi(ID) > root->id)
                root->right = insert(root->right, NAME, ID);
            else if (stoi(ID) == root->id)
                std::cout << "unsuccessful\n";

            // Auto-Balance
            int heightLeft = treeHeight(root->left);
            int heightRight = treeHeight(root->right);
            int height = heightLeft - heightRight;
            if (height < -1)
                root = rotateLeft(root);
            else if (height > 1)
                root = rotateRight(root);

            return root;
        }
        else
            std::cout << "unsuccessful" << std::endl;
    }
    // DELETION
    Student* remove(Student* root, int ID)
    {
        // base case if not found
        if (root == nullptr)
        {
            std::cout << "unsuccessful" << std::endl;
            return root;
        }
        // traversal
        if (ID < root->id)
            root->left = remove(root->left, ID);
        else if (ID > root->id)
            root->right = remove(root->right, ID);
        // deletion and reassignment of pointers
        else
        {
            if (root->left == nullptr)
            {
                Student* student = root->right;
                delete root;
                return student;
            }
            else if (root->right == nullptr)
            {
                Student* student = root->left;
                delete root;
                return student;
            }
            Student* student = smallestID(root->right);
            root->name = student->name;
            root->id = student->id;
            root->right = remove(root->right, student->id);
            std::cout << "successful" << std::endl;
        }
        return root;
    }
    void removeInorder(Student* root, int N)
    {
        // create a stack, which is pushed into vector
        std::stack<Student*> s;
        std::vector<Student*> list;
        // pointer to traverse iteratively
        Student* ptr = root;
        while (ptr != nullptr || s.empty() == false)
        {
            while (ptr != nullptr)
            {
                s.push(ptr);
                ptr = ptr->left;
            }
            ptr = s.top();
            s.pop();
            list.push_back(ptr);
            ptr = ptr->right;
        }
        // checks if N is a bound within index
        if (N < list.size())
        {
            Student* nStudent = list[N];
            root = remove(root, nStudent->id);
            std::cout << "successful\n";
        }
        else
            std::cout << "unsuccessful\n";
       
    }
    // SEARCH
    Student* searchID(Student* root, int ID)
    {
        // base case if not found
        if (root == nullptr)
        {
            std::cout << "unsuccessful" << std::endl;
            return root;
        }
        // root is the Student
        else if (ID == root->id)
        {
            std::cout << root->name << std::endl;
            return root;
        }
        // traversal
        else if (ID < root->id)
            return searchID(root->left, ID);
        else
            return searchID(root->right, ID);
    }
    void searchNAME(Student* root, std::string NAME)
    {
        // base case if not found
        if (root == nullptr)
            std::cout << "unsuccessful\n";
        // output if name matches (in 8 digit format)
        else if (NAME.compare(root->name) == 0)
            std::cout << std::setw(8) << std::setfill('0') << root->id << std::endl;
        // traversal
        else
        {
            searchNAME(root->left, NAME);
            searchNAME(root->right, NAME);
        }
    }
    // PRINT
    void printInorder(Student* root)
    {
        // base case
        if (root == nullptr)
            std::cout << "";
        else
        {
            // root names and comma placement checks
            printInorder(root->left);
            if (root->left != nullptr)
                std::cout << ", ";
            std::cout << root->name;
            if (root->right != nullptr)
                std::cout << ", ";
            printInorder(root->right);
        }
    }
    void printPreorder(Student* root)
    {
        // base case
        if (root == nullptr)
            std::cout << "";
        else
        {
            // root names and comma placement checks
            std::cout << root->name;
            if (root->left != nullptr)
                std::cout << ", ";
            printPreorder(root->left);
            if (root->right != nullptr)
                std::cout << ", ";
            printPreorder(root->right);
        }
    }
    void printPostorder(Student* root)
    {
        // base case
        if (root == nullptr)
            std::cout << "";
        else
        {
            // root names and comma placement checks
            printPostorder(root->left);
            if (root->left != nullptr)
                std::cout << ", ";
            printPostorder(root->right);
            if (root->right != nullptr)
                std::cout << ", ";
            std::cout << root->name;
        }
    }
    void printLevelCount(Student* root)
    {
        std::cout << treeHeight(root) << std::endl;
    }

    // --- HELPER FUNCTIONS ---
    bool validNAME(std::string NAME)
    {
        for (int i = 0; i < NAME.size(); i++)
        {
            if (!isalpha(NAME[i]) && !isspace(NAME[i]))
                return false;
        }
        return true;
    }
    bool validID(std::string ID)
    {
        if (stoi(ID) < 0)
            return false;
        else if (ID.length() != 8)
            return false;
        else
            return true;
    }
    int treeHeight(Student* root)
    {
        int leftHeight;
        int rightHeight;

        if (root == NULL)
            return 0;
        else
            return 1 + std::max(treeHeight(root->left), treeHeight(root->right));
    }
    Student* rotateLeft(Student* student)
    {
        Student* grandchild = student->right->left;
        Student* newParent = student->right;
        newParent->left = student;
        student->right = grandchild;
        return newParent;
    }
    Student* rotateRight(Student* student)
    {
        Student* grandchild = student->left->right;
        Student* newParent = student->left;
        newParent->right = student;
        student->left = grandchild;
        return newParent;
    }
    Student* rotateLeftRight(Student* student)
    {
        Student* child = student->left;
        student->left = rotateLeft(child);
        student = rotateRight(student);
        return student;
    }
    Student* rotateRightLeft(Student* student)
    {
        Student* child = student->right;
        student->right = rotateRight(child);
        student = rotateLeft(student);
        return student;
    }
    Student* smallestID(Student* student)
    {
        Student* point = student;
        while (point && point->left != nullptr)
            point = point->left;
        return point;
    }
    Student* largestID(Student* student)
    {
        Student* point = student;
        while (point && point->right != nullptr)
            point = point->left;
        return point;
    }
};
int main()
{
	// AVL initialization
	Student gators; 
	Student *root = nullptr;

	// input initialization
	int numLines = 0;
	
	std::cin >> numLines;
	// fixes some newline issues with cin, so getline isn't infinite loop
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	while (numLines > 0)
	{
		// PUSH ARGUMENTS INTO A VECTOR
		std::vector<std::string> commands;
		std::string line;
		std::getline(std::cin, line);
		std::stringstream c(line);
		std::string command;
		while (c >> command) { commands.push_back(command); }
		
		// PARSE ARGUMENT PARAMETERS
		std::string arg = commands[0];
		if (arg.compare("insert") == 0) {
			std::string studentName = commands[1].substr(1, commands[1].size() - 2);
			root = gators.insert(root, studentName, commands[2]); }
		else if (arg.compare("search") == 0) {
			if (isdigit(commands[1][0])) { gators.searchID(root, stoi(commands[1])); }
			else { std::string studentName = commands[1].substr(1, commands[1].size() - 2); 
					gators.searchNAME(root, studentName); } }
		else if (arg.compare("printInorder") == 0) { gators.printInorder(root); std::cout << std::endl; }
		else if (arg.compare("printPreorder") == 0) { gators.printPreorder(root); std::cout << std::endl; }
		else if (arg.compare("printPostorder") == 0) { gators.printPostorder(root); std::cout << std::endl; }
		else if (arg.compare("printLevelCount") == 0) { gators.printLevelCount(root); }
		else if (arg.compare("remove") == 0) { gators.remove(root, stoi(commands[1])); }
		else if (arg.compare("removeInorder") == 0) { gators.removeInorder(root, stoi(commands[1])); }
		else
			std::cout << "Invalid argument \n";

		numLines--;
	}
	
	return 0;
}