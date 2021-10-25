#include <iostream>
#include <unordered_map>
#include <assert.h>
#include <string>
#include <string.h>

#include <set>
#include "maptel.h"

using namespace std;

// static unordered_map<unsigned long, unordered_map<string, string> > dicts;
static unsigned long max_id = 0;

unordered_map<unsigned long, unordered_map<string, string> >& dicts() {
    static unordered_map<unsigned long, unordered_map<string, string> > dicts;
    return dicts;
}

unsigned long jnp1::maptel_create(void) {
    dicts().insert(make_pair(max_id, unordered_map<string, string>()));
    return max_id++;
}

void jnp1::maptel_delete(unsigned long id) {
    assert(dicts().count(id)!=0);
    dicts().erase(id);
}

void jnp1::maptel_insert(unsigned long id, char const *tel_src, char const *tel_dst) {
    
    dicts().at(id).insert(make_pair(string(tel_src), string(tel_dst)));
}

void jnp1::maptel_erase(unsigned long id, char const *tel_src) {
    dicts().at(id).erase(string(tel_src));
}

// todo: jak rozumieć specyfikacje tej funkcji?
// czy lepiej robic na stringach czy wskaznikach na char?
void jnp1::maptel_transform(unsigned long id, char const *tel_src, char *tel_dst, size_t len) {
    try {
        unordered_map<string, string>& dict = dicts().at(id);

        if (dict.count(string(tel_src)) == 0) {
            memcpy(tel_dst, tel_src, min(len, jnp1::TEL_NUM_MAX_LEN+1));
            return;
        }

        string tmp_src(tel_src);
        string tmp_dst(dict.at(tmp_src));

        // todo: zmienić na szukanie pętli za pomocy wskaźnikow
        set<string> visited;

        while (visited.count(tmp_dst) == 0 && dict.count(tmp_dst) != 0) {
            visited.insert(tmp_src);
            tmp_src = tmp_dst;
            tmp_dst = dict.at(tmp_src);
        }

        if (visited.count(tmp_dst) != 0) {
            strncpy(tel_dst, tel_src, min(len, jnp1::TEL_NUM_MAX_LEN+1));
        } else {
            strncpy(tel_dst, tmp_dst.c_str(), min(len, jnp1::TEL_NUM_MAX_LEN+1));
        }
    } catch (out_of_range& oor) {
        cerr << "OOR error: " << oor.what() << endl;
        // cout << "key " << id << " not found??" << endl;
    }
}
