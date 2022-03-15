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

void Newton(float x, int maxIter, float eps, float delta) {
    float fx = f(x);

    for (int it = 1; it <= maxIter; it++) {
        float fd = derF(x);
        fd = 1;

        if (abs(fd) < delta) {
            string outcome = "Small slope!";
            cout << outcome << "\n";
            writeSol(x, it, outcome);
            return;
        }

        float d = fx / fd;
        x = x - d;
        fx = f(x);

        if (abs(d) < eps) {
            string outcome = "Algorithm has converged after " + to_string(it) + " iterations!";
            cout << outcome << "\n";
            writeSol(x, it, outcome);
            return;
        }

    }
    string outcome = "Max iterations reached without convergence...";
    cout << outcome << "\n";
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
            string outcome = "Algorithm has converged after " + to_string(it) + " iterations!";
            cout << outcome << "\n";
            writeSol(a, it, outcome);
            return;
        }

        a = a - d;
        fa = f(a);
    }

    string outcome =  "Maximum number of iterations reached!";
    cout << outcome << "\n";
    writeSol(a, maxIter, outcome);
    return;
}

int main()
{
    /*cout << "> polRoot ";
    cin >> filename;*/
    filename = "fun1.pol";

    readFile();
    printPolonomial();
    //Bisection(-10.0, 10.0, 10, pow(2, -23));
    //Newton(10.0, 10, pow(2, -23), 0.00001);
    Secant(-10.0, 10.0, 10, pow(2, -23));

    return 0;
}