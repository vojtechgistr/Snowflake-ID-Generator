#include <iostream>
#include <string>
#include <chrono>
#include <time.h>
#include <vector>
#include <array>
#include <algorithm>
#include <map>

class Snowflake {
    std::vector<uint64_t> lastSnowflakeID;
    uint64_t epoch = 1420070400000;
    
    public:
    uint64_t create() {
        uint64_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        std::string timeSinceEpoch = toBinary(timestamp - epoch);
        std::string internalWorkerID = toBinary((timestamp & 0x3E0000) >> 17);
        std::string internalProcessID = toBinary((timestamp & 0x1F000) >> 12);
        std::string increment = toBinary(timestamp & 0xFFF);

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

    std::array<std::string, 2> createdAt(uint64_t id) {
        std::string binaryID = toBinary(id);
        uint64_t timestamp = (toDecimal(binaryID.substr(0, binaryID.length() - 22)) + epoch);
        time_t timestampToDate = timestamp / 1000;
        return {
            std::to_string(timestamp),
            std::ctime(&timestampToDate)
            };
    };
    
    std::map<std::string, std::string> fetch(uint64_t id) {
        std::map<std::string, std::string> userData;

        /**
        HERE YOU CAN CHECK IF THE SPECIFIED ID IS IN THE DATABASE
        AND INSERT THE USER'S DATA INTO "userData" (or custom) VARIABLE
        **/
        
        userData.insert(std::pair<std::string, std::string>("name", "value"));
        userData.insert(std::pair<std::string, std::string>("example", "value"));
        return userData;
    };
    
    std::map<std::string, std::string> get(uint64_t id) {
        return fetch(id);
    };

    private:
    uint64_t toDecimal(std::string n) {
        uint64_t result = strtoull(n.c_str(), 0, 2);
        return result;
    };

    std::string toBinary(uint64_t n) {
        std::string result;
        while (n != 0){
            result = ((n % 2 == 0) ? "0" : "1") + result;
            n /= 2;
        }
        return result;
    };
};

int main() {
    Snowflake id;
    std::cout << "Here you can see, that every generated ID is unique: \n";
    std::cout << id.create() << "\n";
    std::cout << id.create() << "\n";
    std::cout << id.create() << "\n";
    std::cout << id.create() << "\n";
    std::cout << id.create() << "\n\nConversion: \n";
    
    uint64_t generatedSnowflake = id.create();
    std::cout << "Generated Snowflake ID: " << generatedSnowflake;
    std::cout << "\nThis snowflake was created at [TIMESTAMP]: " << id.createdAt(generatedSnowflake)[0];
    std::cout << "\nThis snowflake was created at [DATE]: " << id.createdAt(generatedSnowflake)[1];
    std::cout << "\nSnowflake ID name: " << id.fetch(generatedSnowflake)["name"];
}
