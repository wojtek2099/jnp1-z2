#include <iostream>
#include <unordered_map>

#include "maptel.h"

using namespace std;

unordered_map<unsigned long, unordered_map<const char*, const char*> > dicts;

// todo: jak indeksować słowniki?
unsigned long maptel_create(void) {
    unsigned long id = 0;
    dicts.insert(make_pair(id, unordered_map<const char*, const char*>()));
    return id;
}

void maptel_delete(unsigned long id) {
    dicts.erase(id);
}

void maptel_insert(unsigned long id, char const *tel_src, char const *tel_dst) {
    dicts.at(id).insert(make_pair(tel_src, tel_dst));
}

void maptel_erase(unsigned long id, char const *tel_src) {
    dicts.at(id).erase(tel_src);
}

// todo: jak rozumieć specyfikacje tej funkcji?
void maptel_transform(unsigned long id, char const *tel_src, char *tel_dst, size_t len) {
    if (dicts.at(id).count(tel_src) == 0) {
        memcpy(tel_dst, tel_src, TEL_NUM_MAX_LEN);
    }

    // TODO
}

int main(void) {
    cout << maptel_create() << endl;
    return 0;
}