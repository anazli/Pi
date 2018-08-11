#include<iostream>
#include<fstream>
#include<iomanip>
#include<cstdlib>
#include "../include/vec3.h"

using namespace std;

Vec3 markovChain(const Vec3& v, const double& delta, int& acpt);

int main(int argc, char *argv[])
{

    ofstream out;
    out.open("visualization/data.dat");
    unsigned int N{0};
    if(argc > 1)
    {
        N = strtol(argv[1], nullptr, 10);
    }
    else
    {
        cout << "Please enter the number of iterations!" << endl;
        return EXIT_FAILURE;
    }
    out << N << endl;

    double delta{0.8};
    int acpt{0};

    Vec3 point = randomVector(); //No need for randomVectorOnUnitSphere, we want random vectors inside cube.
    unsigned int in_sphere = 0;
    for(int i = 0 ; i < N ; ++i)
    {
        if(point.magnitude() <= 1.)
        {
            in_sphere++;
  	    out << point.x() << " " << point.y() << " " << point.z() << endl;
        }
        point = markovChain(point, delta, acpt);
        
    }

    double volume_ratio = (double)in_sphere/(double)N;
    double pi = 8. * (3./4.) * volume_ratio;
    cout << "Estimated Pi value:" << setprecision(10) << pi << endl;
    cout << "Total steps:" << N << endl;
    cout << "Accepted steps:" << acpt << endl;

    out.close();

    return 0;
}


Vec3 markovChain(const Vec3& v, const double& delta, int& acpt)
{
    /* Inputs:
     * Vec3 v is the current point in the Markov chain.
     * double delta is the parameter which determines the displacement.
     * int acpt is the number of the accepted steps of the algorithm.
     * returns:
     * A Vec3 which may be the same input point v or a new random
     * point temp.  
     */
    double a = -delta;
    double b =  delta;
    Vec3 dv =  a + (b - a) * randomVector();
    Vec3 temp = v + dv;
    double u{1.};
    double l{-1.};
    if(temp.x() < l || temp.y() < l || temp.z() < l)
    {
        return v;
    }
    if(temp.x() > u || temp.y() > u || temp.z() > u)
    {
        return v;
    }
    acpt++;
    return temp;
}
