#include "user_cfg.h"

/**
 * This module initialize user configuration, dynamically creates JSON document
 * and store data in JSON document.
 * User can fetch any data from JSON via uGetUserParameter() function.
 */

#include "./system/definitions.h"
#include "./libs_3rd_party/ArduinoJson-v6.14.1/ArduinoJson-v6.14.1.h"
#include "./libs_3rd_party/micro-sdcard/mySD.h"
#include "./fs/json_memory.h"
#include "./fs/sys_logs_data.h"

DynamicJsonDocument userConfigDoc(0);

/**
 * @brief Load user configuration from SD card
 * @return  0 - OK
 *          1 - Failed to open file
 *          2 - Not enough memory
 */
int InitUserParameters( void)
{
    /* Open file */
    File UserCfgFile = SD.open(USER_CFG_PATH, FILE_READ);
    if (!UserCfgFile)
    {
        systemLog(tERROR, "Failed to load user configuration");
        return 1;
    }

    /* Measure the size of file data */
    int MemSizeForJson = CalculateDynamicMemorySizeForJson( UserCfgFile);
    if (ESP.getMaxAllocHeap() < MemSizeForJson)
    {
        systemLog(tERROR, "User config file too big. Not enough RAM.");
        return 2;
    }

    /* Allocate memory, and deserialize JSON file */
    DynamicJsonDocument doc(MemSizeForJson);
    UserCfgFile.seek(0);

    deserializeJson(doc, UserCfgFile);

    /* Make it visible to global userConfigDoc */
    userConfigDoc = std::move(doc);

    /* Close file and exit */
    UserCfgFile.close();

    /* TEST CODE */
    #if 0
        Serial.print("Size:");
        Serial.println(MemSizeForJson);

        char ValStr[50];
        int ValInt;
        bool ValBool;
        if (uGetUserParameter( "mqtt_server", ValStr))    Serial.println(ValStr);
        else                                              Serial.println("Nema");
        if (uGetUserParameter( "pivo", ValStr))           Serial.println(ValStr);
        else                                              Serial.println("Nema");
        if (uGetUserParameter( "mqtt_port", &ValInt))     Serial.println(ValInt);
        else                                              Serial.println("Nema");
        if (uGetUserParameter( "piva", &ValInt))          Serial.println(ValInt);
        else                                              Serial.println("Nema");
        if (uGetUserParameter( "rof", &ValBool))          Serial.println(ValBool);
        else                                              Serial.println("Nema");
        if (uGetUserParameter( "pive", &ValBool))         Serial.println(ValBool);
        else                                              Serial.println("Nema");
    #endif

    return 0;
}

/**
 * @brief Get user parameters, output type string
 * 
 */
bool uGetUserParameter( const char *Element, char *Output)
{
    if (userConfigDoc.containsKey(Element))
    {
        strcpy(Output, userConfigDoc[Element]);
        return true;
    }
    return false;
}
/**
 * @brief Gets user parameters, output type int
 * 
 */
bool uGetUserParameter( const char *Element, int *Output)
{
    if (userConfigDoc.containsKey(Element))
    {
        *Output = userConfigDoc[Element];
        return true;
    }
    return false;
}
/**
 * @brief Gets user parameter, output type bool
 * 
 */
bool uGetUserParameter( const char *Element, bool *Output)
{
    if (userConfigDoc.containsKey(Element))
    {
        *Output = userConfigDoc[Element];
        return true;
    }
    return false;
}
