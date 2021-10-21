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
        return;
    }

    char *tmp_src = (char*) tel_src;
    char *tmp_dst = tel_dst;

    // dopoki nie znajdziemy pętli
    while (tmp_dst != tel_src) {
        tmp_src = tmp_dst;
        tmp_dst = (char*) dicts.at(id).at((const char*) tmp_dst);
    }

    if (tmp_dst == tel_src) {
        memcpy(tel_dst, tel_src, TEL_NUM_MAX_LEN);
        return;
    } 

    memcpy(tel_dst, tmp_dst, TEL_NUM_MAX_LEN);
}

int main(void) {
    cout << maptel_create() << endl;
    return 0;
}