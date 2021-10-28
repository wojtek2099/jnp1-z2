#include <iostream>
#include <unordered_map>
#include <assert.h>
#include <string>
#include <string.h>
#include <set>
#include "maptel.h"

using namespace std;

unordered_map<unsigned long, unordered_map<string, string> >& dicts() {
    static unordered_map<unsigned long, unordered_map<string, string> > dicts;
    return dicts;
}

namespace {
    bool has_loop(unsigned long id, char const *tel_src) {
        unordered_map<string, string>& dict = dicts().at(id);

        string a(tel_src);
        string b = a;
        if (dict.count(a) == 0) {
            return false;
        }

        while (true) {
            a = dict.at(a);
            if (dict.count(a) == 0) return false;

            b = dict.at(b);
            if (dict.count(b) == 0) return false;
            b = dict.at(b);
            if (dict.count(b) == 0) return false;

            if (a == b) return true;
        }   
    }
}

unsigned long jnp1::maptel_create(void) {
    static unsigned long max_id = 0;
    dicts().insert(make_pair(max_id, unordered_map<string, string>()));
    return max_id++;
}

void jnp1::maptel_delete(unsigned long id) {
    assert(dicts().count(id)!=0);
    dicts().erase(id);
}

void jnp1::maptel_insert(unsigned long id, char const *tel_src, char const *tel_dst) {
    dicts().at(id).insert_or_assign(string(tel_src), string(tel_dst));
}

void jnp1::maptel_erase(unsigned long id, char const *tel_src) {
    dicts().at(id).erase(string(tel_src));
}

void jnp1::maptel_transform(unsigned long id, char const *tel_src, char *tel_dst, size_t len) {
    try {
        unordered_map<string, string>& dict = dicts().at(id);

        if (dict.count(string(tel_src)) == 0) {
            memcpy(tel_dst, tel_src, min(len, jnp1::TEL_NUM_MAX_LEN+1));
            return;
        }

        string tmp_src(tel_src);
        string tmp_dst(dict.at(tmp_src));

        if (::has_loop(id, tel_src)) {
            strncpy(tel_dst, tel_src, len);
            return;
        }

        while (dict.count(tmp_dst) != 0) {
            tmp_src = tmp_dst;
            tmp_dst = dict.at(tmp_src);
        }

        strncpy(tel_dst, tmp_dst.c_str(), len);

    } catch (out_of_range& oor) {
        cerr << "OOR error: " << oor.what() << endl;
    }
}
