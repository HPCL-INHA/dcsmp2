#include "dc_mqtt.hpp"
#include "dc_mqtt_msg.hpp"
#include "user.hpp"

#include <csignal>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <jsoncpp/json/json.h>
#include <mosquitto.h>

using namespace std;

#ifdef TEST
void generateTestDcMqttActionMessage();
void generateTestDcMqttSensorMessage();

void requestTestDcMqttEamMessage(int sig);

const char TEST_ACTION_TOPIC[] = "buddybot/action_test/";
const char TEST_SENSOR_TOPIC[] = "buddybot/sensordata_test/";
const char TEST_EAM_TOPIC[] = "buddybot/eam_test/";
const char TEST_USER_ID[] = "userId";
const char TEST_DEVICE_ID[] = "deviceId";
const char TEST_SENSOR_ID[] = "sensorId";
const auto TEST_NUM_ACTION_MSG_PER_ROUND = 10;
const auto TEST_DELAY_LOOP_IN_USEC = 1000000;
const auto TEST_MAX_ACCEL_DATA = 50;
const auto TEST_LIM_XYZ_MAG = 1.65;
const auto TEST_LIM_STEP_GROWTH = 5;

Json::Value testActionMsgTemplate;
Json::Value testSensorMsgTemplate;
Json::Value testEamMsgTemplate;

deque<string> testSubtopics;

bool testcalibrating = false;
#endif

void mosq_connect_callback(struct mosquitto *mosq, void *obj, int result);
void mosq_message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg);

void mosq_terminate(int sig);

struct mosquitto *mosq;

const auto DELAY_START_IN_SEC = 1;
const auto DELAY_LOOP_IN_USEC = 100000;
const auto DELAY_TERM_IN_SEC = 1;

map<string, USER> userIdToUser;
bool running = true;

void testFunction(int sig);
string lastUserId;
string lastDeviceId;
bool calibrating = false;

int main(int argc, char *argv[]){
    if(argc != 5){
        cout << "Usage: " << argv[0] << " DOMAIN PORT ID PSWD" << endl;
        return EXIT_SUCCESS;
    }

#ifdef TEST
    cout << "< SELF TEST MODE >" << endl;
#endif

    const string SERVER_DOMAIN = argv[1];
    const string SERVER_PORT = argv[2];
    const string SERVER_ID = argv[3];
    const string SERVER_PSWD = argv[4];
    cout << "SERVER_DOMAIN: " << argv[1] << endl;
    cout << "SERVER_PORT: " << argv[2] << endl;
    cout << "SERVER_ID: " << argv[3] << endl;
    cout << "SERVER_PSWD: " << argv[4] << endl;
    cout << endl;

    // Get IP address of the domain
    struct hostent *hostent;
    hostent = gethostbyname(SERVER_DOMAIN.c_str());
    if (!hostent)
    {
        perror("gethostbyname()");
        return EXIT_FAILURE;
    }
    const string SERVER_IP = inet_ntoa(*(struct in_addr *)hostent->h_addr_list[0]);
    cout << "SERVER IP: " << SERVER_IP << endl;
    cout << endl;

    // Set signal handler for program termination
    signal(SIGINT, mosq_terminate);
    signal(SIGQUIT, mosq_terminate);
#ifdef TEST
    signal(SIGTSTP, requestTestDcMqttEamMessage);
#else
    signal(SIGTSTP, testFunction);
#endif

    // Initialize MQTT broker
    mosquitto_lib_init();
    mosq = mosquitto_new(argv[0], true, NULL);
    if (!mosq){
        perror("mosquitto_new()");
        return EXIT_FAILURE;
    }
    mosquitto_connect_callback_set(mosq, mosq_connect_callback);
    mosquitto_message_callback_set(mosq, mosq_message_callback);
    int ret = mosquitto_username_pw_set(mosq, SERVER_ID.c_str(), SERVER_PSWD.c_str());
    if (ret){
        perror("mosquitto_username_pw_set()");
        return EXIT_FAILURE;
    }
    ret = mosquitto_connect(mosq, SERVER_IP.c_str(), atoi(SERVER_PORT.c_str()), DC_MQTT_KEEPALIVE_VAL);
    if (ret){
        perror("mosquitto_connect()");
        return EXIT_FAILURE;
    }

    // Subscribe MQTT topics
#ifdef TEST
    ret = mosquitto_subscribe(mosq, NULL, getMqttRecursiveTopic(TEST_ACTION_TOPIC).c_str(), 0);
#else
    ret = mosquitto_subscribe(mosq, NULL, getMqttRecursiveTopic(DC_MQTT_ACTION_TOPIC).c_str(), 0);
#endif
    if(ret){
        perror("mosquitto_subscribe()");
        return EXIT_FAILURE;
    }
#ifdef TEST
    ret = mosquitto_subscribe(mosq, NULL, getMqttRecursiveTopic(TEST_SENSOR_TOPIC).c_str(), 0);
#else
    ret = mosquitto_subscribe(mosq, NULL, getMqttRecursiveTopic(DC_MQTT_SENSOR_TOPIC).c_str(), 0);
#endif
    if(ret){
        perror("mosquitto_subscribe()");
        return EXIT_FAILURE;
    }
#ifdef TEST
    ret = mosquitto_subscribe(mosq, NULL, getMqttRecursiveTopic(TEST_EAM_TOPIC).c_str(), 0);
#else
    ret = mosquitto_subscribe(mosq, NULL, getMqttRecursiveTopic(DC_MQTT_EAM_TOPIC).c_str(), 0);
#endif
    if(ret){
        perror("mosquitto_subscribe()");
        return EXIT_FAILURE;
    }

#ifdef TEST
    // Set subtopic for the test
    testSubtopics.push_back(TEST_USER_ID);
    testSubtopics.push_back(TEST_DEVICE_ID);

    // Prepare test action message template
    // 구현 필요

    // Prepare test sensor message template
    testSensorMsgTemplate[DC_MQTT_SENSOR_MSG_KEYS.USER_ID] = TEST_USER_ID;
    testSensorMsgTemplate[DC_MQTT_SENSOR_MSG_KEYS.DEVICE_ID] = TEST_DEVICE_ID;
    testSensorMsgTemplate[DC_MQTT_SENSOR_MSG_KEYS.SENSOR_ID] = TEST_SENSOR_ID;

    // Prepare test EAM message template
    testEamMsgTemplate[DC_MQTT_EAM_MSG_KEYS.USER_ID] = TEST_USER_ID;
    testEamMsgTemplate[DC_MQTT_EAM_MSG_KEYS.DEVICE_ID] = TEST_DEVICE_ID;
#endif

    // Loop
    sleep(DELAY_START_IN_SEC);
    while(running){
#ifdef TEST
        generateTestDcMqttActionMessage();
        generateTestDcMqttSensorMessage();
        usleep(TEST_DELAY_LOOP_IN_USEC);
#endif
        mosquitto_loop(mosq, -1, 1);
        usleep(DELAY_LOOP_IN_USEC);
    }

    // Terminate program
    sleep(DELAY_TERM_IN_SEC);
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    return EXIT_SUCCESS;
}

void mosq_connect_callback(struct mosquitto *mosq, void *obj, int result){
    cout << "Connected." << endl;
    cout << endl;
}
void mosq_message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg){
    // Check the message's topic
    string msgTopic = msg->topic;
    stringstream msgRootSs;
    msgRootSs << (char *)msg->payload;
    cout << "Incoming message..." << endl;
    cout << "[msgTopic]: " << msgTopic << endl;
    cout << endl;

    // Get trivial information
    Json::Value msgRoot;
    msgRootSs >> msgRoot;
    string userId = msgRoot[DC_MQTT_SENSOR_MSG_KEYS.USER_ID].asString();
    string deviceId = msgRoot[DC_MQTT_SENSOR_MSG_KEYS.DEVICE_ID].asString();
    string timestamp = msgRoot[DC_MQTT_SENSOR_MSG_KEYS.DATETIME].asString();

    lastUserId = userId;
    lastDeviceId = deviceId;

    // Register user if new
    if(userIdToUser.find(userId) == userIdToUser.end())
        userIdToUser[userId] = USER(userId, deviceId);
    USER *userPtr = &userIdToUser[userId];

    // Process action message if exists
#ifdef TEST
    if(msgTopic.find(TEST_ACTION_TOPIC) != msgTopic.npos){
#else
    if(msgTopic.find(DC_MQTT_ACTION_TOPIC) != msgTopic.npos){
#endif
        // 구현 필요
    }

    // Process sensor message if exists
#ifdef TEST
    else if(msgTopic.find(TEST_SENSOR_TOPIC) != msgTopic.npos){
#else
    else if(msgTopic.find(DC_MQTT_SENSOR_TOPIC) != msgTopic.npos){
#endif
        // Get trivial sensor information
        string sensorId = msgRoot[DC_MQTT_SENSOR_MSG_KEYS.SENSOR_ID].asString();
        userPtr->attachSensor(deviceId, sensorId);

        // Update step counter
        size_t steps = strtoul(msgRoot[DC_MQTT_SENSOR_MSG_KEYS.STEP_CNT].asString().c_str(), NULL, 10);
        userPtr->setCurrentSteps(steps);
        
        // Progress calibration
        if(userIdToUser[userId].isCalibrating()){
            cout << "Calibration in progress... " << endl;
            cout << endl;
            Json::Value sensorMsgData = msgRoot[DC_MQTT_SENSOR_MSG_KEYS.DATA];
            size_t numSensorMsgData = sensorMsgData.size();
            double unitTimeInSec = 1.0 / numSensorMsgData;
            for (auto iter = sensorMsgData.begin(); iter != sensorMsgData.end(); iter++){
                string ax, ay, az;
                ax = (*iter)[DC_MQTT_SENSOR_MSG_KEYS.DATA_ELEM_KEYS.AX].asString();
                ay = (*iter)[DC_MQTT_SENSOR_MSG_KEYS.DATA_ELEM_KEYS.AY].asString();
                az = (*iter)[DC_MQTT_SENSOR_MSG_KEYS.DATA_ELEM_KEYS.AZ].asString();
                cout << "ax: " << ax << " | ay: " << ay << " | az: " << az << endl;
                XYZ axyz(atof(ax.c_str()), atof(ay.c_str()), atof(az.c_str()));

                // Calculate m/s^2 acceleration
                ACCEL accel(axyz, 9.8, 1.0);

                // Accumulate acceleration
                userPtr->progressCalibration(accel, unitTimeInSec);
            }
        }

        // 나머지 구현 필요
    }
   
    // Process EAM message if exists
#ifdef TEST
    if(msgTopic.find(TEST_EAM_TOPIC) != msgTopic.npos){
#else
    if(msgTopic.find(DC_MQTT_EAM_TOPIC) != msgTopic.npos){
#endif
        // Get order for calibration
        bool enableCalibration = atoi(msgRoot[DC_MQTT_EAM_MSG_KEYS.ORDER_ENABLE_CALC_STEP_LEN].asString().c_str());
        bool disableCalibration = atoi(msgRoot[DC_MQTT_EAM_MSG_KEYS.ORDER_DISABLE_CALC_STEP_LEN].asString().c_str());
        if(enableCalibration)
            userPtr->startCalibration(timestamp);
        if(disableCalibration){
            userPtr->endCalibration(timestamp);

            // Prepare trivial sensor information
            Json::Value eamMsgRoot;
            eamMsgRoot[DC_MQTT_EAM_MSG_KEYS.USER_ID] = userId;
            eamMsgRoot[DC_MQTT_EAM_MSG_KEYS.DEVICE_ID] = deviceId;
            DC_MQTT_DATETIME datetime;
            eamMsgRoot[DC_MQTT_EAM_MSG_KEYS.DATETIME] = datetime.getDcMqttTimestamp();
            eamMsgRoot[DC_MQTT_EAM_MSG_KEYS.ORDER_DISABLE_CALC_STEP_LEN] = "0";
            eamMsgRoot[DC_MQTT_EAM_MSG_KEYS.ORDER_ENABLE_CALC_STEP_LEN] = "0";

            // Prepare user calibration information
            bool strideValid, walkingSpeedValid;
            eamMsgRoot[DC_MQTT_MSG_KEY_STRIDE] = userPtr->getCalibratedStride(&strideValid);
            if(strideValid)
                eamMsgRoot[DC_MQTT_MSG_KEY_STRIDE_VALID] = "1";
            else
                eamMsgRoot[DC_MQTT_MSG_KEY_STRIDE_VALID] = "0";
            eamMsgRoot[DC_MQTT_MSG_KEY_WALKING_SPEED] = userPtr->getCalibratedWalkingSpeed(&walkingSpeedValid);
            if(walkingSpeedValid)
                eamMsgRoot[DC_MQTT_MSG_KEY_WALKING_SPEED_VALID] = "1";
            else
                eamMsgRoot[DC_MQTT_MSG_KEY_WALKING_SPEED_VALID] = "0";
            
            // 나머지 구현 필요

            // Send EAM message
            stringstream eamSs;
            eamSs << eamMsgRoot;
            deque<string> subtopics;
            subtopics.push_back(lastUserId);
            subtopics.push_back(lastDeviceId);
            mosquitto_publish(mosq, NULL, getMqttSubtopic(DC_MQTT_EAM_TOPIC, subtopics).c_str(), eamSs.str().size(), eamSs.str().c_str(), DC_MQTT_QOS_LEVEL, false);

            userPtr->printCalibrationDebugMessage();
        }
    }
}

void mosq_terminate(int sig){
    cout << "Terminate program..." << endl;
    cout << endl;
    running = false;
}

#ifdef TEST
void generateTestDcMqttActionMessage(){
    // 구현 필요
}
void generateTestDcMqttSensorMessage(){
    // Generate trivial sensor information
    Json::Value testSensorMsgRoot = testSensorMsgTemplate;
    DC_MQTT_DATETIME datetime;
    testSensorMsgRoot[DC_MQTT_SENSOR_MSG_KEYS.DATETIME] = datetime.getDcMqttTimestamp();

    // Generate sensor acceleration data
    Json::Value testSensorMsgData(Json::arrayValue);
    for(size_t i = 0; i <= TEST_MAX_ACCEL_DATA; i++){
        Json::Value entry;
        entry[DC_MQTT_SENSOR_MSG_KEYS.DATA_ELEM_KEYS.INDEX] = to_string(i);
        entry[DC_MQTT_SENSOR_MSG_KEYS.DATA_ELEM_KEYS.AX] = to_string((double)rand() * TEST_LIM_XYZ_MAG / RAND_MAX);
        entry[DC_MQTT_SENSOR_MSG_KEYS.DATA_ELEM_KEYS.AY] = to_string((double)rand() * TEST_LIM_XYZ_MAG / RAND_MAX);
        entry[DC_MQTT_SENSOR_MSG_KEYS.DATA_ELEM_KEYS.AZ] = to_string((double)rand() * TEST_LIM_XYZ_MAG / RAND_MAX);
        testSensorMsgData.append(entry);
    }
    testSensorMsgRoot[DC_MQTT_SENSOR_MSG_KEYS.DATA] = testSensorMsgData;
    testSensorMsgRoot[DC_MQTT_SENSOR_MSG_KEYS.STATE] = "1";

    // Generate sensor step counter
    static auto test_step_cnt = 0;
    test_step_cnt += rand() % (TEST_LIM_STEP_GROWTH + 1);
    testSensorMsgRoot[DC_MQTT_SENSOR_MSG_KEYS.STEP_CNT] = to_string(test_step_cnt);

    // 나머지 구현 필요

    // Send generated sensor message
    stringstream testSensorSs;
    testSensorSs << testSensorMsgRoot;
    mosquitto_publish(mosq, NULL, getMqttSubtopic(TEST_SENSOR_TOPIC, testSubtopics).c_str(), testSensorSs.str().size(), testSensorSs.str().c_str(), DC_MQTT_QOS_LEVEL, false);
}

void requestTestDcMqttEamMessage(int sig){
    // Generate trivial sensor information
    Json::Value testEamMsgRoot = testEamMsgTemplate;
    DC_MQTT_DATETIME datetime;
    testEamMsgRoot[DC_MQTT_EAM_MSG_KEYS.DATETIME] = datetime.getDcMqttTimestamp();
    
    // Generate calibration order
    if(testcalibrating){
        cout << "Request to end calibration..." << endl;
        testEamMsgRoot[DC_MQTT_EAM_MSG_KEYS.ORDER_DISABLE_CALC_STEP_LEN] = "1";
        testEamMsgRoot[DC_MQTT_EAM_MSG_KEYS.ORDER_ENABLE_CALC_STEP_LEN] = "0";
        testcalibrating = false;
    }
    else{
        cout << "Request to start calibration..." << endl;
        testEamMsgRoot[DC_MQTT_EAM_MSG_KEYS.ORDER_DISABLE_CALC_STEP_LEN] = "0";
        testEamMsgRoot[DC_MQTT_EAM_MSG_KEYS.ORDER_ENABLE_CALC_STEP_LEN] = "1";
        testcalibrating = true;
    }
    cout << endl;

    // 나머지 구현 필요

    // Send generated EAM message
    stringstream testEamSs;
    testEamSs << testEamMsgRoot;
    mosquitto_publish(mosq, NULL, getMqttSubtopic(TEST_EAM_TOPIC, testSubtopics).c_str(), testEamSs.str().size(), testEamSs.str().c_str(), DC_MQTT_QOS_LEVEL, false);
}
#endif

void testFunction(int sig){
    // Generate trivial sensor information
    Json::Value testEamMsgRoot;
    testEamMsgRoot[DC_MQTT_EAM_MSG_KEYS.USER_ID] = lastUserId;
    testEamMsgRoot[DC_MQTT_EAM_MSG_KEYS.DEVICE_ID] = lastDeviceId;
    DC_MQTT_DATETIME datetime;
    testEamMsgRoot[DC_MQTT_EAM_MSG_KEYS.DATETIME] = datetime.getDcMqttTimestamp();
    
    // Generate calibration order
    if(calibrating){
        cout << "Request to end calibration..." << endl;
        testEamMsgRoot[DC_MQTT_EAM_MSG_KEYS.ORDER_DISABLE_CALC_STEP_LEN] = "1";
        testEamMsgRoot[DC_MQTT_EAM_MSG_KEYS.ORDER_ENABLE_CALC_STEP_LEN] = "0";
        calibrating = false;
    }
    else{
        cout << "Request to start calibration..." << endl;
        testEamMsgRoot[DC_MQTT_EAM_MSG_KEYS.ORDER_DISABLE_CALC_STEP_LEN] = "0";
        testEamMsgRoot[DC_MQTT_EAM_MSG_KEYS.ORDER_ENABLE_CALC_STEP_LEN] = "1";
        calibrating = true;
    }
    cout << endl;

    // 나머지 구현 필요

    // Send generated EAM message
    stringstream testEamSs;
    testEamSs << testEamMsgRoot;
    deque<string> subtopics;
    subtopics.push_back(lastUserId);
    subtopics.push_back(lastDeviceId);
    mosquitto_publish(mosq, NULL, getMqttSubtopic(DC_MQTT_EAM_TOPIC, subtopics).c_str(), testEamSs.str().size(), testEamSs.str().c_str(), DC_MQTT_QOS_LEVEL, false);
}