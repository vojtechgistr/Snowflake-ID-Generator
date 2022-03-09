#include <iostream>
#include <string>
#include <chrono>
#include <time.h>
#include <vector>
#include <array>
#include <algorithm>
#include <map>

using namespace std;

class Snowflake {
    vector<uint64_t> lastSnowflakeID;
    uint64_t epoch = 1420070400000;
    
    public:
    uint64_t create() {
        uint64_t timestamp = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
        string timeSinceEpoch = toBinary(timestamp - epoch);
        string internalWorkerID = toBinary((timestamp & 0x3E0000) >> 17);
        string internalProcessID = toBinary((timestamp & 0x1F000) >> 12);
        string increment = toBinary(timestamp & 0xFFF);

        while(internalWorkerID.length() != 5) internalWorkerID = "0" + internalWorkerID;
        while(internalProcessID.length() != 5) internalProcessID = "0" + internalProcessID;
        while(increment.length() != 12) increment = "0" + increment;

        uint64_t finalID = toDecimal(timeSinceEpoch + internalWorkerID + internalProcessID + increment);
        
        while(find(lastSnowflakeID.begin(), lastSnowflakeID.end(), finalID) != lastSnowflakeID.end()) {
            finalID += 1;
        }
        
        if(lastSnowflakeID.size() == 999) lastSnowflakeID.clear();
        lastSnowflakeID.push_back(finalID);
        return finalID;
    };

    array<string, 2> createdAt(uint64_t id) {
        string binaryID = toBinary(id);
        uint64_t timestamp = (toDecimal(binaryID.substr(0, binaryID.length() - 22)) + epoch);
        time_t timestampToDate = timestamp / 1000;
        return {
            to_string(timestamp),
            ctime(&timestampToDate)
            };
    };
    
    map<string, string> fetch(uint64_t id) {
        map<string, string> userData;

        /**
        HERE YOU CAN CHECK IF THE SPECIFIED ID IS IN THE DATABASE
        AND INSERT THE USER'S DATA INTO "userData" (or custom) VARIABLE
        **/
        
        userData.insert(pair<string, string>("name", "value"));
        userData.insert(pair<string, string>("example", "value"));
        return userData;
    };
    
    map<string, string> get(uint64_t id) {
        return fetch(id);
    };

    private:
    uint64_t toDecimal(string n) {
        uint64_t result = strtoull(n.c_str(), 0, 2);
        return result;
    };

    string toBinary(uint64_t n) {
        string result;
        while (n != 0){
            result = ((n % 2 == 0) ? "0" : "1") + result;
            n /= 2;
        }
        return result;
    };
};

int main() {
    Snowflake id;
    cout << "Here you can see, that every generated ID is unique: \n";
    cout << id.create() << endl;
    cout << id.create() << endl;
    cout << id.create() << endl;
    cout << id.create() << endl;
    cout << id.create() << endl << endl << "Conversion: \n";
    
    uint64_t generatedSnowflake = id.create();
    cout << "Generated Snowflake ID: " << generatedSnowflake << endl;
    cout << "This snowflake was created at [TIMESTAMP]: " << id.createdAt(generatedSnowflake)[0] << endl;
    cout << "This snowflake was created at [DATE]: " << id.createdAt(generatedSnowflake)[1] << endl;
    cout << "Snowflake ID name: " << id.fetch(generatedSnowflake)["name"];
}
