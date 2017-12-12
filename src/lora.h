#ifndef LORA_H
#define LORA_H

#include <Arduino.h>
#include <RHReliableDatagram.h>
#include <RH_RF95.h>
#include <RadioHead.h>

#include <action.h>
#include <device.h>
#include <hexdump.h>
#include <lora.h>

typedef enum lora_status_e { OK = 0, ERROR = 1 } lora_status_e;

lora_status_e lora_read(struct TUT__device *dev, struct TUT__action *act, RHReliableDatagram *mgr,
                        int address, int timeout = 250);

lora_status_e lora_write(struct TUT__action *act, struct TUT__device *dev,
                         bool led_state, RHReliableDatagram *mgr, int address,
                         int timeout = 250);

int get_action_write(struct TUT__action *act);

int get_action_read(struct TUT__action *act);

int get_device_led(struct TUT__device *dev);

int get_device_poti(struct TUT__device *dev);

#endif
