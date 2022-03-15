#include <iostream>
#include <string>
#include <fstream>
#include <vector>
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
    return x*x*x - x*x + polynomial[n];
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
            string outcome = "Algorithm has converged after " + to_string(it) + " iterations!";
            cout << outcome << "\n";
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

    string outcome = "Max iterations reached without convergence...";
    cout << outcome << "\n";
    writeSol(c, maxIter, outcome);
    return;
}

/*void Newton(/*derF : float -> float, *//*float x, int maxIter, float eps, float delta) {
    float fx = f(x);

    for (int it = 1; it <= maxIter; it++) {
        //float fd = derF(x);
        float fd = 1;

        if (abs(fd) < delta) {
            cout << "Small slope!";
            return x;
        }

        float d = fx / fd;
        x = x - d;
        fx = f(x);

        if (abs(d) < eps) {
            cout << "Algorithm has converged after #{it} iterations!"
            return x;
        }

    }
    cout << "Max iterations reached without convergence...";
    return x;
}*/

int main()
{
    /*cout << "> polRoot ";
    cin >> filename;*/
    filename = "fun1.pol";

    readFile();
    printPolonomial();
    Bisection(-10, 10, 1000, pow(2, -23));

    return 0;
}