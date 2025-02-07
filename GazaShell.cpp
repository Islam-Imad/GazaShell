#include <iostream>
#include <map>
#include <string>
#include <cassert>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <vector>

using namespace std;

#define MAX_LENGTH 100

class ColorId
{
private:
    map<string, int> colorMap;

public:
    ColorId()
    {
        colorMap["black"] = 40;
        colorMap["red"] = 41;
        colorMap["green"] = 42;
        colorMap["yellow"] = 43;
        colorMap["blue"] = 44;
        colorMap["magenta"] = 45;
        colorMap["cyan"] = 46;
        colorMap["white"] = 47;
    }
    string getForegroundCode(string color)
    {
        return to_string(colorMap[color] - 10);
    }
    string getBackgroundCode(string color)
    {
        return to_string(colorMap[color]);
    }
};

class Printer
{
public:
    void setBackForegroundColor(string back, string fore, ColorId colorId)
    {
        string foreColorCode = colorId.getForegroundCode(fore);
        string backColorCode = colorId.getBackgroundCode(back);
        printf("\033[%s;%sm", backColorCode.c_str(), foreColorCode.c_str());
    }
    void resetColor()
    {
        printf("\033[0m");
    }
    string getArrow()
    {
        return "\uE0B0";
    }
    string getInputArrow()
    {
        return "\uE0B1";
    }
};

long long getMicroSeconds()
{
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return tp.tv_sec * 1000000 + tp.tv_usec;
}

string get_path()
{
    string path = "-1";
    int g_fd[2];
    pipe(g_fd);
    pid_t g_pid = fork();
    if (g_pid == 0)
    {
        dup2(g_fd[1], STDOUT_FILENO);
        char *command = new char[4];
        strcpy((char *)command, "pwd");
        char *args[] = {command, NULL};
        execvp(args[0], args);
        exit(1);
    }
    else
    {
        wait(NULL);
        char buffer[100];
        int bytes = read(g_fd[0], buffer, 100);
        buffer[bytes - 1] = '\0';
        path = buffer;
    }
    return path;
}

vector<string> split(const string &str, char delimiter)
{
    vector<string> tokens;
    int n = str.length();
    int last = -1, cur = 0;
    for (cur; cur <= n; ++cur)
    {
        if (str[cur] == delimiter or cur == n)
        {
            if (cur - last > 1)
            {
                tokens.push_back(str.substr(last + 1, cur - last - 1));
            }
            last = cur;
        }
    }
    return tokens;
}

string concatenate(const vector<string> &tokens, char delimiter)
{
    string result = "";
    for (int i = 0; i < tokens.size(); i++)
    {
        result += tokens[i];
        if (i != tokens.size() - 1)
        {
            result += delimiter;
        }
    }
    return result;
}

string concatenate(const vector<string> &tokens, int start, int end, char delimiter)
{
    string result = "";
    for (int i = start; i < end; i++)
    {
        result += tokens[i];
        if (i != end - 1)
        {
            result += delimiter;
        }
    }
    return result;
}

int main()
{
    string path = "-1";
    string displayed_path = "-1";
    vector<string> path_tokens;
    int nested_dir = 0;
    Printer printer;
    ColorId colorId;

    long long lastElapsedTime = -1;
    char *prv[MAX_LENGTH];

    for (int i = 0; i < MAX_LENGTH; i++)
    {
        prv[i] = NULL;
    }

    int should_run = 1;
    while (should_run)
    {
        cout.flush();

        path = get_path();
        path_tokens = split(path, '/');
        nested_dir = path_tokens.size();
        displayed_path = concatenate(path_tokens, max(0, nested_dir - 3), nested_dir, '/');

        printer.setBackForegroundColor("blue", "black", colorId);
        cout << " Gaza Shell ";
        printer.setBackForegroundColor("black", "blue", colorId);
        cout << printer.getArrow();

        printer.setBackForegroundColor("green", "black", colorId);
        cout << printer.getArrow() << displayed_path << " " << printer.getInputArrow() << printer.getInputArrow() << printer.getInputArrow();
        printer.setBackForegroundColor("black", "green", colorId);
        cout << printer.getArrow();

        printer.setBackForegroundColor("blue", "black", colorId);
        cout << printer.getArrow() << " \u23F0 : ";

        if (lastElapsedTime != -1)
        {
            cout << lastElapsedTime;
        }
        else
        {
            cout << "0";
        }
        cout << " micro";
        printer.setBackForegroundColor("black", "blue", colorId);
        cout << printer.getArrow();
        printer.resetColor();
        cout << " ";

        string command;
        getline(cin, command);
        cin.clear();

        stringstream ss(command);
        string token;
        char *args[MAX_LENGTH];

        int index = 0;
        int input = 0;
        int output = 0;
        int ampersand = 0;
        int _pipe = 0;
        int pipeIndex = -1;
        char *inputFile = NULL;
        char *outputFile = NULL;
        while (ss >> token)
        {
            if (token == "|")
            {
                _pipe += 1;
                args[index] = NULL;
                index++;
                pipeIndex = index;
            }
            else if (token == "&")
            {
                ampersand = 1;
                break;
            }
            else if (token == "<")
            {
                ss >> token;
                inputFile = new char[token.length() + 1];
                strcpy(inputFile, token.c_str());
                input = 1;
            }
            else if (token == ">")
            {
                ss >> token;
                outputFile = new char[token.length() + 1];
                strcpy(outputFile, token.c_str());
                output = 1;
            }
            else
            {
                args[index] = new char[token.length() + 1];
                strcpy(args[index], token.c_str());
                index++;
            }
        }
        args[index] = NULL;
        if (_pipe >= 2)
        {
            cout << "Multiple pipes are not supported" << endl;
            continue;
        }
        else if (_pipe and (input or output))
        {
            cout << "Pipes and redirection at the same time is not supported" << endl;
            continue;
        }
        else if (index == 0)
        {
            continue;
        }
        else if (strcmp(args[0], "exit") == 0)
        {
            should_run = 0;
            continue;
        }
        else if (input and output)
        {
            cout << "Input and output redirection at the same time is not supported" << endl;
            continue;
        }
        else if (strcmp(args[0], "cd") == 0)
        {
            if (chdir(args[1]) != 0)
            {
                cout << "No such file or directory" << endl;
            }
        }
        else
        {
            if (strcmp(args[0], "!!") == 0)
            {
                if (prv[0] == NULL)
                {
                    cout << "No previous command" << endl;
                    continue;
                }
                for (int i = 0; i < MAX_LENGTH; i++)
                {
                    args[i] = prv[i];
                }
            }

            int mp[2];
            pipe(mp);

            pid_t pid = fork();
            if (pid == 0)
            {
                long long start = getMicroSeconds();
                write(mp[1], &start, sizeof(start));
                int fd = -1;

                if (input)
                {
                    fd = open(inputFile, O_RDONLY | O_CREAT, 0644);
                    dup2(fd, STDIN_FILENO);
                }
                if (output)
                {
                    fd = open(outputFile, O_WRONLY | O_CREAT, 0644);
                    dup2(fd, STDOUT_FILENO);
                }

                if (_pipe)
                {
                    int nmp[2];
                    pipe(nmp);
                    pid_t pid2 = fork();
                    if (pid2 == 0)
                    {
                        close(nmp[0]);
                        dup2(nmp[1], STDOUT_FILENO);
                        close(nmp[1]);
                        execvp(args[0], args);
                        perror("execvp");
                        exit(1);
                    }
                    else
                    {
                        close(nmp[1]);
                        wait(NULL);

                        char fd_path[20];
                        snprintf(fd_path, sizeof(fd_path), "/dev/fd/%d", nmp[0]);
                        args[index] = fd_path;
                        args[index + 1] = NULL;

                        execvp(args[pipeIndex], args + pipeIndex);
                        perror("execvp");
                        exit(1);
                    }
                }
                else
                {
                    execvp(args[0], args);
                }
                if (errno == 2)
                {
                    cout << "Command not found" << endl;
                }
                close(mp[0]);
                close(mp[1]);
                exit(0);
            }
            else
            {
                for (int i = 0; i < MAX_LENGTH; i++)
                {
                    prv[i] = args[i];
                }

                wait(NULL);
                long long start;
                read(mp[0], &start, sizeof(start));
                long long end = getMicroSeconds();
                lastElapsedTime = end - start;
                cout << endl;
            }
        }
    }
    return 0;
}

