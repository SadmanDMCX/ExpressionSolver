#include <iostream> 
#include <vector>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <stack>
#define OPARATOR_ARRAY_SIZE 4
using namespace std;

// Global Variables
char oparatorArray[OPARATOR_ARRAY_SIZE] = {'+', '-', '*', '/'};

struct DeveloperCall {
    string call() {
        return "SadmanDMCX";
    }
} dc;
struct NameValuePair {
    string name;
    int value;
} nvp;
struct StringArray {
    string str[1024];
};

class string_ {
public:
    int length(string line) {
        int i = 0;
        while (line[i] != '\0') {
            i++;
        }
        return i;
    }
    bool contains(string line, char key) {
        for (int i = 0; i < line.length(); i++) {
            if (line[i] == key) {
                return true;
            }
        }
        return false;
    }
    bool contains(string line, string key) {
        if (line.find(key) < 1000) {
            return true;
        }
        return false;
    }
} str_;

void cleanSpaces(string &line) {
    for (int i = 0; i < line.length(); i++) {
        if (line[i] == ' ') {
            for (int j = i; j < line.length(); j++) {
                line[j] = line[j+1];
            }
            i--;
        }
    }
}

NameValuePair extractNumbers(string &line) {
    int value; string name;
    for (int i = 0; i < line.length(); i++) {
        if (line[i] == '=') {
            // extract the assignment oparator 
            name = line.substr(0, line.find("="));
            // get the string into the string stream
            stringstream converter(line.substr(line.find("=")+1, line.length()-line.find("=")+1));
            // put the value into the number property after convent into string to int
            converter >> value;
        }
    }
    nvp.name = name;
    nvp.value = value;
    
    return nvp;
}

StringArray extractAssignmentVariable(string &line) {
    int start = 0, length = 0; StringArray sar;
    bool startFound = false;
    for (int i = 0; i < str_.length(line); i++) {
        for (int j = 0; j < OPARATOR_ARRAY_SIZE; j++) {
            if (line[i] == oparatorArray[j]) {
                line[i] = ' ';
            } 
        }
    }
    
    char cline[1024]; int k = 0;
    strcpy(cline, line.c_str());
    char *tok = strtok(cline, " ");
    while (tok != NULL) {
        sar.str[k] = tok;
        tok = strtok(NULL, " ");
        k++;
    }
    
    return sar;
}

bool createExpression(string &line, vector<NameValuePair> nvpVector) {
    bool isCompilationError = false;
    int start = 0, p = 0, lineLength = str_.length(line);
    string temp = line;
    
    // Error checker 
    for (int i = 0; i < OPARATOR_ARRAY_SIZE; i++) {
        if (line[lineLength-1] == oparatorArray[i]) {
            isCompilationError = true;
            break;
        }
    }
    if (!isCompilationError) {        
        string temp = line;
        vector<NameValuePair> tempVector;
        StringArray sar = extractAssignmentVariable(temp);
        
        for (int i = 0; i < 1024; i++) {
            if (isCompilationError) {
                break;
            }
            if (sar.str[i].length() > 0) {
                for (vector<NameValuePair>::iterator itx = nvpVector.begin(); itx != nvpVector.end(); itx++) {
                    if (sar.str[i] != itx->name) {
                        isCompilationError = true;
                    } else {
                        isCompilationError = false;
                        break;
                    }
                }
            }
        }
    }
    
    if (isCompilationError)
        cout << "Compilation Error" << endl;
    else if (!isCompilationError) {
        for (int i = 0; i < line.length(); i++) {
            for (vector<NameValuePair>::iterator j = nvpVector.begin(); j != nvpVector.end(); j++) {
                if (str_.contains(line, j->name)) {
                    ostringstream str;
                    str << j->value;
                    line.replace(line.find(j->name), j->name.length(), str.str());
                } 
            }
        }
    }
    
    if (!isCompilationError) {
        return true;
    }
    
    return false;
}

int getWeight(char ch) {
   switch (ch) {
      case '/':
      case '*': return 2;
      case '+':
      case '-': return 1;
      default : return 0;
   }
}

void infix2postfix(string infix, string &postfix, int size) {
   stack<char> s;
   int weight;
   int i = 0;
   int k = 0;
   char ch;
   while (i < size) {
      ch = infix[i];
      if (ch == '(') {
         s.push(ch);
         i++;
         continue;
      }
      if (ch == ')') {
         while (!s.empty() && s.top() != '(') {
            postfix[k++] = s.top();
            s.pop();

         }
         if (!s.empty()) {
            s.pop();
         }
         i++;
         continue;
      }
      weight = getWeight(ch);
      if (weight == 0) {
         postfix[k++] = ch;

      }
      else {
         if (s.empty()) {
            s.push(ch);
         }
         else {
            while (!s.empty() && s.top() != '(' &&
                  weight <= getWeight(s.top())) {
               postfix[k++] = s.top();
               s.pop();

            }
            s.push(ch);
         }
      }
      i++;
   }
   while (!s.empty()) {
      postfix[k++] = s.top();
      s.pop();
   }
   postfix[k] = 0; 
}

void calculateExpression(string &line) {   
    int length = str_.length(line);
    infix2postfix(line, line, length);
    
    stack<int> s;
    int a, b;
    for (int i = 0; i < str_.length(line); i++)
    {
        if (line[i] >= 48 && line[i] <= 57)
            s.push(line[i]-48);
        else if (line[i] >= 42 && line[i] <= 47)
        {
            a=s.top();
            s.pop();
            b=s.top();
            s.pop();
            switch(line[i])
            {
            case '+':
                s.push(b+a);
                break;
            case '-':
                s.push(b-a);
                break;
            case '*':
                s.push(b*a);
                break;
            case '/':      
                s.push(b/a);
                break;
            }
        }
    }
    
    while (!s.empty()) {
        cout << s.top() << endl;
        s.pop();
    }   
}

int main() {
    
    // variables
    string inFileName = "in.txt", outFileName = "";
    string line;
    vector<NameValuePair> nvpVector;
    bool isExpression = false;
    
    // Initialize files
    fstream infile, outfile;
    // Open file
    infile.open("in.txt", ios::in);
    // Read the file 
    while (!infile.eof()) {
        getline(infile, line);
        cleanSpaces(line);
        if (line.length() != 0) {
            if (str_.contains(line, '=')) {
                nvpVector.push_back(extractNumbers(line));
                isExpression = false;
            } else {
                isExpression = true;
            }
            if (isExpression) {
                bool isExpression = createExpression(line, nvpVector);
                if (isExpression) {
                    cout << line << " : ";
                    calculateExpression(line);
//                    cout << line << endl;
                }
            }
        }    
    }
    
    cout << endl << "Developer: " << dc.call() << endl;
    return 0;
}