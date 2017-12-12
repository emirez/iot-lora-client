#ifndef UNIT_TEST

#include <Arduino.h>
#include <RHReliableDatagram.h>
#include <RH_RF95.h>
#include <RadioHead.h>

#include <action.h>
#include <device.h>
#include <hexdump.h>
#include <lora.h>

#define MY_ADDRESS 1
#define OTHER_ADDRESS 2

static bool led_state = 0;

RH_RF95 driver;
RHReliableDatagram manager(driver, MY_ADDRESS);

const bool serialdebug = false;

struct TUT__device device;
struct TUT__action action;

void setup() {
  Serial.begin(9600);
  if (manager.init()) {
    Serial.println("RadioHead Datagram Manager initialized");
  } else {
    Serial.println("ERROR initializing RadioHead Datagram Manager");
  }
  driver.setTxPower(5, false);
  Serial.println("TF Lora Client initialized");
}

void loop() {
	TUT__device_init(&device);
	TUT__action_init(&action);

  if (lora_read(&device, &action, &manager, OTHER_ADDRESS) == OK) {
    Serial.print("device.led=");
    Serial.println(device.led);
    Serial.print("device.poti=");
    Serial.println(device.poti);
  }

  delay(3000);
  led_state = !led_state;
  lora_write(&action, &device, led_state, &manager, OTHER_ADDRESS);
  delay(3000);
}
#endif
