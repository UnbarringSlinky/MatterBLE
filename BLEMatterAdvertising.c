#include <furi_hal_bt.h>

//Created by @UnbarringSlinky

//Struct for advertising data: Matter Payload
typedef struct {
    uint8_t length_flags;
    uint8_t type_flags;
    uint8_t flags;
    uint8_t length_service_data;
    uint8_t type_service_data;
    uint8_t matter_uuid[2];
    uint8_t opcode;
    uint8_t discriminator[2];
    uint8_t vendor_id[2];
    uint8_t product_id[2];
    uint8_t additional_data_flags;
} MatterAdvData;

// Function to create the Matter BLE advertisement payload
void static create_matter_ble_adv_payload(MatterAdvData* adv_data) {
    //Ensure data integrity
    if (adv_data == NULL) {
       return;
    }
    memset(adv_data,0);

    // Set up flags
    adv_data.length_flags = 0x02; // Length of Flags
    adv_data.type_flags = 0x01;   // Type of Flags
    adv_data.flags = 0x06;        // Flags (General Discoverability, BR/EDR Not Supported)

    // Set up service data
    adv_data.length_service_data = 0x0B; // Length of Service Data
    adv_data.type_service_data = 0x16;   // Type of Service Data (Service Data - 16-bit UUID)
    adv_data.matter_uuid[0] = 0xF6;      // Matter UUID
    adv_data.matter_uuid[1] = 0xFF;

    adv_data.opcode = 0x00; // Matter BLE OpCode (Commissionable)

    // 12-bit discriminator (0x3AB), packed into 2 bytes with Advertisement version
    adv_data.discriminator[0] = 0xAB;
    adv_data.discriminator[1] = 0x03;

    // Vendor ID
    adv_data.vendor_id[0] = 0xF1;
    adv_data.vendor_id[1] = 0xFF;

    // Product ID
    adv_data.product_id[0] = 0x00;
    adv_data.product_id[1] = 0x80;

    // Additional Data Flags
    adv_data.additional_data_flags = 0x01; // GATT-based Additional Data present
}

int32_t static matterEntry(void* p) {
    //Mark arg as unused
    UNUSED(p);

    //Create Advertising Data
    MatterAdvData *adv_data = malloc(sizeof(MatterAdvData));
    create_matter_ble_adv_payload(&adv_data);

    //Setup payload in Furi_hal_bt
    furi_hal_bt_reinit();
    if (!furi_hal_bt_custom_adv_set(adv_payload,sizeof(MatterAdvData))) {

        return 0; //Unsuccessful Adv Set
    }

    //Successful Advertising Set
    uint8_t macAddr[] = { 0x6c, 0x7a, 0xd8, 0xac, 0x57, 0x72 };
    //Custom Advertisement Start
    furi_hal_bt_custom_adv_start(500,10000,0x01,macAddr,0x1F);

    //TODO Exit conditions
    free(adv_data);
    //furi_hal_bt_custom_adv_stop()
    return 1;
}
