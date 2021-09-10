#include "_Plugin_Helper.h"
#ifdef USES_P220

// #######################################################################################################
// #################################### Plugin 220: Adafruit MCP4725 v2 ##############################
// #######################################################################################################

// Adafruit MCP4725 v2
// like this one: https://www.adafruit.com/products/1438
// based on this library: https://github.com/adafruit/Adafruit_DAC_Shield_V2_Library
// written by https://github.com/krikk
// Currently DC DACs and Steppers are implemented, Servos are in default firmware!!!


#include <Wire.h>
#include <Adafruit_MCP4725.h>


#define PLUGIN_220
#define PLUGIN_ID_220         220
#define PLUGIN_NAME_220       "DAC - Adafruit MCP4725 [TESTING]"
#define PLUGIN_VALUENAME1_220 "MCP4725"

#define Plugin_220_MCP4725_address     PCONFIG(0)
#define Plugin_220_DacSteps            PCONFIG(1)
#define Plugin_220_DacMem              PCONFIG(2)

boolean Plugin_220(uint8_t function, struct EventStruct *event, String& string) {
  boolean success = false;

  Adafruit_MCP4725 dac;


  switch (function) {
    case PLUGIN_DEVICE_ADD: {
      Device[++deviceCount].Number           = PLUGIN_ID_220;
      Device[deviceCount].Type               = DEVICE_TYPE_I2C;
      Device[deviceCount].VType              = Sensor_VType::SENSOR_TYPE_NONE;
      Device[deviceCount].Ports              = 0;
      Device[deviceCount].PullUpOption       = false;
      Device[deviceCount].InverseLogicOption = false;
      Device[deviceCount].FormulaOption      = false;
      Device[deviceCount].ValueCount         = 2;
      Device[deviceCount].SendDataOption     = true;
      Device[deviceCount].TimerOption        = true;
      break;
    }

    case PLUGIN_GET_DEVICENAME: {
      string = F(PLUGIN_NAME_220);
      break;
    }

    case PLUGIN_GET_DEVICEVALUENAMES: {
      strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[0],
               PSTR(PLUGIN_VALUENAME1_220));
      break;
    }

    case PLUGIN_WEBFORM_SHOW_I2C_PARAMS:
    {
      addFormTextBox(F("I2C Address (Hex)"), F("i2c_addr"),
                     formatToHex_decimal(Plugin_220_MCP4725_address), 4);

      // FIXME TD-er: Why not using addFormSelectorI2C here?
      break;
    }

    case PLUGIN_SET_DEFAULTS:
    {
      Plugin_220_MCP4725_address    = 0x60;
      Plugin_220_DacSteps           = 0;
      Plugin_220_DacMem             = 0;

      break;
    }


    case PLUGIN_WEBFORM_LOAD: {
      addFormNumericBox(F("Dac: steps"), F("P220_DacSteps")
                        , Plugin_220_DacSteps);

      addFormNumericBox(F("Dac (mem)"), F("P220_DacMem")
                        , Plugin_220_DacMem);

      success = true;
      break;
    }

    case PLUGIN_WEBFORM_SAVE: {
      String plugin1 = webArg(F("i2c_addr"));
      Plugin_220_MCP4725_address = (int)strtol(plugin1.c_str(), 0, 16);

      Plugin_220_DacSteps = getFormItemInt(F("P220_DacSteps"));

      Plugin_220_DacMem = getFormItemInt(F("P220_DacMem"));
      success                 = true;
      break;
    }

    case PLUGIN_INIT: {
      success = true;
      break;
    }

    case PLUGIN_READ: {
      success = false;
      break;
    }

    case PLUGIN_WRITE: {
      String cmd = parseString(string, 1);

      // Commands:
      // MCP4725CMD,<DCDAC>,<DACnumber>,<Forward/Backward/Release>,<Steps>

      if (cmd.equalsIgnoreCase(F("MCP4725CMD")))
      {
        String param1 = parseString(string, 2);
        String param2 = parseString(string, 3);
        String param3 = parseString(string, 4);
        String param4 = parseString(string, 5);
        String param5 = parseString(string, 6);

        int p2_int;
        int p4_int;
        const bool param2_is_int = validIntFromString(param2, p2_int);
        const bool param4_is_int = validIntFromString(param4, p4_int);

        // Create the DAC shield object with the default I2C address
        dac = Adafruit_MCP4725(Plugin_220_MCP4725_address);
        #ifndef BUILD_NO_DEBUG
        if (loglevelActiveFor(LOG_LEVEL_DEBUG)) {
          String log = F("MCP4725: Address: 0x");
          log += String(Plugin_220_MCP4725_address, HEX);
          addLog(LOG_LEVEL_DEBUG, log);
        }
        #endif

        if (param1.equalsIgnoreCase(F("DAC"))) {
          if (param2_is_int && (p2_int > 0) && (p2_int < 5))
          {
           //Adafruit_DCDAC *myDAC;
           //myDAC = AFMS.getDAC(p2_int);

            if (param3.equalsIgnoreCase(F("On")))
            {
              uint16_t Steps = 4095;

              if (param4_is_int && (p4_int >= 0) && (p4_int <= 4095)) {
                Steps = p4_int;
              }
                dac.begin(0x60);
              if (loglevelActiveFor(LOG_LEVEL_INFO)) {
                String log = F("DAC");
                log += param2;
                log += F("->Steps: ");
                log += Steps;
                addLog(LOG_LEVEL_INFO, log);
              }
                dac.setVoltage(Steps, false);
                UserVar[event->BaseVarIndex] = Steps; 
              success = true;
            }
          }
        }
      }
      break;
    }
  }
  return success;
}

#endif // USES_P220
