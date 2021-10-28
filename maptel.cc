// Autorzy: Paweł Olejnik (po417770), Michał Maszkowski (mm421428)

#include <unordered_map>
#include <string>
#include <cstring>
#include <assert.h>
#include "maptel.h"

using namespace std;

namespace {

#ifdef NDEBUG
    const bool debug = false;
#else 
    const bool debug = true;
#endif

    unordered_map<unsigned long, unordered_map<string, string>> &dicts() {
        static unordered_map<unsigned long, unordered_map<string, string>> dicts;
        return dicts;
    }

    [[maybe_unused]]
    bool is_correct_number(char const *candidate) {
        bool result = true;
        size_t current_char_num = 0;
        char current_char = candidate[current_char_num];
        while (result && (current_char_num < jnp1::TEL_NUM_MAX_LEN) && (current_char != 0)) {
            if (current_char < '0' ||  current_char > '9') {
                result = false;
            }
            current_char_num++;
            current_char = candidate[current_char_num];
        }
        if (current_char != 0 || current_char_num == 0) {
            result = false;
        }
        return result;
    }
}

namespace jnp1 {

unsigned long maptel_create(void) {
    static unsigned long max_id = 0;

    if (debug) {
        cerr << "maptel: maptel_create()" << endl;
    }
    ::dicts().insert(make_pair(max_id, unordered_map<string, string>()));

    // checking whether operation succeeded:
    assert(::dicts().find(max_id) != ::dicts().end());

    if (debug) {
        cerr << "maptel: maptel_create: new map id = " << max_id << endl;
    }
    return max_id++;
}

void maptel_delete(unsigned long id) {
    if (debug) {
        cerr << "maptel: maptel_delete(" << id << ")" << endl;
    }
    assert(::dicts().find(id) != ::dicts().end());
    // presume that function's parameters are correct

    ::dicts().erase(id);

    // checking whether operation succeeded:
    assert(::dicts().find(id) == ::dicts().end());
    if (debug) {
        cerr << "maptel: maptel_delete: map " << id << " deleted" << endl;
    }
}

void maptel_insert(unsigned long id, char const *tel_src, char const *tel_dst) {
    if (debug) {
        cerr << "maptel: maptel_insert(" << id << ", " << tel_src << ", " << tel_dst <<  ")" << endl;
    }
    assert(::dicts().find(id) != ::dicts().end());
    assert(tel_src != NULL && tel_dst != NULL);
    // if NDEBUG is not defined then it is already known that pointers != NULL
    assert(::is_correct_number(tel_src) && ::is_correct_number(tel_dst));
    // presume that function's parameters are correct

    dicts().at(id).insert_or_assign(string(tel_src), string(tel_dst));

    // checking whether operation succeeded:
    assert(::dicts().at(id).find(string(tel_src)) != ::dicts().at(id).end());
    assert(::dicts().at(id).at(string(tel_src)) == string(tel_dst));

    if (debug) {
        cerr << "maptel: maptel_insert: inserted" << endl;
    }
}

void maptel_erase(unsigned long id, char const *tel_src) {
    if (debug) {
        cerr << "maptel: maptel_erase(" << id << ", " << tel_src << ")" << endl;
    }
    assert(::dicts().find(id) != ::dicts().end());
    assert(tel_src != NULL && ::is_correct_number(tel_src));
    // presume that function's parameters are correct

    size_t erase_res = dicts().at(id).erase(string(tel_src));

    // checking whether operation succeeded:
    assert(::dicts().at(id).find(string(tel_src)) == ::dicts().at(id).end());
    if (debug) {
        if (erase_res == 0) {
            cerr << "maptel: maptel_erase: nothing to erase" << endl;
        } else {
            cerr << "maptel: maptel_erase: erased" << endl;
        }
    }   
}

void maptel_transform(unsigned long id, char const *tel_src, char *tel_dst, size_t len) {
    if (debug) {
        cerr << "maptel: maptel_transform(" << id << ", " << tel_src << ", ADDR, " << len << ")" << endl;
    }
    assert(::dicts().find(id) != ::dicts().end());
    assert(tel_src != NULL && tel_dst != NULL);
    // if NDEBUG is not defined then it is already known that pointers != NULL
    assert(::is_correct_number(tel_src));
    // presume that function's three first parameters are correct

    unordered_map <string, string> &dict = dicts().at(id);
    string tmp_src = string(tel_src);
    bool found = false;
    string result;

    if (dict.find(tmp_src) == dict.end()) {
        found = true;
        result = tmp_src;
    }

    string a = tmp_src;
    string b = tmp_src;

    while (!found) {
        b = dict.at(b);
        if (dict.find(b) == dict.end()) {
            found = true;
            result = b;
        } else { // b is not last in the chain of changes
            a = dict.at(a);
            b = dict.at(b);

            if (dict.find(b) == dict.end()) {
                found = true;
                result = b;
            }

            if (a == b) {
                found = true;
                result = tmp_src;
                if (debug) {
                    cerr << "maptel: maptel_transform: cycle detected" << endl;
                }
            }
        }        
    }
    
    assert(result.length() < len);
    //enough space to:
    strncpy(tel_dst, result.c_str(), len);
    if (debug) {
        cerr << "maptel: maptel_transform: " << tmp_src << " -> " << result << endl;
    }
}

}
