#include <iostream>
#include <chrono>
#include <cstring>

using namespace std;

const int MOD1 = 1000000007, MOD2 = 1000000009;
const int strLen = 7;
int numOfWords = 10000;
const int c1 = 37, c2 = 41;
pair<int, int> powers[strLen];
pair<int, int> p = {29, 31};
string words[10000];

int my_rand(int mod) {
    long long a = rand();
    long long b = rand();

    return int(((a % mod * MOD1) % mod + b) % mod);
}

string random_word() {
    string newWord;
    for (int j = 0; j < strLen; j++) {
        char ch = 'a' + my_rand(26);
        newWord += ch;
    }
    return newWord;
}



void pow_calc(int mod1, int mod2, int Len) {
    powers[0] = {1, 1};
    for (int i = 1; i < Len; i++) {
        powers[i].first = (powers[i - 1].first * p.first) % mod1;
        powers[i].second = (powers[i - 1].second * p.second) % mod2;
    }
}

int auxHash(string const &word, int mod) {
    int hashVal = 0;
    int len = word.size();

    for (int i = 1; i <= len; i++) {
        hashVal = (hashVal + powers[len - i].second * (word[i - 1] - 'a' + 1) % mod) % mod;
    }
    if (hashVal == 2) hashVal++;
    return (hashVal < 2 ? ((mod - 1) & 1? mod -1 : mod - 2) : (hashVal & 1? hashVal : hashVal - 1));
}

int hash1(string const &word, int mod) {
    int hashVal = 0;
    int len = word.size();

    for (int i = 1; i <= len; i++) {
        hashVal = (hashVal + powers[len - i].first * (word[i - 1] - 'a' + 1) % mod) % mod;
    }

    return hashVal;
}

int hash2(string const &word, int mod) {
    int hashVal = 0;
    int len = word.size();
    for (int i = 0; i < len; i++) {
        hashVal = (hashVal + powers[i].second * (word[i] - 'a' + 1) % mod) % mod;
    }

    return hashVal;
}

class data {
public:
    string key;
    int value;
    data * next, *previous;

    data() {
        value = 0;
        next = previous = nullptr;
    }
    data(string &key, int value) {
        this->value = value;
        this->key = key;
        next = previous = nullptr;
    }
};

class Data {
public:
    string key;
    int value;
    Data() {value = 0; }
    Data(string &key, int value):key(key), value(value){}
};

class ChainHashMap {
public:
    data ** hashMap;
    bool hashOne;
    int Size;
    int collisionCount;
    int totalProbes;

    ChainHashMap(){hashMap = nullptr; hashOne = true; Size = 0; }
    ChainHashMap(int size, bool hashOne);
    bool Insert(string &key, int value);
    bool Search(string &key);
    void Delete(string &key);
    void Insert(int n);
    void Print();
    void SearchNwords(int n);
};

void ChainHashMap::SearchNwords(int n) {
    totalProbes = 0;
    for (int i = 0; i < n; i++) {
        string word = words[my_rand(Size)];///no deletion done
        if(!Search(word))
            cout << "something is wrong!" << endl;
    }
    cout << "Average Number of Probes: " << ((totalProbes * 1.0) / n) << endl;
}

void ChainHashMap::Insert(int n) {
    int i = 1;
    collisionCount = 0;
    while (i <= n) {
        string word = random_word();

        if (Insert(word, i)) {
            words[i - 1] = word;
            i++;
        }
    }
}

void ChainHashMap::Delete(string &key) {
    int hashVal;

    if (hashOne) hashVal = hash1(key, Size);
    else hashVal = hash2(key, Size);

    if (Search(key)) {
        data * temp = hashMap[hashVal];
        if (temp->key == key) {
            hashMap[hashVal] = hashMap[hashVal]->next;
            hashMap[hashVal]->previous = nullptr;
        }
        else {
            data *prev = temp;
            temp = temp->next;
            while (temp->key != key) {
                prev = temp;
                temp = temp->next;
            }

            prev->next = temp->next;
            if (temp->next)
                temp->next->previous = prev;
            delete temp;
        }
    }
}

bool ChainHashMap::Search(string &key) {
    int hashVal;

    if (hashOne) hashVal = hash1(key, Size);
    else hashVal = hash2(key, Size);

    if (key.size() == strLen and hashMap[hashVal] != nullptr) {
        data *temp = hashMap[hashVal];
        do {
            totalProbes++;
            if (temp->key == key) {
                return true;
            }
            temp = temp->next;
        } while (temp != nullptr);
    }

    return false;
}

bool ChainHashMap::Insert(string &key, int value) {
    int hashVal;

    if (hashOne) hashVal = hash1(key, Size);
    else hashVal = hash2(key, Size);

    if (!Search(key)) {///Insert only new value
        if (hashMap[hashVal] == nullptr) {
            hashMap[hashVal] = new data(key, value);
            return true;
        }
        data * temp = hashMap[hashVal];
        data * prev = nullptr;
        while (temp != nullptr) {
            prev = temp;
            temp = temp->next;
            collisionCount++;
        }
        temp = new data(key, value);
        temp->next = nullptr;
        temp->previous = prev;
        if (prev)
            prev->next = temp;

        return true;

    }

    return false;
}

ChainHashMap::ChainHashMap(int size, bool hashOne) {
    this->hashOne = hashOne;
    this->Size = size;
    hashMap = new data*[size];
    for (int i = 0; i < size; i++)
        hashMap[i] = nullptr;
}

void ChainHashMap::Print() {
    int uniqueHashValues = 0;
    for (int i = 0; i < Size; i++) {
        data * temp = hashMap[i];
        int hashVal;

        int cnt = 0;
        while (temp != nullptr) {
            if (hashOne) hashVal = hash1(temp->key, Size);
            else hashVal = hash2(temp->key, Size);
            ///cout << temp->key << temp->value << " " << hashVal << "->";
            temp = temp->next;
            cnt++;
        }

        if (cnt >= 1)
            uniqueHashValues++;

        ///cout << endl;
    }
    cout << "Chain Hash Unique Hash Values for " << (hashOne ? "hash1": "hash2:" ) <<" " <<  uniqueHashValues << endl;
}

class DoubleHashing {
public:
    bool customProbing, hashOne;
    Data * hashMap;
    int Size;
    int collisionCount;
    int totalProbes;
    DoubleHashing(int Size, bool hashOne);
    DoubleHashing(int Size, bool hashOne, bool customProbing);
    int doubleHash(int, int, int);
    int customHash(int, int, int);
    bool Search(string &key);
    bool Insert(string &key, int value);
    void Delete(string &key);
    void Insert(int n);
    void Print();
    void SearchNwords(int n);

};

void DoubleHashing::SearchNwords(int n) {
    totalProbes = 0;
    for (int i = 0; i < n; i++) {
        string word = words[my_rand(Size)];
        if (!Search(word)) {
            cout << "Something is wrong!" << endl;
        }
    }

    cout << "Average Number of Probes: " << ((totalProbes * 1.0) / n) << endl;
}

void DoubleHashing::Print() {
    int maxi = 0;
    int hashVal, aux_hash, curHash;

    int unique[Size], uniqueHashValues = 0;
    for (int i = 0; i < Size; i++) unique[i] = 0;


    for (int i = 0; i < Size; i++) {

        if (hashOne) hashVal = hash1(hashMap[i].key, Size);
        else hashVal = hash2(hashMap[i].key, Size);

        aux_hash = auxHash(hashMap[i].key, Size);
        if (customProbing) curHash = customHash(hashVal, aux_hash, 0);
        else curHash = doubleHash(hashVal, aux_hash, 0);

        unique[curHash]++;

        ///cout << hashMap[i].key  << " " << hashMap[i].value << " " << curHash<< endl;

        maxi = max(maxi, hashMap[i].value);
    }

    for (int i = 0; i < Size; i++) {
        if (unique[i] >= 1)
            uniqueHashValues++;
    }

    cout << "Double Hashing Unique hash values: " << uniqueHashValues << endl;

    ///cout << maxi << endl;
}

void DoubleHashing::Delete(string &key) {
    if (!Search(key)) return;

    int hashVal, aux_hash;
    if (hashOne)
        hashVal = hash1(key, Size);
    else hashVal = hash2(key, Size);

    aux_hash = auxHash(key, Size);

    for (int i = 0; i < Size; i++) {
        int curHash;

        if (customProbing) curHash = customHash(hashVal, aux_hash, i);
        else curHash = doubleHash(hashVal, aux_hash, i);

        if (hashMap[curHash].key == key) {
            hashMap[curHash].value = -1;
            hashMap[curHash].key = "";
            return;
        }
        else if (hashMap[curHash].value == 0) {
            return;
        }
    }


}

void DoubleHashing::Insert(int n) {

    int i = 1, j = 1;
    collisionCount = 0;
    while (i <= n) {
        string word = random_word();

        if (Insert(word, i)) {
            words[i - 1] = word;
            i++;
        }

        j++;
    }
}

bool DoubleHashing::Insert(string &key, int value) {
    int curCol = 0;

    if (Search(key)) return false;

    int hashVal, aux_hash;
    if (hashOne)
        hashVal = hash1(key, Size);
    else hashVal = hash2(key, Size);

    aux_hash = auxHash(key, Size);

    for (int i = 0; i < Size; i++) {
        int curHash;
        if (customProbing) curHash = customHash(hashVal, aux_hash, i);
        else curHash = doubleHash(hashVal, aux_hash, i);

        if (hashMap[curHash].value == 0 or hashMap[curHash].value == -1) {
            hashMap[curHash].value = value;
            hashMap[curHash].key = key;
            collisionCount += curCol;
            return true;
        }
        curCol++;
    }

    return false;
}

bool DoubleHashing::Search(string &key) {
    int hashVal, aux_hash;
    if (hashOne)
        hashVal = hash1(key, Size);
    else hashVal = hash2(key, Size);

    aux_hash = auxHash(key, Size);

    for (int i = 0; i < Size; i++) {
        int curHash;
        if (customProbing) curHash = customHash(hashVal, aux_hash, i);
        else curHash = doubleHash(hashVal, aux_hash, i);
        totalProbes++;

        if (hashMap[curHash].value == 0) return false;
        if (hashMap[curHash].key == key) return true;

    }

    return false;
}

int DoubleHashing::customHash(int hashK, int auxHashK, int i) {
    return (hashK + i * auxHashK % Size * c1 % Size + c2 * i % Size * i % Size) % Size;
}

int DoubleHashing::doubleHash(int hashK, int auxHashK, int i) {
    return (hashK + i * auxHashK % Size) % Size;
}

DoubleHashing::DoubleHashing(int Size, bool hashOne) {
    this->Size = Size;
    this->hashOne = hashOne;
    customProbing = false;
    hashMap = new Data[Size];
}

DoubleHashing::DoubleHashing(int Size, bool hashOne, bool customProbing) {
    this->Size = Size;
    this->hashOne = hashOne;
    this->customProbing = customProbing;
    hashMap = new Data[Size];
}

int main() {

    ///freopen("out.txt", "w", stdout);

    srand(chrono::steady_clock::now().time_since_epoch().count());

    cout << "Enter count of words: ";

    cin >> numOfWords;

    pow_calc(numOfWords, numOfWords, strLen);

    cout << "=======Chain Hashing for hash1==================" << endl;

    ChainHashMap chainHashMap(numOfWords, 1);
    chainHashMap.Insert(numOfWords);


    chainHashMap.Print();
    chainHashMap.SearchNwords(1000);
    cout << "Collisions: " << chainHashMap.collisionCount << endl<<endl;

    cout << "=======Chain Hashing for hash2===================" << endl;

    ChainHashMap chainHashMap2(numOfWords, false);
    chainHashMap2.Insert(numOfWords);


    chainHashMap2.Print();
    chainHashMap2.SearchNwords(1000);
    cout << "Collisions: " << chainHashMap2.collisionCount << endl<<endl;



    ///cout << endl << endl;
    cout << "=======Double Hashing hash1==================" << endl;

    DoubleHashing doubleHashing(numOfWords, 1, 0);
    doubleHashing.Insert(numOfWords);
    doubleHashing.Print();
    doubleHashing.SearchNwords(1000);
    cout << "Collisions: " <<  doubleHashing.collisionCount << endl<<endl;

    cout << "=======Double Hashing hash2==================" << endl;

    DoubleHashing doubleHashing2(numOfWords, 0, 0);
    doubleHashing2.Insert(numOfWords);
    doubleHashing2.Print();
    doubleHashing2.SearchNwords(1000);
    cout << "Collisions: " <<  doubleHashing2.collisionCount << endl<<endl;


    cout << "=======Custom Probing hash1==================" << endl;

    DoubleHashing doubleHashing3(numOfWords, 1, 1);
    doubleHashing3.Insert(numOfWords);
    doubleHashing3.Print();
    doubleHashing3.SearchNwords(1000);
    cout << "Collisions: " <<  doubleHashing3.collisionCount << endl<<endl;

    cout << "=======Custom Probing hash2==================" << endl;

    DoubleHashing doubleHashing4(numOfWords, 0, 1);
    doubleHashing4.Insert(numOfWords);
    doubleHashing4.Print();
    doubleHashing4.SearchNwords(1000);
    cout << "Collisions: " <<  doubleHashing4.collisionCount << endl;




    return 0;
}