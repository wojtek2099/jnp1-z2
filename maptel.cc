#include <iostream>
#include <unordered_map>
#include <assert.h>

using namespace std;

const size_t TEL_NUM_MAX_LEN = 22;

unordered_map<unsigned long, unordered_map<const char*, const char*> > dicts;

namespace jnp1 {

extern "C" {
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
        try {
            unordered_map<const char*, const char*> dict;
            

            dict = dicts.at(id);

            
            if (dict.count(tel_src) == 0) {
                memcpy(tel_dst, tel_src, TEL_NUM_MAX_LEN + 1);
                return;
            }

            char const *tmp_src = (char*) tel_src;
            char const *tmp_dst = dict.at(tmp_src);

            // dopoki nie znajdziemy pętli
            while (tmp_dst != tel_src && dict.count(tmp_dst) != 0) {
                tmp_src = tmp_dst;
                tmp_dst = dict.at(tmp_src);
   

            }

            memcpy(tel_dst, tmp_dst, TEL_NUM_MAX_LEN + 1); // todo: zamiast tego len
        } catch (out_of_range& oor) {
            cerr << "OOR error: " << oor.what() << endl;
        }
    }
}

}
