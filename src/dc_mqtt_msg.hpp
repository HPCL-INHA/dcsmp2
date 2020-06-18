#ifndef DC_MQTT_MSG_HPP
#define DC_MQTT_MSG_HPP

static const char DC_MQTT_MSG_KEY_ACC_ACTION_LST[] = "accident_action_list";
static const char DC_MQTT_MSG_KEY_ACTION_DATETIME[] = "timestamp";
static const char DC_MQTT_MSG_KEY_ACTION_DEG[] = "inclination";
static const char DC_MQTT_MSG_KEY_ACTION_DETECT_ANKLE_L[] = "l_ankle";
static const char DC_MQTT_MSG_KEY_ACTION_DETECT_ANKLE_R[] = "r_ankle";
static const char DC_MQTT_MSG_KEY_ACTION_DETECT_EAR_L[] = "l_hear";
static const char DC_MQTT_MSG_KEY_ACTION_DETECT_EAR_R[] = "r_hear";
static const char DC_MQTT_MSG_KEY_ACTION_DETECT_ELBOW_L[] = "l_elbow";
static const char DC_MQTT_MSG_KEY_ACTION_DETECT_ELBOW_R[] = "r_elbow";
static const char DC_MQTT_MSG_KEY_ACTION_DETECT_EYE_L[] = "l_eye";
static const char DC_MQTT_MSG_KEY_ACTION_DETECT_EYE_R[] = "r_eye";
static const char DC_MQTT_MSG_KEY_ACTION_DETECT_HIP_L[] = "l_hip";
static const char DC_MQTT_MSG_KEY_ACTION_DETECT_HIP_R[] = "r_hip";
static const char DC_MQTT_MSG_KEY_ACTION_DETECT_KNEE_L[] = "l_knee";
static const char DC_MQTT_MSG_KEY_ACTION_DETECT_KNEE_R[] = "r_knee";
static const char DC_MQTT_MSG_KEY_ACTION_DETECT_NOSE[] = "nose";
static const char DC_MQTT_MSG_KEY_ACTION_DETECT_SHLDR_L[] = "l_shoulder";
static const char DC_MQTT_MSG_KEY_ACTION_DETECT_SHLDR_R[] = "r_shoulder";
static const char DC_MQTT_MSG_KEY_ACTION_DETECT_WRIST_L[] = "l_wrist";
static const char DC_MQTT_MSG_KEY_ACTION_DETECT_WRIST_R[] = "r_wrist";
static const char DC_MQTT_MSG_KEY_ACTION_DEVICE_ID[] = "deviceid";
static const char DC_MQTT_MSG_KEY_ACTION_ODD[] = "odd_flag";
static const char DC_MQTT_MSG_KEY_ACTION_SIG[] = "action";
static const char DC_MQTT_MSG_KEY_ACTION_USER_ID[] = "userid";
static const char DC_MQTT_MSG_KEY_ACTION_X_LEN[] = "x_length";
static const char DC_MQTT_MSG_KEY_ACTION_Y_LEN[] = "y_length";
static const char DC_MQTT_MSG_KEY_AX[] = "ax";
static const char DC_MQTT_MSG_KEY_AY[] = "ay";
static const char DC_MQTT_MSG_KEY_AZ[] = "az";
static const char DC_MQTT_MSG_KEY_DATA[] = "data";
static const char DC_MQTT_MSG_KEY_DATETIME[] = "datetime";
static const char DC_MQTT_MSG_KEY_DEVICE_ID[] = "deviceId";
static const char DC_MQTT_MSG_KEY_FALL_AMBIGUOUS[] = "FallingSuspicous";
static const char DC_MQTT_MSG_KEY_FALL_TRUE[] = "Falling";
static const char DC_MQTT_MSG_KEY_HEART_RATE[] = "ppg";
static const char DC_MQTT_MSG_KEY_HEART_RATE_HIGH[] = "HighHeartRate";
static const char DC_MQTT_MSG_KEY_HEART_RATE_LOW[] = "LowHeartRate";
static const char DC_MQTT_MSG_KEY_HEART_RATE_STOP[] = "CardiacArrest";
static const char DC_MQTT_MSG_KEY_INDEX[] = "index";
static const char DC_MQTT_MSG_KEY_ORDER_DISABLE_CALC_STEP_LEN[] = "order_disable_calculate_step_length";
static const char DC_MQTT_MSG_KEY_ORDER_ENABLE_CALC_STEP_LEN[] = "order_enable_calculate_step_length";
static const char DC_MQTT_MSG_KEY_SENSOR_ID[] = "sensorId";
static const char DC_MQTT_MSG_KEY_SENSOR_NO_CHANGE[] = "sensor_no_change";
static const char DC_MQTT_MSG_KEY_STATE[] = "state";
static const char DC_MQTT_MSG_KEY_STEP_CNT[] = "pedoCount";
static const char DC_MQTT_MSG_KEY_STEP_LEN[] = "stepLength";
static const char DC_MQTT_MSG_KEY_STRIDE[] = "calibrated_stride";
static const char DC_MQTT_MSG_KEY_STRIDE_VALID[] = "valid_stride";
static const char DC_MQTT_MSG_KEY_USER_ID[] = "userId";
static const char DC_MQTT_MSG_KEY_WALKING_SPEED[] = "calibrated_walking_speed";
static const char DC_MQTT_MSG_KEY_WALKING_SPEED_VALID[] = "valid_walking_speed";

enum DC_MQTT_ACTION_SIG_ENUM{
    ACTION_SITTING_FLOOR_TO_LYING = 0,
    ACTION_SITTING_FLOOR_TO_STANDING,
    ACTION_SITTING_FLOOR,
    ACTION_LYING_TO_SITTING_FLOOR,
    ACTION_LYING,
    ACTION_SITTING_CHAIR_TO_STANDING,
    ACTION_SITTING_CHAIR,
    ACTION_STANDING_TO_SITTING_FLOOR,
    ACTION_STANDING_TO_SITTING_CHAIR,
    ACTION_STANDING,
    ACTION_WALKING,
    ACTION_NO_SIG,
    ACTION_NO_SIG_READY,
    ACTION_NUM_SIGS
};
const struct{
    const char *ACTION_SIG = DC_MQTT_MSG_KEY_ACTION_SIG;
    const char *DATETIME = DC_MQTT_MSG_KEY_ACTION_DATETIME;
    const char *DEG = DC_MQTT_MSG_KEY_ACTION_DEG;
    const char *DETECT_ANKLE_L = DC_MQTT_MSG_KEY_ACTION_DETECT_ANKLE_L;
    const char *DETECT_ANKLE_R = DC_MQTT_MSG_KEY_ACTION_DETECT_ANKLE_R;
    const char *DETECT_EAR_L = DC_MQTT_MSG_KEY_ACTION_DETECT_EAR_L;
    const char *DETECT_EAR_R = DC_MQTT_MSG_KEY_ACTION_DETECT_EAR_R;
    const char *DETECT_ELBOW_L = DC_MQTT_MSG_KEY_ACTION_DETECT_ELBOW_L;
    const char *DETECT_ELBOW_R = DC_MQTT_MSG_KEY_ACTION_DETECT_ELBOW_R;
    const char *DETECT_EYE_L = DC_MQTT_MSG_KEY_ACTION_DETECT_EYE_L;
    const char *DETECT_EYE_R = DC_MQTT_MSG_KEY_ACTION_DETECT_EYE_R;
    const char *DETECT_HIP_L = DC_MQTT_MSG_KEY_ACTION_DETECT_HIP_L;
    const char *DETECT_HIP_R = DC_MQTT_MSG_KEY_ACTION_DETECT_HIP_R;
    const char *DETECT_KNEE_L = DC_MQTT_MSG_KEY_ACTION_DETECT_KNEE_L;
    const char *DETECT_KNEE_R = DC_MQTT_MSG_KEY_ACTION_DETECT_KNEE_R;
    const char *DETECT_NOSE = DC_MQTT_MSG_KEY_ACTION_DETECT_NOSE;
    const char *DETECT_SHLDR_L = DC_MQTT_MSG_KEY_ACTION_DETECT_SHLDR_L;
    const char *DETECT_SHLDR_R = DC_MQTT_MSG_KEY_ACTION_DETECT_SHLDR_R;
    const char *DETECT_WRIST_L = DC_MQTT_MSG_KEY_ACTION_DETECT_WRIST_L;
    const char *DETECT_WRIST_R = DC_MQTT_MSG_KEY_ACTION_DETECT_WRIST_R;
    const char *DEVICE_ID = DC_MQTT_MSG_KEY_ACTION_DEVICE_ID;
    const char *ODD = DC_MQTT_MSG_KEY_ACTION_ODD;
    const char *USER_ID = DC_MQTT_MSG_KEY_ACTION_USER_ID;
    const char *X_LEN = DC_MQTT_MSG_KEY_ACTION_X_LEN;
    const char *Y_LEN = DC_MQTT_MSG_KEY_ACTION_Y_LEN;
}DC_MQTT_ACTION_MSG_KEYS;

const struct{
    const char *DATETIME = DC_MQTT_MSG_KEY_DATETIME;
    const char *DEVICE_ID = DC_MQTT_MSG_KEY_DEVICE_ID;
    const char *HEART_RATE = DC_MQTT_MSG_KEY_HEART_RATE;
    const char *STEP_CNT = DC_MQTT_MSG_KEY_STEP_CNT;
    const char *SENSOR_ID = DC_MQTT_MSG_KEY_SENSOR_ID;
    const char *STATE = DC_MQTT_MSG_KEY_STATE;
    const char *USER_ID = DC_MQTT_MSG_KEY_USER_ID;
    const char *DATA = DC_MQTT_MSG_KEY_DATA;
    struct{
        const char *INDEX = DC_MQTT_MSG_KEY_INDEX;
        const char *AX = DC_MQTT_MSG_KEY_AX;
        const char *AY = DC_MQTT_MSG_KEY_AY;
        const char *AZ = DC_MQTT_MSG_KEY_AZ;
    }DATA_ELEM_KEYS;
}DC_MQTT_SENSOR_MSG_KEYS;

const struct{
    const char *ACC_ACTION_LIST = DC_MQTT_MSG_KEY_ACC_ACTION_LST;
    const char *DATETIME = DC_MQTT_MSG_KEY_DATETIME;
    const char *DEVICE_ID = DC_MQTT_MSG_KEY_DEVICE_ID;
    const char *FALL_AMBIGUOUS = DC_MQTT_MSG_KEY_FALL_AMBIGUOUS;
    const char *FALL_TRUE = DC_MQTT_MSG_KEY_FALL_TRUE;
    const char *HEART_RATE_HIGH = DC_MQTT_MSG_KEY_HEART_RATE_HIGH;
    const char *HEART_RATE_LOW = DC_MQTT_MSG_KEY_HEART_RATE_LOW;
    const char *HEART_RATE_STOP = DC_MQTT_MSG_KEY_HEART_RATE_STOP;
    const char *ORDER_DISABLE_CALC_STEP_LEN = DC_MQTT_MSG_KEY_ORDER_DISABLE_CALC_STEP_LEN;
    const char *ORDER_ENABLE_CALC_STEP_LEN = DC_MQTT_MSG_KEY_ORDER_ENABLE_CALC_STEP_LEN;
    const char *SENSOR_NO_CHANGE = DC_MQTT_MSG_KEY_SENSOR_NO_CHANGE;
    const char *STATE = DC_MQTT_MSG_KEY_STATE;
    const char *STEP_LENGTH = DC_MQTT_MSG_KEY_STEP_LEN;
    const char *STRIDE = DC_MQTT_MSG_KEY_STRIDE;
    const char *STRIDE_VALID = DC_MQTT_MSG_KEY_STRIDE_VALID;
    const char *USER_ID = DC_MQTT_MSG_KEY_USER_ID;
    const char *WALKING_SPEED = DC_MQTT_MSG_KEY_WALKING_SPEED;
    const char *WALKING_SPEED_VALID = DC_MQTT_MSG_KEY_WALKING_SPEED_VALID;
}DC_MQTT_EAM_MSG_KEYS;

#endif