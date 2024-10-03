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

void test_lt() {
  char msg[] =
    "/ESO5\\000000000_A\r\n"
    "\r\n"
    "0-0:1.0.0(240214101735W)\r\n"
    "1-0:1.8.0(000360.152*kWh)\r\n"
    "1-0:1.8.1(000232.998*kWh)\r\n"
    "1-0:1.8.2(000081.859*kWh)\r\n"
    "1-0:1.8.3(000009.963*kWh)\r\n"
    "1-0:1.8.4(000035.332*kWh)\r\n"
    "1-0:2.8.0(000007.295*kWh)\r\n"
    "1-0:2.8.1(000003.245*kWh)\r\n"
    "1-0:2.8.2(000004.050*kWh)\r\n"
    "1-0:2.8.3(000000.000*kWh)\r\n"
    "1-0:2.8.4(000000.000*kWh)\r\n"
    "1-0:3.8.0(000014.422*kvarh)\r\n"
    "1-0:3.8.1(000010.607*kvarh)\r\n"
    "1-0:3.8.2(000002.494*kvarh)\r\n"
    "1-0:3.8.3(000000.294*kvarh)\r\n"
    "1-0:3.8.4(000001.027*kvarh)\r\n"
    "1-0:4.8.0(000000.002*kvarh)\r\n"
    "1-0:4.8.1(000000.002*kvarh)\r\n"
    "1-0:4.8.2(000000.000*kvarh)\r\n"
    "1-0:4.8.3(000000.000*kvarh)\r\n"
    "1-0:4.8.4(000000.000*kvarh)\r\n"
    "1-0:21.7.0(00.000*kW)\r\n"
    "1-0:41.7.0(00.000*kW)\r\n"
    "1-0:61.7.0(00.000*kW)\r\n"
    "1-0:22.7.0(00.024*kW)\r\n"
    "1-0:42.7.0(00.000*kW)\r\n"
    "1-0:62.7.0(00.000*kW)\r\n"
    "1-0:23.7.0(00.000*kvar)\r\n"
    "1-0:43.7.0(00.000*kvar)\r\n"
    "1-0:63.7.0(00.000*kvar)\r\n"
    "1-0:24.7.0(00.000*kvar)\r\n"
    "1-0:44.7.0(00.000*kvar)\r\n"
    "1-0:64.7.0(00.000*kvar)\r\n"
    "1-0:32.7.0(234.7*V)\r\n"
    "1-0:32.24.0(234.6*V)\r\n"
    "1-0:31.7.0(000*A)\r\n"
    "1-0:52.7.0(235.6*V)\r\n"
    "1-0:52.24.0(234.8*V)\r\n"
    "1-0:51.7.0(000*A)\r\n"
    "1-0:72.7.0(234.0*V)\r\n"
    "1-0:72.24.0(233.9*V)\r\n"
    "1-0:71.7.0(000*A)\r\n"
    "1-0:12.7.0(235.6*V)\r\n"
    "1-0:11.7.0(000*A)\r\n"
    "1-0:91.7.0(000*A)\r\n"
    "1-0:90.7.0(000*A)\r\n"
    "1-0:14.7.0(50.0*Hz)\r\n"
    "1-0:15.7.0(00.024*kW)\r\n"
    "1-0:9.7.0(00.000*kVA)\r\n"
    "1-0:29.7.0(00.000*kVA)\r\n"
    "1-0:49.7.0(00.000*kVA)\r\n"
    "1-0:69.7.0(00.000*kVA)\r\n"
    "1-0:10.7.0(00.025*kVA)\r\n"
    "1-0:30.7.0(00.025*kVA)\r\n"
    "1-0:50.7.0(00.000*kVA)\r\n"
    "1-0:70.7.0(00.000*kVA)\r\n"
    "1-0:1.24.0(00.000*kW)\r\n"
    "1-0:16.24.0(-00.024*kW)\r\n"
    "1-0:15.24.0(00.008*kW)\r\n"
    "1-0:13.7.0(-1.000)\r\n"
    "1-0:33.7.0(-0.913)\r\n"
    "1-0:53.7.0(1.000)\r\n"
    "1-0:73.7.0(1.000)\r\n"
    "1-0:13.3.0(0.000)\r\n"
    "1-0:0.8.2(00900*s)\r\n"
    "1-0:1.4.0(000000.000*kW)\r\n"
    "1-0:2.4.0(000000.024*kW)\r\n"
    "1-0:3.4.0(000000.000*kvar)\r\n"
    "1-0:4.4.0(000000.000*kvar)\r\n"
    "1-0:9.4.0(000000.000*kVA)\r\n"
    "1-0:10.4.0(000000.024*kVA)\r\n"
    "0-0:96.7.21(00039)\r\n"
    "1-0:32.33.0(03042*s)\r\n"
    "1-0:52.33.0(03042*s)\r\n"
    "1-0:72.33.0(03042*s)\r\n"
    "1-0:32.34.0(0000*V)\r\n"
    "1-0:52.34.0(0000*V)\r\n"
    "1-0:72.34.0(0000*V)\r\n"
    "1-0:32.37.0(00000*s)\r\n"
    "1-0:52.37.0(00000*s)\r\n"
    "1-0:72.37.0(00000*s)\r\n"
    "1-0:32.38.0(000*V)\r\n"
    "1-0:52.38.0(000*V)\r\n"
    "1-0:72.38.0(000*V)\r\n"
    "1-0:0.2.0(02.21)\r\n"
    "1-0:0.2.8(1EA43311)\r\n"
    "1-1:0.2.0(02.13)\r\n"
    "1-1:0.2.8(047C71F1)\r\n"
    "0-0:96.13.0(73657450315F6D61785F337068)\r\n"
    "1-0:32.32.0(00001)\r\n"
    "1-0:52.32.0(00001)\r\n"
    "1-0:72.32.0(00001)\r\n"
    "1-0:32.36.0(00000)\r\n"
    "1-0:52.36.0(00000)\r\n"
    "1-0:72.36.0(00000)\r\n"
    "0-0:96.1.1(3030303030303030303030303030303030303030303030303030303030303030)\r\n"
    "!9A1F\r\n";

  ParsedData<
    /* String */ identification,
    /*negative value*/ active_demand_net,
    /*negative value*/ power_factor,
    /*negative value*/ power_factor_l1,
    /*negative value*/ power_factor_l2,
    /*negative value*/ power_factor_l3,
    /*negative value*/ min_power_factor,
    /*HexToString*/ consumer_msg
  > myData;

  ParseResult<void> res = P1Parser::parse(&myData, msg, lengthof(msg));
  if (res.err) {
    char* toPrint = res.fullError(msg, msg + lengthof(msg));
    std::cout << toPrint << std::endl;
    free(toPrint);
  }

  TEST_ASSERT_TRUE(res.err);
  //TEST_ASSERT_EQUAL_INT(strlen("setP1_max_3ph"), strlen(myData.consumer_msg));
  //TEST_ASSERT_EQUAL_STRING("setP1_max_3ph", myData.consumer_msg);
  
  TEST_ASSERT_FLOAT_WITHIN(-0.025, -0.021, myData.active_demand_net.val());
  TEST_ASSERT_FLOAT_WITHIN(-1.0, 1.0 , myData.power_factor_l1.val());
  TEST_ASSERT_FLOAT_WITHIN(-1.0, 1.0, myData.power_factor_l2.val());
  TEST_ASSERT_FLOAT_WITHIN(-1.0, 1.0, myData.power_factor_l3.val());
  TEST_ASSERT_FLOAT_WITHIN(-1.0, 1.0, myData.min_power_factor.val());
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_short);
  RUN_TEST(test_full);
  RUN_TEST(test_be);
  RUN_TEST(test_checksum);
  RUN_TEST(test_lt);
  return UNITY_END();
}
