#include <iostream>
#include <string>
#include <chrono>
#include <time.h>
#include <array>

using namespace std;

class Snowflake {
    public:
    // uint64_t *lastSnowflakeID;
    uint64_t epoch = 1420070400000;
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
        // while(finalID == *lastSnowflakeID) {
        //     finalID += 1;
        // }

        // *lastSnowflakeID = finalID;
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
    }

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
    Snowflake snowflake;
    uint64_t generatedSnowflake = snowflake.create();

    cout << "Generated Snowflake ID: " << generatedSnowflake << endl;
    cout << "This snowflake was created at [TIMESTAMP]: " << snowflake.createdAt(generatedSnowflake)[0] << endl;
    cout << "This snowflake was created at [DATE]: " << snowflake.createdAt(generatedSnowflake)[1] << endl;
}
