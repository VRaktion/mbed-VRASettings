#include "VRASettings.h"

VRASettings::VRASettings(UUID *p_uuid, EventQueue *p_eq, StateChain *p_stateChain,
                         VRAStorage *p_storage)
    : BLEService("settings", p_uuid, p_eq, p_stateChain), eq(p_eq), storage(p_storage) {}

void VRASettings::initCharacteristics()
{

    printf("settings uuid f03de978-d217-478c-a106-8e1b65165100\r\n");

    // // TEMPERATURE, HUMIDITY, PRESSURE, BRIGHTNESS, COUNT
    // this->initCharacteristic(SETTINGS, 0xFF00,
    //                          GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ,
    //                          8); // 8x char

    // this->initCharacteristic(APP_ID, 0xFF01,
    //                          GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ |
    //                              GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE,
    //                          APP_ID_LENGTH);
    // this->setAppIdWriteCb = callback(this, &VRASettings::setAppIdBle);
    // this->setWriteCallback(APP_ID, &this->setAppIdWriteCb);

    // this->initCharacteristic(
    //     LOG, 0xFF02,
    //     GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ |
    //         GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY,
    //     20);

    // this->initService();
}

void VRASettings::init()
{
    // init queue
    this->initCharacteristics();
}

void VRASettings::setAppIdBle(void)
{
    printf("received new app id\n\r");
    this->readFromGatt(APP_ID, this->appId, APP_ID_LENGTH);
    this->printAppId();
    this->storage->set("appId", this->appId, APP_ID_LENGTH);
}

void VRASettings::printAppId(void)
{
    printf("APP ID: ");
    for (int i = 0; i < APP_ID_LENGTH; i++)
    {
        printf("%02X", this->appId[i]);
    }
    printf("\n\r");
}

void VRASettings::pastBleInit()
{
    printf("Settings past BLE init\r\n");

    this->storage->get("settings", this->settings, MAX_SETTINGS);
    this->writeToGatt(SETTINGS, this->settings, MAX_SETTINGS);

    this->storage->get("appId", this->appId, APP_ID_LENGTH);
    this->writeToGatt(APP_ID, this->appId, APP_ID_LENGTH);
    this->printAppId();
    this->log("LOG INIT");
}

void VRASettings::log(char *msg)
{
    uint16_t len;
    for (len = 0; msg[len] && len < 20; len++)
        ; //get size of string
    this->writeToGatt((uint8_t)LOG, (uint8_t *)msg, len + 1);
}

void VRASettings::set(int key, uint8_t value)
{
    this->settings[(int)key] = value;
    if (this->eq->time_left(this->saveSettingsEventId) > -1)//this->saveSettingsEventId != NULL)
    {
        this->eq->cancel(this->saveSettingsEventId);
        // this->saveSettingsEventId = NULL;
    }
    this->saveSettingsEventId = this->eq->call_in(1000, callback(this->storage, &VRAStorage::set),
                                               "settings",
                                               this->settings,
                                               MAX_SETTINGS);
}

void VRASettings::get(int key, uint8_t *value)
{
    // this->get("settings", this->settings, (size_t)SettingsKeys::COUNT);
    *value = this->settings[(int)key];
}