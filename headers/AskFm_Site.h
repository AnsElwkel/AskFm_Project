#ifndef ASKFM_SITE_H
#define ASKFM_SITE_H
#include "Headers.h"
#include "userInterface.h"
using namespace std;

class Site {
public:
    userInterface *_userInterface;
    Site(); // create objects

    void run();

    ~Site(); // delete objects
};


#endif //ASKFM_SITE_H
