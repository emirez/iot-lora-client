#include "action.h"
#include "device.h"
#include "lora.h"
#include "unity.h"
#include <Arduino.h>

#ifdef UNIT_TEST

#define MY_ADDRESS 1
#define OTHER_ADDRESS 2

RH_RF95 driver;
RHReliableDatagram manager(driver, MY_ADDRESS);

const bool serialdebug = false;
static bool led_state = 0;

struct TUT__action my_action;
struct TUT__device my_device;

// TESTING FUNCTIONS
void test_lora_write_led_on(void) {
  TUT__action_init(&my_action);
  TUT__device_init(&my_device);
  lora_write(&my_action, &my_device, led_state, &manager, OTHER_ADDRESS);
  TEST_ASSERT_EQUAL(0, get_action_write(&my_action));
}

void test_lora_write_led_off(void) {
  TUT__action_init(&my_action);
  TUT__device_init(&my_device);
  led_state = !led_state;
  lora_write(&my_action, &my_device, led_state, &manager, OTHER_ADDRESS);
  TEST_ASSERT_EQUAL(1, get_action_write(&my_action));
}

void test_lora_read_str_read(void) {
  TUT__action_init(&my_action);
  TUT__device_init(&my_device);
  lora_read(&my_device, &my_action, &manager, OTHER_ADDRESS);
  TEST_ASSERT_EQUAL(1, get_action_read(&my_action));
}

void test_lora_read_str_write(void) {
  TUT__action_init(&my_action);
  TUT__device_init(&my_device);
  lora_read(&my_device, &my_action, &manager, OTHER_ADDRESS);
  TEST_ASSERT_EQUAL(0, get_action_write(&my_action));
}
// If the second device is connected, then this test fails!
void test_lora_read_status(void){
  TUT__action_init(&my_action);
  TUT__device_init(&my_device);
  lora_status_e result = lora_read(&my_device, &my_action, &manager, OTHER_ADDRESS);
  TEST_ASSERT_EQUAL(1, result); //1 is offline, 0 is online.
}
// If the second device is connected, then this test fails!
void test_lora_write_status(void){
  TUT__action_init(&my_action);
  TUT__device_init(&my_device);
  lora_status_e result = lora_write(&my_action, &my_device, led_state, &manager, OTHER_ADDRESS);
  TEST_ASSERT_EQUAL(1, result); //1 is offline, 0 is online.
}

void setup() {
  Serial.begin(9600);
  manager.init();
  driver.setTxPower(5, false);
  // TESTING
  delay(2000);
  UNITY_BEGIN();
  RUN_TEST(test_lora_write_led_on);
  RUN_TEST(test_lora_write_led_off);
  RUN_TEST(test_lora_read_str_read);
  RUN_TEST(test_lora_read_str_write);
  RUN_TEST(test_lora_read_status);
  RUN_TEST(test_lora_write_status);
  UNITY_END();
  // TESTING ENDS
}

void loop() {

}
#endif
