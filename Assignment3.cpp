#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>
using namespace std;

int n;
vector<int> polynomial;
string filename;

void readFile() {
    polynomial.clear();
    int numInput;
    int i = 0;
    bool firstLine = true;

    ifstream input_file(filename);
    while (input_file >> numInput) {
        if (!firstLine) {
            polynomial.push_back(numInput);
        }
        else {
            n = numInput;
            firstLine = false;
        }
    }
    input_file.close();
}

void printPolonomial() {
    int size = polynomial.size();
    for (int i = 0; i < size; i++) {
        //Don't print if current monomial = 0
        if (polynomial[i] != 0) {
            //Don't print add/subtract signs on the first monomial
            bool notFirst = i > 0;
            if (notFirst) {
                if (polynomial[i] > 0)
                    cout << " + ";
                else if (polynomial[i] < 0)
                    cout << " - ";
            }

            //Print monomial
            if (polynomial[i] != 0)
                cout << abs(polynomial[i]);
            //Print coefficient of monomial
            if (i != size - 1)
                cout << "x^" << n - i;
        }
    }
    cout << "\n";
}

float f(float x) {
    float sum = 0;
    for (int i = 0; i < n; i++) {
        if (polynomial[i] != 0) sum += pow(x, n - i);
    }
    return sum + polynomial[n];
}

float derF(float x) {
    float sum = 0;
    for (int i = 0; i < n; i++) {
        if (polynomial[i] != 0) sum += (n - i) * pow(x, n - i - 1);
    }
    return sum;
}

void writeSol(float root, int iterations, string outcome) {
    ofstream outfile;
    string file = filename.erase(filename.find(".pol"));
    outfile.open(file + ".sol");

    outfile << root << " " << iterations << " " << outcome;
    outfile.close();
}

void Bisection(float a, float b, int maxIter, float eps) {
    float fa = f(a);
    float fb = f(b);

    if (fa * fb >= 0) {
        cout << "Inadequate values for a and b.\n";
        return;
    }

    float error = b - a;
    float c = 0;

    for (int it = 1; it <= maxIter; it++) {
        error = error / 2;
        c = a + error;
        float fc = f(c);

        if (abs(error) < eps || fc == 0) {
            string outcome = "success";
            cout << "Algorithm has converged after " + to_string(it) + " iterations!\n";
            writeSol(c, it, outcome);
            return;
        }
        if (fa * fc  < 0) {
            b = c;
            fb = fc;
        } else {
            a = c;
            fa = fc;
        }
    }
    string outcome = "fail";
    cout << "Max iterations reached without convergence...\n";
    writeSol(c, maxIter, outcome);
    return;
}

void Newton(float x, int maxIter, float eps, float delta) {
    float fx = f(x);

    for (int it = 1; it <= maxIter; it++) {
        float fd = derF(x);
        fd = 1;

        if (abs(fd) < delta) {
            string outcome = "fail";
            cout << "Small slope!\n";
            writeSol(x, it, outcome);
            return;
        }

        float d = fx / fd;
        x = x - d;
        fx = f(x);

        if (abs(d) < eps) {
            string outcome = "success";
            cout << "Algorithm has converged after " + to_string(it) + " iterations!\n";
            writeSol(x, it, outcome);
            return;
        }
    }
    string outcome = "fail";
    cout << "Max iterations reached without convergence...\n";
    writeSol(x, maxIter, outcome);
    return;
}

void Secant(float a, float b, int maxIter, float eps) {
    float fa = f(a);
    float fb = f(b);

    if (abs(fa) > abs(fb)) {
        swap(a, b);
        swap(fa, fb);
    }

    for (int it = 1; it <= maxIter; it++) {
        if  (abs(fa) > abs(fb)) {
            swap(a, b);
            swap(fa, fb);
        }

        float d = (b - a) / (fb - fa);
        b = a;
        fb = fa;
        d = d * fa;

        if  (abs(d) < eps) {
            string outcome = "success";
            cout << "Algorithm has converged after " + to_string(it) + " iterations!\n";
            writeSol(a, it, outcome);
            return;
        }
        a = a - d;
        fa = f(a);
    }
    string outcome = "fail";
    cout << "Max iterations reached without convergence...\n";
    writeSol(a, maxIter, outcome);
    return;
}

void Hybrid(float a, float b, float x, int maxIter, float eps, float delta) {
    float fa = f(a);
    float fb = f(b);

    if (fa * fb >= 0) {
        cout << "Inadequate values for a and b.\n";
        return;
    }

    float error = b - a;
    float c = 0;

    for (int it = 1; it <= maxIter / 2; it++) {
        error = error / 2;
        c = a + error;
        float fc = f(c);

        if (abs(error) < eps || fc == 0) {
            string outcome = "success";
            cout << "Algorithm has converged after " + to_string(it) + " iterations!\n";
            writeSol(c, it, outcome);
            return;
        }

        if (fa * fc  < 0) {
            b = c;
            fb = fc;
        } else {
            a = c;
            fa = fc;
        }
    }

    float fx = f(x);

    for (int it = maxIter / 2; it <= maxIter; it++) {
        float fd = derF(x);
        fd = 1;

        if (abs(fd) < delta) {
            string outcome = "fail";
            cout << "Small slope!\n";
            writeSol(x, it, outcome);
            return;
        }

        float d = fx / fd;
        x = x - d;
        fx = f(x);

        if (abs(d) < eps) {
            string outcome = "success";
            cout << "Algorithm has converged after " + to_string(it) + " iterations!\n";
            writeSol(x, it, outcome);
            return;
        }
    }
    string outcome = "fail";
    cout << "Max iterations reached without convergence...\n";
    writeSol(x, maxIter, outcome);
    return;
}

bool parseInput() {
    int point1;
    int point2;
    int maxIter = 10000;
    string line;

    while (0 == 0) {
        getline (cin, line);
        if (line == "") return false;

        int _pos = line.find("-maxIter");
        if (_pos != string::npos) { //optionally set max iterations
            string temp = line.substr(_pos, line.length());
            int _pos2 = temp.find(" ");
            temp.erase(0, _pos2 + 1);
            int _pos3 = _pos2 = temp.find(" ");
            string maxIterStr = temp.substr(0, _pos3);
            maxIter = stoi(maxIterStr); //set maxIter

            line.erase(_pos, 8 + maxIterStr.length() + 2); //remove command out of the way for next commands
            //          8 is length of "-maxIter"   2 for removing 2 spaces
        } 
        
        _pos = line.find(" ");
        if (_pos != string::npos) {
            if (line.substr(0, _pos) == "-newt") { //run Netwon's method
                line.erase(0, _pos + 1);
                _pos = line.find(" ");
                point1 = stoi(line.substr(0, _pos)); //set initial point

                filename = line.substr(_pos + 1, line.length());
                readFile();

                Newton(point1, maxIter, pow(2, -23), 0.00001);
                return true;
            }
            else if (line.substr(0, _pos) == "-sec") { //run Secant method
                line.erase(0, _pos + 1);
                _pos = line.find(" ");
                point1 = stoi(line.substr(0, _pos)); //set first initial point
                line.erase(0, _pos + 1);
                _pos = line.find(" ");
                point2 = stoi(line.substr(0, _pos)); //set second initial point

                filename = line.substr(_pos + 1, line.length());
                readFile();
                
                Secant(point1, point2, maxIter, pow(2, -23));
                return true;
            }
            else if (line.substr(0, _pos) == "-hybrid") { //run Hybrid method
                line.erase(0, _pos + 1);
                _pos = line.find(" ");
                point1 = stoi(line.substr(0, _pos)); //set first initial point
                line.erase(0, _pos + 1);
                _pos = line.find(" ");
                point2 = stoi(line.substr(0, _pos)); //set second initial point

                filename = line.substr(_pos + 1, line.length());
                readFile();
                
                Hybrid(point1, point2, point1, maxIter, pow(2, -23), 0.00001);
                return true;
            }
            else { //run Bisection method
                point1 = stoi(line.substr(0, _pos)); //set first initial point
                line.erase(0, _pos + 1);
                _pos = line.find(" ");
                point2 = stoi(line.substr(0, _pos)); //set second initial point

                filename = line.substr(_pos + 1, line.length());
                readFile();
                
                Bisection(point1, point2, maxIter, pow(2, -23));
                return true;
            }
        }
        cout << "Input Error. Try again.\n";
    }

}

int main()
{
    cout << "Input nothing to exit\n\n";
    while (0 == 0) {
        cout << "> polRoot ";
        if (!parseInput()) exit(0);
    }
    return 0;
}