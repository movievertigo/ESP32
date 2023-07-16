// By Movie Vertigo
// https://youtube.com/movievertigo
// https://twitter.com/movievertigo
 
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLE2902.h>

class BLEFastUDP : public BLECharacteristicCallbacks, public BLEServerCallbacks
{
public:
  typedef void (*RXCallbackFunction)(const uint8_t*, int);

  void begin(const std::string& name, const std::string& info, const std::string& serviceUUID, RXCallbackFunction rxCallback)
  {
    ServiceUUID = serviceUUID;
    InfoUUID    = MakeCharacteristicUUID(serviceUUID, 1);
    RXUUID      = MakeCharacteristicUUID(serviceUUID, 3);
    RXCallback  = rxCallback;

  	BLEDevice::init(name);
	  BLEServer* server = BLEDevice::createServer();
    server->setCallbacks(this);

    BLEService* service = server->createService(ServiceUUID);
  	service->createCharacteristic(    InfoUUID, BLECharacteristic::PROPERTY_READ  )->setValue(info);
    service->createCharacteristic(      RXUUID, BLECharacteristic::PROPERTY_WRITE )->setCallbacks(this);
  	service->start();

	  BLEAdvertising* advertising = BLEDevice::getAdvertising();
	  advertising->addServiceUUID(ServiceUUID);
	  advertising->start();
  }

private:
  void onConnect(BLEServer* server, esp_ble_gatts_cb_param_t* param)
  {
    Serial.print("Bluetooth connected\n");
    server->updateConnParams(param->connect.remote_bda, 15, 15, 0, param->connect.conn_params.timeout);
    server->startAdvertising();
  }

  void onDisconnect(BLEServer* server)
  {
    Serial.print("Bluetooth disconnected\n");
	  server->startAdvertising();
  }

  void onWrite(BLECharacteristic* characteristic)
  {
	  if (characteristic->getUUID().toString() == RXUUID)
  	{
      std::string value = characteristic->getValue();
      RXCallback((const uint8_t*)value.data(), value.length());
	  }
  }

  std::string MakeCharacteristicUUID(const std::string& serviceUUID, int offset)
  {
    char end[9];
    sprintf(end, "%08x", std::stoul(serviceUUID.substr(serviceUUID.length()-8), nullptr, 16) + offset);
    return serviceUUID.substr(0, serviceUUID.length()-8) + end;
  }

  std::string ServiceUUID;
  std::string InfoUUID;
  std::string RXUUID;
  RXCallbackFunction RXCallback;
};
