#include <Arduino.h>
#include <RHReliableDatagram.h>
#include <RH_RF95.h>
#include <RadioHead.h>

#include <action.h>
#include <device.h>
#include <hexdump.h>
#include <lora.h>

const bool serialdebug = false;

uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];

// LoRa Read Function
lora_status_e lora_read(struct TUT__device *dev, struct TUT__action *act, RHReliableDatagram *mgr,
                        int address, int timeout = 250) {
  lora_status_e result = ERROR;

  act->read = 1;
  act->write = 0;

  size_t len = act->TUT__action_serialize(act, buf, sizeof(buf));

  if (mgr->sendtoWait(buf, len, address)) {
    if (serialdebug)
      Serial.println("Sent.");

    uint8_t len = sizeof(buf);
    if (mgr->recvfromAckTimeout(buf, &len, timeout)) {
      if (serialdebug)
        hexDump("ANSWER", buf, len);

      size_t res = dev->TUT__device_parse(dev, buf, len);
      if (serialdebug) {
        Serial.print("res=");
        Serial.println(res);
      }
      if (res == 0) {
        result = OK;

        if (serialdebug)
          Serial.println("Received device struct");
      } else {
        if (serialdebug)
          Serial.println("ERROR receiving device struct");
      }
    } else {
      if (serialdebug)
        Serial.println("ERROR while waiting for answer");
    }
  } else {
    if (serialdebug)
      Serial.println("ERROR sending");
  }

  return result;
}

// LoRa Write Function
lora_status_e lora_write(struct TUT__action *act, struct TUT__device *dev,
                         bool led_state, RHReliableDatagram *mgr, int address,
                         int timeout = 250) {
  lora_status_e result = ERROR;

  act->read = 0;
  act->write = led_state;

  size_t len = act->TUT__action_serialize(act, buf, sizeof(buf));

  if (mgr->sendtoWait(buf, len, address)) {
    result = OK;
    if (serialdebug)
      Serial.println("Sent.");

    uint8_t len = sizeof(buf);
    if (mgr->recvfromAckTimeout(buf, &len, timeout)) {
      if (serialdebug)
        hexDump("ANSWER", buf, len);

      // size_t res = dev->TUT__device_parse(dev, buf, len);
      // ignore contents
    }
  } else {
    if (serialdebug)
      Serial.println("ERROR sending");
  }

  return result;
}

int get_action_write(struct TUT__action *act) { return act->write; }

int get_action_read(struct TUT__action *act) { return act->read; }

int get_device_led(struct TUT__device *dev) { return dev->led; }

int get_device_poti(struct TUT__device *dev) { return dev->poti; }
