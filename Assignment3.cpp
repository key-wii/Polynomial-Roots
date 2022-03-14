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
    cout << "\n\n";
}

/*function Bisection(f : float -> float, a : float, b : float, maxIter : int, eps : float)
  fa := call f(a)
  fb := call f(b)

  if fa * fb >= 0 then
    print "Inadequate values for a and b."
    return -1.0
  end if

  error := b - a

  for it <- 1 to maxIter
    error := error / 2
    c := a + error
    fc := call f(c)

    if |error| < eps or fc = 0 then
      print "Algorithm has converged after #{it} iterations!"
      return c
    end if

    if fa * fc < 0 then
      b := c
      fb := fc
    else
      a := c
      fa := fc
    end if

  end for


  print "Max iterations reached without convergence..."
  return c
end function*/

int main()
{
    /*cout << "> polRoot ";
    cin >> filename;*/
    filename = "fun1.pol";

    readFile();
    printPolonomial();

    return 0;
}