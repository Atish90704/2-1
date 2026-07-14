#include <bits/stdc++.h>
using namespace std;

const int INITIAL_SIZE = 13;
const double MAX_LOAD = 0.5;
const double MIN_LOAD = 0.25;
const int WORD_LEN = 10;
const int TOTAL_KEYS = 10000;
const int SEARCH_KEYS = 1000;
const int C1 = 1, C2 = 3;

enum SlotState { EMPTY, OCCUPIED, DELETED };

struct Entry {
    string key;
    int value;
    SlotState state;
};

////////////////////////////
bool isPrime(int n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (int i = 3; i * i <= n; i += 2)
        if (n % i == 0) return false;
    return true;
}

int nextPrime(int n) {
    while (!isPrime(n)) n++;
    return n;
}

int prevPrime(int n) {
    for (int i = n - 1; i >= INITIAL_SIZE; i--)
        if (isPrime(i)) return i;
    return INITIAL_SIZE;
}

/////////////////////
unsigned long long Hash1(const string &s) {
    unsigned long long h = 0;
    for (char c : s) h = h * 31 + c;
    return h;
}

unsigned long long Hash2(const string &s) {
    unsigned long long h = 5381;
    for (char c : s) h = ((h << 5) + h) + c;
    return h;
}
/////////////////////

int auxHash(const string &s, int N) {
    return 1 + (Hash2(s) % (N - 1));
}

//random string
string randomWord(int len) {
    static const string chars = "abcdefghijklmnopqrstuvwxyz";
    string s;
    for (int i = 0; i < len; i++)
        s += chars[rand()%26];
    return s;
}

//chaining
class HashChaining {
public:
    vector<vector<pair<string,int>>> table;
    int N, count, lastResizeCount;
    long long collisions;

    HashChaining() {
        N = INITIAL_SIZE;
        table.resize(N);
        count = collisions = 0;
        lastResizeCount = 0;
    }

    double loadFactor() { return (double)count / N; }

    void rehash(int newN) {
        auto old = table;
        table.assign(newN, {});
        N = newN;
        count = 0;
        for (auto &b : old)
            for (auto &p : b)
                insert(p.first, p.second, true);
    }

    void resizeIfNeeded() {
        if (loadFactor() > MAX_LOAD && count - lastResizeCount >= count / 2) {
            rehash(nextPrime(2 * N));
            lastResizeCount = count;
        }
        else if (loadFactor() < MIN_LOAD && N > INITIAL_SIZE &&
                 lastResizeCount - count >= lastResizeCount / 2) {
            rehash(prevPrime(N / 2));
            lastResizeCount = count;
        }
    }

    void insert(const string &key, int value, bool h1) {
        int idx = (h1 ? Hash1(key) : Hash2(key)) % N;
        if (!table[idx].empty()) collisions++;
        table[idx].push_back({key, value});
        count++;
        resizeIfNeeded();
    }

    void remove(const string &key, bool h1) {
        int idx = (h1 ? Hash1(key) : Hash2(key)) % N;
        for (auto it = table[idx].begin(); it != table[idx].end(); ++it) {
            if (it->first == key) {
                table[idx].erase(it);
                count--;
                resizeIfNeeded();
                return;
            }
        }
    }

    int searchHits(const string &key, bool h1) {
        int idx = (h1 ? Hash1(key) : Hash2(key)) % N;
        int hits = 0;
        for (auto &p : table[idx]) {
            hits++;
            if (p.first == key) return hits;
        }
        return hits;
    }
};

//open addressing
class HashOpen {
public:
    vector<Entry> table;
    int N, count, lastResizeCount;
    long long collisions;
    bool custom;

    HashOpen(bool isCustom) {
        N = INITIAL_SIZE;
        table.assign(N, {"", 0, EMPTY});
        count = collisions = 0;
        lastResizeCount = 0;
        custom = isCustom;
    }

    double loadFactor() { return (double)count / N; }

    void rehash(int newN) {
        auto old = table;
        table.assign(newN, {"", 0, EMPTY});
        N = newN;
        count = 0;
        for (auto &e : old)
            if (e.state == OCCUPIED)
                insert(e.key, e.value, true);
    }

    void resizeIfNeeded() {
        if (loadFactor() > MAX_LOAD && count - lastResizeCount >= count / 2) {
            rehash(nextPrime(2 * N));
            lastResizeCount = count;
        }
        else if (loadFactor() < MIN_LOAD && N > INITIAL_SIZE &&
                 lastResizeCount - count >= lastResizeCount / 2) {
            rehash(prevPrime(N / 2));
            lastResizeCount = count;
        }
    }

    void insert(const string &key, int value, bool h1) {
        unsigned long long h = h1 ? Hash1(key) : Hash2(key);
        int idx = h % N;
        int step = auxHash(key, N);

        for (int i = 0; i < N; i++) {
            int pos = custom ?
                (idx + C1 * i * step + C2 * i * i) % N :
                (idx + i * step) % N;

            if (table[pos].state != OCCUPIED) {
                table[pos] = {key, value, OCCUPIED};
                count++;
                resizeIfNeeded();
                return;
            }
            collisions++;
        }
    }

    void remove(const string &key, bool h1) {
        unsigned long long h = h1 ? Hash1(key) : Hash2(key);
        int idx = h % N;
        int step = auxHash(key, N);

        for (int i = 0; i < N; i++) {
            int pos = custom ?
                (idx + C1 * i * step + C2 * i * i) % N :
                (idx + i * step) % N;

            if (table[pos].state == EMPTY) return;
            if (table[pos].state == OCCUPIED && table[pos].key == key) {
                table[pos].state = DELETED;
                count--;
                resizeIfNeeded();
                return;
            }
        }
    }

    int searchHits(const string &key, bool h1) {
        unsigned long long h = h1 ? Hash1(key) : Hash2(key);
        int idx = h % N;
        int step = auxHash(key, N);

        for (int i = 0; i < N; i++) {
            int pos = custom ?
                (idx + C1 * i * step + C2 * i * i) % N :
                (idx + i * step) % N;

            if (table[pos].state == EMPTY) return i + 1;
            if (table[pos].state == OCCUPIED && table[pos].key == key)
                return i + 1;
        }
        return N;
    }
};




//main
int main() {
    srand(time(0));

    unordered_set<string> used;
    vector<string> keys;

    while ((int)keys.size() < TOTAL_KEYS) {
        string w = randomWord(WORD_LEN);
        if (used.insert(w).second)
            keys.push_back(w);
    }

    HashChaining ch1, ch2;
    HashOpen dh1(false), dh2(false);
    HashOpen cp1(true), cp2(true);

    for (int i = 0; i < TOTAL_KEYS; i++) {
        ch1.insert(keys[i], i + 1, true);
        ch2.insert(keys[i], i + 1, false);
        dh1.insert(keys[i], i + 1, true);
        dh2.insert(keys[i], i + 1, false);
        cp1.insert(keys[i], i + 1, true);
        cp2.insert(keys[i], i + 1, false);
    }

    shuffle(keys.begin(), keys.end(), mt19937(time(0)));

    auto avgHits = [&](auto &ht, bool h) {
        long long sum = 0;
        for (int i = 0; i < SEARCH_KEYS; i++)
            sum += ht.searchHits(keys[i], h);
        return (double)sum / SEARCH_KEYS;
    };

    cout << fixed << setprecision(2);
    cout << "Technique\tH1 Coll\tH1 Hits\tH2 Coll\tH2 Hits\n";

    cout << "Chaining\t" << ch1.collisions << "\t"
         << avgHits(ch1, true) << "\t"
         << ch2.collisions << "\t"
         << avgHits(ch2, false) << "\n";

    cout << "DoubleHash\t" << dh1.collisions << "\t"
         << avgHits(dh1, true) << "\t"
         << dh2.collisions << "\t"
         << avgHits(dh2, false) << "\n";

    cout << "CustomProbe\t" << cp1.collisions << "\t"
         << avgHits(cp1, true) << "\t"
         << cp2.collisions << "\t"
         << avgHits(cp2, false) << "\n";
}
