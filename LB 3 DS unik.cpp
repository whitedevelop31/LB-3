#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <Windows.h> 

using namespace std;

// ЕТАП 5: Клас Hash  
class Hash {
public:
    static string toSHA1(string message) {
        // Логіка хешування 
        unsigned long hash = 5381;
        for (char c : message) {
            hash = ((hash << 5) + hash) + c;
        }
        stringstream ss;
        ss << hex << setw(16) << setfill('0') << hash;
        return ss.str();
    }
};

// ЕТАП 4: Клас Operation / Transaction 
class Transaction {
public:
    string sender;
    string receiver;
    int amount;
    string timestamp;

    Transaction(string s, string r, int a) : sender(s), receiver(r), amount(a) {
        time_t now = time(0);
        char buf[26]; // Буфер для часу
        ctime_s(buf, sizeof(buf), &now);
        timestamp = string(buf);
    }

    string toString() const {
        return sender + receiver + to_string(amount) + timestamp;
    }
};

// ЕТАП 5: Клас Block 
class Block {
public:
    string blockID;
    string prevHash;
    vector<Transaction> setOfTransactions;

    Block(vector<Transaction> txs, string lastHash) : setOfTransactions(txs), prevHash(lastHash) {
        blockID = calculateHash();
    }

    string calculateHash() {
        string dataToHash = prevHash;
        for (const auto& tx : setOfTransactions) {
            dataToHash += tx.toString();
        }
        return Hash::toSHA1(dataToHash);
    }
};

// ЕТАП 5: Клас Blockchain 
class Blockchain {
public:
    vector<Block> blockHistory;

    Blockchain() {
        initBlockchain();
    }

    void initBlockchain() {
        // Створення генезис-блоку 
        vector<Transaction> emptyTxs;
        Block genesis(emptyTxs, "0");
        blockHistory.push_back(genesis);
        cout << "Blockchain ініціалізовано. Генезис-блок створено." << endl;
    }

    bool validateAndAddBlock(Block newBlock) {
        // Перевірка зв'язку з попереднім блоком 
        if (newBlock.prevHash == blockHistory.back().blockID) {
            blockHistory.push_back(newBlock);
            return true;
        }
        return false;
    }
};

int main() {

    
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    setlocale(LC_ALL, "Ukrainian");

    Blockchain myChain;

    // Створюємо транзакцію
    vector<Transaction> txs;
    txs.push_back(Transaction("Alice", "Bob", 100));

    // Створюємо блок
    Block newBlock(txs, myChain.blockHistory.back().blockID);

    // Додаємо блок
    if (myChain.validateAndAddBlock(newBlock)) {
        cout << "Блок успішно додано!" << endl;
        cout << "ID блоку: " << newBlock.blockID << endl;
        cout << "Попередній хеш: " << newBlock.prevHash << endl;
    }
    else {
        cout << "Помилка валідації!" << endl;
    }

    return 0;
}