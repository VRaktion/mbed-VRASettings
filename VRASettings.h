#ifndef VRA_SETTINGS_H
#define VRA_SETTINGS_H

#include "BLEService.h"
#include "VRAStorage.h"
#include "ble/BLE.h"
#include "ble/Gap.h"
#include "mbed.h"

#define APP_ID_LENGTH 16
#define MAX_SETTINGS 20

class VRASettings : public BLEService
{
public:
    enum Characteristics
    {
        SETTINGS,
        APP_ID,
        LOG,
        COUNT
    };

    VRASettings(UUID *p_uuid, EventQueue *p_eq, StateChain *p_stateChain, VRAStorage *p_storage);

    void initCharacteristics();

    void init();
    void pastBleInit();

    void setAppIdBle(void);
    Callback<void(void)> setAppIdWriteCb;
    void printAppId(void);

    void set(int key, uint8_t value);
    void get(int key, uint8_t *value);

    void log(char* msg);

private:
    VRAStorage *storage;
    EventQueue *eq;

    uint8_t appId[APP_ID_LENGTH] = {0};

    uint8_t settings[MAX_SETTINGS] = {0};
    int saveSettingsEventId = NULL;
};

#endif