#include <iostream>

#include <unity.h>

#include <dsmr.h>

void setUp() {}
void tearDown() {}

void test_short() {
  char msg[] =
    "/KFM5KAIFA-METER\r\n"
    "\r\n"
    "1-0:1.8.1(000671.578*kWh)\r\n"
    "1-0:1.7.0(00.318*kW)\r\n"
    "!1E1D\r\n";

  ParsedData<
    /* String */ identification,
    /* FixedValue */ power_delivered
  > myData;

  ParseResult<void> res = P1Parser::parse(&myData, msg, lengthof(msg));
  if (res.err) {
    char* toPrint = res.fullError(msg, msg + lengthof(msg));
    std::cout << toPrint << std::endl;
    free(toPrint);
  }

  TEST_ASSERT_FALSE(res.err);
  TEST_ASSERT_EQUAL_INT(strlen("KFM5KAIFA-METER"), strlen(myData.identification));
  TEST_ASSERT_EQUAL_STRING("KFM5KAIFA-METER", myData.identification);
  TEST_ASSERT_EQUAL_UINT32(318, myData.power_delivered.int_val());
  TEST_ASSERT_FLOAT_WITHIN(0.1, 0.318, myData.power_delivered.val());
}

void test_full() {
  char msg[] =
    "/KFM5KAIFA-METER\r\n"
    "\r\n"
    "1-3:0.2.8(40)\r\n"
    "0-0:1.0.0(150117185916W)\r\n"
    "0-0:96.1.1(0000000000000000000000000000000000)\r\n"
    "1-0:1.8.1(000671.578*kWh)\r\n"
    "1-0:1.8.2(000842.472*kWh)\r\n"
    "1-0:2.8.1(000000.000*kWh)\r\n"
    "1-0:2.8.2(000000.000*kWh)\r\n"
    "0-0:96.14.0(0001)\r\n"
    "1-0:1.7.0(00.333*kW)\r\n"
    "1-0:2.7.0(00.000*kW)\r\n"
    "0-0:17.0.0(999.9*kW)\r\n"
    "0-0:96.3.10(1)\r\n"
    "0-0:96.7.21(00008)\r\n"
    "0-0:96.7.9(00007)\r\n"
    "1-0:99.97.0(1)(0-0:96.7.19)(000101000001W)(2147483647*s)\r\n"
    "1-0:32.32.0(00000)\r\n"
    "1-0:32.36.0(00000)\r\n"
    "0-0:96.13.1()\r\n"
    "0-0:96.13.0()\r\n"
    "1-0:31.7.0(001*A)\r\n"
    "1-0:21.7.0(00.332*kW)\r\n"
    "1-0:22.7.0(00.000*kW)\r\n"
    "0-1:24.1.0(003)\r\n"
    "0-1:96.1.0(0000000000000000000000000000000000)\r\n"
    "0-1:24.2.1(150117180000W)(00473.789*m3)\r\n"
    "0-1:24.4.0(1)\r\n"
    "!6F4A\r\n";

  ParsedData<
    /* String */ identification,
    /* String */ p1_version,
    /* String */ timestamp,
    /* String */ equipment_id,
    /* FixedValue */ energy_delivered_tariff1,
    /* FixedValue */ energy_delivered_tariff2,
    /* FixedValue */ energy_returned_tariff1,
    /* FixedValue */ energy_returned_tariff2,
    /* String */ electricity_tariff,
    /* FixedValue */ power_delivered,
    /* FixedValue */ power_returned,
    /* FixedValue */ electricity_threshold,
    /* uint8_t */ electricity_switch_position,
    /* uint32_t */ electricity_failures,
    /* uint32_t */ electricity_long_failures,
    /* String */ electricity_failure_log,
    /* uint32_t */ electricity_sags_l1,
    /* uint32_t */ electricity_sags_l2,
    /* uint32_t */ electricity_sags_l3,
    /* uint32_t */ electricity_swells_l1,
    /* uint32_t */ electricity_swells_l2,
    /* uint32_t */ electricity_swells_l3,
    /* String */ message_short,
    /* String */ message_long,
    /* FixedValue */ voltage_l1,
    /* FixedValue */ voltage_l2,
    /* FixedValue */ voltage_l3,
    /* FixedValue */ current_l1,
    /* FixedValue */ current_l2,
    /* FixedValue */ current_l3,
    /* FixedValue */ power_delivered_l1,
    /* FixedValue */ power_delivered_l2,
    /* FixedValue */ power_delivered_l3,
    /* FixedValue */ power_returned_l1,
    /* FixedValue */ power_returned_l2,
    /* FixedValue */ power_returned_l3,
    /* uint16_t */ gas_device_type,
    /* String */ gas_equipment_id,
    /* uint8_t */ gas_valve_position,
    /* TimestampedFixedValue */ gas_delivered,
    /* uint16_t */ thermal_device_type,
    /* String */ thermal_equipment_id,
    /* uint8_t */ thermal_valve_position,
    /* TimestampedFixedValue */ thermal_delivered,
    /* uint16_t */ water_device_type,
    /* String */ water_equipment_id,
    /* uint8_t */ water_valve_position,
    /* TimestampedFixedValue */ water_delivered,
    /* uint16_t */ sub_device_type,
    /* String */ sub_equipment_id,
    /* uint8_t */ sub_valve_position,
    /* TimestampedFixedValue */ sub_delivered
  > myData;

  ParseResult<void> res = P1Parser::parse(&myData, msg, lengthof(msg));
  if (res.err) {
    char* toPrint = res.fullError(msg, msg + lengthof(msg));
    std::cout << toPrint << std::endl;
    free(toPrint);
  }

  TEST_ASSERT_FALSE(res.err);
  // test each type only once
  TEST_ASSERT_EQUAL_INT(strlen("150117185916W"), strlen(myData.timestamp));
  TEST_ASSERT_EQUAL_STRING("150117185916W", myData.timestamp);
  TEST_ASSERT_EQUAL_STRING("0001", myData.electricity_tariff);
  TEST_ASSERT_EQUAL_UINT32(671578, myData.energy_delivered_tariff1.int_val());  // part of FixedValue
  TEST_ASSERT_FLOAT_WITHIN(0.1, 671.578, myData.energy_delivered_tariff1.val());  // part of FixedValue
  TEST_ASSERT_EQUAL_UINT8(1, myData.electricity_switch_position);
  TEST_ASSERT_EQUAL_UINT32(8, myData.electricity_failures);
  TEST_ASSERT_EQUAL_UINT16(3, myData.gas_device_type);
  TEST_ASSERT_EQUAL_STRING("150117180000W", myData.gas_delivered.timestamp);
  TEST_ASSERT_FLOAT_WITHIN(0.1, 473.789, myData.gas_delivered.val());
}

void test_be() {
  char msg[] =
    "/FLU5\\253769484_A\r\n"
    "\r\n"
    "0-0:96.1.4(50217)\r\n"
    "1-0:1.6.0(240707191500S)(01.811*kW)\r\n"
    "0-0:98.1.0(13)(1-0:1.6.0)(1-0:1.6.0)(230701000000S)(230601003000S)(02.680*kW)(230801000000S)(230731200000S)(02.099*kW)(230901000000S)(230812224500S)(02.802*kW)(231001000000S)(230911210000S)(03.033*kW)(231101000000W)(231003220000S)(02.779*kW)(231201000000W)(231126213000W)(03.407*kW)(240101000000W)(231224193000W)(04.328*kW)(240201000000W)(240126200000W)(03.966*kW)(240301000000W)(240229204500W)(05.834*kW)(240401000000S)(240306193000W)(03.527*kW)(240501000000S)(240414201500S)(03.843*kW)(240601000000S)(240525111500S)(02.768*kW)(240701000000S)(240618191500S)(03.841*kW)\r\n"
    "0-1:24.2.3(240708200459S)(08538.167*m3)\r\n"
    "!2F8F\r\n";

  ParsedData<
    /* String */ identification,
    /* String */ p1_version_be,
    /* String */ equipment_id,
    /* String */ timestamp,
    /* FixedValue */ energy_delivered_tariff1,
    /* FixedValue */ energy_delivered_tariff2,
    /* FixedValue */ energy_returned_tariff1,
    /* FixedValue */ energy_returned_tariff2,
    /* String */ electricity_tariff,
    /* FixedValue */ active_energy_import_current_average_demand,
    /* FixedValue */ active_energy_import_maximum_demand_running_month,
    /* FixedValue */ active_energy_import_maximum_demand_last_13_months,
    /* FixedValue */ power_delivered,
    /* FixedValue */ power_returned,
    /* FixedValue */ power_delivered_l1,
    /* FixedValue */ power_delivered_l2,
    /* FixedValue */ power_delivered_l3,
    /* FixedValue */ power_returned_l1,
    /* FixedValue */ power_returned_l2,
    /* FixedValue */ power_returned_l3,
    /* FixedValue */ voltage_l1,
    /* FixedValue */ voltage_l2,
    /* FixedValue */ voltage_l3,
    /* FixedValue */ current_l1,
    /* FixedValue */ current_l2,
    /* FixedValue */ current_l3,
    /* uint8_t */ electricity_switch_position,
    /* FixedValue */ electricity_threshold,
    /* FixedValue */ current_fuse_l1,
    /* std::string */ message_long,
    /* uint16_t */ gas_device_type,
    /* std::string */ gas_equipment_id_be,
    /* uint8_t */ gas_valve_position,
    /* TimestampedFixedValue */ gas_delivered_be
  > myData;

  ParseResult<void> res = P1Parser::parse(&myData, msg, lengthof(msg));
  if (res.err) {
    char* toPrint = res.fullError(msg, msg + lengthof(msg));
    std::cout << toPrint << std::endl;
    free(toPrint);
  }

  TEST_ASSERT_FALSE(res.err);
  TEST_ASSERT_EQUAL_INT(strlen("50217"), strlen(myData.p1_version_be));
  TEST_ASSERT_EQUAL_STRING("50217", myData.p1_version_be);
  TEST_ASSERT_FLOAT_WITHIN(0.1, 1.811, myData.active_energy_import_maximum_demand_running_month.val());
  TEST_ASSERT_FLOAT_WITHIN(0.1, 3.841, myData.active_energy_import_maximum_demand_last_13_months.val());
  TEST_ASSERT_FLOAT_WITHIN(0.1, 8538.167, myData.gas_delivered_be.val());
}

void test_checksum() {
  char msg[] =
    "/KFM5KAIFA-METER\r\n"
    "\r\n"
    "1-0:1.8.1(000671.578*kWh)\r\n"
    "1-0:1.7.0(00.318*kW)\r\n"
    "!1EED\r\n";

  ParsedData<
    /* String */ identification,
    /* FixedValue */ power_delivered
  > myData;

  ParseResult<void> res = P1Parser::parse(&myData, msg, lengthof(msg));
  if (res.err) {
    char* toPrint = res.fullError(msg, msg + lengthof(msg));
    std::cout << toPrint << std::endl;
    free(toPrint);
  }

  TEST_ASSERT_TRUE(res.err);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_short);
  RUN_TEST(test_full);
  RUN_TEST(test_be);
  RUN_TEST(test_checksum);
  return UNITY_END();
}
